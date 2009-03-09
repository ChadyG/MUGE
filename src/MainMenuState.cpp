/*
 *  MainMenuState.cpp
 *  Destructible
 *
 *  Created by Chad on 2/14/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
 
#include "MainMenuState.h"
#include "Engine/MUGE.h"

/**
*
*/

MainMenuState MainMenuState::m_StateInstance;

void MainMenuState::init(Gosu::Graphics &graphics)
{	
	std::wstring filename = Gosu::resourcePrefix() + L"Images/Menu/Menu_screen.png";
	m_MenuScreen.reset(new Gosu::Image(graphics, filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/arrow.png";
	m_Cursor.reset(new Gosu::Image(graphics, filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/cursor.png";
	m_MouseCursor.reset(new Gosu::Image(graphics, filename, false));
	
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_D.png";
	m_Letter_D.reset(new Gosu::Image(graphics, filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_E.png";
	m_Letter_E.reset(new Gosu::Image(graphics, filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_S.png";
	m_Letter_S.reset(new Gosu::Image(graphics, filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_T.png";
	m_Letter_T.reset(new Gosu::Image(graphics, filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_R.png";
	m_Letter_R.reset(new Gosu::Image(graphics, filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_U.png";
	m_Letter_U.reset(new Gosu::Image(graphics, filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_C.png";
	m_Letter_C.reset(new Gosu::Image(graphics, filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_I.png";
	m_Letter_I.reset(new Gosu::Image(graphics, filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_B.png";
	m_Letter_B.reset(new Gosu::Image(graphics, filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_L.png";
	m_Letter_L.reset(new Gosu::Image(graphics, filename, false));
	
	m_Held = false;
	m_msLeftHeld = false;
	m_CursorPos = 0;
	m_units = 10.0;
	m_width = graphics.width();
	m_height = graphics.height();
	
	// Create a world that is the size of our screen/10 with borders of 2 units (20 pixels at 10 pixels per unit)
	b2AABB worldAABB;
	worldAABB.lowerBound.Set( -2.0f, -2.0f );
	worldAABB.upperBound.Set( 2.0f + m_width / m_units, 2.0f + m_height / m_units);
	b2Vec2 gravity( 0.0f, 10.0f);
	bool do_sleep = true;
	m_Worldp.reset( new b2World( worldAABB, gravity, do_sleep) );
	
	// Physics calculation values
	m_Iterations = 10;
	m_TimeStep = 1.0f / 60.0f;
	
	// Create shapes for borders, they are static and have a density of 0
	b2PolygonDef shape;
	shape.SetAsBox( 2.0f + m_width / m_units, 1.0f);
	shape.density = 0.0f;
	shape.friction = 1.3f;
	shape.restitution = 0.25f;
	
	// Top Bumper
	b2BodyDef bodyDef;
	bodyDef.position.Set( m_width / (2.0f*m_units), -1.0f );
	b2Body* body = m_Worldp->CreateBody( &bodyDef );
	body->CreateShape( &shape );
	body->SetMassFromShapes();
	
	// Bottom Bumper
	bodyDef.position.Set( m_width / (2.0f*m_units), 1.0f + m_height / m_units );
	body = m_Worldp->CreateBody( &bodyDef );
	body->CreateShape( &shape );
	body->SetMassFromShapes();
	
	// Left Bumper
	shape.SetAsBox( 1.0f, m_height / m_units);
	bodyDef.position.Set( -1.0f, m_height / (2.0f*m_units) );
	body = m_Worldp->CreateBody( &bodyDef );
	body->CreateShape( &shape );
	body->SetMassFromShapes();
	
	// Right Bumper
	bodyDef.position.Set( 1.0f + m_width / m_units, m_height / (2.0f*m_units) );
	body = m_Worldp->CreateBody( &bodyDef );
	body->CreateShape( &shape );
	body->SetMassFromShapes();
	
	// Create letterbox objects, specify half-widths for shape size
	// This code brought to you by the Letter D
	shape.SetAsBox( 1.5f, 1.6f);
	shape.density = 1.0f;
	bodyDef.position.Set( m_width / (2.0f*m_units) - 6.0f, m_height / (4.0f*m_units) );
	body = m_Worldp->CreateBody( &bodyDef );
	body->CreateShape( &shape );
	body->SetMassFromShapes();	
	m_Bodies.push_back( body );
	
	// This code brought to you by the Letter E
	shape.SetAsBox( 1.2f, 1.6f);
	bodyDef.position.Set( m_width / (2.0f*m_units) - 6.0f + 2.0f, m_height / (4.0f*m_units) );
	body = m_Worldp->CreateBody( &bodyDef );
	body->CreateShape( &shape );
	body->SetMassFromShapes();
	m_Bodies.push_back( body );
	
	// This code brought to you by the Letter S
	shape.SetAsBox( 1.25f, 1.6f);
	bodyDef.position.Set( m_width / (2.0f*m_units) - 6.0f + 4.0f, m_height / (4.0f*m_units) );
	body = m_Worldp->CreateBody( &bodyDef );
	body->CreateShape( &shape );
	body->SetMassFromShapes();
	m_Bodies.push_back( body );
	
	// This code brought to you by the Letter T
	shape.SetAsBox( 1.5f, 1.6f);
	bodyDef.position.Set( m_width / (2.0f*m_units) - 6.0f + 6.0f, m_height / (4.0f*m_units) );
	body = m_Worldp->CreateBody( &bodyDef );
	body->CreateShape( &shape );
	body->SetMassFromShapes();
	m_Bodies.push_back( body );
	
	// This code brought to you by the Letter R
	shape.SetAsBox( 1.45f, 1.6f);
	bodyDef.position.Set( m_width / (2.0f*m_units) - 6.0f + 8.0f, m_height / (4.0f*m_units) );
	body = m_Worldp->CreateBody( &bodyDef );
	body->CreateShape( &shape );
	body->SetMassFromShapes();
	m_Bodies.push_back( body );
	
	// This code brought to you by the Letter U
	shape.SetAsBox( 1.4f, 1.6f);
	bodyDef.position.Set( m_width / (2.0f*m_units) - 6.0f + 10.0f, m_height / (4.0f*m_units) );
	body = m_Worldp->CreateBody( &bodyDef );
	body->CreateShape( &shape );
	body->SetMassFromShapes();
	m_Bodies.push_back( body );
	
	// This code brought to you by the Letter C
	shape.SetAsBox( 1.35f, 1.6f);
	bodyDef.position.Set( m_width / (2.0f*m_units) - 6.0f + 12.0f, m_height / (4.0f*m_units) );
	body = m_Worldp->CreateBody( &bodyDef );
	body->CreateShape( &shape );
	body->SetMassFromShapes();
	m_Bodies.push_back( body );
	
	// This code brought to you by the Letter T
	shape.SetAsBox( 1.5f, 1.6f);
	bodyDef.position.Set( m_width / (2.0f*m_units) - 6.0f + 14.0f, m_height / (4.0f*m_units) );
	body = m_Worldp->CreateBody( &bodyDef );
	body->CreateShape( &shape );
	body->SetMassFromShapes();
	m_Bodies.push_back( body );
	
	// This code brought to you by the Letter I
	shape.SetAsBox( 0.55f, 1.6f);
	bodyDef.position.Set( m_width / (2.0f*m_units) - 6.0f + 16.0f, m_height / (4.0f*m_units) );
	body = m_Worldp->CreateBody( &bodyDef );
	body->CreateShape( &shape );
	body->SetMassFromShapes();
	m_Bodies.push_back( body );
	
	// This code brought to you by the Letter B
	shape.SetAsBox( 1.35f, 1.6f);
	bodyDef.position.Set( m_width / (2.0f*m_units) - 6.0f + 18.0f, m_height / (4.0f*m_units) );
	body = m_Worldp->CreateBody( &bodyDef );
	body->CreateShape( &shape );
	body->SetMassFromShapes();
	m_Bodies.push_back( body );
	
	// This code brought to you by the Letter L
	shape.SetAsBox( 1.2f, 1.6f);
	bodyDef.position.Set( m_width / (2.0f*m_units) - 6.0f + 20.0f, m_height / (4.0f*m_units) );
	body = m_Worldp->CreateBody( &bodyDef );
	body->CreateShape( &shape );
	body->SetMassFromShapes();
	m_Bodies.push_back( body );
	
	// This code brought to you by the Letter E
	shape.SetAsBox( 1.2f, 1.6f);
	bodyDef.position.Set( m_width / (2.0f*m_units) - 6.0f + 22.0f, m_height / (4.0f*m_units) );
	body = m_Worldp->CreateBody( &bodyDef );
	body->CreateShape( &shape );
	body->SetMassFromShapes();
	m_Bodies.push_back( body );
	
}

void MainMenuState::cleanup()
{
	m_MenuScreen.reset(0);
	m_Cursor.reset(0);
	m_MouseCursor.reset(0);
	m_Letter_D.reset(0);
	m_Letter_E.reset(0);
	m_Letter_S.reset(0);
	m_Letter_T.reset(0);
	m_Letter_R.reset(0);
	m_Letter_U.reset(0);
	m_Letter_C.reset(0);
	m_Letter_I.reset(0);
	m_Letter_B.reset(0);
	m_Letter_L.reset(0);
	while (!m_Bodies.empty()) {
		m_Bodies.pop_front();
	}
	m_Worldp.reset(0);
}

void MainMenuState::pause()
{

}

void MainMenuState::resume()
{

}

void MainMenuState::update(const Gosu::Input &input, MUGE* engine)
{
	m_Worldp->Step( m_TimeStep, m_Iterations );
	
	
	if (input.down(Gosu::kbDown)) {
		if (!m_Held) {
			++m_CursorPos;
			if (m_CursorPos >= 4)
				m_CursorPos = 0;
		
			m_Held = true;
		}
	}else
	if (input.down(Gosu::kbUp)) {
		if (!m_Held) {
			--m_CursorPos;
			if (m_CursorPos < 0)
				m_CursorPos = 3;
		
			m_Held = true;
		}
	}else
		m_Held = false;
	

	// Mouse stuff
	bool lClick = false;
	m_mousePos.x = input.mouseX();
	m_mousePos.y = input.mouseY();
	b2Vec2 mouse2d = m_mousePos;
	mouse2d *= (1.0f/m_units);
	if (m_mousePos.x > 200 && m_mousePos.x < 310 && m_mousePos.y > 200 && m_mousePos.y < 360) {
		m_CursorPos = (m_mousePos.y - 200)/45;
	}
	if (input.down(Gosu::msLeft)) {
		if (!m_msLeftHeld) {
			if (m_mousePos.x > 200 && m_mousePos.x < 310 && m_mousePos.y > 200 && m_mousePos.y < 360) {
				lClick = true;
			}else
			if (m_mouseJoint == NULL) {

				// Make a small box.
				b2AABB aabb;
				b2Vec2 d;
				d.Set(0.001f, 0.001f);
				aabb.lowerBound = mouse2d - d;
				aabb.upperBound = mouse2d + d;

				// Query the world for overlapping shapes.
				const int32 k_maxCount = 10;
				b2Shape* shapes[k_maxCount];
				int32 count = m_Worldp->Query(aabb, shapes, k_maxCount);
				b2Body* body = NULL;
				for (int32 i = 0; i < count; ++i)
				{
					b2Body* shapeBody = shapes[i]->GetBody();
					if (shapeBody->IsStatic() == false)
					{
						bool inside = shapes[i]->TestPoint(shapeBody->GetXForm(), mouse2d);
						if (inside)
						{
							body = shapes[i]->GetBody();
							break;
						}
					}
				}

				if (body)
				{
					b2MouseJointDef md;
					md.body1 = m_Worldp->GetGroundBody();
					md.body2 = body;
					md.target = mouse2d;
					md.maxForce = 1000.0f * body->GetMass();
					m_mouseJoint = (b2MouseJoint*)m_Worldp->CreateJoint(&md);
					body->WakeUp();
				}
			}
		}
		m_msLeftHeld = true;																																							
	}else
		m_msLeftHeld = false;
	
	if (m_mouseJoint) {
		m_mouseJoint->SetTarget(mouse2d);
	}
	
	if (!input.down(Gosu::msLeft) && m_mouseJoint) {
		m_Worldp->DestroyJoint(m_mouseJoint);
		m_mouseJoint = NULL;
	}
	
	// Selection
	if (input.down(Gosu::kbReturn) || lClick) {
		switch (m_CursorPos) {
			case 0:// New
				break;
			case 1:// Load
				break;
			case 2:// About
				break;
			case 3:// Exit
				engine->popState();
				break;
		}
	}
	
	// Exit on escape
	if (input.down(Gosu::kbEscape)) {
		engine->popState();
	}
}

void MainMenuState::draw() const
{
	m_MenuScreen->draw(0,0,0);
	m_Cursor->draw(168, 200 + 45*m_CursorPos, 2);
	m_MouseCursor->draw(m_mousePos.x - 7, m_mousePos.y, 2);
	
	b2Vec2 pos;
	float angle;
	std::list< b2Body* >::const_iterator BIter;
	int i = 0;
	BIter = m_Bodies.begin();
	pos = (*BIter)->GetPosition();
	angle = (*BIter)->GetAngle() * (180.0f/(float)Gosu::pi);
	m_Letter_D->drawRot( pos.x * m_units, pos.y * m_units, 1, angle);
	
	pos = (*++BIter)->GetPosition();
	angle = (*BIter)->GetAngle() * (180.0f/(float)Gosu::pi);
	m_Letter_E->drawRot( pos.x * m_units, pos.y * m_units, 1, angle);
	
	pos = (*++BIter)->GetPosition();
	angle = (*BIter)->GetAngle() * (180.0f/(float)Gosu::pi);
	m_Letter_S->drawRot( pos.x * m_units, pos.y * m_units, 1, angle);
	
	pos = (*++BIter)->GetPosition();
	angle = (*BIter)->GetAngle() * (180.0f/(float)Gosu::pi);
	m_Letter_T->drawRot( pos.x * m_units, pos.y * m_units, 1, angle);
	
	pos = (*++BIter)->GetPosition();
	angle = (*BIter)->GetAngle() * (180.0f/(float)Gosu::pi);
	m_Letter_R->drawRot( pos.x * m_units, pos.y * m_units, 1, angle);
	
	pos = (*++BIter)->GetPosition();
	angle = (*BIter)->GetAngle() * (180.0f/(float)Gosu::pi);
	m_Letter_U->drawRot( pos.x * m_units, pos.y * m_units, 1, angle);
	
	pos = (*++BIter)->GetPosition();
	angle = (*BIter)->GetAngle() * (180.0f/(float)Gosu::pi);
	m_Letter_C->drawRot( pos.x * m_units, pos.y * m_units, 1, angle);
	
	pos = (*++BIter)->GetPosition();
	angle = (*BIter)->GetAngle() * (180.0f/(float)Gosu::pi);
	m_Letter_T->drawRot( pos.x * m_units, pos.y * m_units, 1, angle);
	
	pos = (*++BIter)->GetPosition();
	angle = (*BIter)->GetAngle() * (180.0f/(float)Gosu::pi);
	m_Letter_I->drawRot( pos.x * m_units, pos.y * m_units, 1, angle);
	
	pos = (*++BIter)->GetPosition();
	angle = (*BIter)->GetAngle() * (180.0f/(float)Gosu::pi);
	m_Letter_B->drawRot( pos.x * m_units, pos.y * m_units, 1, angle);
	
	pos = (*++BIter)->GetPosition();
	angle = (*BIter)->GetAngle() * (180.0f/(float)Gosu::pi);
	m_Letter_L->drawRot( pos.x * m_units, pos.y * m_units, 1, angle);
	
	pos = (*++BIter)->GetPosition();
	angle = (*BIter)->GetAngle() * (180.0f/(float)Gosu::pi);
	m_Letter_E->drawRot( pos.x * m_units, pos.y * m_units, 1, angle);
	
}
