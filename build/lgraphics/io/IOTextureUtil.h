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
    @brief 2�ׂ̂���̃T�C�Y�ɕϊ�
    @param ppBuffer ... ���o�́B
    @param width ... ��
    @param height ... ����
    @param format ... �o�b�t�@�t�H�[�}�b�g

    �o�b�t�@��j�󂷂�̂Œ���
    */
    void convertToPow2Image(u8** ppBuffer, u32& width, u32& height, BufferFormat format);


    //-----------------------------------------------------
    /**
    @brief �����̃T�C�Y�̃e�N�X�`���쐬
    @param dst ... �o�́B
    @param src ... ���́B
    @param sw ... ���͂̕�
    @param sh ... ���͂̍���
    @param bpp ... �s�N�Z���ӂ�̃o�C�g��

    ���͂͂Q�ׂ̂���ł��邱��
    */
    void createHalfSizeImage(u8* dst, u32 dw, u32 dh, const u8* src, u32 sw, u32 sh, u32 bpp);

    //-----------------------------------------------------
    void calcHalfSize(u32& dw, u32& dh, u32 sw, u32 sh);

    //-----------------------------------------------------
    /// �t�H�[�}�b�g����s�N�Z���ӂ�̃o�C�g���v�Z
    u32 calcBpp(BufferFormat format);


    //-----------------------------------------------------
    /**
    @brief �~�b�v�}�b�v�쐬
    @param src ...
    @param width ... 
    @param height ... 
    @param format ... 

    �t�����x���Ń~�b�v�}�b�v�쐬�BconvertToPow2Image�łQ�ׂ̂���ɕϊ�����B
    �o�b�t�@��j�󂷂�̂Œ��ӁB
    */
    void createMipMap(TextureRef& dst, u8* src, u32 width, u32 height, BufferFormat format, u32 levels);

    //-----------------------------------------------------
    // �~�b�v�}�b�v�쐬
    void createMipMapDebug(TextureRef& dst, u8* src, u32 width, u32 height, BufferFormat format, u32 levels);

    u32 calcMipMapLevels(u32 width, u32 height);
}
}
#endif //INC_IOTEXTUREUTIL_H__
