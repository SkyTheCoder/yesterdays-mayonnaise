//------------------------------------------------------------------------------
//
// File Name:	Controls.cpp
// Author(s):	A.J. Bussman
// Project:		Yesterday's Mayonnaise
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Controls.h"

// Systems
#include "Archetypes.h"
#include "Space.h"
#include <Input.h>
#include "MeshHelper.h"
#include <Texture.h>
#include <SpriteSource.h>
#include <Mesh.h>
#include <Graphics.h>
#include <Camera.h>

// Components
#include "SpriteText.h"
#include <Transform.h>
#include "Button.h"

// Levels
#include "MainMenu.h"
#include "Level1.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

namespace Levels
{
	// Creates an instance of Controls.
	Controls::Controls() : Level("Controls")
	{
	}

	// Load the resources associated with Controls.
	void Controls::Load()
	{
		std::cout << "Controls::Load" << std::endl;

		// Create the mesh and sprite source for the main menu.
		meshBackground = CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5f, 0.5f));
		textureBackground = Texture::CreateTextureFromFile("Controls.png");
		spriteSourceBackground = new SpriteSource(1, 1, textureBackground);

		// Create a new quad mesh for the sprite.
		meshButton = CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5f, 0.5f));

		// Load the player texture.
		textureButton = Texture::CreateTextureFromFile("Button.png");

		// Setup the player sprite source.
		spriteSourceButton = new SpriteSource(1, 1, textureButton);
	}

	// Initialize the memory associated with Controls.
	void Controls::Initialize()
	{
		std::cout << "Controls::Initialize" << std::endl;

		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		objectManager.AddArchetype(*Archetypes::CreateFullScreenImageArchetype(meshBackground, spriteSourceBackground));
		objectManager.AddObject(*new GameObject(*objectManager.GetArchetypeByName("FullScreenImage")));

		objectManager.AddArchetype(*Archetypes::CreateButtonArchetype(meshButton, spriteSourceButton));

		// Add text archetype.
		objectManager.AddArchetype(*Archetypes::CreateText());

		AddMapButton("Main Menu", Vector2D(0.0f, -250.0f), Levels::Map::MainMenu);

		Camera& camera = Graphics::GetInstance().GetCurrentCamera();
		camera.SetTranslation(Vector2D());
		camera.SetDistance(60.0f);
	}

	// Update Controls.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Controls::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
	}

	// Unload the resources associated with Controls.
	void Controls::Unload()
	{
		std::cout << "Controls::Unload" << std::endl;

		delete spriteSourceButton;
		delete textureButton;
		delete meshButton;

		delete spriteSourceBackground;
		delete textureBackground;
		delete meshBackground;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Adds a new map button.
	// name = The name of the level.
	// position = The position of the button.
	// map = The map the button should switch to.
	void Controls::AddMapButton(const char* name_, Vector2D position, Levels::Map map)
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		GameObject* levelButton = new GameObject(*objectManager.GetArchetypeByName("Button"));
		static_cast<Transform*>(levelButton->GetComponent("Transform"))->SetTranslation(position);
		static_cast<Behaviors::Button*>(levelButton->GetComponent("Button"))->SetMap(map);
		objectManager.AddObject(*levelButton);

		GameObject*text = new GameObject(*objectManager.GetArchetypeByName("Text"));
		static_cast<SpriteText*>(text->GetComponent("SpriteText"))->SetText(name_);
		static_cast<SpriteText*>(text->GetComponent("SpriteText"))->SetColor(Color(0.0f, 0.0f, 0.0f));
		static_cast<Transform*>(text->GetComponent("Transform"))->SetTranslation(position);
		objectManager.AddObject(*text);
	}
}
//----------------------------------------------------------------------------