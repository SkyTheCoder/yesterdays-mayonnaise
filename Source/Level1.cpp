//------------------------------------------------------------------------------
//
// File Name:	Level1.cpp
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

#include "Level1.h"

// Systems
#include <Engine.h>
#include "Space.h"
#include "MeshHelper.h"
#include <Mesh.h>
#include "SpriteSource.h"
#include <Texture.h>
#include <Input.h>
#include "Archetypes.h"
#include <glfw3.h>
#include "Tilemap.h"
#include <Graphics.h>

// Components
#include "Sprite.h"
#include "Animation.h"
#include "Transform.h"
#include "Physics.h"
#include "SpriteText.h"
#include "ChipCollectible.h"
#include "WinLevel.h"
#include "CameraFollow.h"
#include "PlayerMovement.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Creates an instance of Level 2.
	Level1::Level1() : Level("Level1"), meshMonkey(nullptr), textureMonkey(nullptr), spriteSourceMonkey(nullptr),
		columnsMonkey(3), rowsMonkey(5), 
		dataMap(nullptr), textureMap(nullptr), spriteSourceMap(nullptr), meshMap(nullptr),
		columnsMap(4), rowsMap(3)
	{
	}

	// Load the resources associated with Level 2.
	void Level1::Load()
	{
		// Create a new quad mesh for the sprite.
		meshMonkey = CreateQuadMesh(Vector2D(1.0f / columnsMonkey, 1.0f / rowsMonkey), Vector2D(0.5f, 0.5f));

		// Load the player texture.
		textureMonkey = Texture::CreateTextureFromFile("Monkey.png");

		// Setup the player sprite source.
		spriteSourceMonkey = new SpriteSource(columnsMonkey, rowsMonkey, textureMonkey);

		// create collectible archetype and add to object manager archetypes
		genericQuadMesh = CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5f, 0.5f));
		textureCollectible = Texture::CreateTextureFromFile("collectible.png");
		spriteSourceCollectible = new SpriteSource(1, 1, textureCollectible);
		GetSpace()->GetObjectManager().AddArchetype(*Archetypes::CreateCollectibleArchetype(genericQuadMesh, spriteSourceCollectible));

		// Load the tilemap.
		dataMap = Tilemap::CreateTilemapFromFile("Assets/Levels/Level1.txt");
		if (dataMap == nullptr)
		{
			std::cout << "Error loading map!" << std::endl;
		}
		else
		{
			// Create a new quad mesh for the sprite tilemap.
			meshMap = CreateQuadMesh(Vector2D(1.0f / columnsMap, 1.0f / rowsMap), Vector2D(0.5f, 0.5f));

			// Load the tilemap texture.
			textureMap = Texture::CreateTextureFromFile("Tilemap.png");

			// Setup the tilemap sprite source.
			spriteSourceMap = new SpriteSource(columnsMap, rowsMap, textureMap);
		}

		// Set the background color to black.
		Graphics::GetInstance().SetBackgroundColor(Colors::Black);
	}

	// Initialize the memory associated with Level 2.
	void Level1::Initialize()
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		
		// Add archetypes to the object manager.
		objectManager.AddArchetype(*Archetypes::CreatePlayer(meshMonkey, spriteSourceMonkey));
		objectManager.AddArchetype(*Archetypes::CreateText());
		objectManager.AddArchetype(*Archetypes::CreateGameController());

		// Create collectible(s)
		GameObject* collectible = new GameObject(*objectManager.GetArchetypeByName("Collectible"));
		collectible->AddComponent(new Behaviors::ChipCollectible());
		static_cast<Transform*>(collectible->GetComponent("Transform"))->SetTranslation(Vector2D(100.0f, 0.0f));
		objectManager.AddObject(*collectible);

		// Create the player and add it to the object manager.
		GameObject* player = new GameObject(*objectManager.GetArchetypeByName("Player"));
		objectManager.AddObject(*player);

		GameObject* player2 = new GameObject(*objectManager.GetArchetypeByName("Player"));
		static_cast<Behaviors::PlayerMovement*>(player2->GetComponent("PlayerMovement"))->SetKeybinds('W', 'A', 'D');
		objectManager.AddObject(*player2);

		// Create test text and add it to the object manager.
		GameObject* text = new GameObject(*objectManager.GetArchetypeByName("Text"));
		static_cast<SpriteText*>(text->GetComponent("SpriteText"))->SetText("sicko mode");
		static_cast<Transform*>(text->GetComponent("Transform"))->SetTranslation(Vector2D(50.0f, 50.0f));
		objectManager.AddObject(*text);

		// Play the player's animation.
		static_cast<Animation*>(player->GetComponent("Animation"))->Play(0, 8, 0.2f, true);

		if (dataMap != nullptr)
		{
			// Create the tilemap and add it to the object manager.
			GameObject* tilemap = Archetypes::CreateTilemapObject(meshMap, spriteSourceMap, dataMap);
			objectManager.AddObject(*tilemap);
		}

		GameObject* gameController = new GameObject(*objectManager.GetArchetypeByName("GameController"));
		static_cast<Behaviors::CameraFollow*>(gameController->GetComponent("CameraFollow"))->AddPlayer(player);
		static_cast<Behaviors::CameraFollow*>(gameController->GetComponent("CameraFollow"))->AddPlayer(player2);
		objectManager.AddObject(*gameController);
	}

	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Level1::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		Input& input = Input::GetInstance();

		// Restart level with 'R'
		if (input.CheckTriggered('R'))
		{
			GetSpace()->RestartLevel();
		}

		// Win game if the goal-collectibles have been collected by slimes
		if (GetSpace()->GetObjectManager().GetObjectCount("Collectible") == 0)
			GetSpace()->SetLevel(new WinLevel());
	}

	// Unload the resources associated with Level 2.
	void Level1::Unload()
	{
		// Free all allocated memory.
		delete spriteSourceMap;
		delete textureMap;
		delete meshMap;
		delete dataMap;
		delete spriteSourceCollectible;
		delete textureCollectible;
		delete spriteSourceMonkey;
		delete textureMonkey;
		delete meshMonkey;
		delete genericQuadMesh;
	}
}

//----------------------------------------------------------------------------
