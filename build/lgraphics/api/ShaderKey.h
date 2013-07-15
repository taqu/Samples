#ifndef INC_LGRAPHICS_SHADERKEY_H__
#define INC_LGRAPHICS_SHADERKEY_H__
/**
@file ShaderKey.h
@author t-sakai
@date 2010/04/26 create

*/
#include "../lgraphicscore.h"

namespace lgraphics
{
    /// シェーダ識別子
    class ShaderKey
    {
    public:
        inline ShaderKey();

        inline ShaderKey(u32 shaderID, u32 inputFlags, u32 featureFlags);

        inline ShaderKey(const ShaderKey&);

        inline ~ShaderKey();

        inline u32 getShaderID() const;
        inline void setShaderID(u32 id);

        inline u32 getInputFlags() const;
        inline void setInputFlags(u32 flags);

        inline void setInputFlag(u32 flag);
        inline void resetInputFlag(u32 flag);
        inline bool checkInputFlag(u32 flag) const;

        inline u32 getFeatureFlags() const;
        inline void setFeatureFlags(u32 flags);

        inline void setFeatureFlag(u32 flag);
        inline void resetFeatureFlag(u32 flag);
        inline bool checkFeatureFlag(u32 flag) const;

        inline ShaderKey& operator=(const ShaderKey& rhs);

        inline bool operator==(const ShaderKey& rhs) const;
        inline bool operator!=(const ShaderKey& rhs) const;
    private:
        u32 shaderID_;
        u32 inputFlags_;   //入力フラグ
        u32 featureFlags_; //フラグ
    };

    //-------------------------------------------------
    inline ShaderKey::ShaderKey()
        :shaderID_(0)
        ,inputFlags_(0)
        ,featureFlags_(0)
    {
    }

    inline ShaderKey::ShaderKey(u32 shaderID, u32 inputFlags, u32 featureFlags)
        :shaderID_(shaderID)
        ,inputFlags_(inputFlags)
        ,featureFlags_(featureFlags)
    {
    }

    inline ShaderKey::ShaderKey(const ShaderKey& rhs)
        :shaderID_(rhs.shaderID_)
        ,inputFlags_(rhs.inputFlags_)
        ,featureFlags_(rhs.featureFlags_)
    {
    }

    inline ShaderKey::~ShaderKey()
    {
    }

    inline u32 ShaderKey::getShaderID() const
    {
        return shaderID_;
    }

    inline void ShaderKey::setShaderID(u32 id)
    {
        shaderID_ = id;
    }

    inline u32 ShaderKey::getInputFlags() const
    {
        return inputFlags_;
    }

    inline void ShaderKey::setInputFlags(u32 flags)
    {
        inputFlags_ = flags;
    }

    inline void ShaderKey::setInputFlag(u32 flag)
    {
        inputFlags_ |= flag;
    }

    inline void ShaderKey::resetInputFlag(u32 flag)
    {
        inputFlags_ &= ~flag;
    }

    inline bool ShaderKey::checkInputFlag(u32 flag) const
    {
        return ((inputFlags_ & flag) != 0);
    }

    inline u32 ShaderKey::getFeatureFlags() const
    {
        return featureFlags_;
    }

    inline void ShaderKey::setFeatureFlags(u32 flags)
    {
        featureFlags_ = flags;
    }

    inline void ShaderKey::setFeatureFlag(u32 flag)
    {
        featureFlags_ |= flag;
    }

    inline void ShaderKey::resetFeatureFlag(u32 flag)
    {
        featureFlags_ &= ~flag;
    }

    inline bool ShaderKey::checkFeatureFlag(u32 flag) const
    {
        return ((featureFlags_ & flag) != 0);
    }

    inline ShaderKey& ShaderKey::operator=(const ShaderKey& rhs)
    {
        shaderID_ = rhs.shaderID_;
        inputFlags_ = rhs.inputFlags_;
        featureFlags_ = rhs.featureFlags_;
        return *this;
    }

    inline bool ShaderKey::operator==(const ShaderKey& rhs) const
    {
        return (shaderID_ == rhs.shaderID_) && (inputFlags_ == rhs.inputFlags_) && (featureFlags_ == rhs.featureFlags_);
    }

    inline bool ShaderKey::operator!=(const ShaderKey& rhs) const
    {
        return (shaderID_ != rhs.shaderID_) || (inputFlags_ != rhs.inputFlags_) || (featureFlags_ != rhs.featureFlags_);
    }
}

#endif //INC_LGRAPHICS_SHADERKEY_H__
