#ifndef INC_LGL2EXT_H__
#define INC_LGL2EXT_H__
/**
@file lgl2ext.h
@author t-sakai
@date 2010/12/02 create
*/
#include <GLES2/gl2ext.h>
#include "../../lgraphicscore.h"

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(LIME_GLES2_API)
#define LIME_GLES2_API extern
#endif

#if !defined(LIME_GLES2_APIENTRY)
#define LIME_GLES2_APIENTRY GL_APIENTRY
#endif

#if !defined(LIME_GLES2_FUNC_NULL)
#define LIME_GLES2_FUNC_NULL
#endif

#if !defined(LIME_ES2_FUNC)
#define LIME_ES2_FUNC(ret, name, args) LIME_GLES2_API ret (LIME_GLES2_APIENTRY *fptr_##name) args LIME_GLES2_FUNC_NULL;
#endif

#if !defined(LIME_ES2_FUNCPTR)
#define LIME_ES2_FUNCPTR(name) fptr_##name
#endif

LIME_ES2_FUNC(void, glEGLImageTargetTexture2DOES, (GLenum target, GLeglImageOES image));
LIME_ES2_FUNC(void, glEGLImageTargetRenderbufferStorageOES, (GLenum target, GLeglImageOES image));
LIME_ES2_FUNC(void, glGetProgramBinaryOES, (GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary));
LIME_ES2_FUNC(void, glProgramBinaryOES, (GLuint program, GLenum binaryFormat, const GLvoid *binary, GLint length));
LIME_ES2_FUNC(void*, glMapBufferOES, (GLenum target, GLenum access));
LIME_ES2_FUNC(GLboolean, glUnmapBufferOES, (GLenum target));
LIME_ES2_FUNC(void, glGetBufferPointervOES, (GLenum target, GLenum pname, GLvoid** params));
LIME_ES2_FUNC(void, glTexImage3DOES, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* pixels));
LIME_ES2_FUNC(void, glTexSubImage3DOES, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels));
LIME_ES2_FUNC(void, glCopyTexSubImage3DOES, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height));
LIME_ES2_FUNC(void, glCompressedTexImage3DOES, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data));
LIME_ES2_FUNC(void, glCompressedTexSubImage3DOES, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data));
LIME_ES2_FUNC(void, glFramebufferTexture3DOES, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset));
LIME_ES2_FUNC(void, glBindVertexArrayOES, (GLuint array));
LIME_ES2_FUNC(void, glDeleteVertexArraysOES, (GLsizei n, const GLuint *arrays));
LIME_ES2_FUNC(void, glGenVertexArraysOES, (GLsizei n, GLuint *arrays));
LIME_ES2_FUNC(GLboolean, glIsVertexArrayOES, (GLuint array));
LIME_ES2_FUNC(void, glGetPerfMonitorGroupsAMD, (GLint *numGroups, GLsizei groupsSize, GLuint *groups));
LIME_ES2_FUNC(void, glGetPerfMonitorCountersAMD, (GLuint group, GLint *numCounters, GLint *maxActiveCounters, GLsizei counterSize, GLuint *counters));
LIME_ES2_FUNC(void, glGetPerfMonitorGroupStringAMD, (GLuint group, GLsizei bufSize, GLsizei *length, GLchar *groupString));
LIME_ES2_FUNC(void, glGetPerfMonitorCounterStringAMD, (GLuint group, GLuint counter, GLsizei bufSize, GLsizei *length, GLchar *counterString));
LIME_ES2_FUNC(void, glGetPerfMonitorCounterInfoAMD, (GLuint group, GLuint counter, GLenum pname, GLvoid *data));
LIME_ES2_FUNC(void, glGenPerfMonitorsAMD, (GLsizei n, GLuint *monitors));
LIME_ES2_FUNC(void, glDeletePerfMonitorsAMD, (GLsizei n, GLuint *monitors));
LIME_ES2_FUNC(void, glSelectPerfMonitorCountersAMD, (GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint *countersList));
LIME_ES2_FUNC(void, glBeginPerfMonitorAMD, (GLuint monitor));
LIME_ES2_FUNC(void, glEndPerfMonitorAMD, (GLuint monitor));
LIME_ES2_FUNC(void, glGetPerfMonitorCounterDataAMD, (GLuint monitor, GLenum pname, GLsizei dataSize, GLuint *data, GLint *bytesWritten));
LIME_ES2_FUNC(void, glBlitFramebufferANGLE, (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter));
LIME_ES2_FUNC(void, glRenderbufferStorageMultisampleANGLE, (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height));
LIME_ES2_FUNC(void, glRenderbufferStorageMultisampleAPPLE, (GLenum, GLsizei, GLenum, GLsizei, GLsizei));
LIME_ES2_FUNC(void, glResolveMultisampleFramebufferAPPLE, (void));
LIME_ES2_FUNC(void, glDiscardFramebufferEXT, (GLenum target, GLsizei numAttachments, const GLenum *attachments));
LIME_ES2_FUNC(void, glMultiDrawArraysEXT, (GLenum, GLint *, GLsizei *, GLsizei));
LIME_ES2_FUNC(void, glMultiDrawElementsEXT, (GLenum, const GLsizei *, GLenum, const GLvoid* *, GLsizei));
LIME_ES2_FUNC(void, glRenderbufferStorageMultisampleIMG, (GLenum, GLsizei, GLenum, GLsizei, GLsizei));
LIME_ES2_FUNC(void, glFramebufferTexture2DMultisampleIMG, (GLenum, GLenum, GLenum, GLuint, GLint, GLsizei));
LIME_ES2_FUNC(void, glDeleteFencesNV, (GLsizei, const GLuint *));
LIME_ES2_FUNC(void, glGenFencesNV, (GLsizei, GLuint *));
LIME_ES2_FUNC(GLboolean, glIsFenceNV, (GLuint));
LIME_ES2_FUNC(GLboolean, glTestFenceNV, (GLuint));
LIME_ES2_FUNC(void, glGetFenceivNV, (GLuint, GLenum, GLint *));
LIME_ES2_FUNC(void, glFinishFenceNV, (GLuint));
LIME_ES2_FUNC(void, glSetFenceNV, (GLuint, GLenum));
LIME_ES2_FUNC(void, glCoverageMaskNV, (GLboolean mask));
LIME_ES2_FUNC(void, glCoverageOperationNV, (GLenum operation));
LIME_ES2_FUNC(void, glGetDriverControlsQCOM, (GLint *num, GLsizei size, GLuint *driverControls));
LIME_ES2_FUNC(void, glGetDriverControlStringQCOM, (GLuint driverControl, GLsizei bufSize, GLsizei *length, GLchar *driverControlString));
LIME_ES2_FUNC(void, glEnableDriverControlQCOM, (GLuint driverControl));
LIME_ES2_FUNC(void, glDisableDriverControlQCOM, (GLuint driverControl));
LIME_ES2_FUNC(void, glExtGetTexturesQCOM, (GLuint *textures, GLint maxTextures, GLint *numTextures));
LIME_ES2_FUNC(void, glExtGetBuffersQCOM, (GLuint *buffers, GLint maxBuffers, GLint *numBuffers));
LIME_ES2_FUNC(void, glExtGetRenderbuffersQCOM, (GLuint *renderbuffers, GLint maxRenderbuffers, GLint *numRenderbuffers));
LIME_ES2_FUNC(void, glExtGetFramebuffersQCOM, (GLuint *framebuffers, GLint maxFramebuffers, GLint *numFramebuffers));
LIME_ES2_FUNC(void, glExtGetTexLevelParameterivQCOM, (GLuint texture, GLenum face, GLint level, GLenum pname, GLint *params));
LIME_ES2_FUNC(void, glExtTexObjectStateOverrideiQCOM, (GLenum target, GLenum pname, GLint param));
LIME_ES2_FUNC(void, glExtGetTexSubImageQCOM, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLvoid *texels));
LIME_ES2_FUNC(void, glExtGetBufferPointervQCOM, (GLenum target, GLvoid **params));
LIME_ES2_FUNC(void, glExtGetShadersQCOM, (GLuint *shaders, GLint maxShaders, GLint *numShaders));
LIME_ES2_FUNC(void, glExtGetProgramsQCOM, (GLuint *programs, GLint maxPrograms, GLint *numPrograms));
LIME_ES2_FUNC(GLboolean, glExtIsProgramBinaryQCOM, (GLuint program));
LIME_ES2_FUNC(void, glExtGetProgramBinarySourceQCOM, (GLuint program, GLenum shadertype, GLchar *source, GLint *length));
LIME_ES2_FUNC(void, glStartTilingQCOM, (GLuint x, GLuint y, GLuint width, GLuint height, GLbitfield preserveMask));
LIME_ES2_FUNC(void, glEndTilingQCOM, (GLbitfield preserveMask));

