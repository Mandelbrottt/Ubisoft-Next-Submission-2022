#include "pch.h"

#include "Application.h"
#include "Init.h"

#include "Components/Camera.h"
#include "Components/LineRenderer.h"
#include "Components/ModelRenderer.h"
#include "Components/Transform.h"
#include "Components/Colliders/SphereCollider.h"

#include "Math/Transformations.h"

#include "Rendering/Renderer.h"

#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"

#include "Scripting/Entity.h"

// Readonly outside of this file
Next::Scene* g_mainLoop_activeScene = nullptr;

Next::Scene* g_mainLoop_sceneToChangeTo = nullptr;

static
void
ProcessCollisions();

static
void
CheckForSceneChange();

namespace Next::SceneManager
{
	extern
	void
	RegisterAllScenes();
}

void
Init()
{
	srand(static_cast<uint32_t>(time(NULL)));

	Next::SceneManager::RegisterAllScenes();

	// Assuming StartingScene macro is used, _STARTING_SCENE is guaranteed to be valid
	auto const& sceneType = Next::Reflection::Type::TryGet(_STARTING_SCENE);

	Next::SceneManager::LoadScene(*sceneType);

	assert(g_mainLoop_sceneToChangeTo);
}

// Declare System Updates manually so as to not expose it unnecessarily
namespace Next
{
	namespace Time
	{
		extern
		void
		Update(float a_deltaTime);
	}

	namespace Input
	{
		extern
		void
		Update();
	}
}

void
Update(float a_deltaTime)
{
	CheckForSceneChange();

	// Convert deltaTime into seconds because NextAPI uses milliseconds
	float timeInSeconds = a_deltaTime / 1000.f;
	Next::Time::Update(timeInSeconds);

	Next::Input::Update();

	Next::Entity::Update();

	ProcessCollisions();
}

namespace Next::Gui
{
	extern
	void
	Flush();
}

void
Render()
{
	using namespace Next;

	float fov  = 90;
	float near = 0.1f;
	float far  = 1000.0f;

	Vector3 cameraPosition { 0 };
	Vector3 cameraForward { 0, 0, 1 };
	Matrix4 viewMatrix = Matrix4::Identity();

	CubeMap skybox;

	static std::vector<Camera*> cameras;
	Entity::GetAllComponents(cameras);

	auto iter = std::find_if(
		cameras.begin(),
		cameras.end(),
		[](Camera* a_camera)
		{
			return a_camera->IsActive();
		}
	);
	
	if (iter != cameras.end())
	{
		auto* camera = *iter;

		fov  = camera->GetFov();
		near = camera->GetNearClippingPlane();
		far  = camera->GetFarClippingPlane();

		auto* transform = camera->Transform();

		cameraPosition = transform->GetPosition();
		cameraForward  = transform->Forward();

		viewMatrix = transform->GetTransformationMatrix();
		viewMatrix = Matrix::ViewInverse(viewMatrix);

		skybox = camera->GetSkybox();
	}

	float aspect = Application::ScreenHeight() / Application::ScreenWidth();

	// Perspective Matrix
	auto perspective = Matrix::Perspective(fov, aspect, near, far);

	Renderer::PrepareSceneDescriptor descriptor = {
		cameraPosition,
		cameraForward,
		viewMatrix,
		perspective,
		std::move(skybox)
	};

	PrepareScene(descriptor);

	// Submit all of the Lights in the scene
	static std::vector<Light*> lights;
	Entity::GetAllComponents(lights);

	for (auto* light : lights)
	{
		if (!light->IsActive())
		{
			continue;
		}
		
		Renderer::Submit(light, light->Transform());
	}

	// Submit all of the ModelRenderers in the scene
	static std::vector<ModelRenderer*> modelRenderers;
	Entity::GetAllComponents(modelRenderers);

	for (auto* modelRenderer : modelRenderers)
	{
		if (!modelRenderer->IsActive())
		{
			continue;
		}
		
		Renderer::Submit(modelRenderer, modelRenderer->Transform());
	}

	Renderer::Flush();

	Gui::Flush();
}

void
Shutdown()
{
	// TODO: cleanup entities
}

void
CheckForSceneChange()
{
	if (g_mainLoop_sceneToChangeTo == nullptr)
	{
		return;
	}

	if (g_mainLoop_activeScene)
	{
		g_mainLoop_activeScene->OnSceneDestroy();
	}

	Next::Entity::Registry().Reset();

	g_mainLoop_activeScene     = g_mainLoop_sceneToChangeTo;
	g_mainLoop_sceneToChangeTo = nullptr;

	g_mainLoop_activeScene->OnSceneCreate();
}

