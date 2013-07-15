#ifndef INC_LGRAPHICS_LGL2_H__
#define INC_LGRAPHICS_LGL2_H__
/**
@file lgl2.h
@author t-sakai
@date 2010/12/02 create
*/
#include <GLES2/gl2.h>
#include "../../lgraphicscore.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef char GLchar;

    bool initializeGLES2();
    void terminateGLES2();
    void dumpGLES2Extensions(const char* filepath);

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

#if defined(LIME_INIT_GLES2)

LIME_ES2_FUNC(void, glActiveTexture, (GLenum texture));
LIME_ES2_FUNC(void, glAttachShader, (GLuint program, GLuint shader));
LIME_ES2_FUNC(void, glBindAttribLocation, (GLuint program, GLuint index, const GLchar* name));
LIME_ES2_FUNC(void, glBindBuffer, (GLenum target, GLuint buffer));
LIME_ES2_FUNC(void, glBindFramebuffer, (GLenum target, GLuint framebuffer));
LIME_ES2_FUNC(void, glBindRenderbuffer, (GLenum target, GLuint renderbuffer));
LIME_ES2_FUNC(void, glBindTexture, (GLenum target, GLuint texture));
LIME_ES2_FUNC(void, glBlendColor, (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha));
LIME_ES2_FUNC(void, glBlendEquation, ( GLenum mode ));
LIME_ES2_FUNC(void, glBlendEquationSeparate, (GLenum modeRGB, GLenum modeAlpha));
LIME_ES2_FUNC(void, glBlendFunc, (GLenum sfactor, GLenum dfactor));
LIME_ES2_FUNC(void, glBlendFuncSeparate, (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha));
LIME_ES2_FUNC(void, glBufferData, (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage));
LIME_ES2_FUNC(void, glBufferSubData, (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data));
LIME_ES2_FUNC(GLenum, glCheckFramebufferStatus, (GLenum target));
LIME_ES2_FUNC(void, glClear, (GLbitfield mask));
LIME_ES2_FUNC(void, glClearColor, (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha));
LIME_ES2_FUNC(void, glClearDepthf, (GLclampf depth));
LIME_ES2_FUNC(void, glClearStencil, (GLint s));
LIME_ES2_FUNC(void, glColorMask, (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha));
LIME_ES2_FUNC(void, glCompileShader, (GLuint shader));
LIME_ES2_FUNC(void, glCompressedTexImage2D, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data));
LIME_ES2_FUNC(void, glCompressedTexSubImage2D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data));
LIME_ES2_FUNC(void, glCopyTexImage2D, (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border));
LIME_ES2_FUNC(void, glCopyTexSubImage2D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height));
LIME_ES2_FUNC(GLuint, glCreateProgram, (void));
LIME_ES2_FUNC(GLuint, glCreateShader, (GLenum type));
LIME_ES2_FUNC(void, glCullFace, (GLenum mode));
LIME_ES2_FUNC(void, glDeleteBuffers, (GLsizei n, const GLuint* buffers));
LIME_ES2_FUNC(void, glDeleteFramebuffers, (GLsizei n, const GLuint* framebuffers));
LIME_ES2_FUNC(void, glDeleteProgram, (GLuint program));
LIME_ES2_FUNC(void, glDeleteRenderbuffers, (GLsizei n, const GLuint* renderbuffers));
LIME_ES2_FUNC(void, glDeleteShader, (GLuint shader));
LIME_ES2_FUNC(void, glDeleteTextures, (GLsizei n, const GLuint* textures));
LIME_ES2_FUNC(void, glDepthFunc, (GLenum func));
LIME_ES2_FUNC(void, glDepthMask, (GLboolean flag));
LIME_ES2_FUNC(void, glDepthRangef, (GLclampf zNear, GLclampf zFar));
LIME_ES2_FUNC(void, glDetachShader, (GLuint program, GLuint shader));
LIME_ES2_FUNC(void, glDisable, (GLenum cap));
LIME_ES2_FUNC(void, glDisableVertexAttribArray, (GLuint index));
LIME_ES2_FUNC(void, glDrawArrays, (GLenum mode, GLint first, GLsizei count));
LIME_ES2_FUNC(void, glDrawElements, (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices));
LIME_ES2_FUNC(void, glEnable, (GLenum cap));
LIME_ES2_FUNC(void, glEnableVertexAttribArray, (GLuint index));
LIME_ES2_FUNC(void, glFinish, (void));
LIME_ES2_FUNC(void, glFlush, (void));
LIME_ES2_FUNC(void, glFramebufferRenderbuffer, (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer));
LIME_ES2_FUNC(void, glFramebufferTexture2D, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level));
LIME_ES2_FUNC(void, glFrontFace, (GLenum mode));
LIME_ES2_FUNC(void, glGenBuffers, (GLsizei n, GLuint* buffers));
LIME_ES2_FUNC(void, glGenerateMipmap, (GLenum target));
LIME_ES2_FUNC(void, glGenFramebuffers, (GLsizei n, GLuint* framebuffers));
LIME_ES2_FUNC(void, glGenRenderbuffers, (GLsizei n, GLuint* renderbuffers));
LIME_ES2_FUNC(void, glGenTextures, (GLsizei n, GLuint* textures));
LIME_ES2_FUNC(void, glGetActiveAttrib, (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name));
LIME_ES2_FUNC(void, glGetActiveUniform, (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name));
LIME_ES2_FUNC(void, glGetAttachedShaders, (GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders));
LIME_ES2_FUNC(int, glGetAttribLocation, (GLuint program, const GLchar* name));
LIME_ES2_FUNC(void, glGetBooleanv, (GLenum pname, GLboolean* params));
LIME_ES2_FUNC(void, glGetBufferParameteriv, (GLenum target, GLenum pname, GLint* params));
LIME_ES2_FUNC(GLenum, glGetError, (void));
LIME_ES2_FUNC(void, glGetFloatv, (GLenum pname, GLfloat* params));
LIME_ES2_FUNC(void, glGetFramebufferAttachmentParameteriv, (GLenum target, GLenum attachment, GLenum pname, GLint* params));
LIME_ES2_FUNC(void, glGetIntegerv, (GLenum pname, GLint* params));
LIME_ES2_FUNC(void, glGetProgramiv, (GLuint program, GLenum pname, GLint* params));
LIME_ES2_FUNC(void, glGetProgramInfoLog, (GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog));
LIME_ES2_FUNC(void, glGetRenderbufferParameteriv, (GLenum target, GLenum pname, GLint* params));
LIME_ES2_FUNC(void, glGetShaderiv, (GLuint shader, GLenum pname, GLint* params));
LIME_ES2_FUNC(void, glGetShaderInfoLog, (GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* infolog));
LIME_ES2_FUNC(void, glGetShaderPrecisionFormat, (GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision));
LIME_ES2_FUNC(void, glGetShaderSource, (GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* source));
LIME_ES2_FUNC(const GLubyte*, glGetString, (GLenum name));
LIME_ES2_FUNC(void, glGetTexParameterfv, (GLenum target, GLenum pname, GLfloat* params));
LIME_ES2_FUNC(void, glGetTexParameteriv, (GLenum target, GLenum pname, GLint* params));
LIME_ES2_FUNC(void, glGetUniformfv, (GLuint program, GLint location, GLfloat* params));
LIME_ES2_FUNC(void, glGetUniformiv, (GLuint program, GLint location, GLint* params));
LIME_ES2_FUNC(int, glGetUniformLocation, (GLuint program, const GLchar* name));
LIME_ES2_FUNC(void, glGetVertexAttribfv, (GLuint index, GLenum pname, GLfloat* params));
LIME_ES2_FUNC(void, glGetVertexAttribiv, (GLuint index, GLenum pname, GLint* params));
LIME_ES2_FUNC(void, glGetVertexAttribPointerv, (GLuint index, GLenum pname, GLvoid** pointer));
LIME_ES2_FUNC(void, glHint, (GLenum target, GLenum mode));
LIME_ES2_FUNC(GLboolean, glIsBuffer, (GLuint buffer));
LIME_ES2_FUNC(GLboolean, glIsEnabled, (GLenum cap));
LIME_ES2_FUNC(GLboolean, glIsFramebuffer, (GLuint framebuffer));
LIME_ES2_FUNC(GLboolean, glIsProgram, (GLuint program));
LIME_ES2_FUNC(GLboolean, glIsRenderbuffer, (GLuint renderbuffer));
LIME_ES2_FUNC(GLboolean, glIsShader, (GLuint shader));
LIME_ES2_FUNC(GLboolean, glIsTexture, (GLuint texture));
LIME_ES2_FUNC(void, glLineWidth, (GLfloat width));
LIME_ES2_FUNC(void, glLinkProgram, (GLuint program));
LIME_ES2_FUNC(void, glPixelStorei, (GLenum pname, GLint param));
LIME_ES2_FUNC(void, glPolygonOffset, (GLfloat factor, GLfloat units));
LIME_ES2_FUNC(void, glReadPixels, (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels));
LIME_ES2_FUNC(void, glReleaseShaderCompiler, (void));
LIME_ES2_FUNC(void, glRenderbufferStorage, (GLenum target, GLenum internalformat, GLsizei width, GLsizei height));
LIME_ES2_FUNC(void, glSampleCoverage, (GLclampf value, GLboolean invert));
LIME_ES2_FUNC(void, glScissor, (GLint x, GLint y, GLsizei width, GLsizei height));
LIME_ES2_FUNC(void, glShaderBinary, (GLsizei n, const GLuint* shaders, GLenum binaryformat, const GLvoid* binary, GLsizei length));
LIME_ES2_FUNC(void, glShaderSource, (GLuint shader, GLsizei count, const GLchar** string, const GLint* length));
LIME_ES2_FUNC(void, glStencilFunc, (GLenum func, GLint ref, GLuint mask));
LIME_ES2_FUNC(void, glStencilFuncSeparate, (GLenum face, GLenum func, GLint ref, GLuint mask));
LIME_ES2_FUNC(void, glStencilMask, (GLuint mask));
LIME_ES2_FUNC(void, glStencilMaskSeparate, (GLenum face, GLuint mask));
LIME_ES2_FUNC(void, glStencilOp, (GLenum fail, GLenum zfail, GLenum zpass));
LIME_ES2_FUNC(void, glStencilOpSeparate, (GLenum face, GLenum fail, GLenum zfail, GLenum zpass));
LIME_ES2_FUNC(void, glTexImage2D, (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels));
LIME_ES2_FUNC(void, glTexParameterf, (GLenum target, GLenum pname, GLfloat param));
LIME_ES2_FUNC(void, glTexParameterfv, (GLenum target, GLenum pname, const GLfloat* params));
LIME_ES2_FUNC(void, glTexParameteri, (GLenum target, GLenum pname, GLint param));
LIME_ES2_FUNC(void, glTexParameteriv, (GLenum target, GLenum pname, const GLint* params));
LIME_ES2_FUNC(void, glTexSubImage2D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels));
LIME_ES2_FUNC(void, glUniform1f, (GLint location, GLfloat x));
LIME_ES2_FUNC(void, glUniform1fv, (GLint location, GLsizei count, const GLfloat* v));
LIME_ES2_FUNC(void, glUniform1i, (GLint location, GLint x));
LIME_ES2_FUNC(void, glUniform1iv, (GLint location, GLsizei count, const GLint* v));
LIME_ES2_FUNC(void, glUniform2f, (GLint location, GLfloat x, GLfloat y));
LIME_ES2_FUNC(void, glUniform2fv, (GLint location, GLsizei count, const GLfloat* v));
LIME_ES2_FUNC(void, glUniform2i, (GLint location, GLint x, GLint y));
LIME_ES2_FUNC(void, glUniform2iv, (GLint location, GLsizei count, const GLint* v));
LIME_ES2_FUNC(void, glUniform3f, (GLint location, GLfloat x, GLfloat y, GLfloat z));
LIME_ES2_FUNC(void, glUniform3fv, (GLint location, GLsizei count, const GLfloat* v));
LIME_ES2_FUNC(void, glUniform3i, (GLint location, GLint x, GLint y, GLint z));
LIME_ES2_FUNC(void, glUniform3iv, (GLint location, GLsizei count, const GLint* v));
LIME_ES2_FUNC(void, glUniform4f, (GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w));
LIME_ES2_FUNC(void, glUniform4fv, (GLint location, GLsizei count, const GLfloat* v));
LIME_ES2_FUNC(void, glUniform4i, (GLint location, GLint x, GLint y, GLint z, GLint w));
LIME_ES2_FUNC(void, glUniform4iv, (GLint location, GLsizei count, const GLint* v));
LIME_ES2_FUNC(void, glUniformMatrix2fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value));
LIME_ES2_FUNC(void, glUniformMatrix3fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value));
LIME_ES2_FUNC(void, glUniformMatrix4fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value));
LIME_ES2_FUNC(void, glUseProgram, (GLuint program));
LIME_ES2_FUNC(void, glValidateProgram, (GLuint program));
LIME_ES2_FUNC(void, glVertexAttrib1f, (GLuint indx, GLfloat x));
LIME_ES2_FUNC(void, glVertexAttrib1fv, (GLuint indx, const GLfloat* values));
LIME_ES2_FUNC(void, glVertexAttrib2f, (GLuint indx, GLfloat x, GLfloat y));
LIME_ES2_FUNC(void, glVertexAttrib2fv, (GLuint indx, const GLfloat* values));
LIME_ES2_FUNC(void, glVertexAttrib3f, (GLuint indx, GLfloat x, GLfloat y, GLfloat z));
LIME_ES2_FUNC(void, glVertexAttrib3fv, (GLuint indx, const GLfloat* values));
LIME_ES2_FUNC(void, glVertexAttrib4f, (GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w));
LIME_ES2_FUNC(void, glVertexAttrib4fv, (GLuint indx, const GLfloat* values));
LIME_ES2_FUNC(void, glVertexAttribPointer, (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr));
LIME_ES2_FUNC(void, glViewport, (GLint x, GLint y, GLsizei width, GLsizei height));

