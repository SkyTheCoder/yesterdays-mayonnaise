//------------------------------------------------------------------------------
//
// File Name:	CameraFollow.h
// Author(s):	David Cohen (david.cohen)
// Project:		Cookie Manor
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h" // base class

#include <Vector2D.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------


typedef class Transform Transform;
typedef class Physics Physics;
typedef struct Event Event;
typedef class Texture Texture;
typedef class Mesh Mesh;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{

	class CameraFollow : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		// Params:
		//   velocityLookScalar = How much velocity should influence the camera's offset.
		//   targetLerp = How far along the path to the target the camera should be over the course of 1 second.
		//   velocityLerp = How close the smoothed velocity should be to the current velocity after 1 second.
		CameraFollow(Vector2D velocityLookScalar = Vector2D(100.0f, 100.0f), float targetLerp = 0.9f, float velocityLerp = 0.9f);

		// Destructor
		~CameraFollow();

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		void SnapToTarget();

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Properties (save to/load from file)
		Vector2D velocityLookScalar;
		float targetLerp;
		float velocityLerp;

		// Components
		Transform* transform;
		Physics* physics;

		// Other variables
		Vector2D velocity;
		Vector2D smoothedVelocity;
	};
}

//------------------------------------------------------------------------------
