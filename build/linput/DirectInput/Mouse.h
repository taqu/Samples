#ifndef INC_LINPUT_DINPUT_MOUSE_H__
#define INC_LINPUT_DINPUT_MOUSE_H__
/**
@file Mouse.h
@author t-sakai
@date 2009/09/08 create
*/
#include "../linputEnum.h"
#include "../DeviceBase.h"
#include "Device.h"

namespace linput
{
    class Mouse : public DeviceBase
    {
    public:
        static const u32 BufferSize = 8;

        Mouse();
        ~Mouse();

        bool initialize(HWND__* hWnd, IDirectInputDevice8* device);
        void poll();
        void clear();

        inline bool isOn(MouseButton button) const;
        inline bool isClick(MouseButton button) const;

        inline s32 getDuration(MouseAxis axis) const;

        inline s32 getX() const;
        inline s32 getY() const;
        inline s32 get(MouseAxis axis) const;

    private:
        void acquire();
        inline void getMousePoint();
        inline void setOn(u32 index);
        inline void resetOn(u32 index);
        inline void setClick(u32 index);

        LONG axisDuration_[MouseAxis_Num];

        struct MousePoint
        {
            s32 value_[MouseAxis_Num];
        };

        HWND__* hWnd_;
        MousePoint mousePoint_;
        DIDEVICEOBJECTDATA devObjectData_[BufferSize];
        u16 onState_;
        u16 clickState_;
    };

    inline bool Mouse::isOn(MouseButton button) const
    {
        LASSERT(MouseButton_0<= button && button < MouseButton_Num);
        return (onState_ & (0x01U<<button)) != 0;
    }

    inline bool Mouse::isClick(MouseButton button) const
    {
        LASSERT(MouseButton_0<= button && button < MouseButton_Num);
        return (clickState_ & (0x01U<<button)) != 0;
    }

    inline s32 Mouse::getDuration(MouseAxis axis) const
    {
        LASSERT(0<= axis && axis < MouseAxis_Num);
        return axisDuration_[axis];
    }

    inline s32 Mouse::getX() const
    {
        return mousePoint_.value_[MouseAxis_X];
    }

    inline s32 Mouse::getY() const
    {
        return mousePoint_.value_[MouseAxis_Y];
    }

    inline s32 Mouse::get(MouseAxis axis) const
    {
        LASSERT(0<=axis && axis<MouseAxis_Num);
        return mousePoint_.value_[axis];
    }
}

#endif //INC_LINPUT_DINPUT_MOUSE_H__
