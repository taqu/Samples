#ifndef INC_LMATH_KDTREE_H__
#define INC_LMATH_KDTREE_H__
/**
@file KDTree.h
@author t-sakai
@date 2009/12/23 create
*/
#include "../lmathcore.h"
#include "../Vector2.h"
#include "../Vector3.h"
#include "RayTest.h"
#include <lcore/Vector.h>
#include "Ray.h"

namespace lmath
{
    namespace kdtree
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

        //------------------------------------------------------------------------
        //---
        //--- Bounding Interval Hierarchy
        //---
        //------------------------------------------------------------------------

        struct BKDNode
        {
            static const u32 MAX_KDNODES = 0xFFFFFFFFU >> 2;
            enum Axis
            {
                Axis_X = 0x00U,
                Axis_Y = 0x01U,
                Axis_Z = 0x02U,
                Axis_Leaf = 0x03U,
            };

            Axis getFlag() const{ return static_cast<Axis>(index_ & 0x03U);}
            void setFlag(Axis axis)
            {
                LASSERT(0<=axis && axis<=Axis_Leaf);
                index_ = (index_ & ~(0x03U)) | axis;
            }

            u32 getChildIndex() const{ return index_>>2;}
            void setChildIndex(u32 index)
            {
                LASSERT(index<=MAX_KDNODES);
                index = index << 2;
                index_ = index | (index_ & 0x03U);
            }

            u32 index_;

            struct Leaf
            {
                u32 index_;
                u32 numItems_;
            };

            union
            {
                Leaf items_;
                f32 clip_[2];
            };
        };


        //--------------------------------------------------------------
        //---
        //--- BKDAlgorithm
        //---
        //--------------------------------------------------------------
        template<class T>
        struct BKDAlgorithm
        {
            static s32 qsort(T* entries, u32 numEntries, f32 split, s32 axis);

            static s32 split(T* entries, u32 numEntries, u32& numLeft, f32& splitLeft, f32& splitRight, const Vector3& bboxMin, const Vector3& bboxMax, s32 axis);

            static s32 findSplitAxis(const Vector3& bboxMin, const Vector3& bboxMax);

            static f32 findSplit(const T* entries, u32 numEntries, s32 axis);

            //-------------------------------------------------------------------------
            static void calcBBox(Vector3& bmin, Vector3& bmax, const T* entries, u32 numEntries);
            //-------------------------------------------------------------------------
            static void calcBBox(f32& bmin, f32& bmax, const T* entries, u32 numEntries, s32 axis);

            //-------------------------------------------------------------------------
            static f32 calcBBoxMin(const T* entries, u32 numEntries, s32 axis);

            //-------------------------------------------------------------------------
            static f32 calcBBoxMax(const T* entries, u32 numEntries, s32 axis);

            //-------------------------------------------------------------------------
            static void calcMedian(Vector3& median, const T* entries, u32 numEntries);

            //-------------------------------------------------------------------------
            static f32 calcMedian(const T* entries, u32 numEntries, s32 axis);

        };

