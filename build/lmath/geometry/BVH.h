#ifndef INC_LMATH_BVH_H__
#define INC_LMATH_BVH_H__
/**
@file BVH.h
@author t-sakai
@date 2010/02/07 create
*/
#include "../lmathcore.h"
#include "../Vector3.h"
#include "Ray.h"

namespace lmath
{
namespace bvh
{
    template<class T>
    struct EntryTemplate
    {
        //----------------------------------
        const Vector3& getBBoxMin() const;
        const Vector3& getBBoxMax() const;

        //----------------------------------
        const Vector3& getMedian() const;
    };


    //--------------------------------------------------------------
    //---
    //--- BVHNode
    //---
    //--------------------------------------------------------------
#if 1
    class BVHNode
    {
    public:
        static const u32 MAX_BVHNODES = 0xFFFFFFFFU >> 2;
        enum Axis
        {
            Axis_Node = 0x00U,
            Axis_Leaf = 0x01U,
        };

        Axis getFlag() const{ return static_cast<Axis>(index_ & 0x03U);}

        void setFlag(Axis axis)
        {
            LASSERT(0<=axis && axis<=Axis_Leaf);
            index_ = (index_ & ~(0x03U)) | axis;
        }

        /// 左部分木インデックス。右はchildIndex_+1
        u32 getLeftChildIndex() const{ return index_>>2;}

        void setChildIndex(u32 index)
        {
            LASSERT(index<=MAX_BVHNODES);
            index = index << 2;
            index_ = index | (index_ & 0x03U);
        }

        u32 getNumItems() const{ return items_.numItems_;}
        u32 getItemIndex() const{ return items_.index_;}


        struct Leaf
        {
            u32 index_;
            u32 numItems_; /// 要素数
        };

        /// 要素インデックス
        u32 index_;

        Leaf items_;

        Vector3 bboxMin_;
        Vector3 bboxMax_;
    };
#else
    template<class T>
    class BVHNode
    {
    public:
        typedef std::list<T> EntryList;

        struct Heuristic
        {
            u32 maxDepth_;
            u32 minEntries_;
        };

        BVHNode(EntryList& entries, u32 depth, const Heuristic& heuristic);
        ~BVHNode();

        u32 getNumEntires() const{ return numEntries_;}
        const T* getEntries() const{ return entries_;}

        BVHNode* getLeftChild() const{ return childIndex_;}
        BVHNode* getRightChild() const{ return childIndex_ + 1;}

        void calcBBox(Vector3& bmin, Vector3& bmax, const EntryList& entries);
        void calcMedian(Vector3& median, const EntryList& entries);

        s32 maxAxis(const Vector3& bmin, const Vector3& bmax);

        Vector3 bboxMin_;
        Vector3 bboxMax_;

        EntryList entries_;

        BVHNode *children_[2];
    };

    template<class T>
    BVHNode<T>::BVHNode(EntryList& entries, u32 depth, const Heuristic& heuristic)
    {
        calcBBox(bboxMin_, bboxMax_, entries);

        if(depth>=heuristic.maxDepth_
            || entries.size() <= heuristic.minEntries_)
        {
            entries_ = entries;

            children_[0] = NULL;
            children_[1] = NULL;
            return;
        }

        Vector3 median;
        calcMedian(median, entries);

        s32 axis = maxAxis(bboxMin_, bboxMax_);

        EntryList childEntries[2];
        for(EntryList::const_iterator itr = entries.begin();
            itr != entries.end();
            ++itr)
        {
            childEntries[ median[axis] > (itr->getMedian())[axis] ].push_back(*itr);
        }

        children_[0] = LIME_NEW BVHNode(childEntries[0], depth+1, heuristic);
        children_[1] = LIME_NEW BVHNode(childEntries[1], depth+1, heuristic);
    }

    template<class T>
    BVHNode<T>::~BVHNode()
    {
        LIME_DELETE(children_[0]);
        LIME_DELETE(children_[1]);
    }

    template<class T>
    void BVHNode<T>::calcMedian(Vector3& median, const EntryList& entries)
    {
        median.set(0.0f, 0.0f, 0.0f);
        f32 invSize = 1.0f/entries.size();

        for(EntryList::const_iterator itr = entries.begin();
            itr != entries.end();
            ++itr)
        {
            median += (*itr).getMedian() * invSize;
        }

    }

