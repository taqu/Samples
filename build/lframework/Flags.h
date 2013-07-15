#ifndef INC_LFRAMEWORK_FLAGS_H__
#define INC_LFRAMEWORK_FLAGS_H__
/**
@file Flags.h
@author t-sakai
@date 2010/12/15 create

*/
#include <lcore/utility.h>
#include "lframework.h"

namespace lframework
{
    class Flags
    {
    public:
        inline Flags();
        explicit inline Flags(u32 flags);

        inline ~Flags();

        inline u32 get() const;
        inline void set(u32 flags);

        inline void setFlag(u32 flag);
        inline void resetFlag(u32 flag);
        inline bool checkFlag(u32 flag) const;

        inline void swap(Flags& rhs);
    private:
        u32 flags_;
    };

    inline Flags::Flags()
        :flags_(0)
    {
    }

    inline Flags::Flags(u32 flags)
        :flags_(flags)
    {
    }

    inline Flags::~Flags()
    {
    }

    inline u32 Flags::get() const
    {
        return flags_;
    }

    inline void Flags::set(u32 flags)
    {
        flags_ = flags;
    }

    inline void Flags::setFlag(u32 flag)
    {
        flags_ |= flag;
    }

    inline void Flags::resetFlag(u32 flag)
    {
        flags_ &= ~flag;
    }

    inline bool Flags::checkFlag(u32 flag) const
    {
        return ((flags_ & flag) != 0);
    }

    inline void Flags::swap(Flags& rhs)
    {
        lcore::swap(flags_, rhs.flags_);
    }
}
#endif //INC_LFRAMEWORK_FLAGS_H__
