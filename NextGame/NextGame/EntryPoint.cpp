#include <Windows.h>

#include <NextAPI/app.h>

#include <NextCore/Graphics/Sprite.h>
#include <NextCore/Util/Time.h>
#include <NextCore/Application/Application.h>
#include <NextCore/Math/Matrix.h>

int APIENTRY wWinMain(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPWSTR    lpCmdLine,
	_In_     int       nCmdShow
);

int main()
{
	HINSTANCE hInstance     = GetModuleHandleW(NULL);
	HINSTANCE hPrevInstance = 0;
	LPWSTR    lpCmdLine     = GetCommandLineW();
	int       nShowCmd      = 0;

	// HACK: Call NextAPIs wWinMain() as a workaround to let us bootstrap NextAPI
	return wWinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}

void
ExampleInit();

void
ExampleUpdate();

void
ExampleRender();

void
ExampleShutdown();

extern
void
Init()
{
	ExampleInit();
}

// Declare Time::Update manually so as to not expose it unnecessarily
namespace NextCore::Time
{
	NEXT_CORE_EXPORT
	extern
	void
	Update(float a_deltaTime);
}

extern
void
Update(float a_deltaTime)
{
	// Convert deltaTime into seconds because NextAPI uses milliseconds
	float timeInSeconds = a_deltaTime / 1000.f;
	NextCore::Time::Update(timeInSeconds);
	
	ExampleUpdate();
}

extern
void
Render()
{
	ExampleRender();
}

extern
void
Shutdown()
{
	ExampleShutdown();

	NextCore::Sprite::Cleanup();
}

//------------------------------------------------------------------------
// Example data....
//------------------------------------------------------------------------
using NextCore::Sprite;
using NextCore::Math::Vector2;

Sprite* g_testSprite;
Sprite* g_testSprite2;

enum
{
	ANIM_FORWARDS,
	ANIM_BACKWARDS,
	ANIM_LEFT,
	ANIM_RIGHT,
};

//------------------------------------------------------------------------

using namespace NextCore;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void
ExampleInit()
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	{
		//testSprite = App::CreateSprite(".\\Resources\\Test.bmp", 8, 4);
		g_testSprite = Sprite::Create(Application::ResourcePath() + "Test.bmp", 8, 4);
		g_testSprite->SetPosition(400.0f, 400.0f);
	
		float speed = 1.0f / 15.0f;
		g_testSprite->CreateAnimation(ANIM_BACKWARDS, speed, { 0, 1, 2, 3, 4, 5, 6, 7 });
		g_testSprite->CreateAnimation(ANIM_LEFT, speed, { 8, 9, 10, 11, 12, 13, 14, 15 });
		g_testSprite->CreateAnimation(ANIM_RIGHT, speed, { 16, 17, 18, 19, 20, 21, 22, 23 });
		g_testSprite->CreateAnimation(ANIM_FORWARDS, speed, { 24, 25, 26, 27, 28, 29, 30, 31 });

		g_testSprite->SetScale(2.0f);
	}
	{
		//testSprite2 = App::CreateSprite(".\\Resources\\Ships.bmp", 2, 12);
		g_testSprite2 = Sprite::Create(Application::ResourcePath() + "Ships.bmp", 2, 12);
		g_testSprite2->SetPosition(400.0f, 400.0f);
		g_testSprite2->SetFrame(2);
		g_testSprite2->SetScale(1.0f);
	}
	//------------------------------------------------------------------------
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void
ExampleUpdate()
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	g_testSprite->Update();
	g_testSprite2->Update();
	
	Vector2 position = g_testSprite->GetPosition();

	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		g_testSprite->SetAnimation(ANIM_RIGHT);
		position.x += 1.0f;
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		g_testSprite->SetAnimation(ANIM_LEFT);
		position.x -= 1.0f;
	}
	if (App::GetController().GetLeftThumbStickY() > 0.5f)
	{
		g_testSprite->SetAnimation(ANIM_FORWARDS);
		position.y += 1.0f;
	}
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		g_testSprite->SetAnimation(ANIM_BACKWARDS);
		position.y -= 1.0f;
	}
	
	g_testSprite->SetPosition(position);

	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
	{
		g_testSprite->SetScale(g_testSprite->GetScale() + 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
	{
		g_testSprite->SetScale(g_testSprite->GetScale() - 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
	{
		g_testSprite->SetAngle(g_testSprite->GetAngle() + 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	{
		g_testSprite->SetAngle(g_testSprite->GetAngle() - 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
	{
		g_testSprite->SetAnimation(-1);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		g_testSprite->SetVertex(0, g_testSprite->GetVertex(0) + Vector2(5.0f, 0));
	}
	//------------------------------------------------------------------------
	// Sample Sound.
	//------------------------------------------------------------------------
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		std::string path = Application::ResourcePath() + "Test.wav";
		App::PlaySound(path.c_str());
	}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void
ExampleRender()
{
	//------------------------------------------------------------------------
	// Example Line Drawing.
	//------------------------------------------------------------------------
	static float a = 0.0f;
	float        r = 1.0f;
	float        g = 1.0f;
	float        b = 1.0f;
	a += 10 * Time::DeltaTime();
	for (int i = 0; i < 20; i++)
	{
		float sx = 200 + sinf(a + i * 0.1f) * 60.0f;
		float sy = 200 + cosf(a + i * 0.1f) * 60.0f;
		float ex = 700 - sinf(a + i * 0.1f) * 60.0f;
		float ey = 700 - cosf(a + i * 0.1f) * 60.0f;
		g        = (float) i / 20.0f;
		b        = (float) i / 20.0f;
		App::DrawLine(sx, sy, ex, ey, r, g, b);
	}

	//------------------------------------------------------------------------
	// Example Sprite Code....
	g_testSprite->Render();
	g_testSprite2->Render();
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	// Example Text.
	//------------------------------------------------------------------------
	App::Print(100, 100, "Sample Text");
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void
ExampleShutdown()
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	//------------------------------------------------------------------------
}
