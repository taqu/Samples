#ifndef INC_LCORE_BITSET_H__
#define INC_LCORE_BITSET_H__
/**
@file BitSet.h
@author t-sakai
@date 2010/04/09 create

*/
#include "lcore.h"

namespace lcore
{
    //----------------------------------------------------
    /**
    @brief ビットフラグセット
    */
    template<u32 SizeInByte>
    class BitSet
    {
    public:
        typedef BitSet<SizeInByte> this_type;

        BitSet()
        {
            clear();
        }

        ~BitSet()
        {
        }

        void clear()
        {
            for(u32 i=0; i<SizeInByte; ++i){
                bits_[i] = 0;
            }
        }

        /**
        @brief フラグチェック
        @param flag
        */
        inline bool check(u32 index) const
        {
            u8 bit = index & 0x07U;
            index = index >> 3;
            return ((bits_[index] & (0x01U<<bit)) != 0);
        }

        /**
        @brief フラグ立てる
        */
        inline void set(u32 index)
        {
            u8 bit = index & 0x07U;
            index = index >> 3;
            bits_[index] |= (0x01U<<bit);
        }

        /**
        @brief フラグ
        */
        inline void set(u32 index, bool flag)
        {
            if(flag){
                set(index);
            }else{
                reset(index);
            }
        }

        /**
        @brief フラグ降ろす
        */
        inline void reset(u32 index)
        {
            u8 bit = index & 0x07U;
            index = index >> 3;
            bits_[index] &= ~(0x01U<<bit);
        }

        inline void set(const this_type& rhs)
        {
            for(u32 i=0; i<SizeInByte; ++i){
                bits_[i] = rhs.bits_[i];
            }
        }

        inline u8 getByte(u32 index) const{ return bits_[index];}
    private:
        u8 bits_[SizeInByte];
    };

    //----------------------------------------------------
    /**
    @brief ビットフラグセット比較
    */
    template<u32 SizeInByte>
    inline bool compare(const BitSet<SizeInByte>& b0, const BitSet<SizeInByte>& b1)
    {
        for(u32 i=0; i<SizeInByte; ++i){
            if(b0.getByte(i) != b1.getByte(i)){
                return false;
            }
        }
        return true;
    }



    //----------------------------------------------------
    /**
    @brief ビットフラグセット
    */
    template<>
    class BitSet<4>
    {
    public:
        typedef BitSet<4> this_type;

        BitSet()
            :bits_(0)
        {
        }

        ~BitSet()
        {
        }

        void clear()
        {
            bits_ = 0;
        }

        /**
        @brief フラグチェック
        @param flag
        */
        inline bool check(u32 index) const
        {
            return ((bits_ & (0x01U<<index)) != 0);
        }

        /**
        @brief フラグ立てる
        */
        inline void set(u32 index)
        {
            bits_ |= (0x01U<<index);
        }

        /**
        @brief フラグ降ろす
        */
        inline void reset(u32 index)
        {
            bits_ &= ~(0x01U<<index);
        }

        inline void set(const this_type& rhs)
        {
            bits_ = rhs.bits_;
        }
    private:
        friend bool compare(const BitSet<4>& b0, const BitSet<4>& b1);

        u32 bits_;
    };

    //----------------------------------------------------
    /**
    @brief ビットフラグセット比較
    */
    inline bool compare(const BitSet<4>& b0, const BitSet<4>& b1)
    {
        return (b0.bits_ == b1.bits_);
    }


    typedef BitSet<4> BitSet32;
}

#endif //INC_LCORE_BITSET_H__
