#ifndef INC_LCORE_LIOSTREAM_
#define INC_LCORE_LIOSTREAM_
/**
@file lfstream
@author t-sakai
@date 2010/05/25 create
*/
#include <stdio.h>
#include "lcore.h"
#include "clibrary.h"

namespace lcore
{
    typedef int OpenMode;
    
    class ios
    {
    public:
        enum OpenMode
        {
            OpenMode_ForceByte = 0xFFU,
        };
        static const int in = 0x01;
	    static const int out = 0x02;
	    static const int ate = 0x04;
	    static const int app = 0x08;
	    static const int trunc = 0x10;
	    static const int binary = 0x20;
	    
	    enum SeekDir
	    {
	        SeekDir_ForceByte = 0xFFU,
	    };
	    static const int beg = 0x00;
	    static const int cur = 0x01;
	    static const int end = 0x02;
	    
	    
	    enum Mode
        {
            Mode_R = 0,
            Mode_W,
            Mode_A,
            Mode_RB,
            Mode_WB,
            Mode_AB,
            Mode_RP,
            Mode_WP,
            Mode_AP,
            Mode_RBP,
            Mode_WBP,
            Mode_ABP,
            Mode_Num,
        };
        
        static int ModeInt[Mode_Num];
        
        static const Char* ModeString[Mode_Num];
        
        static const Char* getModeString(int mode);
    };
    
    //----------------------------------------------------------
    //---
    //--- istream
    //---
    //----------------------------------------------------------
    class istream
    {
    public:
        virtual bool eof() =0;
        virtual bool good() =0;
        
        virtual bool seekg(s32 offset, int dir) =0;
        virtual s32 tellg() =0;
        
        virtual lsize_t read(Char* dst, u32 count) =0;
        
    protected:
        istream(){}
        ~istream(){}

        istream(const istream&);
        istream& operator=(const istream&);

        template<class T> friend istream& operator>>(istream& strm, T& t);
    };
    
    template<class T>
    inline istream& operator>>(istream& strm, T& t)
    {
        strm.read((Char*)&t, sizeof(T));
        return strm;
    }
    
    //----------------------------------------------------------
    //---
    //--- ostream
    //---
    //----------------------------------------------------------
    class ostream
    {
    public:
        virtual bool eof() =0;
        virtual bool good() =0;
        
        virtual bool seekg(s32 offset, int dir) =0;
        virtual s32 tellg() =0;
        
        virtual lsize_t write(const Char* src, u32 count) =0;
    protected:
        ostream(){}
        ~ostream(){}
        ostream(const ostream&);
        ostream& operator=(const ostream&);
    };
    
    
    //----------------------------------------------------------
    //---
    //--- fstream_base
    //---
    //----------------------------------------------------------
    template<class Base>
    class fstream_base : public Base
    {
    public:
        bool is_open() const{ return (file_ != NULL);}
        void close();
        
        virtual bool eof();
        virtual bool good();
        
        virtual bool seekg(s32 offset, int dir);
        virtual s32 tellg();
        
        void swap(fstream_base& rhs)
        {
            lcore::swap(file_, rhs.file_);
        }

        u32 getSize(s32 afterOffset)
        {
            seekg(0, lcore::ios::end);
            u32 size = tellg();
            seekg(afterOffset, lcore::ios::beg);
            return size;
        }
    protected:
        fstream_base();
        fstream_base(const Char* filepath, int mode);
        ~fstream_base();

        bool open(const Char* filepath, int mode);
        
        virtual lsize_t read(Char* dst, u32 count);
        virtual lsize_t write(const Char* src, u32 count);
        
        FILE *file_;
    };
    
    template<class Base>
    fstream_base<Base>::fstream_base()
        :file_(NULL)
    {
    }

    template<class Base>
    fstream_base<Base>::fstream_base(const Char* filepath, int mode)
        :file_(NULL)
    {
        open(filepath, mode);
    }

    template<class Base>
    fstream_base<Base>::~fstream_base()
    {
        close();
    }

    template<class Base>
    bool fstream_base<Base>::open(const Char* filepath, int mode)
    {
        LASSERT(filepath != NULL);
        close();
        const Char* modeStr = ios::getModeString(mode);
        LASSERT(modeStr != NULL);
#if defined(_WIN32)
        errno_t e = fopen_s(&file_, filepath, modeStr);
        return (e==0);
#else
        file_ = fopen(filepath, modeStr);
        return (file_ != NULL);
#endif
    }

