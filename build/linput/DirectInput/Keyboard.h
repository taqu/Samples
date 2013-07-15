#ifndef INC_LINPUT_DINPUT_KEYBOARD_H__
#define INC_LINPUT_DINPUT_KEYBOARD_H__
/**
@file Keyboard.h
@author t-sakai
@date 2009/05/13 create
*/
#include "../linputEnum.h"
#include "../DeviceBase.h"
#include <lcore/BitSet.h>

namespace linput
{
    class Keyboard: public DeviceBase
    {
    public:
        static const u32 BufferSize = 8;
        static const u32 StateSizeInBytes = 256/8;
        typedef lcore::BitSet<StateSizeInBytes> StateSet;

        Keyboard();
        ~Keyboard();

        bool initialize(HWND__* hWnd, IDirectInputDevice8* device);
        void poll();
        void clear();

        inline bool isOn(KeyCode key) const;
        inline bool isClick(KeyCode key) const;
    private:
        inline bool isOldOn(KeyCode key) const;
        void acquire();

        StateSet onState_;
        StateSet clickState_;
        DIDEVICEOBJECTDATA devObjectData_[BufferSize];
    };

    inline bool Keyboard::isOn(KeyCode key) const
    {
        return onState_.check(key);
    }

    inline bool Keyboard::isClick(KeyCode key) const
    {
        return clickState_.check(key);
    }
}

#endif //INC_LINPUT_DINPUT_KEYBOARD_H__
