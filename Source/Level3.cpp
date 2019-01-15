//------------------------------------------------------------------------------
//
// File Name:	Level3.cpp
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

#include "Level3.h"

// Systems
#include <Texture.h>
#include "SpriteSource.h"
#include "GameObject.h"
#include "MeshHelper.h"
#include "Archetypes.h"
#include "Space.h"
#include "GameObjectManager.h"
#include <Input.h>
#include <Mesh.h>
#include <glfw3.h>

// Components
#include "MeshHelper.h"
#include "Transform.h"
#include "Physics.h"

// Levels
#include "Level1.h"
#include "Level2.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Creates an instance of Level 3.
	Level3::Level3() : Level("Level3"), meshQuad(nullptr), spriteSourceCircle(nullptr),
		textureCircle(nullptr), circleSpeed(0.0f), pointSpeed(0.0f)
	{
	}

	// Load the resources associated with Level 3.
	void Level3::Load()
	{
		// Create a new quad mesh for the sprite.
		meshQuad = CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5f, 0.5f));

		// Load the circle texture.
		textureCircle = Texture::CreateTextureFromFile("Circle.png");

		// Setup the circle sprite source.
		spriteSourceCircle = new SpriteSource(1, 1, textureCircle);
	}

	// Initialize the memory associated with Level 3.
	void Level3::Initialize()
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		// Add various physics objects to the scene.

		// Rectangles.
		GameObject* rectangle = Archetypes::CreateRectangle(meshQuad);
		static_cast<Transform*>(rectangle->GetComponent("Transform"))->SetTranslation(Vector2D(-200.0f, 200.0f));
		static_cast<Physics*>(rectangle->GetComponent("Physics"))->SetVelocity(Vector2D(50.0f, -75.0f));
		objectManager.AddObject(*rectangle);

		rectangle = Archetypes::CreateRectangle(meshQuad);
		static_cast<Transform*>(rectangle->GetComponent("Transform"))->SetTranslation(Vector2D(50.0f, -150.0f));
		static_cast<Physics*>(rectangle->GetComponent("Physics"))->SetVelocity(Vector2D(0.0f, 0.0f));
		objectManager.AddObject(*rectangle);

		// Circles.
		GameObject* circle = Archetypes::CreateCircle(meshQuad, spriteSourceCircle);
		objectManager.AddObject(*circle);

		circle = Archetypes::CreateCircle(meshQuad, spriteSourceCircle);
		static_cast<Transform*>(circle->GetComponent("Transform"))->SetTranslation(Vector2D(125.0f, 375.0f));
		static_cast<Physics*>(circle->GetComponent("Physics"))->SetVelocity(Vector2D(-25.0f, -75.0f));
		objectManager.AddObject(*circle);

		// Points.
		GameObject* point = Archetypes::CreatePoint(meshQuad, spriteSourceCircle);
		static_cast<Transform*>(point->GetComponent("Transform"))->SetTranslation(Vector2D(0.0f, 100.0f));
		static_cast<Physics*>(point->GetComponent("Physics"))->SetVelocity(Vector2D(0.0f, -50.0f));
		objectManager.AddObject(*point);

		// Disale fancy OpenGL smoothing/antialiasing for this level, since it causes a line on quad meshes.
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
	}

	// Update Level 3.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Level3::Update(float dt)
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
			GetSpace()->SetLevel(new Levels::Level2());
		}
		else if (input.CheckTriggered('3'))
		{
			GetSpace()->RestartLevel();
		}
	}

	// Unload the resources associated with Level 3.
	void Level3::Unload()
	{
		// Free all allocated memory.
		delete spriteSourceCircle;
		delete textureCircle;
		delete meshQuad;
	}
}

//----------------------------------------------------------------------------
