//------------------------------------------------------------------------------
//
// File Name:	Controls.h
// Author(s):	A.J. Bussman
// Project:		Yesterday's Mayonnaise
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Vector2D.h"
#include "Level.h"

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Texture;
class Mesh;
class SpriteSource;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	enum class Map : int;

	class Controls : public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Creates an instance of Controls.
		Controls();

		// Load the resources associated with Controls.
		void Load() override;

		// Initialize the memory associated with Controls.
		void Initialize() override;

		// Update Controls.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt) override;

		// Unload the resources associated with Controls.
		void Unload() override;

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Adds a new map button.
		// name = The name of the level.
		// position = The position of the button.
		// map = The map the button should switch to.
		void AddMapButton(const char* name, Vector2D position, Levels::Map map);

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Background
		Mesh* meshBackground;
		Texture* textureBackground;
		SpriteSource* spriteSourceBackground;

		// Button
		Mesh* meshButton;
		Texture* textureButton;
		SpriteSource* spriteSourceButton;
	};
}

//----------------------------------------------------------------------------