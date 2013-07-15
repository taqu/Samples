#ifndef INC_LINPUTENUM_H__
#define INC_LINPUTENUM_H__
/**
@file linputEnum.h
@author t-sakai
@date 2009/05/13 create
*/
#include "linputcore.h"

namespace linput
{
    enum KeyCode
    {
        Key_ESCAPE = DIK_ESCAPE,
        Key_1 = DIK_1,
        Key_2 = DIK_2,
        Key_3 = DIK_3,
        Key_4 = DIK_4,
        Key_5 = DIK_5,
        Key_6 = DIK_6,
        Key_7 = DIK_7,
        Key_8 = DIK_8,
        Key_9 = DIK_9,
        Key_0 = DIK_0,
        Key_MINUS = DIK_MINUS,    /* - on main keyboard */
        Key_EQUALS = DIK_EQUALS,
        Key_BACK = DIK_BACK,    /* backspace */
        Key_TAB = DIK_TAB,
        Key_Q = DIK_Q,
        Key_W = DIK_W,
        Key_E = DIK_E,
        Key_R = DIK_R,
        Key_T = DIK_T,
        Key_Y = DIK_Y,
        Key_U = DIK_U,
        Key_I = DIK_I,
        Key_O = DIK_O,
        Key_P = DIK_P,
        Key_LBRACKET = DIK_LBRACKET,
        Key_RBRACKET = DIK_RBRACKET,
        Key_RETURN = DIK_RETURN,    /* Enter on main keyboard */
        Key_LCONTROL = DIK_LCONTROL,
        Key_A = DIK_A,
        Key_S = DIK_S,
        Key_D = DIK_D,
        Key_F = DIK_F,
        Key_G = DIK_G,
        Key_H = DIK_H,
        Key_J = DIK_J,
        Key_K = DIK_K,
        Key_L = DIK_L,
        Key_SEMICOLON = DIK_SEMICOLON,
        Key_APOSTROPHE = DIK_APOSTROPHE,
        Key_GRAVE = DIK_GRAVE,    /* accent grave */
        Key_LSHIFT = DIK_LSHIFT,
        Key_BACKSLASH = DIK_BACKSLASH,
        Key_Z = DIK_Z,
        Key_X = DIK_X,
        Key_C = DIK_C,
        Key_V = DIK_V,
        Key_B = DIK_B,
        Key_N = DIK_N,
        Key_M = DIK_M,
        Key_COMMA = DIK_COMMA,
        Key_PERIOD = DIK_PERIOD,    /* . on main keyboard */
        Key_SLASH = DIK_SLASH,    /* / on main keyboard */
        Key_RSHIFT = DIK_RSHIFT,
        Key_MULTIPLY = DIK_MULTIPLY,    /* * on numeric keypad */
        Key_LMENU = DIK_LMENU,    /* left Alt */
        Key_SPACE = DIK_SPACE,
        Key_CAPITAL = DIK_CAPITAL,
        Key_F1 = DIK_F1,
        Key_F2 = DIK_F2,
        Key_F3 = DIK_F3,
        Key_F4 = DIK_F4,
        Key_F5 = DIK_F5,
        Key_F6 = DIK_F6,
        Key_F7 = DIK_F7,
        Key_F8 = DIK_F8,
        Key_F9 = DIK_F9,
        Key_F10 = DIK_F10,
        Key_NUMLOCK = DIK_NUMLOCK,
        Key_SCROLL = DIK_SCROLL,    /* Scroll Lock */
        Key_NUMPAD7 = DIK_NUMPAD7,
        Key_NUMPAD8 = DIK_NUMPAD8,
        Key_NUMPAD9 = DIK_NUMPAD9,
        Key_SUBTRACT = DIK_SUBTRACT,    /* - on numeric keypad */
        Key_NUMPAD4 = DIK_NUMPAD4,
        Key_NUMPAD5 = DIK_NUMPAD5,
        Key_NUMPAD6 = DIK_NUMPAD6,
        Key_ADD = DIK_ADD,    /* + on numeric keypad */
        Key_NUMPAD1 = DIK_NUMPAD1,
        Key_NUMPAD2 = DIK_NUMPAD2,
        Key_NUMPAD3 = DIK_NUMPAD3,
        Key_NUMPAD0 = DIK_NUMPAD0,
        Key_DECIMAL = DIK_DECIMAL,    /* . on numeric keypad */
        Key_OEM_102 = DIK_OEM_102,    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
        Key_F11 = DIK_F11,
        Key_F12 = DIK_F12,
        Key_F13 = DIK_F13,    /*                     (NEC PC98) */
        Key_F14 = DIK_F14,    /*                     (NEC PC98) */
        Key_F15 = DIK_F15,    /*                     (NEC PC98) */
        Key_KANA = DIK_KANA,    /* (Japanese keyboard)            */
        Key_ABNT_C1 = DIK_ABNT_C1, /* /? on Brazilian keyboard */
        Key_CONVERT = DIK_CONVERT,    /* (Japanese keyboard)            */
        Key_NOCONVERT = DIK_NOCONVERT,    /* (Japanese keyboard)            */
        Key_YEN = DIK_YEN,    /* (Japanese keyboard)            */
        Key_ABNT_C2 = DIK_ABNT_C2,    /* Numpad . on Brazilian keyboard */
        Key_NUMPADEQUALS = DIK_NUMPADEQUALS,    /* = on numeric keypad (NEC PC98) */
        Key_PREVTRACK = DIK_PREVTRACK,    /* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */
        Key_AT = DIK_AT,    /*                     (NEC PC98) */
        Key_COLON = DIK_COLON,    /*                     (NEC PC98) */
        Key_UNDERLINE = DIK_UNDERLINE,    /*                     (NEC PC98) */
        Key_KANJI = DIK_KANJI,    /* (Japanese keyboard)            */
        Key_STOP = DIK_STOP,    /*                     (NEC PC98) */
        Key_AX = DIK_AX,    /*                     (Japan AX) */
        Key_UNLABELED = DIK_UNLABELED,    /*                        (J3100) */
        Key_NEXTTRACK = DIK_NEXTTRACK,    /* Next Track */
        Key_NUMPADENTER = DIK_NUMPADENTER,    /* Enter on numeric keypad */
        Key_RCONTROL = DIK_RCONTROL,
        Key_MUTE = DIK_MUTE,    /* Mute */
        Key_CALCULATOR = DIK_CALCULATOR,    /* Calculator */
        Key_PLAYPAUSE = DIK_PLAYPAUSE,    /* Play / Pause */
        Key_MEDIASTOP = DIK_MEDIASTOP,    /* Media Stop */
        Key_VOLUMEDOWN = DIK_VOLUMEDOWN,    /* Volume - */
        Key_VOLUMEUP = DIK_VOLUMEUP,    /* Volume + */
        Key_WEBHOME = DIK_WEBHOME,    /* Web home */
        Key_NUMPADCOMMA = DIK_NUMPADCOMMA,    /* , on numeric keypad (NEC PC98) */
        Key_DIVIDE = DIK_DIVIDE,    /* / on numeric keypad */
        Key_SYSRQ = DIK_SYSRQ,
        Key_RMENU = DIK_RMENU,    /* right Alt */
        Key_PAUSE = DIK_PAUSE,    /* Pause */
        Key_HOME = DIK_HOME,    /* Home on arrow keypad */
        Key_UP = DIK_UP,    /* UpArrow on arrow keypad */
        Key_PRIOR = DIK_PRIOR,    /* PgUp on arrow keypad */
        Key_LEFT = DIK_LEFT,    /* LeftArrow on arrow keypad */
        Key_RIGHT = DIK_RIGHT,    /* RightArrow on arrow keypad */
        Key_END = DIK_END,    /* End on arrow keypad */
        Key_DOWN = DIK_DOWN,    /* DownArrow on arrow keypad */
        Key_NEXT = DIK_NEXT,    /* PgDn on arrow keypad */
        Key_INSERT = DIK_INSERT,    /* Insert on arrow keypad */
        Key_DELETE = DIK_DELETE,    /* Delete on arrow keypad */
        Key_LWIN = DIK_LWIN,    /* Left Windows key */
        Key_RWIN = DIK_RWIN,    /* Right Windows key */
        Key_APPS = DIK_APPS,    /* AppMenu key */
        Key_POWER = DIK_POWER,    /* System Power */
        Key_SLEEP = DIK_SLEEP,    /* System Sleep */
        Key_WAKE = DIK_WAKE,    /* System Wake */
        Key_WEBSEARCH = DIK_WEBSEARCH,    /* Web Search */
        Key_WEBFAVORITES = DIK_WEBFAVORITES,    /* Web Favorites */
        Key_WEBREFRESH = DIK_WEBREFRESH,    /* Web Refresh */
        Key_WEBSTOP = DIK_WEBSTOP,    /* Web Stop */
        Key_WEBFORWARD = DIK_WEBFORWARD,    /* Web Forward */
        Key_WEBBACK = DIK_WEBBACK,    /* Web Back */
        Key_MYCOMPUTER = DIK_MYCOMPUTER,    /* My Computer */
        Key_MAIL = DIK_MAIL,    /* Mail */
        Key_MEDIASELECT = DIK_MEDIASELECT,    /* Media Select */

