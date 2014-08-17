#ifndef INC_LINPUT_H__
#define INC_LINPUT_H__
/**
@file linput.h
@author t-sakai
@date 2009/05/13 create
*/
#include "linputcore.h"

struct HWND__;

namespace linput
{
    class Device;
    class DeviceBase;
    class Keyboard;
    class Mouse;
    class Joystick;

    /**
    Inputマネージャ
    */
    class Input
    {
    public:
        enum Error
        {
            Error_None = 0,
            Error_Init,
        };

        struct InitParam
        {
            InitParam();

            HWND__* hWnd_;
            bool initDevices_[DevType_Num];
        };

        /**
        */
        static Error initialize(InitParam& param);
        static void terminate();

        static Input& getInstance();

        void update();
        void clear();

        const Keyboard* getKeyboard() const{ return reinterpret_cast<const Keyboard*>( devices_[DevType_Keyboard] );}
        const Mouse* getMouse() const{ return reinterpret_cast<const Mouse*>( devices_[DevType_Mouse] );}
        const Joystick* getJoystick() const{ return reinterpret_cast<const Joystick*>( devices_[DevType_Joystick] );}

    private:
        Input();
        ~Input();

        Input(const Input&);
        Input& operator=(const Input&);

        InputImpl impl_;

        DeviceBase *devices_[DevType_Num];
    };
}

#endif //INC_LINPUT_H__