#if !defined(LIME_GLES2_NO_FUNCDEF)

/**************************************************************/
#define glEGLImageTargetTexture2DOES	LIME_ES2_FUNCPTR(glEGLImageTargetTexture2DOES)
#define glEGLImageTargetRenderbufferStorageOES	LIME_ES2_FUNCPTR(glEGLImageTargetRenderbufferStorageOES)
#define glGetProgramBinaryOES	LIME_ES2_FUNCPTR(glGetProgramBinaryOES)
#define glProgramBinaryOES	LIME_ES2_FUNCPTR(glProgramBinaryOES)
#define glMapBufferOES	LIME_ES2_FUNCPTR(glMapBufferOES)
#define glUnmapBufferOES	LIME_ES2_FUNCPTR(glUnmapBufferOES)
#define glGetBufferPointervOES	LIME_ES2_FUNCPTR(glGetBufferPointervOES)
#define glTexImage3DOES	LIME_ES2_FUNCPTR(glTexImage3DOES)
#define glTexSubImage3DOES	LIME_ES2_FUNCPTR(glTexSubImage3DOES)
#define glCopyTexSubImage3DOES	LIME_ES2_FUNCPTR(glCopyTexSubImage3DOES)
#define glCompressedTexImage3DOES	LIME_ES2_FUNCPTR(glCompressedTexImage3DOES)
#define glCompressedTexSubImage3DOES	LIME_ES2_FUNCPTR(glCompressedTexSubImage3DOES)
#define glFramebufferTexture3DOES	LIME_ES2_FUNCPTR(glFramebufferTexture3DOES)
#define glBindVertexArrayOES	LIME_ES2_FUNCPTR(glBindVertexArrayOES)
#define glDeleteVertexArraysOES	LIME_ES2_FUNCPTR(glDeleteVertexArraysOES)
#define glGenVertexArraysOES	LIME_ES2_FUNCPTR(glGenVertexArraysOES)
#define glIsVertexArrayOES	LIME_ES2_FUNCPTR(glIsVertexArrayOES)
#define glGetPerfMonitorGroupsAMD	LIME_ES2_FUNCPTR(glGetPerfMonitorGroupsAMD)
#define glGetPerfMonitorCountersAMD	LIME_ES2_FUNCPTR(glGetPerfMonitorCountersAMD)
#define glGetPerfMonitorGroupStringAMD	LIME_ES2_FUNCPTR(glGetPerfMonitorGroupStringAMD)
#define glGetPerfMonitorCounterStringAMD	LIME_ES2_FUNCPTR(glGetPerfMonitorCounterStringAMD)
#define glGetPerfMonitorCounterInfoAMD	LIME_ES2_FUNCPTR(glGetPerfMonitorCounterInfoAMD)
#define glGenPerfMonitorsAMD	LIME_ES2_FUNCPTR(glGenPerfMonitorsAMD)
#define glDeletePerfMonitorsAMD	LIME_ES2_FUNCPTR(glDeletePerfMonitorsAMD)
#define glSelectPerfMonitorCountersAMD	LIME_ES2_FUNCPTR(glSelectPerfMonitorCountersAMD)
#define glBeginPerfMonitorAMD	LIME_ES2_FUNCPTR(glBeginPerfMonitorAMD)
#define glEndPerfMonitorAMD	LIME_ES2_FUNCPTR(glEndPerfMonitorAMD)
#define glGetPerfMonitorCounterDataAMD	LIME_ES2_FUNCPTR(glGetPerfMonitorCounterDataAMD)
#define glBlitFramebufferANGLE	LIME_ES2_FUNCPTR(glBlitFramebufferANGLE)
#define glRenderbufferStorageMultisampleANGLE	LIME_ES2_FUNCPTR(glRenderbufferStorageMultisampleANGLE)
#define glRenderbufferStorageMultisampleAPPLE	LIME_ES2_FUNCPTR(glRenderbufferStorageMultisampleAPPLE)
#define glResolveMultisampleFramebufferAPPLE	LIME_ES2_FUNCPTR(glResolveMultisampleFramebufferAPPLE)
#define glDiscardFramebufferEXT	LIME_ES2_FUNCPTR(glDiscardFramebufferEXT)
#define glMultiDrawArraysEXT	LIME_ES2_FUNCPTR(glMultiDrawArraysEXT)
#define glMultiDrawElementsEXT	LIME_ES2_FUNCPTR(glMultiDrawElementsEXT)
#define glRenderbufferStorageMultisampleIMG	LIME_ES2_FUNCPTR(glRenderbufferStorageMultisampleIMG)
#define glFramebufferTexture2DMultisampleIMG	LIME_ES2_FUNCPTR(glFramebufferTexture2DMultisampleIMG)
#define glDeleteFencesNV	LIME_ES2_FUNCPTR(glDeleteFencesNV)
#define glGenFencesNV	LIME_ES2_FUNCPTR(glGenFencesNV)
#define glIsFenceNV	LIME_ES2_FUNCPTR(glIsFenceNV)
#define glTestFenceNV	LIME_ES2_FUNCPTR(glTestFenceNV)
#define glGetFenceivNV	LIME_ES2_FUNCPTR(glGetFenceivNV)
#define glFinishFenceNV	LIME_ES2_FUNCPTR(glFinishFenceNV)
#define glSetFenceNV	LIME_ES2_FUNCPTR(glSetFenceNV)
#define glCoverageMaskNV	LIME_ES2_FUNCPTR(glCoverageMaskNV)
#define glCoverageOperationNV	LIME_ES2_FUNCPTR(glCoverageOperationNV)
#define glGetDriverControlsQCOM	LIME_ES2_FUNCPTR(glGetDriverControlsQCOM)
#define glGetDriverControlStringQCOM	LIME_ES2_FUNCPTR(glGetDriverControlStringQCOM)
#define glEnableDriverControlQCOM	LIME_ES2_FUNCPTR(glEnableDriverControlQCOM)
#define glDisableDriverControlQCOM	LIME_ES2_FUNCPTR(glDisableDriverControlQCOM)
#define glExtGetTexturesQCOM	LIME_ES2_FUNCPTR(glExtGetTexturesQCOM)
#define glExtGetBuffersQCOM	LIME_ES2_FUNCPTR(glExtGetBuffersQCOM)
#define glExtGetRenderbuffersQCOM	LIME_ES2_FUNCPTR(glExtGetRenderbuffersQCOM)
#define glExtGetFramebuffersQCOM	LIME_ES2_FUNCPTR(glExtGetFramebuffersQCOM)
#define glExtGetTexLevelParameterivQCOM	LIME_ES2_FUNCPTR(glExtGetTexLevelParameterivQCOM)
#define glExtTexObjectStateOverrideiQCOM	LIME_ES2_FUNCPTR(glExtTexObjectStateOverrideiQCOM)
#define glExtGetTexSubImageQCOM	LIME_ES2_FUNCPTR(glExtGetTexSubImageQCOM)
#define glExtGetBufferPointervQCOM	LIME_ES2_FUNCPTR(glExtGetBufferPointervQCOM)
#define glExtGetShadersQCOM	LIME_ES2_FUNCPTR(glExtGetShadersQCOM)
#define glExtGetProgramsQCOM	LIME_ES2_FUNCPTR(glExtGetProgramsQCOM)
#define glExtIsProgramBinaryQCOM	LIME_ES2_FUNCPTR(glExtIsProgramBinaryQCOM)
#define glExtGetProgramBinarySourceQCOM	LIME_ES2_FUNCPTR(glExtGetProgramBinarySourceQCOM)
#define glStartTilingQCOM	LIME_ES2_FUNCPTR(glStartTilingQCOM)
#define glEndTilingQCOM	LIME_ES2_FUNCPTR(glEndTilingQCOM)

#endif //LIME_GLES2_NO_FUNCDEF

#ifdef __cplusplus
}
#endif
#endif //INC_LGL2EXT_H__
