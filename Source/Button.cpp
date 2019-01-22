//------------------------------------------------------------------------------
//
// File Name:	Button.h
// Author(s):	A.J. Bussman
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
#include "Button.h"

#include "Sprite.h"
#include "Transform.h"
#include "ColliderRectangle.h"
#include <GameObject.h>
#include <Intersection2D.h>

#include <Input.h>
#include <Vector2D.h>
#include <Graphics.h>
#include <Space.h>
#include "Level1.h"

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Default constructor
	Button::Button() : Component("Button"), level(0)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* Button::Clone() const
	{
		return new Button(*this);
	}

	// Initialize this component (happens at object creation).
	void Button::Initialize()
	{
		sprite = static_cast<Sprite*>(GetOwner()->GetComponent("Sprite"));
		boundingRact = BoundingRectangle(static_cast<Transform*>(GetOwner()->GetComponent("Transform"))->GetTranslation(), 
			static_cast<ColliderRectangle*>(GetOwner()->GetComponent("Collider"))->GetExtents());
	}

	// Update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void Button::Update(float dt)
	{
		Input& input = Input::GetInstance();
		Vector2D mousePos =	Graphics::GetInstance().ScreenToWorldPosition(input.GetCursorPosition());

		if (PointRectangleIntersection(mousePos, *boundingRact))
		{
			sprite->SetColor(Colors::Green);
			if (input.CheckTriggered(VK_LBUTTON))
				GetOwner()->GetSpace()->SetLevel(new Levels::Level1(level));
		}
	}

	// Returns the level the button loads
	int Button::GetLevel()
	{
		return level;
	}

	// Sets the level the button loads
	void Button::SetLevel(int level_)
	{
		level = level_;
	}
}

//------------------------------------------------------------------------------
