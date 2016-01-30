#pragma once
#include "OpenGL.hpp"

uint GLVersion = 0;

bool LoadOpenGL(void)
{
	// capabilities
	int _caps[4];
	glGetIntegerv(GL_MAJOR_VERSION, &_caps[0]);
	glGetIntegerv(GL_MINOR_VERSION, &_caps[1]);
	GLVersion = _caps[0] * 10 + _caps[1];
	LOG("OpenGL %s on %s, %s", glGetString(GL_VERSION), glGetString(GL_RENDERER), glGetString(GL_VENDOR));

	if (GLVersion < 2.0)
	{
		LOG("Error: OpenGL 2.0 is not supported");
		return false;
	}

	// 1.5

	//GLFUNC_LOAD(glGenQueries);
	//GLFUNC_LOAD(glDeleteQueries);
	//GLFUNC_LOAD(glIsQuery);
	//GLFUNC_LOAD(glBeginQuery);
	//GLFUNC_LOAD(glEndQuery);
	//GLFUNC_LOAD(glGetQueryiv);
	//GLFUNC_LOAD(glGetQueryObjectiv);
	//GLFUNC_LOAD(glGetQueryObjectuiv);
	GLFUNC_LOAD(glBindBuffer);
	GLFUNC_LOAD(glDeleteBuffers);
	GLFUNC_LOAD(glGenBuffers);
	//GLFUNC_LOAD(glIsBuffer);
	GLFUNC_LOAD(glBufferData);
	GLFUNC_LOAD(glBufferSubData);
	GLFUNC_LOAD(glGetBufferSubData);
	GLFUNC_LOAD(glMapBuffer);
	GLFUNC_LOAD(glUnmapBuffer);
	//GLFUNC_LOAD(glGetBufferParameteriv);
	//GLFUNC_LOAD(glGetBufferPointerv);

	return true;
};
