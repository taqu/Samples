/**
@file Input.cpp
@author t-sakai
@date 2011/06/12
*/
#include "stdafx.h"
#include "Input.h"

#include <linput/linput.h>

#include <linput/Keyboard.h>
#include <linput/Mouse.h>
#include <linput/Joystick.h>

namespace fractal
{
    namespace
    {
        static linput::KeyCode KeyMap[] =
        {
            linput::Key_W,
            linput::Key_S,
            linput::Key_A,
            linput::Key_D,

            linput::Key_TAB,

            linput::Key_R,

            linput::Key_RETURN,
        };

        static linput::MouseButton MouseMap[] =
        {
            linput::MouseButton_0,
            linput::MouseButton_1,
        };
    }

    void Input::initialize(HWND__* hWnd)
    {
        LASSERT(hWnd != NULL);

        linput::Input::InitParam initParam;
        initParam.hWnd_ = hWnd;

        initParam.initDevices_[linput::DevType_Keyboard] = true;
        initParam.initDevices_[linput::DevType_Mouse] = true;
        initParam.initDevices_[linput::DevType_Joystick] = true;


        linput::Input::initialize(initParam);

    }

    void Input::terminate()
    {
        linput::Input::terminate();
    }

    void Input::update()
    {
        linput::Input::getInstance().update();
    }


    // キーボード
    //--------------------------------------------------
    Input::Keyboard Input::getKeyboard()
    {
        const linput::Keyboard* keyboard = linput::Input::getInstance().getKeyboard();
        return Keyboard(keyboard);
    }

    bool Input::Keyboard::isOn(Input::Key key) const
    {
        return keyboard_->isOn(KeyMap[key]);
    }

    bool Input::Keyboard::isClick(Input::Key key) const
    {
        return keyboard_->isClick(KeyMap[key]);
    }

    // マウス
    //--------------------------------------------------
    Input::Mouse Input::getMouse()
    {
        const linput::Mouse* mouse = linput::Input::getInstance().getMouse();
        return Mouse(mouse);
    }

    s32 Input::Mouse::getAxisDuration(MouseAxis axis) const
    {
        static linput::MouseAxis toMouse[MouseAxis_Num] =
        {
            linput::MouseAxis_X,
            linput::MouseAxis_Y,
        };
        return mouse_->getDuration( toMouse[axis] );
    }

    bool Input::Mouse::isClick(MouseButton button) const
    {
        return mouse_->isClick(MouseMap[button]);
    }

    bool Input::Mouse::isOn(MouseButton button) const
    {
        return mouse_->isOn(MouseMap[button]);
    }

    // ジョイスティック
    //--------------------------------------------------
    Input::Joystick Input::getJoystick()
    {
        const linput::Joystick* joystick = linput::Input::getInstance().getJoystick();
        return Joystick(joystick);
    }

    s32 Input::Joystick::getAxisPosition(JoystickAxis axis) const
    {
        return joystick_->get((linput::JoystickAxis)axis);
    }

    bool Input::Joystick::isClick(JoystickButton button) const
    {
        return joystick_->isClick((linput::JoystickButton)button);
    }

    bool Input::Joystick::isOn(JoystickButton button) const
    {
        return joystick_->isOn((linput::JoystickButton)button);
    }

}
