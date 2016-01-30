#pragma once

#include "Base.hpp"
#include <Windows.h>
#include <gl/gl.h>
#include "gl/glext.h"

//----------------------------------------------------------------------------//
// Utils
//----------------------------------------------------------------------------//

#define GLFUNC_DEF(R, Name, ...) R (APIENTRY*Name) (##__VA_ARGS__)
#define GLFUNC_LOAD_EX(Func, Name) (Func = reinterpret_cast<decltype(Func)>(wglGetProcAddress(Name)))
#define GLFUNC_LOAD(Name) GLFUNC_LOAD_EX(Name, #Name)

//----------------------------------------------------------------------------//
// Funcs
//----------------------------------------------------------------------------//

// 1.5

//GLFUNC_DEF(void, glGenQueries, GLsizei n, GLuint *ids);
//GLFUNC_DEF(void, glDeleteQueries, GLsizei n, const GLuint *ids);
//GLFUNC_DEF(GLboolean, glIsQuery, GLuint id);
//GLFUNC_DEF(void, glBeginQuery, GLenum target, GLuint id);
//GLFUNC_DEF(void, glEndQuery, GLenum target);
//GLFUNC_DEF(void, glGetQueryiv, GLenum target, GLenum pname, GLint *params);
//GLFUNC_DEF(void, glGetQueryObjectiv, GLuint id, GLenum pname, GLint *params);
//GLFUNC_DEF(void, glGetQueryObjectuiv, GLuint id, GLenum pname, GLuint *params);
GLFUNC_DEF(void, glBindBuffer, GLenum target, GLuint buffer);
GLFUNC_DEF(void, glDeleteBuffers, GLsizei n, const GLuint *buffers);
GLFUNC_DEF(void, glGenBuffers, GLsizei n, GLuint *buffers);
//GL_FUNCDEF(GLboolean, glIsBuffer, GLuint buffer);
GLFUNC_DEF(void, glBufferData, GLenum target, GLsizeiptr size, const void *data, GLenum usage);
GLFUNC_DEF(void, glBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
GLFUNC_DEF(void, glGetBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, void *data);
GLFUNC_DEF(void *, glMapBuffer, GLenum target, GLenum access);
GLFUNC_DEF(GLboolean, glUnmapBuffer, GLenum target);
//GLFUNC_DEF(void, glGetBufferParameteriv, GLenum target, GLenum pname, GLint *params);
//GLFUNC_DEF(void, glGetBufferPointerv, GLenum target, GLenum pname, void **params);


//----------------------------------------------------------------------------//
// API
//----------------------------------------------------------------------------//

extern uint GLVersion;

bool LoadOpenGL(void);

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
