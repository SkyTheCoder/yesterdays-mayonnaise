//------------------------------------------------------------------------------
//
// File Name:	LevelSelect.cpp
// Author(s):	Daniel Walther (daniel.walther)
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
#include "LevelSelect.h"

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
#include "Level1.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

namespace Levels
{
	// Creates an instance of LevelSelect.
	LevelSelect::LevelSelect()
		: Level("LevelSelect")
	{
	}

	// Load the resources associated with LevelSelect.
	void LevelSelect::Load()
	{
		std::cout << "LevelSelect::Load" << std::endl;

		// Create a new quad mesh for the sprite.
		meshButton = CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5f, 0.5f));

		// Load the player texture.
		textureButton = Texture::CreateTextureFromFile("Button.png");

		// Setup the player sprite source.
		spriteSourceButton = new SpriteSource(1, 1, textureButton);
	}

	// Initialize the memory associated with LevelSelect.
	void LevelSelect::Initialize()
	{
		std::cout << "LevelSelect::Initialize" << std::endl;

		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		objectManager.AddArchetype(*Archetypes::CreateButtonArchetype(meshButton, spriteSourceButton));

		// Create and add descriptive text
		objectManager.AddArchetype(*Archetypes::CreateText());
		GameObject* text = new GameObject(*objectManager.GetArchetypeByName("Text"));
		static_cast<SpriteText*>(text->GetComponent("SpriteText"))->SetText("Select Your Level");
		static_cast<Transform*>(text->GetComponent("Transform"))->SetTranslation(Vector2D(0.0f, 250.0f));
		objectManager.AddObject(*text);

		AddMapButton("Tutorial", Vector2D(-175.0f, 150.0f), Levels::Map::Tutorial);
		AddMapButton("Arena 3", Vector2D(175.0f, 150.0f), Levels::Map::Arena3);
		AddMapButton("MediumBoy", Vector2D(-175.0f, 50.0f), Levels::Map::MediumBoy);
		AddMapButton("Channels", Vector2D(175.0f, 50.0f), Levels::Map::Channels);
		AddMapButton("Separation", Vector2D(-175.0f, -50.0f), Levels::Map::Separation);
		AddMapButton("Main Menu", Vector2D(0.0f, -250.0f), Levels::Map::MainMenu);

		Camera& camera = Graphics::GetInstance().GetCurrentCamera();
		camera.SetTranslation(Vector2D());
		camera.SetDistance(60.0f);
	}

	// Update LevelSelect.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void LevelSelect::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
	}

	// Unload the resources associated with LevelSelect.
	void LevelSelect::Unload()
	{
		std::cout << "LevelSelect::Unload" << std::endl;

		delete spriteSourceButton;
		delete textureButton;
		delete meshButton;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Adds a new map button.
	// name = The name of the level.
	// position = The position of the button.
	// map = The map the button should switch to.
	void LevelSelect::AddMapButton(const char* name_, Vector2D position, Levels::Map map)
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
