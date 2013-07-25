#ifndef INC_FRACTAL_INPUT_H__
#define INC_FRACTAL_INPUT_H__
/**
@file Input.h
@author t-sakai
@date 2011/06/12 create
*/
struct HWND__;

namespace linput
{
    class Keyboard;
    class Mouse;
    class Joystick;
}

namespace fractal
{
    class Input
    {
    public:
        enum Key
        {
            Key_W,
            Key_S,
            Key_A,
            Key_D,

            Key_Tab,

            Key_R,
            Key_C,
            Key_Return,
        };

        enum MouseAxis
        {
            MouseAxis_X,
            MouseAxis_Y,
            MouseAxis_Num,
        };

        enum MouseButton
        {
            MouseButton_Left,
            MouseButton_Right,
        };

        enum JoystickAxis
        {
            JoystickAxis_X,
            JoystickAxis_Y,
            JoystickAxis_Num,
        };

        enum JoystickButton
        {
            JoystickButton_0,
            JoystickButton_1,
            JoystickButton_2,
            JoystickButton_3,

            JoystickButton_4,
            JoystickButton_5,
        };

        class Keyboard
        {
        public:
            Keyboard(const linput::Keyboard* keyboard)
                :keyboard_(keyboard)
            {}

            bool isOn(Input::Key key) const;
            bool isClick(Input::Key key) const;
        private:
            const linput::Keyboard* keyboard_;
        };

        class Mouse
        {
        public:
            Mouse(const linput::Mouse* mouse)
                :mouse_(mouse)
            {}

            s32 getAxisDuration(MouseAxis axis) const;
            bool isClick(MouseButton button) const;
            bool isOn(MouseButton button) const;
        private:
            const linput::Mouse* mouse_;
        };

        class Joystick
        {
        public:
            Joystick(const linput::Joystick* joystick)
                :joystick_(joystick)
            {}

            bool valid() const{ return (NULL != joystick_);}

            s32 getAxisPosition(JoystickAxis axis) const;
            bool isClick(JoystickButton button) const;
            bool isOn(JoystickButton button) const;
        private:
            const linput::Joystick* joystick_;
        };

        static void initialize(HWND__* hWnd);
        static void terminate();

        static void update();

        static Keyboard getKeyboard();
        static Mouse getMouse();
        static Joystick getJoystick();
    };
}

#endif //INC_FRACTAL_INPUT_H__