    //-------------------------------------------------------------------------
    template<class T>
    void BVHNode<T>::calcBBox(Vector3& bmin, Vector3& bmax, const EntryList& entries)
    {
        const f32 maxval = std::numeric_limits<f32>::max();
        const f32 minval = -std::numeric_limits<f32>::max();

        bmax.set(minval, minval, minval);
        bmin.set(maxval, maxval, maxval);

        for(EntryList::const_iterator itr = entries.begin();
            itr != entries.end();
            ++itr)
        {
            const Vector3& tmin = (*itr).getBBoxMin();
            const Vector3& tmax = (*itr).getBBoxMax();

            for(int j=0; j<3; ++j){
                if(bmax[j] < tmax[j]){
                    bmax[j] = tmax[j];
                }
                if(bmin[j] > tmin[j]){
                    bmin[j] = tmin[j];
                }
            }
        }
    }

    //-------------------------------------------------------------------------
    template<class T>
    s32 BVHNode<T>::maxAxis(const Vector3& bmin, const Vector3& bmax)
    {
        Vector3 size = bmax;
        size -= bmin;

        s32 axis = 0;
        for(s32 i=1; i<3; ++i){
            if(size[i] > size[axis]){
                axis = i;
            }
        }
        return axis;
    }
#endif

    //--------------------------------------------------------------
    //---
    //--- BVH Algorithm
    //---
    //--------------------------------------------------------------
    template<class T>
    struct BVHAlgorithm
    {
        //-------------------------------------------------------------------------
        static s32 qsort(T* entries, u32 numEntries, f32 split, s32 axis);

        //-------------------------------------------------------------------------
        static s32 split(T* entries, u32 numEntries, u32& numLeft, const Vector3& bboxMin, const Vector3& bboxMax);

        //-------------------------------------------------------------------------
        static s32 findSplitAxis(const Vector3& bboxMin, const Vector3& bboxMax);

        //-------------------------------------------------------------------------
        static f32 findSplit(const T* entries, u32 numEntries, s32 axis);

        //-------------------------------------------------------------------------
        static void calcBBox(Vector3& bmin, Vector3& bmax, const T* entries, u32 numEntries);

        //-------------------------------------------------------------------------
        static f32 calcMedian(const T* entries, u32 numEntries, s32 axis);
    };


    //-------------------------------------------------------------------------
    template<class T>
    s32 BVHAlgorithm<T>::qsort(T* entries, u32 numEntries, f32 split, s32 axis)
    {
        LASSERT(entries != NULL);
        LASSERT(numEntries > 0);

        s32 begin = 0;
        s32 num = static_cast<s32>(numEntries);
        s32 end = num - 1;

        for(;;){
            f32 value = (entries[begin].getMedian())[axis];
            while(value < split){
                ++begin;
                value = (entries[begin].getMedian())[axis];
            }

            value = (entries[end].getMedian())[axis];
            while(value > split){
                --end;
                value = (entries[end].getMedian())[axis];
            }
            if(begin>=end){
                break;
            }
            lcore::swap(entries[begin], entries[end]);
            ++begin;
            --end;
        }

        if(begin>=num){
            return num;
        }
        return begin;
    }

    //-------------------------------------------------------------------------
    template<class T>
    s32 BVHAlgorithm<T>::split(T* entries, u32 numEntries, u32& numLeft, const Vector3& bboxMin, const Vector3& bboxMax)
    {
        s32 nextAxis = BVHAlgorithm<T>::findSplitAxis(bboxMin, bboxMax);
        f32 splitVal = BVHAlgorithm<T>::findSplit(entries, numEntries, nextAxis);
        s32 right = BVHAlgorithm<T>::qsort(entries, numEntries, splitVal, nextAxis);
        numLeft = right;

        //片側の要素数が0なら失敗。
        if((numLeft<=0 || numLeft>=numEntries)){
            return -1;
        }

        return nextAxis;
    }


    //-------------------------------------------------------------------------
    template<class T>
    s32 BVHAlgorithm<T>::findSplitAxis(const Vector3& bboxMin, const Vector3& bboxMax)
    {
        Vector3 bboxSize = bboxMax;
        bboxSize -= bboxMin;

        s32 maxIndex = 0;
        for(s32 i=1; i<3; ++i){
            if(bboxSize[maxIndex] < bboxSize[i]){
                maxIndex = i;
            }
        }
        return maxIndex;
    }