        //
        template<class T>
        s32 BKDAlgorithm<T>::qsort(T* entries, u32 numEntries, f32 split, s32 axis)
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
                std::swap(entries[begin], entries[end]);
                ++begin;
                --end;
            }

            if(begin>=num){
                return num;
            }
            return begin;
        }


        template<class T>
        s32 BKDAlgorithm<T>::split(T* entries, u32 numEntries, u32& numLeft, f32& splitLeft, f32& splitRight, const Vector3& bboxMin, const Vector3& bboxMax, s32 axis)
        {
            s32 nextAxis = (axis+1)%3; //BKDAlgorithm<T>::findSplitAxis(bboxMin, bboxMax);
            f32 splitVal = BKDAlgorithm<T>::findSplit(entries, numEntries, nextAxis);
            s32 right = BKDAlgorithm<T>::qsort(entries, numEntries, splitVal, nextAxis);
            numLeft = right;

            //片側の要素数が0なら失敗。次を試す
            //if((numLeft<=0 || numLeft>=numEntries)){
            //    return -1;
            //}
            while((numLeft<=0 || numLeft>=numEntries)){

                nextAxis = (nextAxis + 1)%3;
                if(nextAxis == axis){
                    return -1;
                }

                splitVal = BKDAlgorithm<T>::findSplit(entries, numEntries, nextAxis);
                right = BKDAlgorithm<T>::qsort(entries, numEntries, splitVal, nextAxis);
                numLeft = right;
            }

#if 0
            for(s32 i=0; i<numLeft; ++i){
                if((entries[i].getMedian())[nextAxis] > splitVal){
                    LASSERT(false);
                }
            }

            for(s32 i=numLeft; i<numEntries-numLeft; ++i){
                if((entries[i].getMedian())[nextAxis] < splitVal){
                    LASSERT(false);
                }
            }
#endif
            f32 lbmax = calcBBoxMax(entries, numLeft, nextAxis); //左部分木要素群の境界ボックス計算
            splitLeft = lbmax;

            entries += numLeft;
            f32 rbmin = calcBBoxMin(entries, numEntries - numLeft, nextAxis); //右部分木要素群の境界ボックス計算
            splitRight = rbmin;

            return nextAxis;
        }


        template<class T>
        s32 BKDAlgorithm<T>::findSplitAxis(const Vector3& bboxMin, const Vector3& bboxMax)
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

        template<class T>
        f32 BKDAlgorithm<T>::findSplit(const T* entries, u32 numEntries, s32 axis)
        {
            LASSERT(entries!=NULL);
            f32 median = calcMedian(entries, numEntries, axis);
            return median;
        }

        //-------------------------------------------------------------------------
        template<class T>
        void BKDAlgorithm<T>::calcBBox(Vector3& bmin, Vector3& bmax, const T* entries, u32 numEntries)
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
        void BKDAlgorithm<T>::calcBBox(f32& bmin, f32& bmax, const T* entries, u32 numEntries, s32 axis)
        {
            LASSERT(entries!=NULL);
            f32 bmin = std::numeric_limits<f32>::max();
            f32 bmax = -std::numeric_limits<f32>::max();

            for(u32 i=0; i<numEntries; ++i){
                const Vector3& tmin = entries[i].getBBoxMin();
                const Vector3& tmax = entries[i].getBBoxMax();

                if(bmax < tmax[axis]){
                    bmax = tmax[axis];
                }
                if(bmin > tmin[axis]){
                    bmin = tmin[axis];
                }
            }
        }

        //-------------------------------------------------------------------------
        template<class T>
        f32 BKDAlgorithm<T>::calcBBoxMin(const T* entries, u32 numEntries, s32 axis)
        {
            LASSERT(entries!=NULL);
            f32 bmin = std::numeric_limits<f32>::max();

            for(u32 i=0; i<numEntries; ++i){
                const Vector3& tmin = entries[i].getBBoxMin();

                if(bmin > tmin[axis]){
                    bmin = tmin[axis];
                }
            }
            return bmin;
        }

        //-------------------------------------------------------------------------
        template<class T>
        f32 BKDAlgorithm<T>::calcBBoxMax(const T* entries, u32 numEntries, s32 axis)
        {
            LASSERT(entries!=NULL);
            f32 bmax = -std::numeric_limits<f32>::max();

            for(u32 i=0; i<numEntries; ++i){
                const Vector3& tmax = entries[i].getBBoxMax();

                if(bmax < tmax[axis]){
                    bmax = tmax[axis];
                }
            }
            return bmax;
        }

        //-------------------------------------------------------------------------
        template<class T>
        void BKDAlgorithm<T>::calcMedian(Vector3& median, const T* entries, u32 numEntries)
        {
            LASSERT(entries!=NULL);
            LASSERT(numEntries>0);

            median.set(0.0f, 0.0f, 0.0f);
            f32 invSize = 1.0f / numEntries;

            for(u32 i=0; i<numEntries_; ++i){
                const Vector3 &tmedian = entries[i].getMedian();
                median += tmedian;
            }

            median *= invSize;
        }



        //-------------------------------------------------------------------------
        template<class T>
        f32 BKDAlgorithm<T>::calcMedian(const T* entries, u32 numEntries, s32 axis)
        {
            LASSERT(entries!=NULL);
            LASSERT(numEntries>0);

            f32 median = 0.0f;
            f32 invSize = 1.0f / numEntries;

            for(u32 i=0; i<numEntries; ++i){
                const Vector3 &tmedian = entries[i].getMedian();
                median += tmedian[axis];
            }

            median *= invSize;
            return median;
        }



        //--------------------------------------------------------------
        //---
        //--- BKD
        //---
        //--------------------------------------------------------------
