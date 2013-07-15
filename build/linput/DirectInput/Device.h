#ifndef INC_LINPUT_DINPUT_DEVICE_H__
#define INC_LINPUT_DINPUT_DEVICE_H__
/**
@file Device.h
@author t-sakai
@date 2009/05/13 create
*/
#include <lcore/utility.h>

struct HWND__;
struct IDirectInputDevice8;

namespace linput
{
    class Device
    {
    public:
        Device();
        Device(IDirectInputDevice8* device);
        ~Device();

        inline bool valid() const;
        void terminate();
        inline void swap(Device& rhs);

        inline IDirectInputDevice8* get();
        inline IDirectInputDevice8* operator->();

        bool setCooperateLevel(HWND__* hWnd);
        bool setDataFormat(DeviceType type);

    private:
        Device(const Device&);
        Device& operator=(const Device&);

        IDirectInputDevice8 *device_;
    };

    inline bool Device::valid() const
    {
        return (device_ != NULL);
    }

    inline IDirectInputDevice8* Device::get()
    {
        return device_;
    }

    inline IDirectInputDevice8* Device::operator->()
    {
        LASSERT(device_ != NULL);
        return device_;
    }

    inline void Device::swap(Device& rhs)
    {
        lcore::swap(device_, rhs.device_);
    }
}

#endif //INC_LINPUT_DINPUT_DEVICE_H__
