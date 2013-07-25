#include <lgraphics/io11/IOBMP.h>
#include <lgraphics/io11/IOTGA.h>
#include <lgraphics/io11/IOPNG.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

namespace
{
    bool isFile(const WIN32_FIND_DATA& data)
    {
        static const DWORD checks[] =
        {
            FILE_ATTRIBUTE_HIDDEN,
            FILE_ATTRIBUTE_SYSTEM,
            FILE_ATTRIBUTE_DIRECTORY,
            FILE_ATTRIBUTE_ENCRYPTED,
            FILE_ATTRIBUTE_TEMPORARY,
            FILE_ATTRIBUTE_SPARSE_FILE,
        };

        int Num = sizeof(checks)/sizeof(DWORD);
        for(int i=0; i<Num; ++i){
            DWORD check = checks[i] & data.dwFileAttributes;
            if(check != 0){
                return false;
            }
        }
        return true;
    }

    enum FileType
    {
        FileType_BMP,
        FileType_TGA,
        FileType_PNG,
        FileType_Unknown,
    };

    static const int InvalidID = -1;

    struct FileEntry
    {
        FileType type_;
        std::string path_;
        std::string name_;

        int paddedWidth_;
        int paddedHeight_;
    };

    bool operator<(const FileEntry& lhs, const FileEntry& rhs)
    {
        int larea = lhs.paddedWidth_*lhs.paddedHeight_;
        int rarea = rhs.paddedWidth_*rhs.paddedHeight_;

        return larea < rarea ;
    }

    bool operator>(const FileEntry& lhs, const FileEntry& rhs)
    {
        int larea = lhs.paddedWidth_*lhs.paddedHeight_;
        int rarea = rhs.paddedWidth_*rhs.paddedHeight_;

        return larea > rarea ;
    }

    typedef std::vector<FileEntry> FileEntryVector;

    struct Rect
    {
        Rect()
            :left_(0)
            ,top_(0)
            ,right_(0)
            ,bottom_(0)
        {}

        Rect(int left,
            int top,
            int right,
            int bottom)
            :left_(left)
            ,top_(top)
            ,right_(right)
            ,bottom_(bottom)
        {}

        int width() const
        {
            return right_ - left_ + 1;
        }

        int height() const
        {
            return bottom_ - top_ + 1;
        }

        int left_;
        int top_;
        int right_;
        int bottom_;
    };

    class Node
    {
    public:
        Node();
        ~Node();

        bool isLeaf() const;

        Node* insert(int imageID, int width, int height);

        /**
        @return -1:小さすぎる、0:fit、1:包含
        */
        int fit(int width, int height) const;

        Node* children_[2];
        Rect rect_;
        int imageID_;
    };

    Node::Node()
        :imageID_(InvalidID)
    {
        children_[0] = NULL;
        children_[1] = NULL;
    }

    Node::~Node()
    {
        for(int i=0; i<2; ++i){
            if(NULL != children_[i]){
                delete children_[i];
                children_[i] = NULL;
            }
        }
    }

    bool Node::isLeaf() const
    {
        return (NULL == children_[0] && NULL == children_[1]);
    }

    int Node::fit(int width, int height) const
    {
        int w = rect_.width();
        int h = rect_.height();
        if(width == w && height == h){
            return 0;
        }

        if(width<=w && height<=h){
            return 1;
        }
        return -1;
    }

    Node* Node::insert(int imageID, int width, int height)
    {
        if(isLeaf()){
            //すでに埋まっているか
            if(InvalidID != imageID_){
                return NULL;
            }

            switch(fit(width, height))
            {
            case 0:
                imageID_ = imageID;
                return this;

            case 1:
                break;

            default:
                return NULL;
            }

            children_[0] = new Node();
            children_[1] = new Node();


            //大きい軸を分割
            int dw = rect_.width() - width;
            int dh = rect_.height() - height;

            if(dh<dw){
                children_[0]->rect_ = Rect(rect_.left_, rect_.top_, rect_.left_+width-1, rect_.bottom_);
                children_[1]->rect_ = Rect(rect_.left_+width, rect_.top_, rect_.right_, rect_.bottom_);
            }else{
                children_[0]->rect_ = Rect(rect_.left_, rect_.top_, rect_.right_, rect_.top_+height-1);
                children_[1]->rect_ = Rect(rect_.left_, rect_.top_+height, rect_.right_, rect_.bottom_);
            }

            return children_[0]->insert(imageID, width, height);

        }else{
            Node* newNode = NULL;
            if(children_[0]){
                newNode = children_[0]->insert(imageID, width, height);
                if(NULL == newNode){
                    newNode = children_[1]->insert(imageID, width, height);
                }
                return newNode;
            }else{
                return children_[1]->insert(imageID, width, height);
            }
        }
    }