// TODO: Refactor collision checking into separate file

struct CollisionPair
{
	Next::EntityId lhsEntityId;
	Next::EntityId rhsEntityId;
};

static std::vector<CollisionPair> g_thisFrameCollisions;
static std::vector<CollisionPair> g_lastFrameCollisions;

static
bool
FindCollisionPairPredicate(CollisionPair const& a_lhs, CollisionPair const& a_rhs)
{
	return (a_lhs.lhsEntityId == a_rhs.lhsEntityId && a_lhs.rhsEntityId == a_rhs.rhsEntityId);
}

static
void
ProcessSphereCollisions(Next::Detail::Registry& a_registry);

void
ProcessCollisions()
{
	ProcessSphereCollisions(Next::Entity::Registry());
}

void
ProcessSphereCollisions(Next::Detail::Registry& a_registry)
{
	using namespace Next;

	static std::vector<SphereCollider*> sphereColliders;

	Entity::GetAllComponents(sphereColliders);

	// Broadphase collision, but for spheres there really only can be broadphase collisions?
	for (int i = 0; i < sphereColliders.size(); i++)
	{
		SphereCollider* lhsCollider = sphereColliders[i];

		Vector3 lhsPosition = lhsCollider->Transform()->GetPosition();
		float   lhsSize     = lhsCollider->radius;
		for (int j = 0; j < sphereColliders.size(); j++)
		{
			SphereCollider* rhsCollider = sphereColliders[j];

			if (lhsCollider == rhsCollider)
			{
				continue;
			}

			Vector3 rhsPosition = rhsCollider->Transform()->GetPosition();
			float   rhsSize     = rhsCollider->radius;

			Vector3 lhsToRhs = lhsPosition - rhsPosition;
			float   distSq   = lhsToRhs.MagnitudeSquared();

			bool colliding = distSq < (lhsSize * lhsSize + rhsSize * rhsSize);

			if (colliding)
			{
				g_thisFrameCollisions.push_back({ lhsCollider->GetEntityId(), rhsCollider->GetEntityId() });
			}
		}
	}

	struct CollisionQueueElement
	{
		EntityId lhsEntityId;

		Collider* rhsCollider;

		enum
		{
			CollisionStart,
			CollisionStay,
			CollisionEnd,
		} collisionFunc;
	};

	std::vector<CollisionQueueElement> collisionQueue;

	for (auto const& collision : g_thisFrameCollisions)
	{
		auto iter = g_lastFrameCollisions.begin();
		auto end  = g_lastFrameCollisions.end();

		while (iter != end)
		{
			if (FindCollisionPairPredicate(collision, *iter))
			{
				break;
			}

			++iter;
		}

		if (iter == g_lastFrameCollisions.end())
		{
			auto rhsCollider = a_registry.GetComponent<SphereCollider>(collision.rhsEntityId);
			collisionQueue.push_back({ collision.lhsEntityId, rhsCollider, CollisionQueueElement::CollisionStart });
		} else
		{
			auto rhsCollider = a_registry.GetComponent<SphereCollider>(iter->rhsEntityId);
			collisionQueue.push_back({ iter->lhsEntityId, rhsCollider, CollisionQueueElement::CollisionStay });

			g_lastFrameCollisions.erase(iter);
		}
	}

	for (auto const& [lhsEntityId, rhsEntityId] : g_lastFrameCollisions)
	{
		auto rhsCollider = a_registry.GetComponent<SphereCollider>(rhsEntityId);
		collisionQueue.push_back({ lhsEntityId, rhsCollider, CollisionQueueElement::CollisionEnd });
	}

	for (auto& [lhsEntityId, rhsCollider, collisionFunc] : collisionQueue)
	{
		// static to avoid allocations
		static std::vector<Component*> components;
		components.clear();

		a_registry.GetAllComponentsForEntity(lhsEntityId, components);

		for (Component* component : components)
		{
			switch (collisionFunc)
			{
				case CollisionQueueElement::CollisionStart:
				{
					component->OnTriggerCollisionStart(rhsCollider);
					break;
				}
				case CollisionQueueElement::CollisionStay:
				{
					component->OnTriggerCollision(rhsCollider);
					break;
				}
				case CollisionQueueElement::CollisionEnd:
				{
					component->OnTriggerCollisionEnd(rhsCollider);
					break;
				}
			}
		}
	}

	g_lastFrameCollisions = std::move(g_thisFrameCollisions);
}
