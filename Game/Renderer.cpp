#pragma once

#include "Renderer.hpp"

//----------------------------------------------------------------------------//
// Shaders
//----------------------------------------------------------------------------//

const char* g_vertexShaderNames[] =
{
	"Test-VS.cso", // VS_Test
	nullptr,
};

const char* g_pixelShaderNames[] =
{
	"Test-PS.cso", // PS_Test
	nullptr,
};


//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Renderer::Renderer(void)
{
	LOG("Create Renderer");

	ASSERT(gGraphics != nullptr); // the graphics must be created before it

	gGraphics->LoadVertexShaders(g_vertexShaderNames);
	gGraphics->LoadPixelShaders(g_pixelShaderNames);
}
//----------------------------------------------------------------------------//
Renderer::~Renderer(void)
{
	LOG("Destroy Renderer");
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

