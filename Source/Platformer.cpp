//------------------------------------------------------------------------------
//
// File Name:	Platformer.cpp
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

#include "Platformer.h"

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
#include "Enemy.h"
#include "SpriteTilemap.h"

// Levels
#include "Level1.h"
#include "Level3.h"

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
	Platformer::Platformer() : Level("Platformer"), meshGeneric(nullptr),
		meshMonkey(nullptr), textureMonkey(nullptr), spriteSourceMonkey(nullptr), columnsMonkey(3), rowsMonkey(5),
		dataMap(nullptr), textureMap(nullptr), spriteSourceMap(nullptr), meshMap(nullptr), columnsMap(4), rowsMap(3),
		textureCollectible(nullptr), spriteSourceCollectible(nullptr),
		textureHazard(nullptr), spriteSourceHazard(nullptr)
	{
	}

	// Load the resources associated with Level 2.
	void Platformer::Load()
	{
		// Create a new generic quad mesh.
		meshGeneric = CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5f, 0.5f));

		// Create a new quad mesh for the sprite.
		meshMonkey = CreateQuadMesh(Vector2D(1.0f / columnsMonkey, 1.0f / rowsMonkey), Vector2D(0.5f, 0.5f));

		// Load the monkey texture.
		textureMonkey = Texture::CreateTextureFromFile("Monkey.png");

		// Setup the monkey sprite source.
		spriteSourceMonkey = new SpriteSource(columnsMonkey, rowsMonkey, textureMonkey);
		
		// Load the items commonly used in the game.
		LoadItem("Collectible.png", textureCollectible, spriteSourceCollectible, Archetypes::CreateCollectible);
		LoadItem("Hazard.png", textureHazard, spriteSourceHazard, Archetypes::CreateHazard);
		LoadItem("Enemy.png", textureEnemy, spriteSourceEnemy, Archetypes::CreateEnemy);

		// Load the tilemap.
		dataMap = Tilemap::CreateTilemapFromFile("Assets/Levels/Platformer.txt");
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
	void Platformer::Initialize()
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		// Create the monkey and add it to the object manager.
		GameObject* monkey = Archetypes::CreateMonkey(meshMonkey, spriteSourceMonkey);
		static_cast<Transform*>(monkey->GetComponent("Transform"))->SetTranslation(Vector2D(-250.0f, 50.0f));
		objectManager.AddObject(*monkey);

		// Play the monkey's animation.
		static_cast<Animation*>(monkey->GetComponent("Animation"))->Play(0, 8, 0.2f, true);

		GameObject* tilemap = nullptr;

		if (dataMap != nullptr)
		{
			// Create the tilemap and add it to the object manager.
			tilemap = Archetypes::CreateTilemapObject(meshMap, spriteSourceMap, dataMap);
			objectManager.AddObject(*tilemap);
		}

		// Helper lambda to set the translation of a game object to a coordinate in tile space.
		auto SetTranslation = [tilemap](GameObject* object, Vector2D tile)
		{
			if (tilemap == nullptr)
				return;

			static_cast<Transform*>(object->GetComponent("Transform"))->SetTranslation(static_cast<SpriteTilemap*>(tilemap->GetComponent("Sprite"))->TileToWorld(tile));
		};

		// Add collectibles.
		GameObject* collectible = new GameObject(*objectManager.GetArchetypeByName("Collectible"));
		SetTranslation(collectible, Vector2D(1.0f, 2.0f));
		objectManager.AddObject(*collectible);

		collectible = new GameObject(*objectManager.GetArchetypeByName("Collectible"));
		SetTranslation(collectible, Vector2D(4.0f, 1.0f));
		objectManager.AddObject(*collectible);

		collectible = new GameObject(*objectManager.GetArchetypeByName("Collectible"));
		SetTranslation(collectible, Vector2D(14.0f, 1.0f));
		objectManager.AddObject(*collectible);

		collectible = new GameObject(*objectManager.GetArchetypeByName("Collectible"));
		SetTranslation(collectible, Vector2D(9.0f, 6.0f));
		objectManager.AddObject(*collectible);

		collectible = new GameObject(*objectManager.GetArchetypeByName("Collectible"));
		SetTranslation(collectible, Vector2D(14.0f, 7.0f));
		objectManager.AddObject(*collectible);

		collectible = new GameObject(*objectManager.GetArchetypeByName("Collectible"));
		SetTranslation(collectible, Vector2D(1.0f, 10.0f));
		objectManager.AddObject(*collectible);

		collectible = new GameObject(*objectManager.GetArchetypeByName("Collectible"));
		SetTranslation(collectible, Vector2D(43.0f, 4.0f));
		objectManager.AddObject(*collectible);

		collectible = new GameObject(*objectManager.GetArchetypeByName("Collectible"));
		SetTranslation(collectible, Vector2D(48.0f, 5.0f));
		objectManager.AddObject(*collectible);

		collectible = new GameObject(*objectManager.GetArchetypeByName("Collectible"));
		SetTranslation(collectible, Vector2D(1.0f, 14.0f));
		objectManager.AddObject(*collectible);

		collectible = new GameObject(*objectManager.GetArchetypeByName("Collectible"));
		SetTranslation(collectible, Vector2D(17.0f, 14.0f));
		objectManager.AddObject(*collectible);

		// Add hazards.
		GameObject* hazard = new GameObject(*objectManager.GetArchetypeByName("Hazard"));
		SetTranslation(hazard, Vector2D(8.0f, 4.0f));
		objectManager.AddObject(*hazard);

		hazard = new GameObject(*objectManager.GetArchetypeByName("Hazard"));
		SetTranslation(hazard, Vector2D(1.0f, 8.0f));
		objectManager.AddObject(*hazard);

		hazard = new GameObject(*objectManager.GetArchetypeByName("Hazard"));
		SetTranslation(hazard, Vector2D(4.0f, 10.0f));
		objectManager.AddObject(*hazard);

		hazard = new GameObject(*objectManager.GetArchetypeByName("Hazard"));
		SetTranslation(hazard, Vector2D(24.0f, 14.0f));
		objectManager.AddObject(*hazard);

		hazard = new GameObject(*objectManager.GetArchetypeByName("Hazard"));
		SetTranslation(hazard, Vector2D(32.0f, 6.0f));
		objectManager.AddObject(*hazard);

		hazard = new GameObject(*objectManager.GetArchetypeByName("Hazard"));
		SetTranslation(hazard, Vector2D(43.0f, 5.0f));
		objectManager.AddObject(*hazard);

		// Add enemies.
		GameObject* enemy = new GameObject(*objectManager.GetArchetypeByName("Enemy"));
		SetTranslation(enemy, Vector2D(7.5f, 10.0f));
		static_cast<Behaviors::Enemy*>(enemy->GetComponent("Enemy"))->SetPlayer(monkey);
		objectManager.AddObject(*enemy);

		enemy = new GameObject(*objectManager.GetArchetypeByName("Enemy"));
		SetTranslation(enemy, Vector2D(62.0f, 14.0f));
		static_cast<Behaviors::Enemy*>(enemy->GetComponent("Enemy"))->SetPlayer(monkey);
		objectManager.AddObject(*enemy);

		// Disale fancy OpenGL smoothing/antialiasing for this level, since it causes a line on quad meshes.
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
	}

	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Platformer::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		if (GetSpace()->GetObjectManager().GetObjectCount("Collectible") == 0)
		{
			Engine::GetInstance().Stop();
		}

		Input& input = Input::GetInstance();

		// Handle level switching.
		if (input.CheckTriggered('1'))
		{
			GetSpace()->SetLevel(new Levels::Level1());
		}
		else if (input.CheckTriggered('2'))
		{
			GetSpace()->RestartLevel();
		}
		else if (input.CheckTriggered('3'))
		{
			GetSpace()->SetLevel(new Levels::Level3());
		}
		else if (input.CheckTriggered('4'))
		{
			GetSpace()->RestartLevel();
		}
	}

	// Unload the resources associated with Level 2.
	void Platformer::Unload()
	{
		// Free all allocated memory.
		delete spriteSourceMap;
		delete textureMap;
		delete meshMap;
		delete dataMap;
		delete spriteSourceEnemy;
		delete textureEnemy;
		delete spriteSourceHazard;
		delete textureHazard;
		delete spriteSourceCollectible;
		delete textureCollectible;
		delete spriteSourceMonkey;
		delete textureMonkey;
		delete meshMonkey;
		delete meshGeneric;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Loads an item's texture & adds its archetype.
	// Params:
	//   textureName = The name of the texture to load.
	//   texture = A reference to the texture pointer to save.
	//   spriteSource = A reference to the sprite source pointer to save.
	//   archetypeFunc = The archetype function to use when creating the object.
	void Platformer::LoadItem(const std::string& textureName, Texture*& texture, SpriteSource*& spriteSource, Archetypes::ItemFunc itemFunc)
	{
		// Load the texture.
		texture = Texture::CreateTextureFromFile(textureName);

		// Setup the hazard sprite source.
		spriteSource = new SpriteSource(1, 1, texture);

		// Create the object and add it as an archetype.
		GetSpace()->GetObjectManager().AddArchetype(*itemFunc(meshGeneric, spriteSource));
	}
}

//----------------------------------------------------------------------------
