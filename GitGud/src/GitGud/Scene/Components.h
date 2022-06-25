#pragma once

#include "Components/CoreComponents.h"
#include "Components/Renderer2DComponents.h"
#include "Components/Physics2DComponents.h"
#include "Components/GameplayComponents.h"

namespace GitGud
{
	template<typename... TCmp>
	struct ComponentGroup
	{
	};

	using AllComponents = ComponentGroup<TransformComponent, SpriteRendererComponent, CameraComponent, CircleRendererComponent, NativeScriptComponent, Rigidbody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent>;
}