#define LMATH_KDTREE_SUMMARIZE_INFO

        /**
        分割平面の選択をSAHにするとBKD
        */
        template<class T, class THitRecord, class TAlgorithm = BKDAlgorithm<T> >
        class BKD
        {
        public:
            typedef BKDNode node_type;
            typedef TAlgorithm BKDAlgo;
            typedef lcore::simple_vector_arena<BKDNode> NodeVector;
            typedef THitRecord hit_record_type;

            static const f64 EPSILON;

            BKD()
                :endNode_(0)
                ,numEntries_(0)
                ,entries_(NULL)
                ,hit_(false)

#if defined(LMATH_KDTREE_SUMMARIZE_INFO)
                ,totalLeafs_(0)
                ,totalLeafItems_(0)
                ,maxDepth_(0)
                ,hitInfoIndex_(0)
                ,hitTestCount_(0)
#endif
            {}

            ~BKD(){}

            bool hit(hit_record_type& hitRecord, const Ray& ray);

            void build(T* entries, u32 numEntries);
        private:
            template<class T, class THitRecord> friend class DebugOut;

            bool hitInternal(s32 current, const Ray& ray, const Vector3& bmin, const Vector3& bmax);

            void buildInternal(s32 nodeIndex, T* entries, u32 numEntries, u32 entryIndex, u32 depth, const Vector3& bboxMin, const Vector3& bboxMax, s32 currentAxis);

            bool intersectAxis(f64& tmin, f64& tmax, const Ray& ray, const Vector3& bmin, const Vector3& bmax, s32 axis);

            bool hitLeft(s32 current, const Ray& ray, Vector3 bmin, Vector3 bmax, s32 axis)
            {
                BKDNode &node = nodes_[current];
                bmax[axis] = node.clip_[0];
                current = node.getChildIndex();
                return hitInternal(current, ray, bmin, bmax);
            }

            bool hitRight(s32 current, const Ray& ray, Vector3 bmin, Vector3 bmax, s32 axis)
            {
                BKDNode &node = nodes_[current];
                bmin[axis] = node.clip_[1];
                current = node.getChildIndex() + 1;
                return hitInternal(current, ray, bmin, bmax);
            }
            
            void calcBBox(Vector3& bmin, Vector3& bmax)
            {
                for(s32 i=0; i<3; ++i){
                    if(bmin[i] > bmax[i]){
                        lmath::swap(bmin[i], bmax[i]);
                    }
                }
            }

            Vector3 bboxMin_;
            Vector3 bboxMax_;

            u32 endNode_;
            NodeVector nodes_;

            BKDAlgo algorithm_;

            bool hit_;
            hit_record_type hitRecord_;

            u32 numEntries_;
            const T* entries_;

#if defined(LMATH_KDTREE_SUMMARIZE_INFO)
            u32 totalLeafs_;
            f32 totalLeafItems_;
            u32 maxDepth_;

        public:
            s32 getNumHitInfo() const{ return hitInfoIndex_;}
            const hit_record_type getHitInfo(s32 index) const
            {
                LASSERT(0<=index && index<hitInfoIndex_);
                return hitInfo_[index];
            }

            static const s32 MAX_RECORD_HIT = 256;
            s32 hitTestCount_;
            s32 hitInfoIndex_;
            hit_record_type hitInfo_[MAX_RECORD_HIT];
        private:
            void addHitInfo(const hit_record_type& record)
            {
                if(hitInfoIndex_ < MAX_RECORD_HIT){
                    hitInfo_[hitInfoIndex_] = record;
                    ++hitInfoIndex_;
                }
            }
#endif
        };

        template<class T, class THitRecord, class TAlgorithm>
        const f64 BKD<T, THitRecord, TAlgorithm>::EPSILON = 1.0e-5f;

        template<class T, class THitRecord, class TAlgorithm>
        bool BKD<T, THitRecord, TAlgorithm>::intersectAxis(f64& tmin, f64& tmax, const Ray& ray, const Vector3& bmin, const Vector3& bmax, s32 axis)
        {
            if(isEqual(ray.direction_[axis], 0.0f)){
                return true;
            }

            f64 invD = 1.0/ray.direction_[axis];
            f64 t1 = (bmin[axis] - ray.origin_[axis]) * invD;
            f64 t2 = (bmax[axis] - ray.origin_[axis]) * invD;

            if(t1>t2){
                if(t2>tmin) tmin = t2;
                if(t1<tmax) tmax = t1;
            }else{
                if(t1>tmin) tmin = t1;
                if(t2<tmax) tmax = t2;
            }

            if(tmin>tmax){
                return false;
            }
            return (tmax>=0.0);
        }

        template<class T, class THitRecord, class TAlgorithm>
        bool BKD<T, THitRecord, TAlgorithm>::hit(hit_record_type& hitRecord, const Ray& ray)
        {

#if defined(LMATH_KDTREE_SUMMARIZE_INFO)
            hitInfoIndex_ = 0;
            hitTestCount_ = 0;
#endif

            if(numEntries_<=0){
                return false;
            }

            hitRecord_.t_ = ray.t_;
            hitRecord_.shape_ = NULL;
            hit_ = false;

            f64 tmin, tmax;
            bool hitTop = lmath::testRayAABB(tmin, tmax, ray, bboxMin_, bboxMax_);
            if(!hitTop){
                return false;
            }


            Vector3 bmin = ray.direction_;
            bmin *= tmin;
            bmin += ray.origin_;

            Vector3 bmax = ray.direction_;
            bmax *= tmax;
            bmax += ray.origin_;

            calcBBox(bmin, bmax);

            hit_ = hitInternal(0, ray, bmin, bmax);

            if(hit_){
                hitRecord = hitRecord_;
            }
            return hit_;
        }

        template<class T, class THitRecord, class TAlgorithm>
        bool BKD<T, THitRecord, TAlgorithm>::hitInternal(s32 current, const Ray& ray, const Vector3& bmin, const Vector3& bmax)
        {
            BKDNode &node = nodes_[current];
            s32 axis = node.getFlag();

            if(axis == BKDNode::Axis_Leaf){
                u32 index = node.items_.index_;
                u32 num = node.items_.numItems_;
                HitRecord hitRecord;
                bool tmpHit = false;
                for(u32 i=0; i<num; ++i){
                    const T &entry = entries_[index+i];
                    bool ret = entry.hit(hitRecord, ray);
                    if(ret){
                        if(hitRecord.t_ < hitRecord_.t_){
                            hitRecord_ = hitRecord;
                            tmpHit = true;
                            hit_ = true;
                        }
                    }
#if defined(LMATH_KDTREE_SUMMARIZE_INFO)
                    if(ret){
                        addHitInfo(hitRecord);
                    }
                    ++hitTestCount_;
#endif
                } // for(u32 i=0; i<num; ++i){
                return tmpHit;
            }

            f32 clip0 = EPSILON + node.clip_[0];
            f32 clip1 = - EPSILON + node.clip_[1];

            if(clip0 < bmin[axis] && clip0 < bmax[axis]){
                //左のスラブの外側
                if(bmin[axis]<clip1 && bmax[axis]<clip1){
                    //右のスラブの外側
                    return false;

                }else{
                    //右だけと交差
                    return hitRight(current, ray, bmin, bmax, axis);
                }

            }else if(bmin[axis]<clip1 && bmax[axis]<clip1){
                //左だけと交差
                return hitLeft(current, ray, bmin, bmax, axis);

            }else if(0.0f<=ray.direction_[axis]){
            //}else if(bmin[axis]<bmax[axis]){

                //両方と交差、左が先
                bool tmpHit = hitLeft(current, ray, bmin, bmax, axis);

                if(tmpHit && (hitRecord_.position_[axis] < node.clip_[1])){
                    //右のスラブと交差しない
                    return true;
                }else{
                    return hitRight(current, ray, bmin, bmax, axis) || tmpHit;
                }

            }else{
                //両方と交差、右が先
                bool tmpHit = hitRight(current, ray, bmin, bmax, axis);
                if(tmpHit && (hitRecord_.position_[axis] > node.clip_[0])){
                    //左のスラブと交差しない
                    return true;
                }else{
                    return hitLeft(current, ray, bmin, bmax, axis) || tmpHit;
                }
            }
        }


        template<class T, class THitRecord, class TAlgorithm>
        void BKD<T, THitRecord, TAlgorithm>::build(T* entries, u32 numEntries)
        {
#if defined(LMATH_KDTREE_SUMMARIZE_INFO)
            totalLeafs_ = 0;
            totalLeafItems_ = 0;
            maxDepth_ = 0;
#endif

            entries_ = entries;
            numEntries_ = numEntries;

            endNode_ = 1;
            nodes_.resize(numEntries * 2);

            BKDAlgo::calcBBox(bboxMin_, bboxMax_, entries, numEntries);

            buildInternal(0, entries, numEntries, 0, 0, bboxMin_, bboxMax_, 0);

        }


        template<class T, class THitRecord, class TAlgorithm>
        void BKD<T, THitRecord, TAlgorithm>::buildInternal(s32 nodeIndex, T* entries, u32 numEntries, u32 entryIndex, u32 depth, const Vector3& bboxMin, const Vector3& bboxMax, s32 currentAxis)
        {
            LASSERT(entries != NULL);
            if(numEntries <= 1){
                nodes_[nodeIndex].setFlag(BKDNode::Axis_Leaf);
                nodes_[nodeIndex].items_.index_ = entryIndex;
                nodes_[nodeIndex].items_.numItems_ = numEntries;

#if defined(LMATH_KDTREE_SUMMARIZE_INFO)
                ++totalLeafs_;
                totalLeafItems_ += numEntries;
                maxDepth_ = (maxDepth_ > depth)? maxDepth_ : depth;
#endif
                return;
            }

            u32 numLeft = 0;
            f32 splitLeft, splitRight;
            s32 axis = BKDAlgo::split(entries, numEntries, numLeft, splitLeft, splitRight, bboxMin, bboxMax, currentAxis);
            if(axis<0){
                nodes_[nodeIndex].setFlag(BKDNode::Axis_Leaf);
                nodes_[nodeIndex].items_.index_ = entryIndex;
                nodes_[nodeIndex].items_.numItems_ = numEntries;

#if defined(LMATH_KDTREE_SUMMARIZE_INFO)
                ++totalLeafs_;
                totalLeafItems_ += numEntries;
                maxDepth_ = (maxDepth_ > depth)? maxDepth_ : depth;
#endif
                return;
            }

            nodes_[nodeIndex].setFlag(static_cast<BKDNode::Axis>(axis));
            nodes_[nodeIndex].setChildIndex(endNode_);
            nodes_[nodeIndex].clip_[0] = splitLeft;//bboxMin[axis];
            nodes_[nodeIndex].clip_[1] = splitRight;//bboxMax[axis];

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
            plane = bboxMax;
            plane[axis] = splitLeft;
            buildInternal(nodeL, entryL, numLeft, indexL, depth+1, bboxMin, plane, axis);

            //Right
            plane = bboxMin;
            plane[axis] = splitRight;
            buildInternal(nodeR, entryR, numRight, indexR, depth+1, plane, bboxMax, axis);

        }

    }
}

#endif //INC_LMATH_KDTREE_H__
