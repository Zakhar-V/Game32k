#pragma  once

#define GL_FUNCDEF(R, N, ...) R (APIENTRY*N)(##__VA_ARGS__) = nullptr
#ifdef _DEBUG
#define GL_FUNCLOAD(N) N = reinterpret_cast<decltype(N)>(wglGetProcAddress(#N)); ++_numLoads; _numFails += N ? 0 : 1
#else
#define GL_FUNCLOAD(N) N = reinterpret_cast<decltype(N)>(wglGetProcAddress(#N))
#endif

// 1.2
//GL_FUNCDEF(void, glDrawRangeElements, GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
//GL_FUNCDEF(void, glTexImage3D, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
//GL_FUNCDEF(void, glTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
//GL_FUNCDEF(void, glCopyTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

// 1.3
GL_FUNCDEF(void, glActiveTexture, GLenum texture);
//GL_FUNCDEF(void, glSampleCoverage, GLfloat value, GLboolean invert);
//GL_FUNCDEF(void, glCompressedTexImage3D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
//GL_FUNCDEF(void, glCompressedTexImage2D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
//GL_FUNCDEF(void, glCompressedTexImage1D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data);
//GL_FUNCDEF(void, glCompressedTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
//GL_FUNCDEF(void, glCompressedTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
//GL_FUNCDEF(void, glCompressedTexSubImage1D, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
//GL_FUNCDEF(void, glGetCompressedTexImage, GLenum target, GLint level, void *img);

