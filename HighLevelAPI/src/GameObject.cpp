//------------------------------------------------------------------------------
//
// File Name:	GameObject.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "GameObject.h"

// Systems
#include "Component.h"
#include "Space.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a new game object.
// Params:
//	 name = The name of the game object being created.
GameObject::GameObject(const std::string& name) : BetaObject(name), numComponents(0), isDestroyed(false)
{
}

// Clone a game object from another game object.
// Params:
//	 other = A reference to the object being cloned.
GameObject::GameObject(const GameObject& other) : BetaObject(other.GetName(), other.GetParent()), numComponents(other.numComponents), isDestroyed(false)
{
	for (size_t i = 0; i < numComponents; i++)
	{
		components[i] = other.components[i]->Clone();
		components[i]->SetParent(this);
	}
}

// Free the memory associated with a game object.
GameObject::~GameObject()
{
	for (size_t i = 0; i < numComponents; i++)
	{
		delete components[i];
	}
}

// Initialize this object's components and set it to active.
void GameObject::Initialize()
{
	for (size_t i = 0; i < numComponents; i++)
	{
		components[i]->Initialize();
	}
}

// Update any components attached to the game object.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameObject::Update(float dt)
{
	if (isDestroyed)
		return;

	for (size_t i = 0; i < numComponents; i++)
	{
		components[i]->Update(dt);
	}
}

// Updates components using a fixed timestep (usually just physics)
// Params:
//	 dt = A fixed change in time, usually 1/60th of a second.
void GameObject::FixedUpdate(float dt)
{
	if (isDestroyed)
		return;

	for (size_t i = 0; i < numComponents; i++)
	{
		components[i]->FixedUpdate(dt);
	}
}

// Draw any visible components attached to the game object.
void GameObject::Draw()
{
	for (size_t i = 0; i < numComponents; i++)
	{
		components[i]->Draw();
	}
}

// Adds a component to the object.
void GameObject::AddComponent(Component* component)
{
	if (numComponents >= 10)
		return;

	components[numComponents++] = component;
	component->SetParent(this);
}

// Retrieves the component with the given name if it exists.
// Params:
//   name = The name of the component to find.
Component* GameObject::GetComponent(const std::string& name_)
{
	for (size_t i = 0; i < numComponents; i++)
	{
		if (components[i]->GetName() == name_)
			return components[i];
	}

	return nullptr;
}

// Mark an object for destruction.
void GameObject::Destroy()
{
	isDestroyed = true;
}

// Whether the object has been marked for destruction.
// Returns:
//		True if the object will be destroyed, false otherwise.
bool GameObject::IsDestroyed() const
{
	return isDestroyed;
}

// Get the space that contains this object.
Space* GameObject::GetSpace() const
{
	return static_cast<Space*>(GetParent());
}

//------------------------------------------------------------------------------
