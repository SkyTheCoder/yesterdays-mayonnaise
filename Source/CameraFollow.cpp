//------------------------------------------------------------------------------
//
// File Name:	CameraFollow.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		Cookie Manor
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2018 DigiPen (USA) Corporation.
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
	//   distanceLerp = How close the smoothed distance should be to the target distance after 1 second.
	CameraFollow::CameraFollow(Vector2D velocityLookScalar, float targetLerp, float velocityLerp, float distanceLerp) : Component("CameraFollow"), velocityLookScalar(velocityLookScalar), targetLerp(targetLerp), velocityLerp(velocityLerp), distanceLerp(distanceLerp)
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
		for (size_t i = 0; i < players.size(); i++)
		{
			GameObject* gameObject = players[i].GetGameObject();
			if (gameObject == nullptr)
			{
				players.erase(players.begin() + i--);
				continue;
			}

			players[i].transform = static_cast<Transform*>(gameObject->GetComponent("Transform"));
			players[i].physics = static_cast<Physics*>(gameObject->GetComponent("Physics"));
		}

		SnapToTarget();
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void CameraFollow::Update(float dt)
	{
		if (players.empty())
			return;

		// Calculate the values to use for lerping so that they are not framerate-dependant.
		float velocityMix = 1.0f - pow(1.0f - velocityLerp, dt);
		float targetMix = 1.0f - pow(1.0f - targetLerp, dt);
		float distanceMix = 1.0f - pow(1.0f - distanceLerp, dt);

		std::vector<Vector2D> targetTranslations;

		for (size_t i = 0; i < players.size(); i++)
		{
			GameObject* gameObject = players[i].GetGameObject();
			if (gameObject == nullptr)
			{
				players.erase(players.begin() + i--);
				continue;
			}

			// Only update velocity when the player is moving.
			if (players[i].physics->GetVelocity().Magnitude() > 100.0f)
			{
				players[i].velocity = players[i].physics->GetVelocity().Normalized();
			}

			// Smoothly interpolate the velocity.
			players[i].smoothedVelocity = Interpolate(players[i].smoothedVelocity, players[i].velocity, velocityMix);

			Vector2D targetTranslation = players[i].transform->GetTranslation() + Vector2D(players[i].smoothedVelocity.x * velocityLookScalar.x, players[i].smoothedVelocity.y * velocityLookScalar.y);
			targetTranslations.push_back(targetTranslation);
		}

		Vector2D targetTranslationSum(0.0f, 0.0f);
		float highestDistance = 0.0f;
		for (size_t i = 0; i < targetTranslations.size(); i++)
		{
			for (size_t j = i + 1; j < targetTranslations.size(); j++)
			{
				float distance = targetTranslations[i].Distance(targetTranslations[j]);

				if (distance > highestDistance)
				{
					highestDistance = distance;
				}
			}

			targetTranslationSum += targetTranslations[i];
		}

		Camera& camera = Graphics::GetInstance().GetCurrentCamera();

		float mix = max(0.0f, min(1.75f, (highestDistance - 150.0f) / 2000.0f));
		float distance = Interpolate(58.75f, 55.75f, mix);
		
		// Smoothly interpolate the camera to its new position and distance.
		camera.SetTranslation(Interpolate(camera.GetTranslation(), targetTranslationSum / static_cast<float>(targetTranslations.size()), targetMix));
		camera.SetDistance(Interpolate(camera.GetDistance(), distance, distanceMix));
	}

	// Snaps the camera to the target.
	void CameraFollow::SnapToTarget()
	{
		if (players.empty())
			return;

		std::vector<Vector2D> targetTranslations;

		for (size_t i = 0; i < players.size(); i++)
		{
			GameObject* gameObject = players[i].GetGameObject();
			if (gameObject == nullptr)
			{
				players.erase(players.begin() + i--);
				continue;
			}

			Vector2D targetTranslation = players[i].transform->GetTranslation() + Vector2D(players[i].smoothedVelocity.x * velocityLookScalar.x, players[i].smoothedVelocity.y * velocityLookScalar.y);
			targetTranslations.push_back(targetTranslation);
		}

		Vector2D targetTranslationSum(0.0f, 0.0f);
		float highestDistance = 0.0f;
		for (size_t i = 0; i < targetTranslations.size(); i++)
		{
			for (size_t j = i + 1; j < targetTranslations.size(); j++)
			{
				float distance = targetTranslations[i].Distance(targetTranslations[j]);

				if (distance > highestDistance)
				{
					highestDistance = distance;
				}
			}

			targetTranslationSum += targetTranslations[i];
		}

		Camera& camera = Graphics::GetInstance().GetCurrentCamera();

		float mix = max(0.0f, min(1.0f, (highestDistance - 150.0f) / 2000.0f));
		float distance = Interpolate(59.5f, 56.5f, mix);

		camera.SetTranslation(targetTranslationSum / static_cast<float>(players.size()));
		camera.SetDistance(distance);
	}

	// Adds a player to the player list.
	// Params:
	//   player = The new player to follow.
	void CameraFollow::AddPlayer(GameObject* player)
	{
		players.push_back(PlayerData(player->GetID()));
	}

	//------------------------------------------------------------------------------
	// Private Structures:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   gameObject = The game object of the player.
	CameraFollow::PlayerData::PlayerData(GUID id) : id(id), transform(nullptr), physics(nullptr), velocity(0.0f, 0.0f), smoothedVelocity(0.0f, 0.0f)
	{
	}

	// Gets the player's game object.
	// Returns:
	//   If it exists, the player's game object, otherwise, nullptr.
	GameObject* CameraFollow::PlayerData::GetGameObject()
	{
		return static_cast<GameObject*>(BetaObject::GetObjectByID(id));
	}
}

//------------------------------------------------------------------------------
