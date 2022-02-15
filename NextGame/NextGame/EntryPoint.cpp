#include "pch.h"

#include <Windows.h>

#include <NextAPI/app.h>

#include <Graphics/Sprite.h>
#include <Application/Time.h>
#include <Application/Application.h>

#include <Scripting/Entity.h>

#include "Player.h"

int APIENTRY wWinMain(
	_In_ HINSTANCE     hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR        lpCmdLine,
	_In_ int           nCmdShow
);

int
main()
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
}

//------------------------------------------------------------------------
// Example data....
//------------------------------------------------------------------------
using namespace NextCore;
using NextCore::Sprite;
using NextCore::Math::Vector2;

std::vector<Scripting::Entity> g_entities;

//Scripting::Entity* g_entity1;
//Scripting::Entity* g_entity2;

//Sprite* g_testSprite;
//Sprite* g_testSprite2;

//------------------------------------------------------------------------

using namespace NextCore;

void
Foo();

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void
ExampleInit()
{
	g_entities.reserve(100);

	//------------------------------------------------------------------------
	// Example Sprite Code....
	{
		g_entities.emplace_back();
		Scripting::Entity& player = g_entities.back();
		player.AddComponent<Player>();
		int a = 2;
	}
	{
		g_entities.emplace_back();		
		Scripting::Entity& randomSprite = g_entities.back();
		
		auto* sprite = randomSprite.AddComponent<Sprite>();
		sprite->LoadFromTexture(Application::ResourcePath() + "Ships.bmp", 2, 12);
		sprite->SetPosition(400.0f, 400.0f);
		sprite->SetFrame(2);
		sprite->SetScale(1.0f);
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
	for (auto& entity : g_entities)
	{
		entity.OnUpdate();
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

	for (auto& entity : g_entities)
	{
		auto* sprite = entity.GetComponent<Sprite>();

		if (!sprite) continue;

		sprite->Render();
	}

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

struct ConstructorBase
{
	virtual
	~ConstructorBase() = default;

	virtual
	void*
	Construct() = 0;

	virtual
	void*
	Construct(void* a_location) = 0;

	virtual
	void
	Deconstruct(void* a_location) = 0;
};

template<typename T>
struct Constructor : ConstructorBase
{
	using value_type = T;

	~Constructor() override = default;

	void*
	Construct() override
	{
		//value_type* p = ::operator new(sizeof(value_type));
		//new(p) value_type;
		//return p;

		return new value_type;
	}

	void*
	Construct(void* a_location) override
	{
		return new(a_location) value_type;
	}

	void
	Deconstruct(void* a_location) override
	{
		value_type* p = static_cast<value_type*>(a_location);
		p->~value_type();
	}
};

struct Type
{
	size_t           size;
	ConstructorBase* constructor;
};

struct Thing
{
	Thing()
		: a(0)
	{
		puts("Thing Constructed");
	}

	virtual ~Thing()
	{
		puts("Thing Destructed");
	}

	virtual void
	Foo()
	{
		puts("Thing");
	}

	int a;
};

struct OtherThing : Thing
{
	OtherThing()
	{
		puts("OtherThing Constructed");
	}

	~OtherThing() override
	{
		puts("OtherThing Destructed");
	}

	void
	Foo() override
	{
		puts("OtherThing");
	}
};

void
Foo()
{
	//Type thing;
	//thing.size = sizeof(Thing);
	//thing.constructor = new Constructor<Thing>;
	//
	//void* construction = thing.constructor->Construct();
	//Thing* tConstruction = static_cast<Thing*>(construction);
	//tConstruction->a = 5;
	//thing.constructor->Deconstruct(construction);

	//puts("");
	//
	//Type other;
	//other.size = sizeof(OtherThing);
	//other.constructor = new Constructor<OtherThing>;

	//std::byte bytes[sizeof(OtherThing)];
	//construction = other.constructor->Construct(bytes);
	//tConstruction = static_cast<OtherThing*>(construction);
	//tConstruction->a = 5;
	//other.constructor->Deconstruct(construction);

	//static_assert(sizeof(Thing) == sizeof(OtherThing));
	//puts("");
	//
	//Thing t;
	//new(&t) OtherThing;
	//t.Foo();
	//(&t)->Foo();
}
