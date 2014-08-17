#ifndef INC_LCORE_LDLL_H__
#define INC_LCORE_LDLL_H__
/**
@file ldll.h
@author t-sakai
@date 2010/12/03 create

*/
#include "lcore.h"

namespace lcore
{
    class DLL
    {
    public:
        DLL();
        DLL(const char* path);
        ~DLL();

        bool open(const char* path);
        bool is_open() const;

        void close();

        void* getProcAddress(const char* name);

        void swap(DLL& rhs)
        {
            lcore::swap(module_, rhs.module_);
        }

    private:
        DLL(const DLL&);
        DLL& operator=(const DLL&);

        LHMODULE module_;
    };

    LHMODULE openDLL(const char* path);
    void closeDLL(LHMODULE module);
    void* getProcAddressDLL(LHMODULE module, const char* name);
}
#endif //INC_LCORE_LDLL_H__
