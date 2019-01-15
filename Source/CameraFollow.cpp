//------------------------------------------------------------------------------
//
// File Name:	CameraFollow.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		Cookie Manor
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "CameraFollow.h"

// Systems
#include "GameObject.h"
#include "Space.h"
#include "GameObjectManager.h"
#include <Input.h>
#include <Parser.h>
#include <Engine.h>
#include <Random.h>
#include <Graphics.h>
#include <Camera.h>
#include <Interpolation.h>

// Components
#include "Transform.h"
#include "Physics.h"
#include "Collider.h"
#include "Sprite.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   velocityLookScalar = How much velocity should influence the camera's offset.
	//   targetLerp = How far along the path to the target the camera should be over the course of 1 second.
	//   velocityLerp = How close the smoothed velocity should be to the current velocity after 1 second.
	CameraFollow::CameraFollow(Vector2D velocityLookScalar, float targetLerp, float velocityLerp) : Component("CameraFollow"), velocityLookScalar(velocityLookScalar), targetLerp(targetLerp), velocityLerp(velocityLerp), velocity(Vector2D(0.0f, 0.0f))
	{
	}

	// Destructor
	CameraFollow::~CameraFollow()
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* CameraFollow::Clone() const
	{
		return new CameraFollow(*this);
	}

	// Initialize this component (happens at object creation).
	void CameraFollow::Initialize()
	{
		// Store the required components for ease of access.
		transform = static_cast<Transform*>(GetOwner()->GetComponent("Transform"));
		physics = static_cast<Physics*>(GetOwner()->GetComponent("Physics"));

		SnapToTarget();
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void CameraFollow::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// Only update velocity when the player is moving.
		if (physics->GetVelocity().Magnitude() > 100.0f)
		{
			velocity = physics->GetVelocity().Normalized();
		}

		Camera& camera = Graphics::GetInstance().GetCurrentCamera();

		// Calculate the values to use for lerping so that they are not framerate-dependant.
		float velocityMix = 1.0f - pow(1.0f - velocityLerp, dt);
		float targetMix = 1.0f - pow(1.0f - targetLerp, dt);

		// Smoothly interpolate the velocity.
		smoothedVelocity = Interpolate(smoothedVelocity, velocity, velocityMix);

		// Smoothly interpolate the camera to its new position.
		Vector2D targetTranslation = transform->GetTranslation() + Vector2D(smoothedVelocity.x * velocityLookScalar.x, smoothedVelocity.y * velocityLookScalar.y);
		camera.SetTranslation(Interpolate(camera.GetTranslation(), targetTranslation, targetMix));
	}

	void CameraFollow::SnapToTarget()
	{
		Graphics::GetInstance().GetCurrentCamera().SetTranslation(transform->GetTranslation());
	}
}

//------------------------------------------------------------------------------