// 1.4
GL_FUNCDEF(void, glBlendFuncSeparate, GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
//GL_FUNCDEF(void, glMultiDrawArrays, GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount);
//GL_FUNCDEF(void, glMultiDrawElements, GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount);
//GL_FUNCDEF(void, glPointParameterf, GLenum pname, GLfloat param);
//GL_FUNCDEF(void, glPointParameterfv, GLenum pname, const GLfloat *params);
//GL_FUNCDEF(void, glPointParameteri, GLenum pname, GLint param);
//GL_FUNCDEF(void, glPointParameteriv, GLenum pname, const GLint *params);
GL_FUNCDEF(void, glBlendColor, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
//GL_FUNCDEF(void, glBlendEquation, GLenum mode);

// 1.5
//GL_FUNCDEF(void, glGenQueries, GLsizei n, GLuint *ids);
//GL_FUNCDEF(void, glDeleteQueries, GLsizei n, const GLuint *ids);
//GL_FUNCDEF(GLboolean, glIsQuery, GLuint id);
//GL_FUNCDEF(void, glBeginQuery, GLenum target, GLuint id);
//GL_FUNCDEF(void, glEndQuery, GLenum target);
//GL_FUNCDEF(void, glGetQueryiv, GLenum target, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetQueryObjectiv, GLuint id, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetQueryObjectuiv, GLuint id, GLenum pname, GLuint *params);
GL_FUNCDEF(void, glBindBuffer, GLenum target, GLuint buffer);
GL_FUNCDEF(void, glDeleteBuffers, GLsizei n, const GLuint *buffers);
GL_FUNCDEF(void, glGenBuffers, GLsizei n, GLuint *buffers);
//GL_FUNCDEF(GLboolean, glIsBuffer, GLuint buffer);
//GL_FUNCDEF(void, glBufferData, GLenum target, GLsizeiptr size, const void *data, GLenum usage);
//GL_FUNCDEF(void, glBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
//GL_FUNCDEF(void, glGetBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, void *data);
//GL_FUNCDEF(void *, glMapBuffer, GLenum target, GLenum access);
//GL_FUNCDEF(GLboolean, glUnmapBuffer, GLenum target);
//GL_FUNCDEF(void, glGetBufferParameteriv, GLenum target, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetBufferPointerv, GLenum target, GLenum pname, void **params);

// 2.0
GL_FUNCDEF(void, glBlendEquationSeparate, GLenum modeRGB, GLenum modeAlpha);
//GL_FUNCDEF(void, glDrawBuffers, GLsizei n, const GLenum *bufs);
GL_FUNCDEF(void, glStencilOpSeparate, GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
GL_FUNCDEF(void, glStencilFuncSeparate, GLenum face, GLenum func, GLint ref, GLuint mask);
//GL_FUNCDEF(void, glStencilMaskSeparate, GLenum face, GLuint mask);
//GL_FUNCDEF(void, glAttachShader, GLuint program, GLuint shader);
//GL_FUNCDEF(void, glBindAttribLocation, GLuint program, GLuint index, const GLchar *name);
//GL_FUNCDEF(void, glCompileShader, GLuint shader);
//GL_FUNCDEF(GLuint, glCreateProgram, void);
//GL_FUNCDEF(GLuint, glCreateShader, GLenum type);
//GL_FUNCDEF(void, glDeleteProgram, GLuint program);
//GL_FUNCDEF(void, glDeleteShader, GLuint shader);
//GL_FUNCDEF(void, glDetachShader, GLuint program, GLuint shader);
//GL_FUNCDEF(void, glDisableVertexAttribArray, GLuint index);
GL_FUNCDEF(void, glEnableVertexAttribArray, GLuint index);
//GL_FUNCDEF(void, glGetActiveAttrib, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
//GL_FUNCDEF(void, glGetActiveUniform, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
//GL_FUNCDEF(void, glGetAttachedShaders, GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
//GL_FUNCDEF(GLint, glGetAttribLocation, GLuint program, const GLchar *name);
GL_FUNCDEF(void, glGetProgramiv, GLuint program, GLenum pname, GLint *params);
#ifdef _DEBUG_OUTPUT
GL_FUNCDEF(void, glGetProgramInfoLog, GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
#endif
//GL_FUNCDEF(void, glGetShaderiv, GLuint shader, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetShaderInfoLog, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
//GL_FUNCDEF(void, glGetShaderSource, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
GL_FUNCDEF(GLint, glGetUniformLocation, GLuint program, const GLchar *name);
//GL_FUNCDEF(void, glGetUniformfv, GLuint program, GLint location, GLfloat *params);
//GL_FUNCDEF(void, glGetUniformiv, GLuint program, GLint location, GLint *params);
//GL_FUNCDEF(void, glGetVertexAttribdv, GLuint index, GLenum pname, GLdouble *params);
//GL_FUNCDEF(void, glGetVertexAttribfv, GLuint index, GLenum pname, GLfloat *params);
//GL_FUNCDEF(void, glGetVertexAttribiv, GLuint index, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetVertexAttribPointerv, GLuint index, GLenum pname, void **pointer);
//GL_FUNCDEF(GLboolean, glIsProgram, GLuint program);
//GL_FUNCDEF(GLboolean, glIsShader, GLuint shader);
//GL_FUNCDEF(void, glLinkProgram, GLuint program);
//GL_FUNCDEF(void, glShaderSource, GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
//GL_FUNCDEF(void, glUseProgram, GLuint program);
//GL_FUNCDEF(void, glUniform1f, GLint location, GLfloat v0);
//GL_FUNCDEF(void, glUniform2f, GLint location, GLfloat v0, GLfloat v1);
//GL_FUNCDEF(void, glUniform3f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
//GL_FUNCDEF(void, glUniform4f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
//GL_FUNCDEF(void, glUniform1i, GLint location, GLint v0);
//GL_FUNCDEF(void, glUniform2i, GLint location, GLint v0, GLint v1);
//GL_FUNCDEF(void, glUniform3i, GLint location, GLint v0, GLint v1, GLint v2);
//GL_FUNCDEF(void, glUniform4i, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
//GL_FUNCDEF(void, glUniform1fv, GLint location, GLsizei count, const GLfloat *value);
//GL_FUNCDEF(void, glUniform2fv, GLint location, GLsizei count, const GLfloat *value);
//GL_FUNCDEF(void, glUniform3fv, GLint location, GLsizei count, const GLfloat *value);
//GL_FUNCDEF(void, glUniform4fv, GLint location, GLsizei count, const GLfloat *value);
//GL_FUNCDEF(void, glUniform1iv, GLint location, GLsizei count, const GLint *value);
//GL_FUNCDEF(void, glUniform2iv, GLint location, GLsizei count, const GLint *value);
//GL_FUNCDEF(void, glUniform3iv, GLint location, GLsizei count, const GLint *value);
//GL_FUNCDEF(void, glUniform4iv, GLint location, GLsizei count, const GLint *value);
//GL_FUNCDEF(void, glUniformMatrix2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glUniformMatrix3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glUniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glValidateProgram, GLuint program);
//GL_FUNCDEF(void, glVertexAttrib1d, GLuint index, GLdouble x);
//GL_FUNCDEF(void, glVertexAttrib1dv, GLuint index, const GLdouble *v);
//GL_FUNCDEF(void, glVertexAttrib1f, GLuint index, GLfloat x);
//GL_FUNCDEF(void, glVertexAttrib1fv, GLuint index, const GLfloat *v);
//GL_FUNCDEF(void, glVertexAttrib1s, GLuint index, GLshort x);
//GL_FUNCDEF(void, glVertexAttrib1sv, GLuint index, const GLshort *v);
//GL_FUNCDEF(void, glVertexAttrib2d, GLuint index, GLdouble x, GLdouble y);
//GL_FUNCDEF(void, glVertexAttrib2dv, GLuint index, const GLdouble *v);
//GL_FUNCDEF(void, glVertexAttrib2f, GLuint index, GLfloat x, GLfloat y);
//GL_FUNCDEF(void, glVertexAttrib2fv, GLuint index, const GLfloat *v);
//GL_FUNCDEF(void, glVertexAttrib2s, GLuint index, GLshort x, GLshort y);
//GL_FUNCDEF(void, glVertexAttrib2sv, GLuint index, const GLshort *v);
//GL_FUNCDEF(void, glVertexAttrib3d, GLuint index, GLdouble x, GLdouble y, GLdouble z);
//GL_FUNCDEF(void, glVertexAttrib3dv, GLuint index, const GLdouble *v);
//GL_FUNCDEF(void, glVertexAttrib3f, GLuint index, GLfloat x, GLfloat y, GLfloat z);
//GL_FUNCDEF(void, glVertexAttrib3fv, GLuint index, const GLfloat *v);
//GL_FUNCDEF(void, glVertexAttrib3s, GLuint index, GLshort x, GLshort y, GLshort z);
//GL_FUNCDEF(void, glVertexAttrib3sv, GLuint index, const GLshort *v);
//GL_FUNCDEF(void, glVertexAttrib4Nbv, GLuint index, const GLbyte *v);
//GL_FUNCDEF(void, glVertexAttrib4Niv, GLuint index, const GLint *v);
//GL_FUNCDEF(void, glVertexAttrib4Nsv, GLuint index, const GLshort *v);
//GL_FUNCDEF(void, glVertexAttrib4Nub, GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
//GL_FUNCDEF(void, glVertexAttrib4Nubv, GLuint index, const GLubyte *v);
//GL_FUNCDEF(void, glVertexAttrib4Nuiv, GLuint index, const GLuint *v);
//GL_FUNCDEF(void, glVertexAttrib4Nusv, GLuint index, const GLushort *v);
//GL_FUNCDEF(void, glVertexAttrib4bv, GLuint index, const GLbyte *v);
//GL_FUNCDEF(void, glVertexAttrib4d, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
//GL_FUNCDEF(void, glVertexAttrib4dv, GLuint index, const GLdouble *v);
//GL_FUNCDEF(void, glVertexAttrib4f, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
//GL_FUNCDEF(void, glVertexAttrib4fv, GLuint index, const GLfloat *v);
//GL_FUNCDEF(void, glVertexAttrib4iv, GLuint index, const GLint *v);
//GL_FUNCDEF(void, glVertexAttrib4s, GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
//GL_FUNCDEF(void, glVertexAttrib4sv, GLuint index, const GLshort *v);
//GL_FUNCDEF(void, glVertexAttrib4ubv, GLuint index, const GLubyte *v);
//GL_FUNCDEF(void, glVertexAttrib4uiv, GLuint index, const GLuint *v);
//GL_FUNCDEF(void, glVertexAttrib4usv, GLuint index, const GLushort *v);
//GL_FUNCDEF(void, glVertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

// 2.1
//GL_FUNCDEF(void, glUniformMatrix2x3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glUniformMatrix3x2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glUniformMatrix2x4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glUniformMatrix4x2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glUniformMatrix3x4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glUniformMatrix4x3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

// 3.0
//GL_FUNCDEF(void, glColorMaski, GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
//GL_FUNCDEF(void, glGetBooleani_v, GLenum target, GLuint index, GLboolean *data);
//GL_FUNCDEF(void, glGetIntegeri_v, GLenum target, GLuint index, GLint *data);
//GL_FUNCDEF(void, glEnablei, GLenum target, GLuint index);
//GL_FUNCDEF(void, glDisablei, GLenum target, GLuint index);
//GL_FUNCDEF(GLboolean, glIsEnabledi, GLenum target, GLuint index);
//GL_FUNCDEF(void, glBeginTransformFeedback, GLenum primitiveMode);
//GL_FUNCDEF(void, glEndTransformFeedback, void);
GL_FUNCDEF(void, glBindBufferRange, GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
//GL_FUNCDEF(void, glBindBufferBase, GLenum target, GLuint index, GLuint buffer);
//GL_FUNCDEF(void, glTransformFeedbackVaryings, GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode);
//GL_FUNCDEF(void, glGetTransformFeedbackVarying, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
//GL_FUNCDEF(void, glClampColor, GLenum target, GLenum clamp);
//GL_FUNCDEF(void, glBeginConditionalRender, GLuint id, GLenum mode);
//GL_FUNCDEF(void, glEndConditionalRender, void);
//GL_FUNCDEF(void, glVertexAttribIPointer, GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
//GL_FUNCDEF(void, glGetVertexAttribIiv, GLuint index, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetVertexAttribIuiv, GLuint index, GLenum pname, GLuint *params);
//GL_FUNCDEF(void, glVertexAttribI1i, GLuint index, GLint x);
//GL_FUNCDEF(void, glVertexAttribI2i, GLuint index, GLint x, GLint y);
//GL_FUNCDEF(void, glVertexAttribI3i, GLuint index, GLint x, GLint y, GLint z);
//GL_FUNCDEF(void, glVertexAttribI4i, GLuint index, GLint x, GLint y, GLint z, GLint w);
//GL_FUNCDEF(void, glVertexAttribI1ui, GLuint index, GLuint x);
//GL_FUNCDEF(void, glVertexAttribI2ui, GLuint index, GLuint x, GLuint y);
//GL_FUNCDEF(void, glVertexAttribI3ui, GLuint index, GLuint x, GLuint y, GLuint z);
//GL_FUNCDEF(void, glVertexAttribI4ui, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
//GL_FUNCDEF(void, glVertexAttribI1iv, GLuint index, const GLint *v);
//GL_FUNCDEF(void, glVertexAttribI2iv, GLuint index, const GLint *v);
//GL_FUNCDEF(void, glVertexAttribI3iv, GLuint index, const GLint *v);
//GL_FUNCDEF(void, glVertexAttribI4iv, GLuint index, const GLint *v);
//GL_FUNCDEF(void, glVertexAttribI1uiv, GLuint index, const GLuint *v);
//GL_FUNCDEF(void, glVertexAttribI2uiv, GLuint index, const GLuint *v);
//GL_FUNCDEF(void, glVertexAttribI3uiv, GLuint index, const GLuint *v);
//GL_FUNCDEF(void, glVertexAttribI4uiv, GLuint index, const GLuint *v);
//GL_FUNCDEF(void, glVertexAttribI4bv, GLuint index, const GLbyte *v);
//GL_FUNCDEF(void, glVertexAttribI4sv, GLuint index, const GLshort *v);
//GL_FUNCDEF(void, glVertexAttribI4ubv, GLuint index, const GLubyte *v);
//GL_FUNCDEF(void, glVertexAttribI4usv, GLuint index, const GLushort *v);
//GL_FUNCDEF(void, glGetUniformuiv, GLuint program, GLint location, GLuint *params);
//GL_FUNCDEF(void, glBindFragDataLocation, GLuint program, GLuint color, const GLchar *name);
//GL_FUNCDEF(GLint, glGetFragDataLocation, GLuint program, const GLchar *name);
//GL_FUNCDEF(void, glUniform1ui, GLint location, GLuint v0);
//GL_FUNCDEF(void, glUniform2ui, GLint location, GLuint v0, GLuint v1);
//GL_FUNCDEF(void, glUniform3ui, GLint location, GLuint v0, GLuint v1, GLuint v2);
//GL_FUNCDEF(void, glUniform4ui, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
//GL_FUNCDEF(void, glUniform1uiv, GLint location, GLsizei count, const GLuint *value);
//GL_FUNCDEF(void, glUniform2uiv, GLint location, GLsizei count, const GLuint *value);
//GL_FUNCDEF(void, glUniform3uiv, GLint location, GLsizei count, const GLuint *value);
//GL_FUNCDEF(void, glUniform4uiv, GLint location, GLsizei count, const GLuint *value);
//GL_FUNCDEF(void, glTexParameterIiv, GLenum target, GLenum pname, const GLint *params);
//GL_FUNCDEF(void, glTexParameterIuiv, GLenum target, GLenum pname, const GLuint *params);
//GL_FUNCDEF(void, glGetTexParameterIiv, GLenum target, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetTexParameterIuiv, GLenum target, GLenum pname, GLuint *params);
//GL_FUNCDEF(void, glClearBufferiv, GLenum buffer, GLint drawbuffer, const GLint *value);
//GL_FUNCDEF(void, glClearBufferuiv, GLenum buffer, GLint drawbuffer, const GLuint *value);
//GL_FUNCDEF(void, glClearBufferfv, GLenum buffer, GLint drawbuffer, const GLfloat *value);
//GL_FUNCDEF(void, glClearBufferfi, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
GL_FUNCDEF(const GLubyte *, glGetStringi, GLenum name, GLuint index);
//GL_FUNCDEF(GLboolean, glIsRenderbuffer, GLuint renderbuffer);
GL_FUNCDEF(void, glBindRenderbuffer, GLenum target, GLuint renderbuffer);
GL_FUNCDEF(void, glDeleteRenderbuffers, GLsizei n, const GLuint *renderbuffers);
GL_FUNCDEF(void, glGenRenderbuffers, GLsizei n, GLuint *renderbuffers);
//GL_FUNCDEF(void, glRenderbufferStorage, GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
//GL_FUNCDEF(void, glGetRenderbufferParameteriv, GLenum target, GLenum pname, GLint *params);
//GL_FUNCDEF(GLboolean, glIsFramebuffer, GLuint framebuffer);
GL_FUNCDEF(void, glBindFramebuffer, GLenum target, GLuint framebuffer);
GL_FUNCDEF(void, glDeleteFramebuffers, GLsizei n, const GLuint *framebuffers);
GL_FUNCDEF(void, glGenFramebuffers, GLsizei n, GLuint *framebuffers);
#ifdef _DEBUG
//GL_FUNCDEF(GLenum, glCheckFramebufferStatus, GLenum target);
#endif
//GL_FUNCDEF(void, glFramebufferTexture1D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
//GL_FUNCDEF(void, glFramebufferTexture2D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
//GL_FUNCDEF(void, glFramebufferTexture3D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
//GL_FUNCDEF(void, glFramebufferRenderbuffer, GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
//GL_FUNCDEF(void, glGetFramebufferAttachmentParameteriv, GLenum target, GLenum attachment, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGenerateMipmap, GLenum target);
GL_FUNCDEF(void, glBlitFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
//GL_FUNCDEF(void, glRenderbufferStorageMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
//GL_FUNCDEF(void, glFramebufferTextureLayer, GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
//GL_FUNCDEF(void*, glMapBufferRange, GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
//GL_FUNCDEF(void, glFlushMappedBufferRange, GLenum target, GLintptr offset, GLsizeiptr length);
GL_FUNCDEF(void, glBindVertexArray, GLuint array);
//GL_FUNCDEF(void, glDeleteVertexArrays, GLsizei n, const GLuint *arrays);
GL_FUNCDEF(void, glGenVertexArrays, GLsizei n, GLuint *arrays);
//GL_FUNCDEF(GLboolean, glIsVertexArray, GLuint array);

// 3.1
GL_FUNCDEF(void, glDrawArraysInstanced, GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
//GL_FUNCDEF(void, glDrawElementsInstanced, GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
//GL_FUNCDEF(void, glTexBuffer, GLenum target, GLenum internalformat, GLuint buffer);
//GL_FUNCDEF(void, glPrimitiveRestartIndex, GLuint index);
//GL_FUNCDEF(void, glCopyBufferSubData, GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
//GL_FUNCDEF(void, glGetUniformIndices, GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices);
//GL_FUNCDEF(void, glGetActiveUniformsiv, GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetActiveUniformName, GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName);
//GL_FUNCDEF(GLuint, glGetUniformBlockIndex, GLuint program, const GLchar *uniformBlockName);
//GL_FUNCDEF(void, glGetActiveUniformBlockiv, GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetActiveUniformBlockName, GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
//GL_FUNCDEF(void, glUniformBlockBinding, GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);

// 3.2
//GL_FUNCDEF(void, glDrawElementsBaseVertex, GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);
//GL_FUNCDEF(void, glDrawRangeElementsBaseVertex, GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex);
GL_FUNCDEF(void, glDrawElementsInstancedBaseVertex, GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex);
//GL_FUNCDEF(void, glMultiDrawElementsBaseVertex, GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount, const GLint *basevertex);
//GL_FUNCDEF(void, glProvokingVertex, GLenum mode);
//GL_FUNCDEF(GLsync, glFenceSync, GLenum condition, GLbitfield flags);
//GL_FUNCDEF(GLboolean, glIsSync, GLsync sync);
//GL_FUNCDEF(void, glDeleteSync, GLsync sync);
//GL_FUNCDEF(GLenum, glClientWaitSync, GLsync sync, GLbitfield flags, GLuint64 timeout);
//GL_FUNCDEF(void, glWaitSync, GLsync sync, GLbitfield flags, GLuint64 timeout);
//GL_FUNCDEF(void, glGetInteger64v, GLenum pname, GLint64 *data);
//GL_FUNCDEF(void, glGetSynciv, GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
//GL_FUNCDEF(void, glGetInteger64i_v, GLenum target, GLuint index, GLint64 *data);
//GL_FUNCDEF(void, glGetBufferParameteri64v, GLenum target, GLenum pname, GLint64 *params);
//GL_FUNCDEF(void, glFramebufferTexture, GLenum target, GLenum attachment, GLuint texture, GLint level);
//GL_FUNCDEF(void, glTexImage2DMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
//GL_FUNCDEF(void, glTexImage3DMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
//GL_FUNCDEF(void, glGetMultisamplefv, GLenum pname, GLuint index, GLfloat *val);
//GL_FUNCDEF(void, glSampleMaski, GLuint maskNumber, GLbitfield mask);

// 3.3
//GL_FUNCDEF(void, glBindFragDataLocationIndexed, GLuint program, GLuint colorNumber, GLuint index, const GLchar *name);
//GL_FUNCDEF(GLint, glGetFragDataIndex, GLuint program, const GLchar *name);
GL_FUNCDEF(void, glGenSamplers, GLsizei count, GLuint *samplers);
GL_FUNCDEF(void, glDeleteSamplers, GLsizei count, const GLuint *samplers);
//GL_FUNCDEF(GLboolean, glIsSampler, GLuint sampler);
GL_FUNCDEF(void, glBindSampler, GLuint unit, GLuint sampler);
GL_FUNCDEF(void, glSamplerParameteri, GLuint sampler, GLenum pname, GLint param);
//GL_FUNCDEF(void, glSamplerParameteriv, GLuint sampler, GLenum pname, const GLint *param);
GL_FUNCDEF(void, glSamplerParameterf, GLuint sampler, GLenum pname, GLfloat param);
//GL_FUNCDEF(void, glSamplerParameterfv, GLuint sampler, GLenum pname, const GLfloat *param);
//GL_FUNCDEF(void, glSamplerParameterIiv, GLuint sampler, GLenum pname, const GLint *param);
//GL_FUNCDEF(void, glSamplerParameterIuiv, GLuint sampler, GLenum pname, const GLuint *param);
//GL_FUNCDEF(void, glGetSamplerParameteriv, GLuint sampler, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetSamplerParameterIiv, GLuint sampler, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetSamplerParameterfv, GLuint sampler, GLenum pname, GLfloat *params);
//GL_FUNCDEF(void, glGetSamplerParameterIuiv, GLuint sampler, GLenum pname, GLuint *params);
//GL_FUNCDEF(void, glQueryCounter, GLuint id, GLenum target);
//GL_FUNCDEF(void, glGetQueryObjecti64v, GLuint id, GLenum pname, GLint64 *params);
//GL_FUNCDEF(void, glGetQueryObjectui64v, GLuint id, GLenum pname, GLuint64 *params);
//GL_FUNCDEF(void, glVertexAttribDivisor, GLuint index, GLuint divisor);
//GL_FUNCDEF(void, glVertexAttribP1ui, GLuint index, GLenum type, GLboolean normalized, GLuint value);
//GL_FUNCDEF(void, glVertexAttribP1uiv, GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
//GL_FUNCDEF(void, glVertexAttribP2ui, GLuint index, GLenum type, GLboolean normalized, GLuint value);
//GL_FUNCDEF(void, glVertexAttribP2uiv, GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
//GL_FUNCDEF(void, glVertexAttribP3ui, GLuint index, GLenum type, GLboolean normalized, GLuint value);
//GL_FUNCDEF(void, glVertexAttribP3uiv, GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
//GL_FUNCDEF(void, glVertexAttribP4ui, GLuint index, GLenum type, GLboolean normalized, GLuint value);
//GL_FUNCDEF(void, glVertexAttribP4uiv, GLuint index, GLenum type, GLboolean normalized, const GLuint *value);

// 4.0
//GL_FUNCDEF(void, glMinSampleShading, GLfloat value);
//GL_FUNCDEF(void, glBlendEquationi, GLuint buf, GLenum mode);
//GL_FUNCDEF(void, glBlendEquationSeparatei, GLuint buf, GLenum modeRGB, GLenum modeAlpha);
//GL_FUNCDEF(void, glBlendFunci, GLuint buf, GLenum src, GLenum dst);
//GL_FUNCDEF(void, glBlendFuncSeparatei, GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
//GL_FUNCDEF(void, glDrawArraysIndirect, GLenum mode, const void *indirect);
//GL_FUNCDEF(void, glDrawElementsIndirect, GLenum mode, GLenum type, const void *indirect);
//GL_FUNCDEF(void, glUniform1d, GLint location, GLdouble x);
//GL_FUNCDEF(void, glUniform2d, GLint location, GLdouble x, GLdouble y);
//GL_FUNCDEF(void, glUniform3d, GLint location, GLdouble x, GLdouble y, GLdouble z);
//GL_FUNCDEF(void, glUniform4d, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
//GL_FUNCDEF(void, glUniform1dv, GLint location, GLsizei count, const GLdouble *value);
//GL_FUNCDEF(void, glUniform2dv, GLint location, GLsizei count, const GLdouble *value);
//GL_FUNCDEF(void, glUniform3dv, GLint location, GLsizei count, const GLdouble *value);
//GL_FUNCDEF(void, glUniform4dv, GLint location, GLsizei count, const GLdouble *value);
//GL_FUNCDEF(void, glUniformMatrix2dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glUniformMatrix3dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glUniformMatrix4dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glUniformMatrix2x3dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glUniformMatrix2x4dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glUniformMatrix3x2dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glUniformMatrix3x4dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glUniformMatrix4x2dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glUniformMatrix4x3dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glGetUniformdv, GLuint program, GLint location, GLdouble *params);
//GL_FUNCDEF(GLint, glGetSubroutineUniformLocation, GLuint program, GLenum shadertype, const GLchar *name);
//GL_FUNCDEF(GLuint, glGetSubroutineIndex, GLuint program, GLenum shadertype, const GLchar *name);
//GL_FUNCDEF(void, glGetActiveSubroutineUniformiv, GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint *values);
//GL_FUNCDEF(void, glGetActiveSubroutineUniformName, GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
//GL_FUNCDEF(void, glGetActiveSubroutineName, GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
//GL_FUNCDEF(void, glUniformSubroutinesuiv, GLenum shadertype, GLsizei count, const GLuint *indices);
//GL_FUNCDEF(void, glGetUniformSubroutineuiv, GLenum shadertype, GLint location, GLuint *params);
//GL_FUNCDEF(void, glGetProgramStageiv, GLuint program, GLenum shadertype, GLenum pname, GLint *values);
//GL_FUNCDEF(void, glPatchParameteri, GLenum pname, GLint value);
//GL_FUNCDEF(void, glPatchParameterfv, GLenum pname, const GLfloat *values);
//GL_FUNCDEF(void, glBindTransformFeedback, GLenum target, GLuint id);
//GL_FUNCDEF(void, glDeleteTransformFeedbacks, GLsizei n, const GLuint *ids);
//GL_FUNCDEF(void, glGenTransformFeedbacks, GLsizei n, GLuint *ids);
//GL_FUNCDEF(GLboolean, glIsTransformFeedback, GLuint id);
//GL_FUNCDEF(void, glPauseTransformFeedback, void);
//GL_FUNCDEF(void, glResumeTransformFeedback, void);
//GL_FUNCDEF(void, glDrawTransformFeedback, GLenum mode, GLuint id);
//GL_FUNCDEF(void, glDrawTransformFeedbackStream, GLenum mode, GLuint id, GLuint stream);
//GL_FUNCDEF(void, glBeginQueryIndexed, GLenum target, GLuint index, GLuint id);
//GL_FUNCDEF(void, glEndQueryIndexed, GLenum target, GLuint index);
//GL_FUNCDEF(void, glGetQueryIndexediv, GLenum target, GLuint index, GLenum pname, GLint *params);

// 4.1 (GL_ARB_separate_shader_objects)
//GL_FUNCDEF(void, glReleaseShaderCompiler, void);
//GL_FUNCDEF(void, glShaderBinary, GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length);
//GL_FUNCDEF(void, glGetShaderPrecisionFormat, GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
//GL_FUNCDEF(void, glDepthRangef, GLfloat n, GLfloat f);
//GL_FUNCDEF(void, glClearDepthf, GLfloat d);
//GL_FUNCDEF(void, glGetProgramBinary, GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);
//GL_FUNCDEF(void, glProgramBinary, GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);
//GL_FUNCDEF(void, glProgramParameteri, GLuint program, GLenum pname, GLint value);
GL_FUNCDEF(void, glUseProgramStages, GLuint pipeline, GLbitfield stages, GLuint program);
GL_FUNCDEF(void, glActiveShaderProgram, GLuint pipeline, GLuint program);
GL_FUNCDEF(GLuint, glCreateShaderProgramv, GLenum type, GLsizei count, const GLchar *const*strings);
GL_FUNCDEF(void, glBindProgramPipeline, GLuint pipeline);
GL_FUNCDEF(void, glDeleteProgramPipelines, GLsizei n, const GLuint *pipelines);
GL_FUNCDEF(void, glGenProgramPipelines, GLsizei n, GLuint *pipelines);
//GL_FUNCDEF(GLboolean, glIsProgramPipeline, GLuint pipeline);
#ifdef _DEBUG_OUTPUT
GL_FUNCDEF(void, glGetProgramPipelineiv, GLuint pipeline, GLenum pname, GLint *params);
#endif
GL_FUNCDEF(void, glProgramUniform1i, GLuint program, GLint location, GLint v0);
//GL_FUNCDEF(void, glProgramUniform1iv, GLuint program, GLint location, GLsizei count, const GLint *value);
//GL_FUNCDEF(void, glProgramUniform1f, GLuint program, GLint location, GLfloat v0);
//GL_FUNCDEF(void, glProgramUniform1fv, GLuint program, GLint location, GLsizei count, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniform1d, GLuint program, GLint location, GLdouble v0);
//GL_FUNCDEF(void, glProgramUniform1dv, GLuint program, GLint location, GLsizei count, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniform1ui, GLuint program, GLint location, GLuint v0);
//GL_FUNCDEF(void, glProgramUniform1uiv, GLuint program, GLint location, GLsizei count, const GLuint *value);
//GL_FUNCDEF(void, glProgramUniform2i, GLuint program, GLint location, GLint v0, GLint v1);
//GL_FUNCDEF(void, glProgramUniform2iv, GLuint program, GLint location, GLsizei count, const GLint *value);
//GL_FUNCDEF(void, glProgramUniform2f, GLuint program, GLint location, GLfloat v0, GLfloat v1);
//GL_FUNCDEF(void, glProgramUniform2fv, GLuint program, GLint location, GLsizei count, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniform2d, GLuint program, GLint location, GLdouble v0, GLdouble v1);
//GL_FUNCDEF(void, glProgramUniform2dv, GLuint program, GLint location, GLsizei count, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniform2ui, GLuint program, GLint location, GLuint v0, GLuint v1);
//GL_FUNCDEF(void, glProgramUniform2uiv, GLuint program, GLint location, GLsizei count, const GLuint *value);
//GL_FUNCDEF(void, glProgramUniform3i, GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
//GL_FUNCDEF(void, glProgramUniform3iv, GLuint program, GLint location, GLsizei count, const GLint *value);
//GL_FUNCDEF(void, glProgramUniform3f, GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
//GL_FUNCDEF(void, glProgramUniform3fv, GLuint program, GLint location, GLsizei count, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniform3d, GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2);
//GL_FUNCDEF(void, glProgramUniform3dv, GLuint program, GLint location, GLsizei count, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniform3ui, GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
//GL_FUNCDEF(void, glProgramUniform3uiv, GLuint program, GLint location, GLsizei count, const GLuint *value);
//GL_FUNCDEF(void, glProgramUniform4i, GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
//GL_FUNCDEF(void, glProgramUniform4iv, GLuint program, GLint location, GLsizei count, const GLint *value);
//GL_FUNCDEF(void, glProgramUniform4f, GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
//GL_FUNCDEF(void, glProgramUniform4fv, GLuint program, GLint location, GLsizei count, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniform4d, GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
//GL_FUNCDEF(void, glProgramUniform4dv, GLuint program, GLint location, GLsizei count, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniform4ui, GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
//GL_FUNCDEF(void, glProgramUniform4uiv, GLuint program, GLint location, GLsizei count, const GLuint *value);
//GL_FUNCDEF(void, glProgramUniformMatrix2fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniformMatrix3fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniformMatrix4fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniformMatrix2dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniformMatrix3dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniformMatrix4dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniformMatrix2x3fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniformMatrix3x2fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniformMatrix2x4fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniformMatrix4x2fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniformMatrix3x4fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniformMatrix4x3fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniformMatrix2x3dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniformMatrix3x2dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniformMatrix2x4dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniformMatrix4x2dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniformMatrix3x4dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniformMatrix4x3dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
#ifdef _DEBUG_OUTPUT
GL_FUNCDEF(void, glValidateProgramPipeline, GLuint pipeline);
GL_FUNCDEF(void, glGetProgramPipelineInfoLog, GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
#endif
//GL_FUNCDEF(void, glVertexAttribL1d, GLuint index, GLdouble x);
//GL_FUNCDEF(void, glVertexAttribL2d, GLuint index, GLdouble x, GLdouble y);
//GL_FUNCDEF(void, glVertexAttribL3d, GLuint index, GLdouble x, GLdouble y, GLdouble z);
//GL_FUNCDEF(void, glVertexAttribL4d, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
//GL_FUNCDEF(void, glVertexAttribL1dv, GLuint index, const GLdouble *v);
//GL_FUNCDEF(void, glVertexAttribL2dv, GLuint index, const GLdouble *v);
//GL_FUNCDEF(void, glVertexAttribL3dv, GLuint index, const GLdouble *v);
//GL_FUNCDEF(void, glVertexAttribL4dv, GLuint index, const GLdouble *v);
//GL_FUNCDEF(void, glVertexAttribLPointer, GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
//GL_FUNCDEF(void, glGetVertexAttribLdv, GLuint index, GLenum pname, GLdouble *params);
//GL_FUNCDEF(void, glViewportArrayv, GLuint first, GLsizei count, const GLfloat *v);
//GL_FUNCDEF(void, glViewportIndexedf, GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
//GL_FUNCDEF(void, glViewportIndexedfv, GLuint index, const GLfloat *v);
//GL_FUNCDEF(void, glScissorArrayv, GLuint first, GLsizei count, const GLint *v);
//GL_FUNCDEF(void, glScissorIndexed, GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
//GL_FUNCDEF(void, glScissorIndexedv, GLuint index, const GLint *v);
//GL_FUNCDEF(void, glDepthRangeArrayv, GLuint first, GLsizei count, const GLdouble *v);
//GL_FUNCDEF(void, glDepthRangeIndexed, GLuint index, GLdouble n, GLdouble f);
//GL_FUNCDEF(void, glGetFloati_v, GLenum target, GLuint index, GLfloat *data);
//GL_FUNCDEF(void, glGetDoublei_v, GLenum target, GLuint index, GLdouble *data);

// 4.2
//GL_FUNCDEF(void, glDrawArraysInstancedBaseInstance, GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance);
//GL_FUNCDEF(void, glDrawElementsInstancedBaseInstance, GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLuint baseinstance);
//GL_FUNCDEF(void, glDrawElementsInstancedBaseVertexBaseInstance, GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance);
//GL_FUNCDEF(void, glGetInternalformativ, GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params);
//GL_FUNCDEF(void, glGetActiveAtomicCounterBufferiv, GLuint program, GLuint bufferIndex, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glBindImageTexture, GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
//GL_FUNCDEF(void, glMemoryBarrier, GLbitfield barriers);
//GL_FUNCDEF(void, glTexStorage1D, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
//GL_FUNCDEF(void, glTexStorage2D, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
//GL_FUNCDEF(void, glTexStorage3D, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
//GL_FUNCDEF(void, glDrawTransformFeedbackInstanced, GLenum mode, GLuint id, GLsizei instancecount);
//GL_FUNCDEF(void, glDrawTransformFeedbackStreamInstanced, GLenum mode, GLuint id, GLuint stream, GLsizei instancecount);

// 4.3 (GL_ARB_vertex_attrib_binding, GL_ARB_debug_output)
//GL_FUNCDEF(void, glClearBufferData, GLenum target, GLenum internalformat, GLenum format, GLenum type, const void *data);
//GL_FUNCDEF(void, glClearBufferSubData, GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
//GL_FUNCDEF(void, glDispatchCompute, GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
//GL_FUNCDEF(void, glDispatchComputeIndirect, GLintptr indirect);
//GL_FUNCDEF(void, glCopyImageSubData, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
//GL_FUNCDEF(void, glFramebufferParameteri, GLenum target, GLenum pname, GLint param);
//GL_FUNCDEF(void, glGetFramebufferParameteriv, GLenum target, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetInternalformati64v, GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64 *params);
//GL_FUNCDEF(void, glInvalidateTexSubImage, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);
//GL_FUNCDEF(void, glInvalidateTexImage, GLuint texture, GLint level);
//GL_FUNCDEF(void, glInvalidateBufferSubData, GLuint buffer, GLintptr offset, GLsizeiptr length);
//GL_FUNCDEF(void, glInvalidateBufferData, GLuint buffer);
//GL_FUNCDEF(void, glInvalidateFramebuffer, GLenum target, GLsizei numAttachments, const GLenum *attachments);
//GL_FUNCDEF(void, glInvalidateSubFramebuffer, GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
//GL_FUNCDEF(void, glMultiDrawArraysIndirect, GLenum mode, const void *indirect, GLsizei drawcount, GLsizei stride);
//GL_FUNCDEF(void, glMultiDrawElementsIndirect, GLenum mode, GLenum type, const void *indirect, GLsizei drawcount, GLsizei stride);
//GL_FUNCDEF(void, glGetProgramInterfaceiv, GLuint program, GLenum programInterface, GLenum pname, GLint *params);
//GL_FUNCDEF(GLuint, glGetProgramResourceIndex, GLuint program, GLenum programInterface, const GLchar *name);
//GL_FUNCDEF(void, glGetProgramResourceName, GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);
//GL_FUNCDEF(void, glGetProgramResourceiv, GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params);
//GL_FUNCDEF(GLint, glGetProgramResourceLocation, GLuint program, GLenum programInterface, const GLchar *name);
//GL_FUNCDEF(GLint, glGetProgramResourceLocationIndex, GLuint program, GLenum programInterface, const GLchar *name);
//GL_FUNCDEF(void, glShaderStorageBlockBinding, GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);
//GL_FUNCDEF(void, glTexBufferRange, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
//GL_FUNCDEF(void, glTexStorage2DMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
//GL_FUNCDEF(void, glTexStorage3DMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
//GL_FUNCDEF(void, glTextureView, GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
GL_FUNCDEF(void, glBindVertexBuffer, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
GL_FUNCDEF(void, glVertexAttribFormat, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
GL_FUNCDEF(void, glVertexAttribIFormat, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
//GL_FUNCDEF(void, glVertexAttribLFormat, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
GL_FUNCDEF(void, glVertexAttribBinding, GLuint attribindex, GLuint bindingindex);
GL_FUNCDEF(void, glVertexBindingDivisor, GLuint bindingindex, GLuint divisor);
#ifdef _DEBUG_OUTPUT
GL_FUNCDEF(void, glDebugMessageControl, GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
//GL_FUNCDEF(void, glDebugMessageInsert, GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
GL_FUNCDEF(void, glDebugMessageCallback, GLDEBUGPROC callback, const void *userParam);
//GL_FUNCDEF(GLuint, glGetDebugMessageLog, GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
//GL_FUNCDEF(void, glPushDebugGroup, GLenum source, GLuint id, GLsizei length, const GLchar *message);
//GL_FUNCDEF(void, glPopDebugGroup, void);
//GL_FUNCDEF(void, glObjectLabel, GLenum identifier, GLuint name, GLsizei length, const GLchar *label);
//GL_FUNCDEF(void, glGetObjectLabel, GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label);
//GL_FUNCDEF(void, glObjectPtrLabel, const void *ptr, GLsizei length, const GLchar *label);
//GL_FUNCDEF(void, glGetObjectPtrLabel, const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label);
#endif
// 4.4
//GL_FUNCDEF(void, glBufferStorage, GLenum target, GLsizeiptr size, const void *data, GLbitfield flags);
//GL_FUNCDEF(void, glClearTexImage, GLuint texture, GLint level, GLenum format, GLenum type, const void *data);
//GL_FUNCDEF(void, glClearTexSubImage, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data);
//GL_FUNCDEF(void, glBindBuffersBase, GLenum target, GLuint first, GLsizei count, const GLuint *buffers);
//GL_FUNCDEF(void, glBindBuffersRange, GLenum target, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizeiptr *sizes);
//GL_FUNCDEF(void, glBindTextures, GLuint first, GLsizei count, const GLuint *textures);
//GL_FUNCDEF(void, glBindSamplers, GLuint first, GLsizei count, const GLuint *samplers);
//GL_FUNCDEF(void, glBindImageTextures, GLuint first, GLsizei count, const GLuint *textures);
//GL_FUNCDEF(void, glBindVertexBuffers, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides);

// 4.5
//GL_FUNCDEF(void, glClipControl, GLenum origin, GLenum depth);
//GL_FUNCDEF(void, glCreateTransformFeedbacks, GLsizei n, GLuint *ids);
//GL_FUNCDEF(void, glTransformFeedbackBufferBase, GLuint xfb, GLuint index, GLuint buffer);
//GL_FUNCDEF(void, glTransformFeedbackBufferRange, GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
//GL_FUNCDEF(void, glGetTransformFeedbackiv, GLuint xfb, GLenum pname, GLint *param);
//GL_FUNCDEF(void, glGetTransformFeedbacki_v, GLuint xfb, GLenum pname, GLuint index, GLint *param);
//GL_FUNCDEF(void, glGetTransformFeedbacki64_v, GLuint xfb, GLenum pname, GLuint index, GLint64 *param);
//GL_FUNCDEF(void, glCreateBuffers, GLsizei n, GLuint *buffers);
//GL_FUNCDEF(void, glNamedBufferStorage, GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);
//GL_FUNCDEF(void, glNamedBufferData, GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);
//GL_FUNCDEF(void, glNamedBufferSubData, GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
//GL_FUNCDEF(void, glCopyNamedBufferSubData, GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
//GL_FUNCDEF(void, glClearNamedBufferData, GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data);
//GL_FUNCDEF(void, glClearNamedBufferSubData, GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
//GL_FUNCDEF(void *, glMapNamedBuffer, GLuint buffer, GLenum access);
//GL_FUNCDEF(void *, glMapNamedBufferRange, GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
//GL_FUNCDEF(GLboolean, glUnmapNamedBuffer, GLuint buffer);
//GL_FUNCDEF(void, glFlushMappedNamedBufferRange, GLuint buffer, GLintptr offset, GLsizeiptr length);
//GL_FUNCDEF(void, glGetNamedBufferParameteriv, GLuint buffer, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetNamedBufferParameteri64v, GLuint buffer, GLenum pname, GLint64 *params);
//GL_FUNCDEF(void, glGetNamedBufferPointerv, GLuint buffer, GLenum pname, void **params);
//GL_FUNCDEF(void, glGetNamedBufferSubData, GLuint buffer, GLintptr offset, GLsizeiptr size, void *data);
//GL_FUNCDEF(void, glCreateFramebuffers, GLsizei n, GLuint *framebuffers);
//GL_FUNCDEF(void, glNamedFramebufferRenderbuffer, GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
//GL_FUNCDEF(void, glNamedFramebufferParameteri, GLuint framebuffer, GLenum pname, GLint param);
//GL_FUNCDEF(void, glNamedFramebufferTexture, GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
//GL_FUNCDEF(void, glNamedFramebufferTextureLayer, GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
//GL_FUNCDEF(void, glNamedFramebufferDrawBuffer, GLuint framebuffer, GLenum buf);
//GL_FUNCDEF(void, glNamedFramebufferDrawBuffers, GLuint framebuffer, GLsizei n, const GLenum *bufs);
//GL_FUNCDEF(void, glNamedFramebufferReadBuffer, GLuint framebuffer, GLenum src);
//GL_FUNCDEF(void, glInvalidateNamedFramebufferData, GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments);
//GL_FUNCDEF(void, glInvalidateNamedFramebufferSubData, GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
//GL_FUNCDEF(void, glClearNamedFramebufferiv, GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint *value);
//GL_FUNCDEF(void, glClearNamedFramebufferuiv, GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint *value);
//GL_FUNCDEF(void, glClearNamedFramebufferfv, GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat *value);
//GL_FUNCDEF(void, glClearNamedFramebufferfi, GLuint framebuffer, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
//GL_FUNCDEF(void, glBlitNamedFramebuffer, GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
#ifdef _DEBUG
//GL_FUNCDEF(GLenum, glCheckNamedFramebufferStatus, GLuint framebuffer, GLenum target);
#endif
//GL_FUNCDEF(void, glGetNamedFramebufferParameteriv, GLuint framebuffer, GLenum pname, GLint *param);
//GL_FUNCDEF(void, glGetNamedFramebufferAttachmentParameteriv, GLuint framebuffer, GLenum attachment, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glCreateRenderbuffers, GLsizei n, GLuint *renderbuffers);
//GL_FUNCDEF(void, glNamedRenderbufferStorage, GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
//GL_FUNCDEF(void, glNamedRenderbufferStorageMultisample, GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
//GL_FUNCDEF(void, glGetNamedRenderbufferParameteriv, GLuint renderbuffer, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glCreateTextures, GLenum target, GLsizei n, GLuint *textures);
//GL_FUNCDEF(void, glTextureBuffer, GLuint texture, GLenum internalformat, GLuint buffer);
//GL_FUNCDEF(void, glTextureBufferRange, GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
//GL_FUNCDEF(void, glTextureStorage1D, GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width);
//GL_FUNCDEF(void, glTextureStorage2D, GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
//GL_FUNCDEF(void, glTextureStorage3D, GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
//GL_FUNCDEF(void, glTextureStorage2DMultisample, GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
//GL_FUNCDEF(void, glTextureStorage3DMultisample, GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
//GL_FUNCDEF(void, glTextureSubImage1D, GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
//GL_FUNCDEF(void, glTextureSubImage2D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
//GL_FUNCDEF(void, glTextureSubImage3D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
//GL_FUNCDEF(void, glCompressedTextureSubImage1D, GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
//GL_FUNCDEF(void, glCompressedTextureSubImage2D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
//GL_FUNCDEF(void, glCompressedTextureSubImage3D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
//GL_FUNCDEF(void, glCopyTextureSubImage1D, GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
//GL_FUNCDEF(void, glCopyTextureSubImage2D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
//GL_FUNCDEF(void, glCopyTextureSubImage3D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
//GL_FUNCDEF(void, glTextureParameterf, GLuint texture, GLenum pname, GLfloat param);
//GL_FUNCDEF(void, glTextureParameterfv, GLuint texture, GLenum pname, const GLfloat *param);
//GL_FUNCDEF(void, glTextureParameteri, GLuint texture, GLenum pname, GLint param);
//GL_FUNCDEF(void, glTextureParameterIiv, GLuint texture, GLenum pname, const GLint *params);
//GL_FUNCDEF(void, glTextureParameterIuiv, GLuint texture, GLenum pname, const GLuint *params);
//GL_FUNCDEF(void, glTextureParameteriv, GLuint texture, GLenum pname, const GLint *param);
//GL_FUNCDEF(void, glGenerateTextureMipmap, GLuint texture);
//GL_FUNCDEF(void, glBindTextureUnit, GLuint unit, GLuint texture);
//GL_FUNCDEF(void, glGetTextureImage, GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
//GL_FUNCDEF(void, glGetCompressedTextureImage, GLuint texture, GLint level, GLsizei bufSize, void *pixels);
//GL_FUNCDEF(void, glGetTextureLevelParameterfv, GLuint texture, GLint level, GLenum pname, GLfloat *params);
//GL_FUNCDEF(void, glGetTextureLevelParameteriv, GLuint texture, GLint level, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetTextureParameterfv, GLuint texture, GLenum pname, GLfloat *params);
//GL_FUNCDEF(void, glGetTextureParameterIiv, GLuint texture, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetTextureParameterIuiv, GLuint texture, GLenum pname, GLuint *params);
//GL_FUNCDEF(void, glGetTextureParameteriv, GLuint texture, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glCreateVertexArrays, GLsizei n, GLuint *arrays);
//GL_FUNCDEF(void, glDisableVertexArrayAttrib, GLuint vaobj, GLuint index);
//GL_FUNCDEF(void, glEnableVertexArrayAttrib, GLuint vaobj, GLuint index);
//GL_FUNCDEF(void, glVertexArrayElementBuffer, GLuint vaobj, GLuint buffer);
//GL_FUNCDEF(void, glVertexArrayVertexBuffer, GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
//GL_FUNCDEF(void, glVertexArrayVertexBuffers, GLuint vaobj, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides);
//GL_FUNCDEF(void, glVertexArrayAttribBinding, GLuint vaobj, GLuint attribindex, GLuint bindingindex);
//GL_FUNCDEF(void, glVertexArrayAttribFormat, GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
//GL_FUNCDEF(void, glVertexArrayAttribIFormat, GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
//GL_FUNCDEF(void, glVertexArrayAttribLFormat, GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
//GL_FUNCDEF(void, glVertexArrayBindingDivisor, GLuint vaobj, GLuint bindingindex, GLuint divisor);
//GL_FUNCDEF(void, glGetVertexArrayiv, GLuint vaobj, GLenum pname, GLint *param);
//GL_FUNCDEF(void, glGetVertexArrayIndexediv, GLuint vaobj, GLuint index, GLenum pname, GLint *param);
//GL_FUNCDEF(void, glGetVertexArrayIndexed64iv, GLuint vaobj, GLuint index, GLenum pname, GLint64 *param);
//GL_FUNCDEF(void, glCreateSamplers, GLsizei n, GLuint *samplers);
//GL_FUNCDEF(void, glCreateProgramPipelines, GLsizei n, GLuint *pipelines);
//GL_FUNCDEF(void, glCreateQueries, GLenum target, GLsizei n, GLuint *ids);
//GL_FUNCDEF(void, glGetQueryBufferObjecti64v, GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
//GL_FUNCDEF(void, glGetQueryBufferObjectiv, GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
//GL_FUNCDEF(void, glGetQueryBufferObjectui64v, GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
//GL_FUNCDEF(void, glGetQueryBufferObjectuiv, GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
//GL_FUNCDEF(void, glMemoryBarrierByRegion, GLbitfield barriers);
//GL_FUNCDEF(void, glGetTextureSubImage, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
//GL_FUNCDEF(void, glGetCompressedTextureSubImage, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void *pixels);
//GL_FUNCDEF(GLenum, glGetGraphicsResetStatus, void);
//GL_FUNCDEF(void, glGetnCompressedTexImage, GLenum target, GLint lod, GLsizei bufSize, void *pixels);
//GL_FUNCDEF(void, glGetnTexImage, GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
//GL_FUNCDEF(void, glGetnUniformdv, GLuint program, GLint location, GLsizei bufSize, GLdouble *params);
//GL_FUNCDEF(void, glGetnUniformfv, GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
//GL_FUNCDEF(void, glGetnUniformiv, GLuint program, GLint location, GLsizei bufSize, GLint *params);
//GL_FUNCDEF(void, glGetnUniformuiv, GLuint program, GLint location, GLsizei bufSize, GLuint *params);
//GL_FUNCDEF(void, glReadnPixels, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);
//GL_FUNCDEF(void, glTextureBarrier, void);

// GL_EXT_direct_state_access
//GL_FUNCDEF(void, glMatrixLoadfEXT, GLenum mode, const GLfloat *m);
//GL_FUNCDEF(void, glMatrixLoaddEXT, GLenum mode, const GLdouble *m);
//GL_FUNCDEF(void, glMatrixMultfEXT, GLenum mode, const GLfloat *m);
//GL_FUNCDEF(void, glMatrixMultdEXT, GLenum mode, const GLdouble *m);
//GL_FUNCDEF(void, glMatrixLoadIdentityEXT, GLenum mode);
//GL_FUNCDEF(void, glMatrixRotatefEXT, GLenum mode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
//GL_FUNCDEF(void, glMatrixRotatedEXT, GLenum mode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
//GL_FUNCDEF(void, glMatrixScalefEXT, GLenum mode, GLfloat x, GLfloat y, GLfloat z);
//GL_FUNCDEF(void, glMatrixScaledEXT, GLenum mode, GLdouble x, GLdouble y, GLdouble z);
//GL_FUNCDEF(void, glMatrixTranslatefEXT, GLenum mode, GLfloat x, GLfloat y, GLfloat z);
//GL_FUNCDEF(void, glMatrixTranslatedEXT, GLenum mode, GLdouble x, GLdouble y, GLdouble z);
//GL_FUNCDEF(void, glMatrixFrustumEXT, GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
//GL_FUNCDEF(void, glMatrixOrthoEXT, GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
//GL_FUNCDEF(void, glMatrixPopEXT, GLenum mode);
//GL_FUNCDEF(void, glMatrixPushEXT, GLenum mode);
//GL_FUNCDEF(void, glClientAttribDefaultEXT, GLbitfield mask);
//GL_FUNCDEF(void, glPushClientAttribDefaultEXT, GLbitfield mask);
//GL_FUNCDEF(void, glTextureParameterfEXT, GLuint texture, GLenum target, GLenum pname, GLfloat param);
//GL_FUNCDEF(void, glTextureParameterfvEXT, GLuint texture, GLenum target, GLenum pname, const GLfloat *params);
GL_FUNCDEF(void, glTextureParameteriEXT, GLuint texture, GLenum target, GLenum pname, GLint param);
//GL_FUNCDEF(void, glTextureParameterivEXT, GLuint texture, GLenum target, GLenum pname, const GLint *params);
//GL_FUNCDEF(void, glTextureImage1DEXT, GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
GL_FUNCDEF(void, glTextureImage2DEXT, GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
//GL_FUNCDEF(void, glTextureSubImage1DEXT, GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
GL_FUNCDEF(void, glTextureSubImage2DEXT, GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
//GL_FUNCDEF(void, glCopyTextureImage1DEXT, GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
//GL_FUNCDEF(void, glCopyTextureImage2DEXT, GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
//GL_FUNCDEF(void, glCopyTextureSubImage1DEXT, GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
//GL_FUNCDEF(void, glCopyTextureSubImage2DEXT, GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GL_FUNCDEF(void, glGetTextureImageEXT, GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, void *pixels);
//GL_FUNCDEF(void, glGetTextureParameterfvEXT, GLuint texture, GLenum target, GLenum pname, GLfloat *params);
//GL_FUNCDEF(void, glGetTextureParameterivEXT, GLuint texture, GLenum target, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetTextureLevelParameterfvEXT, GLuint texture, GLenum target, GLint level, GLenum pname, GLfloat *params);
//GL_FUNCDEF(void, glGetTextureLevelParameterivEXT, GLuint texture, GLenum target, GLint level, GLenum pname, GLint *params);
GL_FUNCDEF(void, glTextureImage3DEXT, GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
GL_FUNCDEF(void, glTextureSubImage3DEXT, GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
//GL_FUNCDEF(void, glCopyTextureSubImage3DEXT, GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GL_FUNCDEF(void, glBindMultiTextureEXT, GLenum texunit, GLenum target, GLuint texture);
//GL_FUNCDEF(void, glMultiTexCoordPointerEXT, GLenum texunit, GLint size, GLenum type, GLsizei stride, const void *pointer);
//GL_FUNCDEF(void, glMultiTexEnvfEXT, GLenum texunit, GLenum target, GLenum pname, GLfloat param);
//GL_FUNCDEF(void, glMultiTexEnvfvEXT, GLenum texunit, GLenum target, GLenum pname, const GLfloat *params);
//GL_FUNCDEF(void, glMultiTexEnviEXT, GLenum texunit, GLenum target, GLenum pname, GLint param);
//GL_FUNCDEF(void, glMultiTexEnvivEXT, GLenum texunit, GLenum target, GLenum pname, const GLint *params);
//GL_FUNCDEF(void, glMultiTexGendEXT, GLenum texunit, GLenum coord, GLenum pname, GLdouble param);
//GL_FUNCDEF(void, glMultiTexGendvEXT, GLenum texunit, GLenum coord, GLenum pname, const GLdouble *params);
//GL_FUNCDEF(void, glMultiTexGenfEXT, GLenum texunit, GLenum coord, GLenum pname, GLfloat param);
//GL_FUNCDEF(void, glMultiTexGenfvEXT, GLenum texunit, GLenum coord, GLenum pname, const GLfloat *params);
//GL_FUNCDEF(void, glMultiTexGeniEXT, GLenum texunit, GLenum coord, GLenum pname, GLint param);
//GL_FUNCDEF(void, glMultiTexGenivEXT, GLenum texunit, GLenum coord, GLenum pname, const GLint *params);
//GL_FUNCDEF(void, glGetMultiTexEnvfvEXT, GLenum texunit, GLenum target, GLenum pname, GLfloat *params);
//GL_FUNCDEF(void, glGetMultiTexEnvivEXT, GLenum texunit, GLenum target, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetMultiTexGendvEXT, GLenum texunit, GLenum coord, GLenum pname, GLdouble *params);
//GL_FUNCDEF(void, glGetMultiTexGenfvEXT, GLenum texunit, GLenum coord, GLenum pname, GLfloat *params);
//GL_FUNCDEF(void, glGetMultiTexGenivEXT, GLenum texunit, GLenum coord, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glMultiTexParameteriEXT, GLenum texunit, GLenum target, GLenum pname, GLint param);
//GL_FUNCDEF(void, glMultiTexParameterivEXT, GLenum texunit, GLenum target, GLenum pname, const GLint *params);
//GL_FUNCDEF(void, glMultiTexParameterfEXT, GLenum texunit, GLenum target, GLenum pname, GLfloat param);
//GL_FUNCDEF(void, glMultiTexParameterfvEXT, GLenum texunit, GLenum target, GLenum pname, const GLfloat *params);
//GL_FUNCDEF(void, glMultiTexImage1DEXT, GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
//GL_FUNCDEF(void, glMultiTexImage2DEXT, GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
//GL_FUNCDEF(void, glMultiTexSubImage1DEXT, GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
//GL_FUNCDEF(void, glMultiTexSubImage2DEXT, GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
//GL_FUNCDEF(void, glCopyMultiTexImage1DEXT, GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
//GL_FUNCDEF(void, glCopyMultiTexImage2DEXT, GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
//GL_FUNCDEF(void, glCopyMultiTexSubImage1DEXT, GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
//GL_FUNCDEF(void, glCopyMultiTexSubImage2DEXT, GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
//GL_FUNCDEF(void, glGetMultiTexImageEXT, GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, void *pixels);
//GL_FUNCDEF(void, glGetMultiTexParameterfvEXT, GLenum texunit, GLenum target, GLenum pname, GLfloat *params);
//GL_FUNCDEF(void, glGetMultiTexParameterivEXT, GLenum texunit, GLenum target, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetMultiTexLevelParameterfvEXT, GLenum texunit, GLenum target, GLint level, GLenum pname, GLfloat *params);
//GL_FUNCDEF(void, glGetMultiTexLevelParameterivEXT, GLenum texunit, GLenum target, GLint level, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glMultiTexImage3DEXT, GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
//GL_FUNCDEF(void, glMultiTexSubImage3DEXT, GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
//GL_FUNCDEF(void, glCopyMultiTexSubImage3DEXT, GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
//GL_FUNCDEF(void, glEnableClientStateIndexedEXT, GLenum array, GLuint index);
//GL_FUNCDEF(void, glDisableClientStateIndexedEXT, GLenum array, GLuint index);
//GL_FUNCDEF(void, glGetFloatIndexedvEXT, GLenum target, GLuint index, GLfloat *data);
//GL_FUNCDEF(void, glGetDoubleIndexedvEXT, GLenum target, GLuint index, GLdouble *data);
//GL_FUNCDEF(void, glGetPointerIndexedvEXT, GLenum target, GLuint index, void **data);
//GL_FUNCDEF(void, glEnableIndexedEXT, GLenum target, GLuint index);
//GL_FUNCDEF(void, glDisableIndexedEXT, GLenum target, GLuint index);
//GL_FUNCDEF(GLboolean, glIsEnabledIndexedEXT, GLenum target, GLuint index);
//GL_FUNCDEF(void, glGetIntegerIndexedvEXT, GLenum target, GLuint index, GLint *data);
//GL_FUNCDEF(void, glGetBooleanIndexedvEXT, GLenum target, GLuint index, GLboolean *data);
//GL_FUNCDEF(void, glCompressedTextureImage3DEXT, GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *bits);
//GL_FUNCDEF(void, glCompressedTextureImage2DEXT, GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *bits);
//GL_FUNCDEF(void, glCompressedTextureImage1DEXT, GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *bits);
GL_FUNCDEF(void, glCompressedTextureSubImage3DEXT, GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *bits);
GL_FUNCDEF(void, glCompressedTextureSubImage2DEXT, GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *bits);
//GL_FUNCDEF(void, glCompressedTextureSubImage1DEXT, GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *bits);
GL_FUNCDEF(void, glGetCompressedTextureImageEXT, GLuint texture, GLenum target, GLint lod, void *img);
//GL_FUNCDEF(void, glCompressedMultiTexImage3DEXT, GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *bits);
//GL_FUNCDEF(void, glCompressedMultiTexImage2DEXT, GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *bits);
//GL_FUNCDEF(void, glCompressedMultiTexImage1DEXT, GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *bits);
//GL_FUNCDEF(void, glCompressedMultiTexSubImage3DEXT, GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *bits);
//GL_FUNCDEF(void, glCompressedMultiTexSubImage2DEXT, GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *bits);
//GL_FUNCDEF(void, glCompressedMultiTexSubImage1DEXT, GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *bits);
//GL_FUNCDEF(void, glGetCompressedMultiTexImageEXT, GLenum texunit, GLenum target, GLint lod, void *img);
//GL_FUNCDEF(void, glMatrixLoadTransposefEXT, GLenum mode, const GLfloat *m);
//GL_FUNCDEF(void, glMatrixLoadTransposedEXT, GLenum mode, const GLdouble *m);
//GL_FUNCDEF(void, glMatrixMultTransposefEXT, GLenum mode, const GLfloat *m);
//GL_FUNCDEF(void, glMatrixMultTransposedEXT, GLenum mode, const GLdouble *m);
GL_FUNCDEF(void, glNamedBufferDataEXT, GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);
GL_FUNCDEF(void, glNamedBufferSubDataEXT, GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
GL_FUNCDEF(void *, glMapNamedBufferEXT, GLuint buffer, GLenum access);
GL_FUNCDEF(GLboolean, glUnmapNamedBufferEXT, GLuint buffer);
//GL_FUNCDEF(void, glGetNamedBufferParameterivEXT, GLuint buffer, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetNamedBufferPointervEXT, GLuint buffer, GLenum pname, void **params);
//GL_FUNCDEF(void, glGetNamedBufferSubDataEXT, GLuint buffer, GLintptr offset, GLsizeiptr size, void *data);
//GL_FUNCDEF(void, glProgramUniform1fEXT, GLuint program, GLint location, GLfloat v0);
//GL_FUNCDEF(void, glProgramUniform2fEXT, GLuint program, GLint location, GLfloat v0, GLfloat v1);
//GL_FUNCDEF(void, glProgramUniform3fEXT, GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
//GL_FUNCDEF(void, glProgramUniform4fEXT, GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
//GL_FUNCDEF(void, glProgramUniform1iEXT, GLuint program, GLint location, GLint v0);
//GL_FUNCDEF(void, glProgramUniform2iEXT, GLuint program, GLint location, GLint v0, GLint v1);
//GL_FUNCDEF(void, glProgramUniform3iEXT, GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
//GL_FUNCDEF(void, glProgramUniform4iEXT, GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
//GL_FUNCDEF(void, glProgramUniform1fvEXT, GLuint program, GLint location, GLsizei count, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniform2fvEXT, GLuint program, GLint location, GLsizei count, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniform3fvEXT, GLuint program, GLint location, GLsizei count, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniform4fvEXT, GLuint program, GLint location, GLsizei count, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniform1ivEXT, GLuint program, GLint location, GLsizei count, const GLint *value);
//GL_FUNCDEF(void, glProgramUniform2ivEXT, GLuint program, GLint location, GLsizei count, const GLint *value);
//GL_FUNCDEF(void, glProgramUniform3ivEXT, GLuint program, GLint location, GLsizei count, const GLint *value);
//GL_FUNCDEF(void, glProgramUniform4ivEXT, GLuint program, GLint location, GLsizei count, const GLint *value);
//GL_FUNCDEF(void, glProgramUniformMatrix2fvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniformMatrix3fvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniformMatrix4fvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniformMatrix2x3fvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniformMatrix3x2fvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniformMatrix2x4fvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniformMatrix4x2fvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniformMatrix3x4fvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glProgramUniformMatrix4x3fvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
//GL_FUNCDEF(void, glTextureBufferEXT, GLuint texture, GLenum target, GLenum internalformat, GLuint buffer);
//GL_FUNCDEF(void, glMultiTexBufferEXT, GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer);
//GL_FUNCDEF(void, glTextureParameterIivEXT, GLuint texture, GLenum target, GLenum pname, const GLint *params);
//GL_FUNCDEF(void, glTextureParameterIuivEXT, GLuint texture, GLenum target, GLenum pname, const GLuint *params);
//GL_FUNCDEF(void, glGetTextureParameterIivEXT, GLuint texture, GLenum target, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetTextureParameterIuivEXT, GLuint texture, GLenum target, GLenum pname, GLuint *params);
//GL_FUNCDEF(void, glMultiTexParameterIivEXT, GLenum texunit, GLenum target, GLenum pname, const GLint *params);
//GL_FUNCDEF(void, glMultiTexParameterIuivEXT, GLenum texunit, GLenum target, GLenum pname, const GLuint *params);
//GL_FUNCDEF(void, glGetMultiTexParameterIivEXT, GLenum texunit, GLenum target, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetMultiTexParameterIuivEXT, GLenum texunit, GLenum target, GLenum pname, GLuint *params);
//GL_FUNCDEF(void, glProgramUniform1uiEXT, GLuint program, GLint location, GLuint v0);
//GL_FUNCDEF(void, glProgramUniform2uiEXT, GLuint program, GLint location, GLuint v0, GLuint v1);
//GL_FUNCDEF(void, glProgramUniform3uiEXT, GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
//GL_FUNCDEF(void, glProgramUniform4uiEXT, GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
//GL_FUNCDEF(void, glProgramUniform1uivEXT, GLuint program, GLint location, GLsizei count, const GLuint *value);
//GL_FUNCDEF(void, glProgramUniform2uivEXT, GLuint program, GLint location, GLsizei count, const GLuint *value);
//GL_FUNCDEF(void, glProgramUniform3uivEXT, GLuint program, GLint location, GLsizei count, const GLuint *value);
//GL_FUNCDEF(void, glProgramUniform4uivEXT, GLuint program, GLint location, GLsizei count, const GLuint *value);
//GL_FUNCDEF(void, glNamedProgramLocalParameters4fvEXT, GLuint program, GLenum target, GLuint index, GLsizei count, const GLfloat *params);
//GL_FUNCDEF(void, glNamedProgramLocalParameterI4iEXT, GLuint program, GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
//GL_FUNCDEF(void, glNamedProgramLocalParameterI4ivEXT, GLuint program, GLenum target, GLuint index, const GLint *params);
//GL_FUNCDEF(void, glNamedProgramLocalParametersI4ivEXT, GLuint program, GLenum target, GLuint index, GLsizei count, const GLint *params);
//GL_FUNCDEF(void, glNamedProgramLocalParameterI4uiEXT, GLuint program, GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
//GL_FUNCDEF(void, glNamedProgramLocalParameterI4uivEXT, GLuint program, GLenum target, GLuint index, const GLuint *params);
//GL_FUNCDEF(void, glNamedProgramLocalParametersI4uivEXT, GLuint program, GLenum target, GLuint index, GLsizei count, const GLuint *params);
//GL_FUNCDEF(void, glGetNamedProgramLocalParameterIivEXT, GLuint program, GLenum target, GLuint index, GLint *params);
//GL_FUNCDEF(void, glGetNamedProgramLocalParameterIuivEXT, GLuint program, GLenum target, GLuint index, GLuint *params);
//GL_FUNCDEF(void, glEnableClientStateiEXT, GLenum array, GLuint index);
//GL_FUNCDEF(void, glDisableClientStateiEXT, GLenum array, GLuint index);
//GL_FUNCDEF(void, glGetFloati_vEXT, GLenum pname, GLuint index, GLfloat *params);
//GL_FUNCDEF(void, glGetDoublei_vEXT, GLenum pname, GLuint index, GLdouble *params);
//GL_FUNCDEF(void, glGetPointeri_vEXT, GLenum pname, GLuint index, void **params);
//GL_FUNCDEF(void, glNamedProgramStringEXT, GLuint program, GLenum target, GLenum format, GLsizei len, const void *string);
//GL_FUNCDEF(void, glNamedProgramLocalParameter4dEXT, GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
//GL_FUNCDEF(void, glNamedProgramLocalParameter4dvEXT, GLuint program, GLenum target, GLuint index, const GLdouble *params);
//GL_FUNCDEF(void, glNamedProgramLocalParameter4fEXT, GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
//GL_FUNCDEF(void, glNamedProgramLocalParameter4fvEXT, GLuint program, GLenum target, GLuint index, const GLfloat *params);
//GL_FUNCDEF(void, glGetNamedProgramLocalParameterdvEXT, GLuint program, GLenum target, GLuint index, GLdouble *params);
//GL_FUNCDEF(void, glGetNamedProgramLocalParameterfvEXT, GLuint program, GLenum target, GLuint index, GLfloat *params);
//GL_FUNCDEF(void, glGetNamedProgramivEXT, GLuint program, GLenum target, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glGetNamedProgramStringEXT, GLuint program, GLenum target, GLenum pname, void *string);
//GL_FUNCDEF(void, glNamedRenderbufferStorageEXT, GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
//GL_FUNCDEF(void, glGetNamedRenderbufferParameterivEXT, GLuint renderbuffer, GLenum pname, GLint *params);
GL_FUNCDEF(void, glNamedRenderbufferStorageMultisampleEXT, GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
//GL_FUNCDEF(void, glNamedRenderbufferStorageMultisampleCoverageEXT, GLuint renderbuffer, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
#ifdef _DEBUG
GL_FUNCDEF(GLenum, glCheckNamedFramebufferStatusEXT, GLuint framebuffer, GLenum target);
#endif
//GL_FUNCDEF(void, glNamedFramebufferTexture1DEXT, GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GL_FUNCDEF(void, glNamedFramebufferTexture2DEXT, GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GL_FUNCDEF(void, glNamedFramebufferTexture3DEXT, GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
GL_FUNCDEF(void, glNamedFramebufferRenderbufferEXT, GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
//GL_FUNCDEF(void, glGetNamedFramebufferAttachmentParameterivEXT, GLuint framebuffer, GLenum attachment, GLenum pname, GLint *params);
GL_FUNCDEF(void, glGenerateTextureMipmapEXT, GLuint texture, GLenum target);
//GL_FUNCDEF(void, glGenerateMultiTexMipmapEXT, GLenum texunit, GLenum target);
//GL_FUNCDEF(void, glFramebufferDrawBufferEXT, GLuint framebuffer, GLenum mode);
GL_FUNCDEF(void, glFramebufferDrawBuffersEXT, GLuint framebuffer, GLsizei n, const GLenum *bufs);
//GL_FUNCDEF(void, glFramebufferReadBufferEXT, GLuint framebuffer, GLenum mode);
//GL_FUNCDEF(void, glGetFramebufferParameterivEXT, GLuint framebuffer, GLenum pname, GLint *params);
GL_FUNCDEF(void, glNamedCopyBufferSubDataEXT, GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
GL_FUNCDEF(void, glNamedFramebufferTextureEXT, GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
GL_FUNCDEF(void, glNamedFramebufferTextureLayerEXT, GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
//GL_FUNCDEF(void, glNamedFramebufferTextureFaceEXT, GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLenum face);
//GL_FUNCDEF(void, glTextureRenderbufferEXT, GLuint texture, GLenum target, GLuint renderbuffer);
//GL_FUNCDEF(void, glMultiTexRenderbufferEXT, GLenum texunit, GLenum target, GLuint renderbuffer);
//GL_FUNCDEF(void, glVertexArrayVertexOffsetEXT, GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
//GL_FUNCDEF(void, glVertexArrayColorOffsetEXT, GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
//GL_FUNCDEF(void, glVertexArrayEdgeFlagOffsetEXT, GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset);
//GL_FUNCDEF(void, glVertexArrayIndexOffsetEXT, GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
//GL_FUNCDEF(void, glVertexArrayNormalOffsetEXT, GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
//GL_FUNCDEF(void, glVertexArrayTexCoordOffsetEXT, GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
//GL_FUNCDEF(void, glVertexArrayMultiTexCoordOffsetEXT, GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset);
//GL_FUNCDEF(void, glVertexArrayFogCoordOffsetEXT, GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
//GL_FUNCDEF(void, glVertexArraySecondaryColorOffsetEXT, GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
//GL_FUNCDEF(void, glVertexArrayVertexAttribOffsetEXT, GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset);
//GL_FUNCDEF(void, glVertexArrayVertexAttribIOffsetEXT, GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
//GL_FUNCDEF(void, glEnableVertexArrayEXT, GLuint vaobj, GLenum array);
//GL_FUNCDEF(void, glDisableVertexArrayEXT, GLuint vaobj, GLenum array);
//GL_FUNCDEF(void, glEnableVertexArrayAttribEXT, GLuint vaobj, GLuint index);
//GL_FUNCDEF(void, glDisableVertexArrayAttribEXT, GLuint vaobj, GLuint index);
//GL_FUNCDEF(void, glGetVertexArrayIntegervEXT, GLuint vaobj, GLenum pname, GLint *param);
//GL_FUNCDEF(void, glGetVertexArrayPointervEXT, GLuint vaobj, GLenum pname, void **param);
//GL_FUNCDEF(void, glGetVertexArrayIntegeri_vEXT, GLuint vaobj, GLuint index, GLenum pname, GLint *param);
//GL_FUNCDEF(void, glGetVertexArrayPointeri_vEXT, GLuint vaobj, GLuint index, GLenum pname, void **param);
GL_FUNCDEF(void *, glMapNamedBufferRangeEXT, GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
//GL_FUNCDEF(void, glFlushMappedNamedBufferRangeEXT, GLuint buffer, GLintptr offset, GLsizeiptr length);
//GL_FUNCDEF(void, glNamedBufferStorageEXT, GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);
//GL_FUNCDEF(void, glClearNamedBufferDataEXT, GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data);
//GL_FUNCDEF(void, glClearNamedBufferSubDataEXT, GLuint buffer, GLenum internalformat, GLsizeiptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
//GL_FUNCDEF(void, glNamedFramebufferParameteriEXT, GLuint framebuffer, GLenum pname, GLint param);
//GL_FUNCDEF(void, glGetNamedFramebufferParameterivEXT, GLuint framebuffer, GLenum pname, GLint *params);
//GL_FUNCDEF(void, glProgramUniform1dEXT, GLuint program, GLint location, GLdouble x);
//GL_FUNCDEF(void, glProgramUniform2dEXT, GLuint program, GLint location, GLdouble x, GLdouble y);
//GL_FUNCDEF(void, glProgramUniform3dEXT, GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z);
//GL_FUNCDEF(void, glProgramUniform4dEXT, GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
//GL_FUNCDEF(void, glProgramUniform1dvEXT, GLuint program, GLint location, GLsizei count, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniform2dvEXT, GLuint program, GLint location, GLsizei count, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniform3dvEXT, GLuint program, GLint location, GLsizei count, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniform4dvEXT, GLuint program, GLint location, GLsizei count, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniformMatrix2dvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniformMatrix3dvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniformMatrix4dvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniformMatrix2x3dvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniformMatrix2x4dvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniformMatrix3x2dvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniformMatrix3x4dvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniformMatrix4x2dvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glProgramUniformMatrix4x3dvEXT, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
//GL_FUNCDEF(void, glTextureBufferRangeEXT, GLuint texture, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
//GL_FUNCDEF(void, glTextureStorage1DEXT, GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
GL_FUNCDEF(void, glTextureStorage2DEXT, GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
GL_FUNCDEF(void, glTextureStorage3DEXT, GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
//GL_FUNCDEF(void, glTextureStorage2DMultisampleEXT, GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
//GL_FUNCDEF(void, glTextureStorage3DMultisampleEXT, GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
//GL_FUNCDEF(void, glVertexArrayBindVertexBufferEXT, GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
//GL_FUNCDEF(void, glVertexArrayVertexAttribFormatEXT, GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
//GL_FUNCDEF(void, glVertexArrayVertexAttribIFormatEXT, GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
//GL_FUNCDEF(void, glVertexArrayVertexAttribLFormatEXT, GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
//GL_FUNCDEF(void, glVertexArrayVertexAttribBindingEXT, GLuint vaobj, GLuint attribindex, GLuint bindingindex);
//GL_FUNCDEF(void, glVertexArrayVertexBindingDivisorEXT, GLuint vaobj, GLuint bindingindex, GLuint divisor);
//GL_FUNCDEF(void, glVertexArrayVertexAttribLOffsetEXT, GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
//GL_FUNCDEF(void, glTexturePageCommitmentEXT, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit);
//GL_FUNCDEF(void, glVertexArrayVertexAttribDivisorEXT, GLuint vaobj, GLuint index, GLuint divisor);

// WGL_EXT_swap_control
GL_FUNCDEF(BOOL, wglSwapIntervalEXT, int interval);
GL_FUNCDEF(int, wglGetSwapIntervalEXT, void);


void LoadOpenGL(void)
{
#ifdef _DEBUG
	uint _numLoads = 0;
	uint _numFails = 0;
#endif

	// 1.2

	// 1.3
	GL_FUNCLOAD(glActiveTexture);

	// 1.4
	GL_FUNCLOAD(glBlendFuncSeparate);
	GL_FUNCLOAD(glBlendColor);

	// 1.5
	GL_FUNCLOAD(glBindBuffer);
	GL_FUNCLOAD(glDeleteBuffers);
	GL_FUNCLOAD(glGenBuffers);

	// 2.0
	GL_FUNCLOAD(glBlendEquationSeparate);
	//GL_FUNCLOAD(glDrawBuffers);
	GL_FUNCLOAD(glStencilOpSeparate);
	GL_FUNCLOAD(glStencilFuncSeparate);
	//GL_FUNCLOAD(glStencilMaskSeparate);
	//GL_FUNCLOAD(glBindAttribLocation);
	//GL_FUNCLOAD(glDeleteProgram);
	//GL_FUNCLOAD(glDisableVertexAttribArray);
	GL_FUNCLOAD(glEnableVertexAttribArray);
	GL_FUNCLOAD(glGetProgramiv);
#ifdef _DEBUG_OUTPUT
	GL_FUNCLOAD(glGetProgramInfoLog);
#endif
	GL_FUNCLOAD(glGetUniformLocation);

	// 2.1

	// 3.0
	GL_FUNCLOAD(glBindBufferRange);
	//GL_FUNCLOAD(glBindBufferBase);
	GL_FUNCLOAD(glGetStringi);
	GL_FUNCLOAD(glBindRenderbuffer);
	GL_FUNCLOAD(glDeleteRenderbuffers);
	GL_FUNCLOAD(glGenRenderbuffers);
	GL_FUNCLOAD(glBindFramebuffer);
	GL_FUNCLOAD(glDeleteFramebuffers);
	GL_FUNCLOAD(glGenFramebuffers);
	GL_FUNCLOAD(glBlitFramebuffer);
	GL_FUNCLOAD(glBindVertexArray);
	//GL_FUNCLOAD(glDeleteVertexArrays);
	GL_FUNCLOAD(glGenVertexArrays);


	// 3.1
	GL_FUNCLOAD(glDrawArraysInstanced);
	//GL_FUNCLOAD(glDrawElementsInstanced);
	//GL_FUNCLOAD(glPrimitiveRestartIndex);
	//GL_FUNCLOAD(glGetUniformBlockIndex);
	//GL_FUNCLOAD(glUniformBlockBinding);

	// 3.2
	//GL_FUNCLOAD(glDrawElementsBaseVertex);
	//GL_FUNCLOAD(glDrawRangeElementsBaseVertex);
	GL_FUNCLOAD(glDrawElementsInstancedBaseVertex);
	//GL_FUNCLOAD(glTexImage2DMultisample);
	//GL_FUNCLOAD(glTexImage3DMultisample);

	// 3.3
	GL_FUNCLOAD(glGenSamplers);
	GL_FUNCLOAD(glDeleteSamplers);
	GL_FUNCLOAD(glBindSampler);
	GL_FUNCLOAD(glSamplerParameteri);
	GL_FUNCLOAD(glSamplerParameterf);

	// 4.0

	// 4.1 (GL_ARB_separate_shader_objects)
	GL_FUNCLOAD(glUseProgramStages);
	GL_FUNCLOAD(glActiveShaderProgram);
	GL_FUNCLOAD(glCreateShaderProgramv);
	GL_FUNCLOAD(glBindProgramPipeline);
	GL_FUNCLOAD(glDeleteProgramPipelines);
	GL_FUNCLOAD(glGenProgramPipelines);
#ifdef _DEBUG_OUTPUT
	GL_FUNCLOAD(glGetProgramPipelineiv);
#endif
	GL_FUNCLOAD(glProgramUniform1i);
	//GL_FUNCLOAD(glProgramUniform1iv);
	//GL_FUNCLOAD(glProgramUniform1fv);
	//GL_FUNCLOAD(glProgramUniform2iv);
	//GL_FUNCLOAD(glProgramUniform2fv);
	//GL_FUNCLOAD(glProgramUniform3iv);
	//GL_FUNCLOAD(glProgramUniform3fv);
	//GL_FUNCLOAD(glProgramUniform4iv);
	//GL_FUNCLOAD(glProgramUniform4fv);
	//GL_FUNCLOAD(glProgramUniformMatrix3dv);
	//GL_FUNCLOAD(glProgramUniformMatrix4dv);
	//GL_FUNCLOAD(glProgramUniformMatrix3x4fv);
#ifdef _DEBUG
	GL_FUNCLOAD(glValidateProgramPipeline);
	GL_FUNCLOAD(glGetProgramPipelineInfoLog);
#endif

	// 4.2

	// 4.3 (GL_ARB_vertex_attrib_binding, GL_ARB_debug_output)
	GL_FUNCLOAD(glBindVertexBuffer);
	GL_FUNCLOAD(glVertexAttribFormat);
	GL_FUNCLOAD(glVertexAttribIFormat);
	GL_FUNCLOAD(glVertexAttribBinding);
	GL_FUNCLOAD(glVertexBindingDivisor);
#ifdef _DEBUG
	GL_FUNCLOAD(glDebugMessageControl);
	GL_FUNCLOAD(glDebugMessageCallback);
#endif

	// 4.4

	// 4.5

	// GL_EXT_direct_state_access
	//GL_FUNCLOAD(glTextureParameterfEXT);
	GL_FUNCLOAD(glTextureParameteriEXT);
	GL_FUNCLOAD(glTextureImage2DEXT);
	GL_FUNCLOAD(glTextureSubImage2DEXT);
	GL_FUNCLOAD(glGetTextureImageEXT);
	GL_FUNCLOAD(glTextureImage3DEXT);
	GL_FUNCLOAD(glTextureSubImage3DEXT);
	GL_FUNCLOAD(glBindMultiTextureEXT);
	GL_FUNCLOAD(glCompressedTextureSubImage3DEXT);
	GL_FUNCLOAD(glCompressedTextureSubImage2DEXT);
	GL_FUNCLOAD(glGetCompressedTextureImageEXT);
	GL_FUNCLOAD(glNamedBufferDataEXT);
	GL_FUNCLOAD(glNamedBufferSubDataEXT);
	GL_FUNCLOAD(glMapNamedBufferEXT);
	GL_FUNCLOAD(glUnmapNamedBufferEXT);
	//GL_FUNCLOAD(glNamedRenderbufferStorageEXT);
	GL_FUNCLOAD(glNamedRenderbufferStorageMultisampleEXT);
#ifdef _DEBUG
	GL_FUNCLOAD(glCheckNamedFramebufferStatusEXT);
#endif
	GL_FUNCLOAD(glNamedFramebufferTexture2DEXT);
	GL_FUNCLOAD(glNamedFramebufferTexture3DEXT);
	GL_FUNCLOAD(glNamedFramebufferRenderbufferEXT);
	GL_FUNCLOAD(glGenerateTextureMipmapEXT);
	GL_FUNCLOAD(glFramebufferDrawBuffersEXT);
	GL_FUNCLOAD(glNamedCopyBufferSubDataEXT);
	GL_FUNCLOAD(glNamedFramebufferTextureEXT);
	GL_FUNCLOAD(glNamedFramebufferTextureLayerEXT);
	GL_FUNCLOAD(glMapNamedBufferRangeEXT);
	GL_FUNCLOAD(glTextureStorage2DEXT);
	GL_FUNCLOAD(glTextureStorage3DEXT);
	//GL_FUNCLOAD(glTextureStorage2DMultisampleEXT);
	//GL_FUNCLOAD(glTextureStorage3DMultisampleEXT);

	// WGL_EXT_swap_control
	GL_FUNCLOAD(wglSwapIntervalEXT);
	GL_FUNCLOAD(wglGetSwapIntervalEXT);

#ifdef _DEBUG
	LOG("OpenGL functions: %d/%d", _numLoads - _numFails, _numLoads);
	//ASSERT(_numFails == 0 || (_numFails == 2 && !glDebugMessageControl && !glDebugMessageCallback));
#endif
}

bool HasOpenGLExtension(const char* _ext)
{
	const char* e = (const char*)glGetStringi(GL_EXTENSIONS, 0);
	for (int i = 0; e; e = (const char*)glGetStringi(GL_EXTENSIONS, ++i))
	{
		if (!strcmp(e, _ext))
			return true;
	}
	return false;
}
