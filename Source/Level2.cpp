//------------------------------------------------------------------------------
//
// File Name:	Level2.cpp
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

#include "Level2.h"

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
	Level2::Level2() : Level("Level2"), meshMonkey(nullptr), textureMonkey(nullptr), spriteSourceMonkey(nullptr),
		columnsMonkey(3), rowsMonkey(5), 
		dataMap(nullptr), textureMap(nullptr), spriteSourceMap(nullptr), meshMap(nullptr),
		columnsMap(4), rowsMap(3)
	{
	}

	// Load the resources associated with Level 2.
	void Level2::Load()
	{
		std::cout << "Level2::Load" << std::endl;

		// Create a new quad mesh for the sprite.
		meshMonkey = CreateQuadMesh(Vector2D(1.0f / columnsMonkey, 1.0f / rowsMonkey), Vector2D(0.5f, 0.5f));

		// Load the monkey texture.
		textureMonkey = Texture::CreateTextureFromFile("Monkey.png");

		// Setup the monkey sprite source.
		spriteSourceMonkey = new SpriteSource(columnsMonkey, rowsMonkey, textureMonkey);

		// Load the tilemap.
		dataMap = Tilemap::CreateTilemapFromFile("Assets/Levels/Level2.txt");
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
	void Level2::Initialize()
	{
		std::cout << "Level2::Initialize" << std::endl;

		// Create the monkey and add it to the object manager.
		GameObject* monkey = Archetypes::CreateMonkey(meshMonkey, spriteSourceMonkey);
		GetSpace()->GetObjectManager().AddObject(*monkey);

		// Play the monkey's animation.
		static_cast<Animation*>(monkey->GetComponent("Animation"))->Play(0, 8, 0.2f, true);

		if (dataMap != nullptr)
		{
			// Create the tilemap and add it to the object manager.
			GameObject* tilemap = Archetypes::CreateTilemapObject(meshMap, spriteSourceMap, dataMap);
			GetSpace()->GetObjectManager().AddObject(*tilemap);
		}

		// Disale fancy OpenGL smoothing/antialiasing for this level, since it causes a line on quad meshes.
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
	}

	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Level2::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

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
	}

	// Unload the resources associated with Level 2.
	void Level2::Unload()
	{
		std::cout << "Level2::Unload" << std::endl;
		
		// Free all allocated memory.
		delete spriteSourceMap;
		delete textureMap;
		delete meshMap;
		delete dataMap;
		delete spriteSourceMonkey;
		delete textureMonkey;
		delete meshMonkey;
	}
}

//----------------------------------------------------------------------------
