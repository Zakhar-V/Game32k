#pragma once
#include "App.hpp"
#include "Game.hpp"
#include "Resources.hpp"
#include "Device.hpp"

//----------------------------------------------------------------------------//
// App
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
App::App(void)
{
	LOG("Create App");
	new	Resources;
	new Game;
}
//----------------------------------------------------------------------------//
App::~App(void)
{
	LOG("Destroy App");
	delete gGame;
	delete gResources;
}
//----------------------------------------------------------------------------//
void App::Run(void)
{
	double _st, _et;
	float _dt = 0, _tt = 0, _pt = 0;
	float _fps = 0;
	int _frames = 0;

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

		//_scene->Update(_dt);
		gGame->Update(_dt);

		gGraphics->BeginFrame();
		gGraphics->ClearFrameBuffers(FBT_Color | FBT_DepthStencil, _clearColor, 1, 0xff);

		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_CLAMP);
		//glDepthRange(-1, 1);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//gRenderer->Draw(_scene);
		gGame->Draw();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glDisable(GL_CULL_FACE);



		gGraphics->SetShader(FS_NoTexture);


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

		/*if (gPlayer->GetHealth() <= 0)
		{
		LOG("Game over");
		break;
		} */

#if 1
		{
			float _dist = gPlayer->GetWorldPosition().Distance(gTargetPos);
			//AlignedBox _worldBB = _scene->GetDbvt()->Root()->box;
			//gDevice->SetTitle("%f %f %f  %f %f %f", _worldBB.mn.x, _worldBB.mn.y, _worldBB.mn.z, _worldBB.mx.x, _worldBB.mx.y, _worldBB.mx.z);
			gDevice->SetTitle("Health: %.1f, Distance: %.1f", gPlayer->GetHealth(), _dist);
		}
#endif
	}

}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