    template<class Base>
    void fstream_base<Base>::close()
    {
        if(file_){
            fclose(file_);
            file_ = NULL;
        }
    }

    template<class Base>
    bool fstream_base<Base>::eof()
    {
        LASSERT(file_ != NULL);
        return (ferror(file_) == 0) && (feof(file_) != 0);
    }

    template<class Base>
    bool fstream_base<Base>::good()
    {
        if(file_ == NULL){
            return false;
        }
        return (ferror(file_) == 0);
    }

    template<class Base>
    inline bool fstream_base<Base>::seekg(s32 offset, int dir)
    {
        LASSERT(file_ != NULL);
        LASSERT(0<=ios::beg && dir<=ios::end);
        return (0 == fseek(file_, offset, dir));

    }

    template<class Base>
    inline s32 fstream_base<Base>::tellg()
    {
        LASSERT(file_ != NULL);
        return ftell(file_);
    }

    template<class Base>
    inline lsize_t fstream_base<Base>::read(Char* dst, u32 count)
    {
        LASSERT(file_ != NULL);
        return fread((void*)dst, count, 1, file_);
    }

    template<class Base>
    inline lsize_t fstream_base<Base>::write(const Char* src, u32 count)
    {
        return fwrite((void*)src, count, 1, file_);
    }


    //----------------------------------------------------------
    //---
    //--- ifstream
    //---
    //----------------------------------------------------------
    class ifstream : public fstream_base<istream>
    {
    public:
        typedef fstream_base<istream> super_type;
        
        ifstream(){}
        ifstream(const Char* filepath, int mode=ios::in)
            :super_type(filepath, mode|ios::in)
        {
        }
        
        bool open(const Char* filepath, int mode=ios::in)
        {
            return super_type::open(filepath, mode|ios::in);
        }
        
        virtual lsize_t read(Char* dst, u32 count){ return super_type::read(dst, count);}

        s32 get(){ return fgetc(file_);}
    private:
        ifstream(const ifstream&);
        ifstream& operator=(const ifstream&);
    };
    
    
    //----------------------------------------------------------
    //---
    //--- ofstream
    //---
    //----------------------------------------------------------
    class ofstream : public fstream_base<ostream>
    {
    public:
        typedef fstream_base<ostream> super_type;
    
        ofstream(){}
        ofstream(const Char* filepath, int mode=ios::out)
            :super_type(filepath, mode|ios::out)
        {
        }
        
        bool open(const Char* filepath, int mode=ios::out)
        {
            return super_type::open(filepath, mode|ios::out);
        }
        
        virtual lsize_t write(const Char* src, u32 count){ return super_type::write(src, count);}

        int print(const Char* format, ... );
    private:
        ofstream(const ofstream&);
        ofstream& operator=(const ofstream&);
    };
    
    
    
    //----------------------------------------------------------
    //---
    //--- sstream_base
    //---
    //----------------------------------------------------------
    template<class Base>
    class sstream_base : public Base
    {
    public:
        virtual bool eof();
        virtual bool good();
        
        virtual bool seekg(s32 offset, int dir);
        virtual s32 tellg();
        
    protected:
        sstream_base();
        sstream_base(Char* buffer, u32 size);
        ~sstream_base(){}
        
        virtual lsize_t read(Char* dst, u32 count);
        virtual lsize_t write(const Char* src, u32 count);

        void expand();
        
        Char* buffer_;
        s32 current_;
        s32 capacity_;
    };
    
    template<class Base>
    sstream_base<Base>::sstream_base()
        :buffer_(NULL)
        ,current_(0)
        ,capacity_(0)
    {
    }

    template<class Base>
    sstream_base<Base>::sstream_base(Char* buffer, u32 size)
        :buffer_(buffer)
        ,current_(0)
        ,capacity_(size)
    {
    }

    template<class Base>
    bool sstream_base<Base>::eof()
    {
        return (current_>=capacity_);
    }

    template<class Base>
    bool sstream_base<Base>::good()
    {
        if(buffer_ == NULL){
            return false;
        }
        return (current_<=capacity_);
    }

    template<class Base>
    bool sstream_base<Base>::seekg(s32 offset, int dir)
    {
        switch(dir)
        {
        case ios::beg:
            if(capacity_<=offset){
                current_ = capacity_;
            }else{
                current_ += offset;
            }
            break;
            
        case ios::cur:
            if(offset>=0){
                if((current_+offset)>=capacity_){
                    current_ = capacity_;
                    break;
                }
            }else{
                if((current_-offset)<0){
                    current_ = 0;
                    break;
                }
            }
            current_ += offset;
            break;
                        
        case ios::end:
            if(capacity_ <= offset){
                current_ = 0;
            }else{
                current_ = capacity_ - offset;
            }
            break;
            
        default:
            break;
        };
        
        return true;
    }

