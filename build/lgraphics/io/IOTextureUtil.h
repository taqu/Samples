#ifndef INC_IOTEXTUREUTIL_H__
#define INC_IOTEXTUREUTIL_H__
/**
@file IOTextureUtil.h
@author t-sakai
@date 2011/04/30 create
*/
#include "../lgraphicscore.h"
#include "../api/Enumerations.h"

namespace lgraphics
{
    class TextureRef;

namespace io
{
    //-----------------------------------------------------
    /**
    @brief 2のべき乗のサイズに変換
    @param ppBuffer ... 入出力。
    @param width ... 幅
    @param height ... 高さ
    @param format ... バッファフォーマット

    バッファを破壊するので注意
    */
    void convertToPow2Image(u8** ppBuffer, u32& width, u32& height, BufferFormat format);


    //-----------------------------------------------------
    /**
    @brief 半分のサイズのテクスチャ作成
    @param dst ... 出力。
    @param src ... 入力。
    @param sw ... 入力の幅
    @param sh ... 入力の高さ
    @param bpp ... ピクセル辺りのバイト数

    入力は２のべき乗であること
    */
    void createHalfSizeImage(u8* dst, u32 dw, u32 dh, const u8* src, u32 sw, u32 sh, u32 bpp);

    //-----------------------------------------------------
    void calcHalfSize(u32& dw, u32& dh, u32 sw, u32 sh);

    //-----------------------------------------------------
    /// フォーマットからピクセル辺りのバイト数計算
    u32 calcBpp(BufferFormat format);


    //-----------------------------------------------------
    /**
    @brief ミップマップ作成
    @param src ...
    @param width ... 
    @param height ... 
    @param format ... 

    フルレベルでミップマップ作成。convertToPow2Imageで２のべき乗に変換する。
    バッファを破壊するので注意。
    */
    void createMipMap(TextureRef& dst, u8* src, u32 width, u32 height, BufferFormat format, u32 levels);

    //-----------------------------------------------------
    // ミップマップ作成
    void createMipMapDebug(TextureRef& dst, u8* src, u32 width, u32 height, BufferFormat format, u32 levels);

    u32 calcMipMapLevels(u32 width, u32 height);
}
}
#endif //INC_IOTEXTUREUTIL_H__
