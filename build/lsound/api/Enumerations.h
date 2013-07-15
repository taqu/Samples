#ifndef INC_LSOUND_ENUMERATIONS_H__
#define INC_LSOUND_ENUMERATIONS_H__
/**
@file Enumerations.h
@author t-sakai
@date 2012/08/01 create

*/
#include <mmsystem.h>

namespace lsound
{
    //enum Flag
    //{
    //    Flag_CTRL3D        = DSBCAPS_CTRL3D,
    //    Flag_CTRLFREQUENCY = DSBCAPS_CTRLFREQUENCY,
    //    Flag_CTRLFX        = DSBCAPS_CTRLFX,
    //    Flag_CTRLPAN       = DSBCAPS_CTRLPAN,
    //    Flag_CTRLVOLUME    = DSBCAPS_CTRLVOLUME,
    //    Flag_CTRLPOSITIONNOTIFY  = DSBCAPS_CTRLPOSITIONNOTIFY,
    //    Flag_GETCURRENTPOSITION2 = DSBCAPS_GETCURRENTPOSITION2,
    //    Flag_GLOBALFOCUS   = DSBCAPS_GLOBALFOCUS,
    //    Flag_LOCDEFER      = DSBCAPS_LOCDEFER,
    //    Flag_LOCHARDWARE   = DSBCAPS_LOCHARDWARE,
    //    Flag_LOCSOFTWARE   = DSBCAPS_LOCSOFTWARE,
    //    Flag_MUTE3DATMAXDISTANCE = DSBCAPS_MUTE3DATMAXDISTANCE,
    //    Flag_PRIMARYBUFFER = DSBCAPS_PRIMARYBUFFER,
    //    Flag_STATIC        = DSBCAPS_STATIC,
    //    Flag_STICKYFOCUS   = DSBCAPS_STICKYFOCUS,
    //};

    enum Format
    {
        Format_PCM = WAVE_FORMAT_PCM,
        //Format_ADPCM = WAVE_FORMAT_ADPCM,
    };
}
#endif //INC_LSOUND_ENUMERATIONS_H__
