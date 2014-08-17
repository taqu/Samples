#ifndef INC_LCORE_UTILITY_H__
#define INC_LCORE_UTILITY_H__
/**
@file utility.h
@author t-sakai
@date 2011/02/05 create
*/
#include "lcore.h"
#include <limits>

namespace lcore
{
    /**
    @brief 後方から文字探索
    @return 見つからなければNULL
    @param src ... 入力
    @param c ... 探索文字
    @param size ... 文字列長
    */
    const Char* rFindChr(const Char* src, Char c, u32 size);


    /**
    @brief パスからディレクトリパス抽出
    @return dstの長さ。ヌル含まず
    @param dst ... 出力バッファ。ヌル込みで十分なサイズがあること
    @param path ... 解析パス
    @param length ... 解析パスの長さ。ヌル含まず
    */
    u32 extractDirectoryPath(Char* dst, const Char* path, u32 length);


    template<class T>
    class ScopedPtr
    {
    public:
        ScopedPtr(T* pointer)
            :pointer_(pointer)
        {
        }

        ~ScopedPtr()
        {
            LIME_DELETE(pointer_);
        }

        T* get()
        {
            return pointer_;
        }

        T* release()
        {
            T* tmp = pointer_;
            pointer_ = NULL;
            return tmp;
        }

        T* operator->()
        {
            LASSERT(pointer_ != NULL);
            return pointer_;
        }

        T& operator*() const
        {
            LASSERT(pointer_ != NULL);
            return *pointer_;
        }

        operator bool() const
        {
            return pointer_ != NULL;
        }

        bool operator!() const
        {
            return pointer_ == NULL;
        }
    private:
        // コピー禁止
        explicit ScopedPtr(const ScopedPtr&);
        ScopedPtr& operator=(const ScopedPtr&);

        T *pointer_;
    };

    template<class T>
    class ScopedArrayPtr
    {
    public:
        ScopedArrayPtr(T* pointer)
            :pointer_(pointer)
        {
        }

        ~ScopedArrayPtr()
        {
            LIME_DELETE_ARRAY(pointer_);
        }

        T* get()
        {
            return pointer_;
        }

        T* release()
        {
            T* tmp = pointer_;
            pointer_ = NULL;
            return tmp;
        }

        T& operator[](int index)
        {
            LASSERT(pointer_ != NULL);
            return pointer_[index];
        }

        const T& operator[](int index) const
        {
            LASSERT(pointer_ != NULL);
            return pointer_[index];
        }

        operator bool() const
        {
            return pointer_ != NULL;
        }

        bool operator!() const
        {
            return pointer_ == NULL;
        }
    private:
        // コピー禁止
        explicit ScopedArrayPtr(const ScopedArrayPtr&);
        ScopedArrayPtr& operator=(const ScopedArrayPtr&);

        T *pointer_;
    };

    //---------------------------------------------------------
    //---
    //--- タイム関係
    //---
    //---------------------------------------------------------

    /// カウント取得
    ClockType getPerformanceCounter();

    /// 秒間カウント数
    ClockType getPerformanceFrequency();

    /// 秒単位の時間差分計算
    f64 calcTime64(ClockType prevTime, ClockType currentTime);

    inline f32 calcTime(ClockType prevTime, ClockType currentTime)
    {
        return static_cast<f32>(calcTime64(prevTime, currentTime));
    }

    /// ミリ秒単位の時間を取得
    u32 getTime();


    template<bool enable>
    struct Timer
    {
        Timer()
            :time_(0)
            ,count_(0)
            ,totalTime_(0.0f)
        {}

        void begin()
        {
            time_ = getPerformanceCounter();
        }

        void end()
        {
            totalTime_ += calcTime64(time_, getPerformanceCounter());
            ++count_;
        }

        f64 getAverage() const
        {
            return (0 == count_)? 0.0 : totalTime_/count_;
        }

        ClockType time_;
        s32 count_;
        f64 totalTime_;
    };

    template<>
    struct Timer<false>
    {
        void begin(){}
        void end(){}
        f64 getAverage() const{return 0.0;}
    };

    //---------------------------------------------------------
    //---
    //--- Utility
    //---
    //---------------------------------------------------------

    template<class Itr>
    struct iterator_traits
    {
        typedef typename Itr::iterator_category iterator_category;
        typedef typename Itr::value_type value_type;
        typedef typename Itr::difference_type difference_type;
        typedef typename Itr::difference_type distance_type;
        typedef typename Itr::pointer pointer;
        typedef typename Itr::reference reference;
    };

    template<class T>
    struct iterator_traits<T*>
    {
        //typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef ptrdiff_t distance_type;	// retained
        typedef T *pointer;
        typedef T& reference;
    };

    template<class FwdIt, class T>
    inline FwdIt lower_bound(FwdIt first, FwdIt last, const T& val)
    {
        typename iterator_traits<FwdIt>::difference_type count = last - first;
        while(0<count){
            typename iterator_traits<FwdIt>::difference_type d = count/2;
            FwdIt m = first + d;
            if(*m<val){
                first = ++m;
                count -= d+1;
            }else{
                count = d;
            }
        }
        return first;
    }


    template<class FwdIt, class T>
    inline FwdIt upper_bound(FwdIt first, FwdIt last, const T& val)
    {
        typename iterator_traits<FwdIt>::difference_type count = last - first;
        while(0<count){
            typename iterator_traits<FwdIt>::difference_type d = count/2;
            FwdIt m = first + d;
            if(*m<=val){
                first = ++m;
                count -= d+1;
            }else{
                count = d;
            }
        }
        return first;
    }
}
#endif //INC_LCORE_UTILITY_H__
