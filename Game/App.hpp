#pragma once

#include "Object.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gApp App::Get()

//----------------------------------------------------------------------------//
// App
//----------------------------------------------------------------------------//

class App : public Singleton<App>
{
public:

	App(void);
	~App(void);
	void Run(void);

protected:

	Settings m_settings;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//