    //-------------------------------------------------------------------------
    template<class T>
    f32 BVHAlgorithm<T>::findSplit(const T* entries, u32 numEntries, s32 axis)
    {
        LASSERT(entries!=NULL);
        f32 median = calcMedian(entries, numEntries, axis);
        return median;
    }

    //-------------------------------------------------------------------------
    template<class T>
    void BVHAlgorithm<T>::calcBBox(Vector3& bmin, Vector3& bmax, const T* entries, u32 numEntries)
    {
        LASSERT(entries!=NULL);
        const f32 maxval = std::numeric_limits<f32>::max();
        const f32 minval = -std::numeric_limits<f32>::max();

        bmax.set(minval, minval, minval);
        bmin.set(maxval, maxval, maxval);

        for(u32 i=0; i<numEntries; ++i){
            const Vector3& tmin = entries[i].getBBoxMin();
            const Vector3& tmax = entries[i].getBBoxMax();

            for(int j=0; j<3; ++j){
                if(bmax[j] < tmax[j]){
                    bmax[j] = tmax[j];
                }
                if(bmin[j] > tmin[j]){
                    bmin[j] = tmin[j];
                }
            }
        }
    }


    //-------------------------------------------------------------------------
    template<class T>
    f32 BVHAlgorithm<T>::calcMedian(const T* entries, u32 numEntries, s32 axis)
    {
        LASSERT(entries!=NULL);
        LASSERT(numEntries>0);

        f32 median = 0.0f;
        f32 invSize = 1.0f / numEntries;

        for(u32 i=0; i<numEntries; ++i){
            const Vector3 &tmedian = entries[i].getMedian();
            median += tmedian[axis];// * invSize;
        }

        return median * invSize;
    }



    //--------------------------------------------------------------
    //---
    //--- BVH
    //---
    //--------------------------------------------------------------
#define LMATH_BVH_SUMMARIZE_INFO

    template<class T, class THitRecord, class TAlgorithm = BVHAlgorithm<T> >
    class BVH
    {
    public:
        typedef BVHNode node_type;
        typedef TAlgorithm BVHAlgo;
        typedef lcore::simple_vector_arena<BVHNode> NodeVector;
        typedef THitRecord hit_record_type;

        BVH()
            :numEntries_(0)
            ,entries_(NULL)
            ,hit_(false)

#if defined(LMATH_BVH_SUMMARIZE_INFO)
            ,totalLeafs_(0)
            ,totalLeafItems_(0)
            ,maxDepth_(0)
            ,hitTestCount_(0)
#endif
        {
        }

        ~BVH()
        {
        }

        bool hit(hit_record_type& hitRecord, const Ray& ray);

        void build(T* entries, u32 numEntries);
    private:
        template<class T, class THitRecord> friend class DebugOut;

        void hitInternal(s32 current, const Ray& ray);

        void buildInternal(s32 nodeIndex, T* entries, u32 numEntries, u32 entryIndex, u32 depth);

        u32 endNode_;
        NodeVector nodes_;

        u32 numEntries_;
        const T* entries_;

        bool hit_;
        hit_record_type hitRecord_;

#if defined(LMATH_BVH_SUMMARIZE_INFO)
    public:
        u32 totalLeafs_;
        f32 totalLeafItems_;
        u32 maxDepth_;
        s32 hitTestCount_;
#endif
    };

    //--------------------------------------------------------
    template<class T, class THitRecord, class TAlgorithm>
    bool BVH<T, THitRecord, TAlgorithm>::hit(hit_record_type& hitRecord, const Ray& ray)
    {

#if defined(LMATH_BVH_SUMMARIZE_INFO)
        hitTestCount_ = 0;
#endif

        if(numEntries_<=0){
            return false;
        }

        hitRecord_.t_ = ray.t_;
        hitRecord_.shape_ = NULL;
        hit_ = false;

        hitInternal(0, ray);

        if(hit_){
            hitRecord = hitRecord_;
        }
        return hit_;
    }


