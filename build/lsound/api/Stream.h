#ifndef INC_LSOUND_STREAM_H__
#define INC_LSOUND_STREAM_H__
/**
@file Stream.h
@author t-sakai
@date 2013/02/24 create
*/
#include "../Type.h"

namespace lsound
{
    class FLACFileDecoder;

    class Stream
    {
    public:
        enum State
        {
            State_Play,
            State_Pause,
            State_Stop,
        };

        Stream();
        ~Stream();

        bool valid();

        State getState();

        void play();
        void pause();
        void stop();

        void release();

        bool isLoop();
        void setLoop(bool loop);

        f32 getVolume();
        void setVolume(f32 volume);

        Stream& operator=(Stream& rhs);
    private:
        friend class SoundManager;

        explicit Stream(FLACFileDecoder* decoder);

        FLACFileDecoder* decoder_;
    };
}
#endif //INC_LSOUND_STREAM_H__