    class Pack
    {
    public:
        Pack(int padding, int size, FileEntryVector& entries, const lcore::Char* enumFile, const lcore::Char* listFile);

        void save(Node& root, lcore::u8* buffer);
    private:
        void saveNode(Node* node);
        void saveImage(Node& node);

        int padding_;
        int size_;
        FileEntryVector& entries_;
        lcore::ofstream enumFile_;
        lcore::ofstream listFile_;

        lcore::u8* buffer_;
    };

    Pack::Pack(int padding, int size, FileEntryVector& entries, const lcore::Char* enumFile, const lcore::Char* listFile)
        :padding_(padding)
        ,size_(size)
        ,entries_(entries)
    {
        if(NULL != enumFile){
            enumFile_.open(enumFile, lcore::ios::binary);
        }

        if(NULL != listFile){
            listFile_.open(listFile, lcore::ios::binary);
        }
    }

    void Pack::save(Node& root, lcore::u8* buffer)
    {
        LASSERT(NULL != buffer);
        buffer_ = buffer;

        saveNode(&root);
    }

    void Pack::saveNode(Node* node)
    {
        if(InvalidID != node->imageID_){
            saveImage(*node);
        }

        if(NULL != node->children_[0]){
            saveNode(node->children_[0]);
        }

        if(NULL != node->children_[1]){
            saveNode(node->children_[1]);
        }
    }

    void Pack::saveImage(Node& node)
    {
        FileEntry& entry = entries_[node.imageID_];

        lcore::ifstream in(entry.path_.c_str(), lcore::ios::binary);

        if(!in.is_open()){
            return;
        }

        const Rect& rect = node.rect_;
        if(enumFile_.is_open()){
            enumFile_.print("%s,\n", entry.name_.c_str());
        }

        if(listFile_.is_open()){
            listFile_.print("{%d, %d, %d, %d},\n", rect.left_, rect.top_, rect.right_, rect.bottom_);
        }

        lcore::u32 w = entry.paddedWidth_ - 2*padding_;
        lcore::u32 h = entry.paddedHeight_ - 2*padding_;

        lcore::u8* buffer = new lcore::u8[w*h*4];

        bool success = false;
        lcore::u32 width, height, rowBytes;
        lgraphics::DataFormat format;

        switch(entry.type_)
        {
        case FileType_BMP:
            success = lgraphics::io::IOBMP::read(in, buffer, width, height, format);
            rowBytes = 4*width;
            break;
        case FileType_TGA:
            success = lgraphics::io::IOTGA::read(in, buffer, width, height, format, true);
            rowBytes = 4*width;
            break;
        case FileType_PNG:
            success = lgraphics::io::IOPNG::read(in, buffer, width, height, rowBytes, format);
            break;
        }
        in.close();

        if(success){
            int top = size_ - rect.top_ - rect.height();
            int bottom = top + rect.height();

            for(lcore::s32 i=top+padding_; i<(bottom-padding_); ++i){
                for(lcore::s32 j=rect.left_+padding_; j<(rect.right_-padding_); ++j){
                    int dst = (i*size_ + j)*4;


                    int src = (i-top-padding_)*rowBytes + (j-rect.left_-padding_)*4;

                    buffer_[dst + 0] = buffer[src + 0];
                    buffer_[dst + 1] = buffer[src + 1];
                    buffer_[dst + 2] = buffer[src + 2];
                    buffer_[dst + 3] = buffer[src + 3];
                }
            }
        }
        delete[] buffer;
    }

}

