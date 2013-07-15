#ifndef INC_XML_WRITER_H__
#define INC_XML_WRITER_H__
/**
@file Writer.h
@author t-sakai
@date 2011/08/18 create
*/
#include "Core.h"
#include <stdio.h>

namespace xml
{
    class Node;

    class Writer
    {
    public:
        Writer()
            :file_(NULL)
        {}

        bool write(const Node* root, const Char* filepath);

    private:
        void writeNode(const Node* node);

        FILE* file_;
    };
}
#endif //INC_XML_WRITER_H__