#if !defined(LIME_GLES2_NO_FUNCDEF)
/**************************************************************/
#define glActiveTexture	LIME_ES2_FUNCPTR(glActiveTexture)
#define glAttachShader	LIME_ES2_FUNCPTR(glAttachShader)
#define glBindAttribLocation	LIME_ES2_FUNCPTR(glBindAttribLocation)
#define glBindBuffer	LIME_ES2_FUNCPTR(glBindBuffer)
#define glBindFramebuffer	LIME_ES2_FUNCPTR(glBindFramebuffer)
#define glBindRenderbuffer	LIME_ES2_FUNCPTR(glBindRenderbuffer)
#define glBindTexture	LIME_ES2_FUNCPTR(glBindTexture)
#define glBlendColor	LIME_ES2_FUNCPTR(glBlendColor)
#define glBlendEquation	LIME_ES2_FUNCPTR(glBlendEquation)
#define glBlendEquationSeparate	LIME_ES2_FUNCPTR(glBlendEquationSeparate)
#define glBlendFunc	LIME_ES2_FUNCPTR(glBlendFunc)
#define glBlendFuncSeparate	LIME_ES2_FUNCPTR(glBlendFuncSeparate)
#define glBufferData	LIME_ES2_FUNCPTR(glBufferData)
#define glBufferSubData	LIME_ES2_FUNCPTR(glBufferSubData)
#define glCheckFramebufferStatus	LIME_ES2_FUNCPTR(glCheckFramebufferStatus)
#define glClear	LIME_ES2_FUNCPTR(glClear)
#define glClearColor	LIME_ES2_FUNCPTR(glClearColor)
#define glClearDepthf	LIME_ES2_FUNCPTR(glClearDepthf)
#define glClearStencil	LIME_ES2_FUNCPTR(glClearStencil)
#define glColorMask	LIME_ES2_FUNCPTR(glColorMask)
#define glCompileShader	LIME_ES2_FUNCPTR(glCompileShader)
#define glCompressedTexImage2D	LIME_ES2_FUNCPTR(glCompressedTexImage2D)
#define glCompressedTexSubImage2D	LIME_ES2_FUNCPTR(glCompressedTexSubImage2D)
#define glCopyTexImage2D	LIME_ES2_FUNCPTR(glCopyTexImage2D)
#define glCopyTexSubImage2D	LIME_ES2_FUNCPTR(glCopyTexSubImage2D)
#define glCreateProgram	LIME_ES2_FUNCPTR(glCreateProgram)
#define glCreateShader	LIME_ES2_FUNCPTR(glCreateShader)
#define glCullFace	LIME_ES2_FUNCPTR(glCullFace)
#define glDeleteBuffers	LIME_ES2_FUNCPTR(glDeleteBuffers)
#define glDeleteFramebuffers	LIME_ES2_FUNCPTR(glDeleteFramebuffers)
#define glDeleteProgram	LIME_ES2_FUNCPTR(glDeleteProgram)
#define glDeleteRenderbuffers	LIME_ES2_FUNCPTR(glDeleteRenderbuffers)
#define glDeleteShader	LIME_ES2_FUNCPTR(glDeleteShader)
#define glDeleteTextures	LIME_ES2_FUNCPTR(glDeleteTextures)
#define glDepthFunc	LIME_ES2_FUNCPTR(glDepthFunc)
#define glDepthMask	LIME_ES2_FUNCPTR(glDepthMask)
#define glDepthRangef	LIME_ES2_FUNCPTR(glDepthRangef)
#define glDetachShader	LIME_ES2_FUNCPTR(glDetachShader)
#define glDisable	LIME_ES2_FUNCPTR(glDisable)
#define glDisableVertexAttribArray	LIME_ES2_FUNCPTR(glDisableVertexAttribArray)
#define glDrawArrays	LIME_ES2_FUNCPTR(glDrawArrays)
#define glDrawElements	LIME_ES2_FUNCPTR(glDrawElements)
#define glEnable	LIME_ES2_FUNCPTR(glEnable)
#define glEnableVertexAttribArray	LIME_ES2_FUNCPTR(glEnableVertexAttribArray)
#define glFinish	LIME_ES2_FUNCPTR(glFinish)
#define glFlush	LIME_ES2_FUNCPTR(glFlush)
#define glFramebufferRenderbuffer	LIME_ES2_FUNCPTR(glFramebufferRenderbuffer)
#define glFramebufferTexture2D	LIME_ES2_FUNCPTR(glFramebufferTexture2D)
#define glFrontFace	LIME_ES2_FUNCPTR(glFrontFace)
#define glGenBuffers	LIME_ES2_FUNCPTR(glGenBuffers)
#define glGenerateMipmap	LIME_ES2_FUNCPTR(glGenerateMipmap)
#define glGenFramebuffers	LIME_ES2_FUNCPTR(glGenFramebuffers)
#define glGenRenderbuffers	LIME_ES2_FUNCPTR(glGenRenderbuffers)
#define glGenTextures	LIME_ES2_FUNCPTR(glGenTextures)
#define glGetActiveAttrib	LIME_ES2_FUNCPTR(glGetActiveAttrib)
#define glGetActiveUniform	LIME_ES2_FUNCPTR(glGetActiveUniform)
#define glGetAttachedShaders	LIME_ES2_FUNCPTR(glGetAttachedShaders)
#define glGetAttribLocation	LIME_ES2_FUNCPTR(glGetAttribLocation)
#define glGetBooleanv	LIME_ES2_FUNCPTR(glGetBooleanv)
#define glGetBufferParameteriv	LIME_ES2_FUNCPTR(glGetBufferParameteriv)
#define glGetError	LIME_ES2_FUNCPTR(glGetError)
#define glGetFloatv	LIME_ES2_FUNCPTR(glGetFloatv)
#define glGetFramebufferAttachmentParameteriv	LIME_ES2_FUNCPTR(glGetFramebufferAttachmentParameteriv)
#define glGetIntegerv	LIME_ES2_FUNCPTR(glGetIntegerv)
#define glGetProgramiv	LIME_ES2_FUNCPTR(glGetProgramiv)
#define glGetProgramInfoLog	LIME_ES2_FUNCPTR(glGetProgramInfoLog)
#define glGetRenderbufferParameteriv	LIME_ES2_FUNCPTR(glGetRenderbufferParameteriv)
#define glGetShaderiv	LIME_ES2_FUNCPTR(glGetShaderiv)
#define glGetShaderInfoLog	LIME_ES2_FUNCPTR(glGetShaderInfoLog)
#define glGetShaderPrecisionFormat	LIME_ES2_FUNCPTR(glGetShaderPrecisionFormat)
#define glGetShaderSource	LIME_ES2_FUNCPTR(glGetShaderSource)
#define glGetString	LIME_ES2_FUNCPTR(glGetString)
#define glGetTexParameterfv	LIME_ES2_FUNCPTR(glGetTexParameterfv)
#define glGetTexParameteriv	LIME_ES2_FUNCPTR(glGetTexParameteriv)
#define glGetUniformfv	LIME_ES2_FUNCPTR(glGetUniformfv)
#define glGetUniformiv	LIME_ES2_FUNCPTR(glGetUniformiv)
#define glGetUniformLocation	LIME_ES2_FUNCPTR(glGetUniformLocation)
#define glGetVertexAttribfv	LIME_ES2_FUNCPTR(glGetVertexAttribfv)
#define glGetVertexAttribiv	LIME_ES2_FUNCPTR(glGetVertexAttribiv)
#define glGetVertexAttribPointerv	LIME_ES2_FUNCPTR(glGetVertexAttribPointerv)
#define glHint	LIME_ES2_FUNCPTR(glHint)
#define glIsBuffer	LIME_ES2_FUNCPTR(glIsBuffer)
#define glIsEnabled	LIME_ES2_FUNCPTR(glIsEnabled)
#define glIsFramebuffer	LIME_ES2_FUNCPTR(glIsFramebuffer)
#define glIsProgram	LIME_ES2_FUNCPTR(glIsProgram)
#define glIsRenderbuffer	LIME_ES2_FUNCPTR(glIsRenderbuffer)
#define glIsShader	LIME_ES2_FUNCPTR(glIsShader)
#define glIsTexture	LIME_ES2_FUNCPTR(glIsTexture)
#define glLineWidth	LIME_ES2_FUNCPTR(glLineWidth)
#define glLinkProgram	LIME_ES2_FUNCPTR(glLinkProgram)
#define glPixelStorei	LIME_ES2_FUNCPTR(glPixelStorei)
#define glPolygonOffset	LIME_ES2_FUNCPTR(glPolygonOffset)
#define glReadPixels	LIME_ES2_FUNCPTR(glReadPixels)
#define glReleaseShaderCompiler	LIME_ES2_FUNCPTR(glReleaseShaderCompiler)
#define glRenderbufferStorage	LIME_ES2_FUNCPTR(glRenderbufferStorage)
#define glSampleCoverage	LIME_ES2_FUNCPTR(glSampleCoverage)
#define glScissor	LIME_ES2_FUNCPTR(glScissor)
#define glShaderBinary	LIME_ES2_FUNCPTR(glShaderBinary)
#define glShaderSource	LIME_ES2_FUNCPTR(glShaderSource)
#define glStencilFunc	LIME_ES2_FUNCPTR(glStencilFunc)
#define glStencilFuncSeparate	LIME_ES2_FUNCPTR(glStencilFuncSeparate)
#define glStencilMask	LIME_ES2_FUNCPTR(glStencilMask)
#define glStencilMaskSeparate	LIME_ES2_FUNCPTR(glStencilMaskSeparate)
#define glStencilOp	LIME_ES2_FUNCPTR(glStencilOp)
#define glStencilOpSeparate	LIME_ES2_FUNCPTR(glStencilOpSeparate)
#define glTexImage2D	LIME_ES2_FUNCPTR(glTexImage2D)
#define glTexParameterf	LIME_ES2_FUNCPTR(glTexParameterf)
#define glTexParameterfv	LIME_ES2_FUNCPTR(glTexParameterfv)
#define glTexParameteri	LIME_ES2_FUNCPTR(glTexParameteri)
#define glTexParameteriv	LIME_ES2_FUNCPTR(glTexParameteriv)
#define glTexSubImage2D	LIME_ES2_FUNCPTR(glTexSubImage2D)
#define glUniform1f	LIME_ES2_FUNCPTR(glUniform1f)
#define glUniform1fv	LIME_ES2_FUNCPTR(glUniform1fv)
#define glUniform1i	LIME_ES2_FUNCPTR(glUniform1i)
#define glUniform1iv	LIME_ES2_FUNCPTR(glUniform1iv)
#define glUniform2f	LIME_ES2_FUNCPTR(glUniform2f)
#define glUniform2fv	LIME_ES2_FUNCPTR(glUniform2fv)
#define glUniform2i	LIME_ES2_FUNCPTR(glUniform2i)
#define glUniform2iv	LIME_ES2_FUNCPTR(glUniform2iv)
#define glUniform3f	LIME_ES2_FUNCPTR(glUniform3f)
#define glUniform3fv	LIME_ES2_FUNCPTR(glUniform3fv)
#define glUniform3i	LIME_ES2_FUNCPTR(glUniform3i)
#define glUniform3iv	LIME_ES2_FUNCPTR(glUniform3iv)
#define glUniform4f	LIME_ES2_FUNCPTR(glUniform4f)
#define glUniform4fv	LIME_ES2_FUNCPTR(glUniform4fv)
#define glUniform4i	LIME_ES2_FUNCPTR(glUniform4i)
#define glUniform4iv	LIME_ES2_FUNCPTR(glUniform4iv)
#define glUniformMatrix2fv	LIME_ES2_FUNCPTR(glUniformMatrix2fv)
#define glUniformMatrix3fv	LIME_ES2_FUNCPTR(glUniformMatrix3fv)
#define glUniformMatrix4fv	LIME_ES2_FUNCPTR(glUniformMatrix4fv)
#define glUseProgram	LIME_ES2_FUNCPTR(glUseProgram)
#define glValidateProgram	LIME_ES2_FUNCPTR(glValidateProgram)
#define glVertexAttrib1f	LIME_ES2_FUNCPTR(glVertexAttrib1f)
#define glVertexAttrib1fv	LIME_ES2_FUNCPTR(glVertexAttrib1fv)
#define glVertexAttrib2f	LIME_ES2_FUNCPTR(glVertexAttrib2f)
#define glVertexAttrib2fv	LIME_ES2_FUNCPTR(glVertexAttrib2fv)
#define glVertexAttrib3f	LIME_ES2_FUNCPTR(glVertexAttrib3f)
#define glVertexAttrib3fv	LIME_ES2_FUNCPTR(glVertexAttrib3fv)
#define glVertexAttrib4f	LIME_ES2_FUNCPTR(glVertexAttrib4f)
#define glVertexAttrib4fv	LIME_ES2_FUNCPTR(glVertexAttrib4fv)
#define glVertexAttribPointer	LIME_ES2_FUNCPTR(glVertexAttribPointer)
#define glViewport	LIME_ES2_FUNCPTR(glViewport)

#endif //LIME_FUNCDEF_GLES2

#endif //LIME_INIT_GLES2

#ifdef __cplusplus
}
#endif
#endif //INC_LGRAPHICS_LGL2_H__