int main(int argc, char** argv)
{
    if(argc<3){
        std::cerr << "TextureAtlas <input directory> <output tga file>" << std::endl;
        std::cerr << "options:" << std::endl;
        std::cerr << "\t-enum <output name list file>\t(-enum enum.txt)" << std::endl;
        std::cerr << "\t-list <output rectangle list file>\t(-list list.txt)" << std::endl;
        std::cerr << "\t-size <output image size>\t(-size 1024)" << std::endl;
        std::cerr << "\t-pad <padding width>\t(-pad 1)" << std::endl;

        return 0;
    }

    //入力、出力、パディング、リストファイルパース
    int inIndex = -1;
    int outIndex = -1;
    int enumIndex = -1;
    int listIndex = -1;
    int padding = 1;
    int size = 1024;
    for(int i=1; i<argc; ++i){
        if(strcmp(argv[i], "-enum") == 0){
            int j = i+1;
            if(j<argc){
                enumIndex = j;
                ++i;
            }
        }else if(strcmp(argv[i], "-list") == 0){
            int j = i+1;
            if(j<argc){
                listIndex = j;
                ++i;
            }
        }else if(strcmp(argv[i], "-size") == 0){
            int j = i+1;
            if(j<argc){
                size = ::atoi(argv[j]);
                ++i;
            }

        }else if(strcmp(argv[i], "-pad") == 0){
            int j = i+1;
            if(j<argc){
                padding = ::atoi(argv[j]);
                ++i;
            }
        }else if(inIndex<0){
            inIndex = i;
        }else{
            outIndex = i;
        }
    }
    if(inIndex<0 || outIndex<0){
        std::cerr << "invalid args" << std::endl;
        return 0;
    }

    //ディレクトリ内の画像ファイル列挙
    std::string path = argv[inIndex];
    std::size_t len = path.size();
    if('\\' != path[len-1] && '/' != path[len-1]){
        path += '\\';
    }
    std::string inpath = path;
    path += "*";
    WIN32_FIND_DATA findData;
    HANDLE handle = FindFirstFile(path.c_str(), &findData);
    if(INVALID_HANDLE_VALUE == handle){
        std::cerr << "can't open " << path << std::endl;
        return 0;
    }

    FileEntry fileEntry;
    FileEntryVector entries;

    for(;;){
        if(!isFile(findData)){
            if(FALSE == FindNextFile(handle, &findData)){
                break;
            }
            continue;
        }

        //std::cerr << findData.cFileName << std::endl;

        //拡張子抜きの名前だけ抽出。.bmp, .tga以外の拡張子のファイルは無視
        const lcore::Char* ext = lcore::rFindChr(findData.cFileName, '.', lcore::strlen(findData.cFileName));
        if(NULL != ext){
            FileType type = FileType_Unknown;
            if(0 == lcore::strncmp(ext, ".bmp", 4)){
                type = FileType_BMP;
            }else if(0 == lcore::strncmp(ext, ".tga", 4)){
                type = FileType_TGA;
            }else if(0 == lcore::strncmp(ext, ".png", 4)){
                type = FileType_PNG;
            }

            if(FileType_Unknown != type){

                fileEntry.type_ = type;
                fileEntry.path_ = inpath;
                fileEntry.path_.append(findData.cFileName);

                fileEntry.name_.assign( findData.cFileName, ext );
                for(std::size_t i=0; i<fileEntry.name_.size(); ++i){
                    fileEntry.name_[i] = ::toupper(fileEntry.name_[i]);
                }

                lcore::ifstream in(fileEntry.path_.c_str(), lcore::ios::binary);

                bool success = false;
                lcore::u32 width, height, rowBytes;
                lgraphics::DataFormat format;

                if(in.is_open()){
                    switch(type)
                    {
                    case FileType_BMP:
                        success = lgraphics::io::IOBMP::read(in, NULL, width, height, format);
                        break;
                    case FileType_TGA:
                        success = lgraphics::io::IOTGA::read(in, NULL, width, height, format);
                        break;
                    case FileType_PNG:
                        success = lgraphics::io::IOPNG::read(in, NULL, width, height, rowBytes, format);
                        break;
                    }
                }
                if(format != lgraphics::Data_R8G8B8A8_UNorm_SRGB){
                    std::cerr << "invalid format" << std::endl;
                }
                if(success){
                    fileEntry.paddedWidth_ = width + padding*2;
                    fileEntry.paddedHeight_ = height + padding*2;
                    entries.push_back(fileEntry);
                }
            }
        }

        if(FALSE == FindNextFile(handle, &findData)){
            break;
        }
    }
    FindClose(handle);

    //ソート
    std::sort(entries.begin(), entries.end(), std::greater<FileEntry>());

    Node root;
    root.rect_ = Rect(0, 0, size, size);

    int count = 0;
    for(FileEntryVector::iterator itr = entries.begin();
        itr != entries.end();
        ++itr)
    {
        if(NULL == root.insert(count, itr->paddedWidth_, itr->paddedHeight_)){
            std::cerr << "can't insert " << itr->name_ << std::endl;
        }
        ++count;
    }

    
    lcore::Char* enumFile = (0<=enumIndex)? argv[enumIndex] : NULL;
    lcore::Char* listFile = (0<=listIndex)? argv[listIndex] : NULL;
    Pack pack(padding, size, entries, enumFile, listFile);

    lcore::u8* buffer = new lcore::u8[size*size*4];
    lcore::memset(buffer, 0, size*size*4);
    pack.save(root, buffer);
    
    lcore::ofstream os(argv[outIndex], lcore::ios::binary);
    if(os.is_open()){
        lgraphics::io::IOTGA::write(os, buffer, size, size);
    }
    delete[] buffer;
    return 0;
}