    //--------------------------------------------------------
    template<class T, class THitRecord, class TAlgorithm>
    void BVH<T, THitRecord, TAlgorithm>::hitInternal(s32 current, const Ray& ray)
    {
        BVHNode &node = nodes_[current];

        f32 tmin, tmax;
        bool hitAABB = lmath::testRayAABB(tmin, tmax, ray, node.bboxMin_, node.bboxMax_);
        if(hitAABB == false){
            return;
        }

        s32 axis = node.getFlag();

        if(axis == BVHNode::Axis_Leaf){
            u32 index = node.items_.index_;
            u32 num = node.items_.numItems_;
            HitRecord hitRecord;
            for(u32 i=0; i<num; ++i){
                const T &entry = entries_[index+i];
                bool ret = entry.hit(hitRecord, ray);
                if(ret){
                    if(hitRecord.t_ < hitRecord_.t_){
                        hitRecord_ = hitRecord;
                        hit_ = true;
                    }
                }
#if defined(LMATH_BVH_SUMMARIZE_INFO)
                ++hitTestCount_;
#endif
            } // for(u32 i=0; i<num; ++i){
        }else{
            u32 left = node.getLeftChildIndex();
            u32 right = left+1;

            hitInternal(left, ray);
            hitInternal(right, ray);
        }
    }


    //--------------------------------------------------------
    template<class T, class THitRecord, class TAlgorithm>
    void BVH<T, THitRecord, TAlgorithm>::build(T* entries, u32 numEntries)
    {
#if defined(LMATH_BVH_SUMMARIZE_INFO)
        totalLeafs_ = 0;
        totalLeafItems_ = 0;
        maxDepth_ = 0;
#endif

        entries_ = entries;
        numEntries_ = numEntries;

        endNode_ = 1;

        nodes_.resize(numEntries_ * 2);

        buildInternal(0, entries, numEntries_, 0, 0);
    }

    //--------------------------------------------------------
    template<class T, class THitRecord, class TAlgorithm>
    void BVH<T, THitRecord, TAlgorithm>::buildInternal(s32 nodeIndex, T* entries, u32 numEntries, u32 entryIndex, u32 depth)
    {
        Vector3 &bboxMin = nodes_[nodeIndex].bboxMin_;
        Vector3 &bboxMax= nodes_[nodeIndex].bboxMax_;

        BVHAlgo::calcBBox(bboxMin, bboxMax, entries, numEntries);

        if(numEntries<=1){
            nodes_[nodeIndex].setFlag(BVHNode::Axis_Leaf);
            nodes_[nodeIndex].items_.index_ = entryIndex;
            nodes_[nodeIndex].items_.numItems_ = numEntries;

#if defined(LMATH_BVH_SUMMARIZE_INFO)
            ++totalLeafs_;
            totalLeafItems_ += numEntries;
            maxDepth_ = (maxDepth_ > depth)? maxDepth_ : depth;
#endif
            return;
        }


        u32 numLeft = 0;

        s32 axis = BVHAlgo::split(entries, numEntries, numLeft, bboxMin, bboxMax);
        if(axis<0){
            nodes_[nodeIndex].setFlag(BVHNode::Axis_Leaf);
            nodes_[nodeIndex].items_.index_ = entryIndex;
            nodes_[nodeIndex].items_.numItems_ = numEntries;

#if defined(LMATH_BVH_SUMMARIZE_INFO)
            ++totalLeafs_;
            totalLeafItems_ += numEntries;
            maxDepth_ = (maxDepth_ > depth)? maxDepth_ : depth;
#endif
            return;
        }

        nodes_[nodeIndex].setFlag(BVHNode::Axis_Node);
        nodes_[nodeIndex].setChildIndex(endNode_);

        s32 indexL = entryIndex;
        T* entryL = entries;
        s32 nodeL = endNode_;

        s32 indexR = indexL + numLeft;
        T* entryR = entries + numLeft;
        s32 nodeR = endNode_ + 1;
        s32 numRight = numEntries - numLeft;

        endNode_ += 2;

        Vector3 plane;

        //Left
        buildInternal(nodeL, entryL, numLeft, indexL, depth+1);

        //Right
        buildInternal(nodeR, entryR, numRight, indexR, depth+1);
    }
}
}
#endif //INC_LMATH_BVH_H__
