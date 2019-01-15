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

#include <iostream>

// Systems
#include "Level.h"
#include <Engine.h>
#include "Space.h"
#include "MeshHelper.h"
#include <Color.h>
#include <Input.h>
#include "Archetypes.h"
#include <System.h>
#include <glfw3.h>
#include "SoundManager.h"

// Components
#include <Mesh.h>
#include "Sprite.h"
#include "Transform.h"
#include "Physics.h"

// Levels
#include "Level2.h"
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

	// Creates an instance of Level 1.
	Level1::Level1() : Level("Level1"), meshShip(nullptr), meshBullet(nullptr)
	{
	}

	// Load the resources associated with Level 1.
	void Level1::Load()
	{
		std::cout << "Level1::Load" << std::endl;

		// Load sound effects.
		soundManager = Engine::GetInstance().GetModule<SoundManager>();
		soundManager->AddMusic("Asteroid_Field.mp3");
		soundManager->AddEffect("teleport.wav");
		soundManager->AddBank("Master Bank.strings.bank");
		soundManager->AddBank("Master Bank.bank");

		// Create a new triangle mesh for the ship.
		meshShip = CreateTriangleMesh(Color(1.0f, 0.0f, 0.0f, 1.0f), Color(0.0f, 1.0f, 0.0f, 1.0f), Color(0.0f, 0.0f, 1.0f, 1.0f));
		
		// Create a new quad mesh for the bullet.
		meshBullet = CreateQuadMesh(Vector2D(), Vector2D(0.5f, 0.5f));
		
		// Create the bullet archetype and add it to the object manager.
		GameObject* bullet = Archetypes::CreateBulletArchetype(meshBullet);
		GetSpace()->GetObjectManager().AddArchetype(*bullet);
	}

	// Initialize the memory associated with Level 1.
	void Level1::Initialize()
	{
		std::cout << "Level1::Initialize" << std::endl;

		// Create the player ship and add it to the object manager.
		GameObject* ship = Archetypes::CreateShip(meshShip);
		GetSpace()->GetObjectManager().AddObject(*ship);

		// Enable fancy OpenGL smoothing/antialiasing for this level.
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		// Play background music.
		musicChannel = soundManager->PlaySound("Asteroid Field");
	}

	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Level1::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		
		// Count the number of bullets.
		int numBullets = GetSpace()->GetObjectManager().GetObjectCount("Bullet");

		// Print the number of bullets to a string and set it as the window title.
		sprintf_s(windowTitle, titleStringLength, "Bullets: %i", numBullets);
		System::GetInstance().SetWindowTitle(windowTitle);

		Input& input = Input::GetInstance();

		// Play the teleport sound when the T key is pressed.
		if (input.CheckTriggered('T'))
		{
			soundManager->PlaySound("teleport.wav");
		}

		// Handle level switching.
		if (input.CheckTriggered('1'))
		{
			GetSpace()->RestartLevel();
		}
		else if (input.CheckTriggered('2'))
		{
			GetSpace()->SetLevel(new Levels::Level2());
		}
		else if (input.CheckTriggered('3'))
		{
			GetSpace()->SetLevel(new Levels::Level3());
		}
	}

	// Destroy objects associated with level 1.
	void Level1::Shutdown()
	{
		// Stop background music.
		musicChannel->stop();
	}

	// Unload the resources associated with Level 1.
	void Level1::Unload()
	{
		std::cout << "Level1::Unload" << std::endl;

		// Free all allocated memory.
		delete meshShip;
		delete meshBullet;

		// Unload all sounds.
		soundManager->Shutdown();
	}
}

//----------------------------------------------------------------------------
