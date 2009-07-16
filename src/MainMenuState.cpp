/*
   MainMenuState.cpp
   Mizzou Game Engine
 
   Created by Chad Godsey on 2/14/08.
  
 Copyright 2009 Mizzou Game Design.

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
 */
 
#include "MainMenuState.h"
#include "AdventureState.h"
#include "Core/MUGE.h"
#include "UI/GUIObjects.h"
#include <boost/bind.hpp>

/**
*
*/

MainMenuState::MainMenuState( std::wstring _config )
{

}

void MainMenuState::init( MUGE* _engine)
{	
	m_Engine = _engine;

	m_Engine->hookIntoCommand("Menu.CursorDown:Down", boost::bind(&MainMenuState::CursorDown, this));
	m_Engine->hookIntoCommand("Menu.CursorUp:Down", boost::bind(&MainMenuState::CursorUp, this));
	m_Engine->setCurrentContext("Menu");


	std::wstring filename = Gosu::resourcePrefix() + L"Images/Menu/Menu_screen.png";
	m_MenuScreen.reset(new Gosu::Image(m_Engine->graphics(), filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/arrow.png";
	m_Cursor.reset(new Gosu::Image(m_Engine->graphics(), filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/cursor.png";
	m_MouseCursor.reset(new Gosu::Image(m_Engine->graphics(), filename, false));
	
	//Sounds!
	filename = Gosu::resourcePrefix() + L"Sound/cursor_move.wav";
	m_CursorMove.reset(new Gosu::Sample(m_Engine->audio(), filename));
	filename = Gosu::resourcePrefix() + L"Sound/takin_drugz.wav";
	m_CursorSelect.reset(new Gosu::Sample(m_Engine->audio(), filename));
	filename = Gosu::resourcePrefix() + L"Sound/hit.wav";
	m_PhysHit.reset(new Gosu::Sample(m_Engine->audio(), filename));
	filename = Gosu::resourcePrefix() + L"Sound/bigger_hit.wav";
	m_PhysBigHit.reset(new Gosu::Sample(m_Engine->audio(), filename));
	
	// Letter nonsense
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_D.png";
	m_LettersV.push_back(new Gosu::Image(m_Engine->graphics(), filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_E.png";
	m_LettersV.push_back(new Gosu::Image(m_Engine->graphics(), filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_S.png";
	m_LettersV.push_back(new Gosu::Image(m_Engine->graphics(), filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_T.png";
	m_LettersV.push_back(new Gosu::Image(m_Engine->graphics(), filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_R.png";
	m_LettersV.push_back(new Gosu::Image(m_Engine->graphics(), filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_U.png";
	m_LettersV.push_back(new Gosu::Image(m_Engine->graphics(), filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_C.png";
	m_LettersV.push_back(new Gosu::Image(m_Engine->graphics(), filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_I.png";
	m_LettersV.push_back(new Gosu::Image(m_Engine->graphics(), filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_B.png";
	m_LettersV.push_back(new Gosu::Image(m_Engine->graphics(), filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/Letter_L.png";
	m_LettersV.push_back(new Gosu::Image(m_Engine->graphics(), filename, false));
	
	m_Held = false;
	m_msLeftHeld = false;
	m_CursorPos = 0;
	m_lastCursorPos = 0;
	m_units = 10.0;
	m_width = m_Engine->graphics().width();
	m_height = m_Engine->graphics().height();
	m_mouseJoint = NULL;
	
	// Create a world that is the size of our screen/10 with borders of 2 units (20 pixels at 10 pixels per unit)
	b2AABB worldAABB;
	worldAABB.lowerBound.Set( -2.0f, -2.0f );
	worldAABB.upperBound.Set( 2.0f + m_width / m_units, 2.0f + m_height / m_units);
	b2Vec2 gravity( 0.0f, 10.0f);
	bool do_sleep = true;
	m_Worldp.reset( new b2World( worldAABB, gravity, do_sleep) );
	m_Worldp->SetContactListener( &m_ContactListener );
	
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
	const float32 widths[] = { 1.5f, 1.6f, 
		1.2f, 1.6f,
		1.25f, 1.6f,
		1.5f, 1.6f,
		1.45f, 1.6f,
		1.4f, 1.6f,
		1.35f, 1.6f,
		1.5f, 1.6f,
		0.55f, 1.6f,
		1.35f, 1.6f,
		1.2f, 1.6f,
		1.2f, 1.6f};
	
	shape.density = 1.0f;
	bodyDef.linearDamping = 0.2f;
	// This code brought to you by the Letter E
	for (int i=0; i< 12; ++i) {
		shape.SetAsBox( widths[i*2], widths[i*2 + 1]);//1.5f, 1.6f);
		bodyDef.position.Set( m_width / (2.0f*m_units) - 6.0f + 2.0f*i, m_height / (4.0f*m_units) );
		body = m_Worldp->CreateBody( &bodyDef );
		body->CreateShape( &shape );
		body->SetMassFromShapes();	
		m_Bodies.push_back( body );
	}
	
	// UI Stuff (finally!)
	m_UI = new UISheet(m_Engine->graphics(), m_Engine->input());
	
	buttonDef bDef;
	bDef.height = 32;
	bDef.width = 70;
	bDef.x = 400;
	bDef.y = 200;
	m_Button = m_UI->createButton(bDef);
	filename = Gosu::resourcePrefix() + L"Images/StartButton.png";
	m_Button->setImage(filename);
	
	windowDef wDef;
	wDef.width = 200;
	wDef.height = 200;
	m_UIWin = m_UI->createWindow(wDef);
	
	m_UI->giveFocus();
	m_UIWin->hide();
	
}

void MainMenuState::cleanup()
{
	m_MenuScreen.reset(0);
	m_Cursor.reset(0);
	m_MouseCursor.reset(0);
	m_LettersV.clear();
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

void MainMenuState::CursorUp()
{
	--m_CursorPos;
	if (m_CursorPos < 0)
		m_CursorPos = 3;
}

void MainMenuState::CursorDown()
{
	++m_CursorPos;
	if (m_CursorPos >= 4)
		m_CursorPos = 0;
}

void MainMenuState::update()
{
	m_Worldp->Step( m_TimeStep, m_Iterations );
	
	// Contact Listener for physics sounds
	int hits = m_ContactListener.Update();
	if (hits % 2 == 1)
		m_PhysHit->play();
	if (hits > 1)
		m_PhysBigHit->play();
	
	
	m_UI->update();
	if (m_UIWin->isClosed() && m_UIWin->isVisible()) {
		m_UIWin->takeFocus();
		m_UIWin->hide();
	}
	
	
	// Mouse stuff
	bool lClick = false;
	m_mousePos.x = m_Engine->input().mouseX();
	m_mousePos.y = m_Engine->input().mouseY();
	b2Vec2 mouse2d = m_mousePos;
	mouse2d *= (1.0f/m_units);
	if (m_mousePos.x > 200 && m_mousePos.x < 310 && m_mousePos.y > 200 && m_mousePos.y < 360) {
		m_CursorPos = (m_mousePos.y - 200)/45;
	}
	
	if (m_Engine->input().down(Gosu::msLeft)) {
		// Is this a new click or hold?
		if (!m_msLeftHeld) {
			// Are we over the menu items?
			if (m_mousePos.x > 200 && m_mousePos.x < 310 && m_mousePos.y > 200 && m_mousePos.y < 360) {
				lClick = true;
			}else// No, try and drag an object
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
	
	
	// Mouse Physics
	if (m_mouseJoint) {
		m_mouseJoint->SetTarget(mouse2d);
	}
	
	if (!m_Engine->input().down(Gosu::msLeft) && m_mouseJoint) {
		m_Worldp->DestroyJoint(m_mouseJoint);
		m_mouseJoint = NULL;
	}
	
	// Selection sounds
	if (m_lastCursorPos != m_CursorPos) {
		m_CursorMove->play();
		m_lastCursorPos = m_CursorPos;
	}
	
	AdventureState *state;
	// Selection
	if (/*m_Engine->input().down(Gosu::kbReturn) || */lClick) {
		m_CursorSelect->play();
		switch (m_CursorPos) {
			case 0:// New
				state = new AdventureState( std::wstring(L"AdventureState") );
				m_Engine->changeState( state );
				break;
			case 1:// Load
				break;
			case 2:// About
				m_UIWin->show();
				m_UIWin->giveFocus();
				break;
			case 3:// Exit
				m_Engine->popState();
				break;
		}
	}
	/*
	// Exit on escape
	if (m_Engine->input().down(Gosu::kbEscape)) {
		m_Engine->popState();
	}
	*/
}

void MainMenuState::draw() const
{
	m_MenuScreen->draw(0,0,0);
	m_Cursor->draw(168, 200 + 45*m_CursorPos, 2);
	m_MouseCursor->draw(m_mousePos.x - 7, m_mousePos.y, 3);
	
	b2Vec2 pos;
	float angle;
	std::list< b2Body* >::const_iterator BIter;
	int i = 0;
	const int letterTable[] = {0,1,2,3,4,5,6,3,7,8,9,1};
	for (BIter = m_Bodies.begin(); BIter != m_Bodies.end(); ++BIter, ++i) {
		pos = (*BIter)->GetPosition();
		angle = (*BIter)->GetAngle() * (180.0f/(float)Gosu::pi);
		m_LettersV[letterTable[i]]->drawRot( pos.x * m_units, pos.y * m_units, 1, angle);
	}
	
	m_UI->draw(2);
}
