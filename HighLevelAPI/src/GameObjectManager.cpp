//------------------------------------------------------------------------------
//
// File Name:	GameObjectManager.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "GameObjectManager.h"

// Systems
#include "Space.h"
#include <Vector2D.h>

// Components
#include "Collider.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor(s)
GameObjectManager::GameObjectManager(Space* space) : BetaObject("GameObjectmanager", space),
	fixedUpdateDt(1.0f / 60.0f),
	numObjects(0), numArchetypes(0), timeAccumulator(0.0f)
{
}

// Destructor
GameObjectManager::~GameObjectManager()
{
	Shutdown();
	Unload();
}

// Update all objects in the active game objects list.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameObjectManager::Update(float dt)
{
	Space* space = static_cast<Space*>(GetParent());
	if (!space->IsPaused())
	{
		VariableUpdate(dt);
		FixedUpdate(dt);
	}

	DestroyObjects();

	Draw();
}

// Shutdown the game object manager, destroying all active objects.
void GameObjectManager::Shutdown(void)
{
	for (size_t i = 0; i < numObjects; i++)
	{
		delete gameObjectActiveList[i];
	}

	numObjects = 0;
}

// Unload the game object manager, destroying all object archetypes.
void GameObjectManager::Unload(void)
{
	for (size_t i = 0; i < numArchetypes; i++)
	{
		delete gameObjectArchetypes[i];
	}

	numArchetypes = 0;
}

// Add a game object to the active game object list.
// Params:
//	 gameObject = Reference to the game object to be added to the list.
void GameObjectManager::AddObject(GameObject& gameObject)
{
	// Check if the object array is full.
	if (numObjects >= maxObjects)
	{
		// Delete the object since we won't be keeping track of it.
		delete &gameObject;
		return;
	}

	gameObjectActiveList[numObjects++] = &gameObject;
	gameObject.SetParent(GetParent());
	gameObject.Initialize();
}

// Add a game object to the game object archetype list.
// Params:
//	 gameObject = Reference to the game object to be added to the list.
void GameObjectManager::AddArchetype(GameObject& gameObject)
{
	// Check if the archetype array is full.
	if (numArchetypes >= maxArchetypes)
	{
		// Delete the object since we won't be keeping track of it.
		delete &gameObject;
		return;
	}

	gameObjectArchetypes[numArchetypes++] = &gameObject;
}

// Returns a pointer to the first active game object matching the specified name.
// Params:
//	 objectName = The name of the object to be returned, if found.
// Returns:
//   If the named object is found,
//	   then return the pointer to the named game object,
//	   else return nullptr.
GameObject* GameObjectManager::GetObjectByName(const std::string& objectName) const
{
	for (size_t i = 0; i < numObjects; i++)
	{
		if (gameObjectActiveList[i]->GetName() == objectName)
			return gameObjectActiveList[i];
	}

	return nullptr;
}

// Returns a pointer to the first game object archetype matching the specified name.
// Params:
//	 objectName = The name of the archetype to be returned, if found.
// Returns:
//   If the named archetype is found,
//	   then return the pointer to the named game object archetype,
//	   else return nullptr.
GameObject* GameObjectManager::GetArchetypeByName(const std::string& objectName) const
{
	for (size_t i = 0; i < numArchetypes; i++)
	{
		if (gameObjectArchetypes[i]->GetName() == objectName)
			return gameObjectArchetypes[i];
	}

	return nullptr;
}

// Returns the number of active objects with the given name.
// Params:
//   objectName = The name of the objects that should be counted.
unsigned GameObjectManager::GetObjectCount(const std::string& objectName) const
{
	unsigned count = 0;

	for (size_t i = 0; i < numObjects; i++)
	{
		// If the object is not destroyed and its name matches the search, increment count.
		if (!gameObjectActiveList[i]->IsDestroyed() && gameObjectActiveList[i]->GetName() == objectName)
			++count;
	}

	return count;
}

// Update object logic using variable timestep.
void GameObjectManager::VariableUpdate(float dt)
{
	for (size_t i = 0; i < numObjects; i++)
	{
		gameObjectActiveList[i]->Update(dt);
	}
}

// Update object physics using fixed timestep.
void GameObjectManager::FixedUpdate(float dt)
{
	timeAccumulator += dt;

	// Call the FixedUpdate functions the appropriate amount of times.
	while (timeAccumulator >= fixedUpdateDt)
	{
		for (size_t i = 0; i < numObjects; i++)
		{
			gameObjectActiveList[i]->FixedUpdate(fixedUpdateDt);
		}

		CheckCollisions();

		timeAccumulator -= fixedUpdateDt;
	}
}

// Destroy any objects marked for destruction.
void GameObjectManager::DestroyObjects()
{
	for (size_t i = 0; i < numObjects; i++)
	{
		if (gameObjectActiveList[i]->IsDestroyed())
		{
			delete gameObjectActiveList[i];

			// To avoid empty entries, move the last object in the array to the index of the freshly destroyed object.
			gameObjectActiveList[i] = gameObjectActiveList[--numObjects];
		}
	}
}

// Draw all game objects in the active game object list.
void GameObjectManager::Draw(void)
{
	for (size_t i = 0; i < numObjects; i++)
	{
		gameObjectActiveList[i]->Draw();
	}
}

// Check for collisions between each pair of objects.
void GameObjectManager::CheckCollisions()
{
	// Iterate over every object.
	for (size_t i = 0; i < numObjects; i++)
	{
		// If the current object is marked for destruction, skip it.
		if (gameObjectActiveList[i]->IsDestroyed())
			continue;

		const char* name = gameObjectActiveList[i]->GetName().c_str();
		if (strcmp(name, "Player") && strcmp(name, "Tilemap"))
			continue;

		// Get the current object's collider.
		Collider* collider = static_cast<Collider*>(gameObjectActiveList[i]->GetComponent("Collider"));

		// If the current object does not have a collider, skip it.
		if (collider == nullptr)
			continue;

		// Iterate over all remaining objects.
		for (size_t j = i + 1; j < numObjects; j++)
		{
			// If the current object is marked for destruction, skip it.
			if (gameObjectActiveList[j]->IsDestroyed())
				continue;

			// Get the current object's collider.
			Collider* collider2 = static_cast<Collider*>(gameObjectActiveList[j]->GetComponent("Collider"));

			// If the current object does not have a collider, skip it.
			if (collider2 == nullptr)
				continue;

			// Check for collisions between the two objects.
			collider->CheckCollision(*collider2);
		}
	}
}

//------------------------------------------------------------------------------
