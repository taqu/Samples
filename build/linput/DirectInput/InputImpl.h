#ifndef INC_LINPUT_DINPUT_INPUTIMPL_H__
#define INC_LINPUT_DINPUT_INPUTIMPL_H__
/**
@file InputImpl.h
@author t-sakai
@date 2010/07/22 create

*/
struct _GUID;
struct HWND__;
struct IDirectInput8;
struct IDirectInputDevice8;

namespace linput
{
    class InputImpl
    {
    public:
        InputImpl();
        ~InputImpl();

        inline IDirectInput8* get();
        inline bool valid() const;
        bool initialize(HWND__* hWnd);
        void terminate();

        IDirectInputDevice8 *createDevice(const _GUID& deviceID);
    private:
        // コピー禁止
        InputImpl(const InputImpl&);
        InputImpl& operator=(const InputImpl&);

        IDirectInput8* dinput_;
    };

    inline IDirectInput8* InputImpl::get()
    {
        return dinput_;
    }

    inline bool InputImpl::valid() const
    {
        return (dinput_ != NULL);
    }
}
#endif //INC_LINPUT_DINPUT_INPUTIMPL_H__