    template<class Base>
    s32 sstream_base<Base>::tellg()
    {
        return current_;
    }

    template<class Base>
    lsize_t sstream_base<Base>::read(Char* dst, u32 count)
    {
        s32 end = current_ + count;
        end = (end>capacity_)? capacity_ : end;
        count = end - current_;

        while(current_<end){
            *dst = buffer_[current_];
            ++dst;
            ++current_;
        }
        return count;
    }

    template<class Base>
    lsize_t sstream_base<Base>::write(const Char* src, u32 count)
    {
        s32 end = current_ + count;
        if(end > capacity_){
            expand();
        }
        
        while(current_<end){
            buffer_[current_] = *src;
            ++src;
            ++current_;
        }
        return count;
    }
    
    template<class Base>
    void sstream_base<Base>::expand()
    {
        u32 newSize = capacity_ << 1;
        //printf("size %d => %d\n", capacity_, newSize);
        
        Char* newBuffer = LIME_NEW Char[newSize];
        
        if(buffer_ != NULL){
            lcore::memcpy(newBuffer, buffer_, capacity_);
        }
        LIME_DELETE_ARRAY(buffer_);
        
        buffer_ = newBuffer;
        capacity_ = newSize;
    }

    //----------------------------------------------------------
    //---
    //--- sstream_base<istream>
    //---
    //----------------------------------------------------------
    template<>
    class sstream_base<istream> : public istream
    {
    public:
        virtual bool eof();
        virtual bool good();
        
        virtual bool seekg(s32 offset, int dir);
        virtual s32 tellg();
        
    protected:
        sstream_base();
        sstream_base(const Char* buffer, u32 size);
        ~sstream_base(){}
        
        virtual lsize_t read(Char* dst, u32 count);
        virtual lsize_t write(const Char* /*src*/, u32 /*count*/){return 0;}; //何もしない
        
        const Char* buffer_;
        s32 current_;
        s32 capacity_;
    };

    
    //----------------------------------------------------------
    //---
    //--- isstream
    //---
    //----------------------------------------------------------
    class isstream : public sstream_base<istream>
    {
    public:
        typedef sstream_base<istream> super_type;
        
        isstream(){}
        isstream(const Char* buffer, u32 size)
            :super_type(buffer, size)
        {
        }

        ~isstream()
        {
            buffer_ = NULL;
            current_ = 0;
            capacity_ = 0;
        }
        
        virtual lsize_t read(Char* dst, u32 count){ return super_type::read(dst, count);}
    private:
        isstream(const isstream&);
        isstream& operator=(const isstream&);
    };
    
    
    //----------------------------------------------------------
    //---
    //--- osstream
    //---
    //----------------------------------------------------------
    class osstream : public sstream_base<ostream>
    {
    public:
        typedef sstream_base<ostream> super_type;
    
        osstream(){}
        osstream(u32 size)
        {
            LASSERT(size>0);
            buffer_ = LIME_NEW Char[size];
            current_ = 0;
            capacity_ = size;
        }
        
        ~osstream()
        {
            LIME_DELETE_ARRAY(buffer_);
            current_ = 0;
            capacity_ = 0;
        }
        
        virtual lsize_t write(const Char* src, u32 count){ return super_type::write(src, count);}
        
        const Char* c_str() const{ return buffer_;}
    private:
        osstream(const osstream&);
        osstream& operator=(const osstream&);
    };
    
namespace io
{
    template<class T>
    inline lsize_t write(lcore::ostream& of, const T& value)
    {
        return of.write(reinterpret_cast<const Char*>(&value), sizeof(T));
    }

    template<class T>
    inline lsize_t write(lcore::ostream& of, const T* value, u32 size)
    {
        return of.write(reinterpret_cast<const Char*>(value), size);
    }

    template<class T>
    inline lsize_t read(lcore::istream& in, T& value)
    {
        return in.read(reinterpret_cast<Char*>(&value), sizeof(T));
    }

    template<class T>
    inline lsize_t read(lcore::istream& in, T* value, u32 size)
    {
        return in.read(reinterpret_cast<Char*>(value), size);
    }
}
}

#endif //INC_LCORE_LIOSTREAM_
