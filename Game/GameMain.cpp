
//----------------------------------------------------------------------------//
// Modules
//----------------------------------------------------------------------------//

#include "Base.cpp"
#include "File.cpp"
#include "Math.cpp"
#include "Device.cpp"
#include "Graphics.cpp"
#include "BuiltinData.cpp"
#include "Image.cpp"
#include "Geometry.cpp"

#include "Scene.cpp"
#include "Physics.cpp"
#include "Render.cpp"
#include "Level.cpp"

#include "Resources.cpp"
#include "GameLogic.cpp"
#include "Game.cpp"
#include "App.cpp"

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

void main()
{
	new App;
	gApp->Run();
	delete gApp;

#ifdef _DEBUG
	system("pause");
#endif
	ExitProcess(0);
}

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
