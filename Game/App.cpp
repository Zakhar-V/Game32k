#pragma once
#include "App.hpp"
#include "Game.hpp"
#include "Resources.hpp"
#include "Device.hpp"
#include "File.hpp"
#include "BuiltinData.hpp"

#undef CharLower

//----------------------------------------------------------------------------//
// App
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
App::App(void)
{
	LOG("Startup ...");
	LOG("Build %s", __DATE__);
	LOG("Create App");


	// load settings
	{
		RawData _data = LoadFile("Settings.txt");
		_data.Push(0);
		if (_data.Size() > 10)
		{
			LOG("Load settings");
			const char* s = (const char*)_data.Ptr();
			String _token;
			float _val;
			while (*s)
			{
				_token.Clear();
				_val = 1;
				while (*s && strchr(" \t\n\r", *s))
					++s;
				while (*s && ((*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z') || strchr("_", *s)))
					_token.Append(CharLower(*s++));
				while (*s && strchr(" \t\n\r:=", *s))
					++s;
				if (!*s || !sscanf(s, "%f", &_val))
					break;
				while (*s && strchr(" \t\n\r+-.0123456789", *s))
					++s;

				LOG("%s = %f", _token.CStr(), _val);

				if (_token == "cam_x")
					gSettings->invCameraH = _val;
				else if (_token == "cam_y")
					gSettings->invCameraV = _val;
				else if (_token == "mouse_sens")
					gSettings->mouseSens = _val;
				else if (_token == "cam_fov")
					gSettings->cameraFov = _val;
			}
		}
	}

	CreateDirectory("Data", nullptr);
	SetCurrentDirectory("Data");
	bool _bdOutOfDate = ExtractBuiltinData();
	/*if (_bdOutOfDate)
	{
		LOG("Generate resources ...");
	}*/


	new Device;
	new Graphics;
	new Renderer;
	new	Resources;
	new Game;
}
//----------------------------------------------------------------------------//
App::~App(void)
{
	LOG("Destroy App");

	{
		LOG("Save settings");
		SetCurrentDirectory("../");
		char _buff[1024];
		sprintf(_buff,
			"cam_x %f\ncam_y %f\nmouse_sens %f\ncam_fov %f\n",
			gSettings->invCameraH,
			gSettings->invCameraV,
			gSettings->mouseSens,
			gSettings->cameraFov);

		SaveFile("Settings.txt", _buff, strlen(_buff));
	}

	delete gGame;
	delete gResources;
	delete gRenderer;
	delete gGraphics;
	delete gDevice;
}
//----------------------------------------------------------------------------//
void App::Run(void)
{
	double _st, _et;
	float _dt = 0, _tt = 0, _pt = 0;
	float _fps = 0;
	int _frames = 0;
	float _timeToQuit = 0;
	int _status = 0;

	bool _quit = false;

	const Color _clearColor(0x7f7f9fff);

	gDevice->SetCursorMode(CM_Camera);

	while (!_quit && !gDevice->ShouldClose())
	{
		_st = Time();
		gDevice->PollEvents();
		_et = Time();
		_pt += (float)(_et - _st);
		_st = _et;

		_quit = GetAsyncKeyState(VK_ESCAPE) != 0;

		gGame->Update(_dt);

		gGraphics->BeginFrame();
		gGraphics->ClearFrameBuffers(FBT_Color | FBT_DepthStencil, _clearColor, 1, 0xff);

		gGame->Draw();

		gGraphics->EndFrame();
		Sleep(1);

		_et = Time();
		_tt += (float)(_et - _st);
		++_frames;
		if (_tt > 0.2f)
		{
			if (_pt < _tt)
				_tt += _pt;
			_dt = _tt / _frames;
			_fps = 1 / _dt;
			_tt = 0;
			_pt = 0;
			_frames = 0;
		}

		float _dist = gPlayer->GetWorldPosition().Distance(gTargetPos);
		float _health = gPlayer->GetHealth();

		if (_status)
		{
			if (_status > 0)
			{
				gDevice->SetTitle("MISSION COMPLETE", gPlayer->GetHealth(), _dist);
			}
			else
			{
				gDevice->SetTitle("MISSION FAIL", gPlayer->GetHealth(), _dist);
			}

			_timeToQuit -= _dt;
			if (_timeToQuit <= 0)
				_quit = true;
		}
		else
		{
			gDevice->SetTitle("Health: %.1f, Distance: %.1f", gPlayer->GetHealth(), _dist);

			if (_dist < 5.5f)
			{
				_status = 1;
				LOG("Mission complete");
			}
			else if (_health <= 0)
			{
				_status = -1;
				LOG("Mission fail");
			}

			_timeToQuit = 3;
		}
	}

}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