        /*
        *  Alternate names for keys, to facilitate transition from DOS.
        */
        Key_BACKSPACE = Key_BACK,            /* backspace */
        Key_NUMPADSTAR = Key_MULTIPLY,        /* * on numeric keypad */
        Key_LALT = Key_LMENU,           /* left Alt */
        Key_CAPSLOCK = Key_CAPITAL,         /* CapsLock */
        Key_NUMPADMINUS = Key_SUBTRACT,        /* - on numeric keypad */
        Key_NUMPADPLUS = Key_ADD,             /* + on numeric keypad */
        Key_NUMPADPERIOD = Key_DECIMAL,         /* . on numeric keypad */
        Key_NUMPADSLASH = Key_DIVIDE,          /* / on numeric keypad */
        Key_RALT = Key_RMENU,           /* right Alt */
        Key_UPARROW = Key_UP,              /* UpArrow on arrow keypad */
        Key_PGUP = Key_PRIOR,           /* PgUp on arrow keypad */
        Key_LEFTARROW = Key_LEFT,            /* LeftArrow on arrow keypad */
        Key_RIGHTARROW = Key_RIGHT,           /* RightArrow on arrow keypad */
        Key_DOWNARROW = Key_DOWN,            /* DownArrow on arrow keypad */
        Key_PGDN = Key_NEXT,            /* PgDn on arrow keypad */
    };


    enum MouseButton
    {
        MouseButton_0 = 0,
        MouseButton_1,
        MouseButton_2,
        MouseButton_3,
        //MouseButton_4,
        //MouseButton_5,
        //MouseButton_6,
        //MouseButton_7,
        MouseButton_Num,
    };

    enum MouseAxis
    {
        MouseAxis_X =0,
        MouseAxis_Y,
        MouseAxis_Z,
        MouseAxis_Num,
    };

    enum JoystickButton
    {
        JoystickButton_0 = 0,
        JoystickButton_1,
        JoystickButton_2,
        JoystickButton_3,
        JoystickButton_4,
        JoystickButton_5,
        JoystickButton_Num,
    };

    enum JoystickAxis
    {
        JoystickAxis_X = 0,
        JoystickAxis_Y,
        JoystickAxis_Num,
    };
}

#endif //INC_LINPUTENUM_H__
