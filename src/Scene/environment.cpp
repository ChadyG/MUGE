/*
   Environment.cpp
   Mizzou Game Engine
 
   Created by Chad Godsey on 11/12/08.
  
  
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
 
#include "../Core/MUGE.h"
#include "Environment.h"
#include "../Input/JSONFile.hpp"
#include "../Physics/ContactListener.h"
#include "../Scene/Player.h"
#include <string>
#include <iostream>
#include <fstream>

Environment::Environment(MUGE* _engine, std::wstring _config)
{
	m_Engine = _engine;
	
	m_TimeStep = 1.0f / 50.0f;
	m_Iterations = 10.0f;
	
	m_Width = m_Engine->graphics().width();
	m_Height = m_Engine->graphics().height();
	m_Zoom = 1.0;
	
	// box2D stuff
	/*
	b2AABB worldAABB;
	worldAABB.lowerBound.Set( -500.0f, -500.0f);
	worldAABB.upperBound.Set( 500.0f, 500.0f);
	b2Vec2 gravity( 0.0f, 0.0f);
	bool do_sleep = true;

	m_World = new b2World(worldAABB, gravity, do_sleep);
	m_World->SetContactListener( &m_ContactListener );

	m_Iterations = 10;
	*/
	//
	// Read in JSON encoded file
	// TinyJSON library will perform lexing and parsing
	// pull out data using JSONFile class
	//
	
	JSONFile jFile(Gosu::narrow(Gosu::resourcePrefix() + L"Data/" + _config + L".json"));
	json::grammar<char>::array::const_iterator it, it2;
	json::grammar<char>::array arr, arr2;
	json::grammar<char>::object o;
	int i, j;
	std::string tString;
	
	
	m_PlayerPos.Set(jFile.get< double >("PlayerSpawn[0]"), jFile.get< double >("PlayerSpawn[1]"));
	
	arr = jFile.get<json::grammar<char>::array>("CanvasColor");
	m_canvasColor = Gosu::Color( 
							jFile.get< int >("CanvasColor[0]"), 
							jFile.get< int >("CanvasColor[1]"),
							jFile.get< int >("CanvasColor[2]"),
							jFile.get< int >("CanvasColor[3]"));
	
	
	m_Scale = jFile.get< int >("Scale");
	
	// Physics shape cache here
	std::map< std::string, b2ShapeDef* > tShapes;
	arr = jFile.get<json::grammar<char>::array>("PhysicsShapes");
	for (i = 0, it = arr.begin(); it != arr.end(); ++it, ++i) {
		tString = jFile.get< std::string >("Type", *it);
		if (tString == "Rectangle") {
			b2PolygonDef *pDef = new b2PolygonDef();
			// width
			// height
			pDef->density = jFile.get< double >("Density", *it);
			pDef->friction = jFile.get< double >("Friction", *it);
			pDef->restitution = jFile.get< double >("Restitution", *it);
			// LinearDamping if exists
			
			tShapes[jFile.get<std::string>("ID", *it)] = pDef;
		}
		if (tString == "Circle") {
			b2CircleDef *cDef = new b2CircleDef();
			cDef->radius = jFile.get< double >("Radius", *it);
			cDef->density = jFile.get< double >("Density", *it);
			cDef->friction = jFile.get< double >("Friction", *it);
			cDef->restitution = jFile.get< double >("Restitution", *it);
			
			tShapes[jFile.get<std::string>("ID", *it)] = cDef;
		}
		if (tString == "Polygon") {
			b2PolygonDef *pDef = new b2PolygonDef();
			// Verts
			pDef->density = jFile.get< double >("Density", *it);
			pDef->friction = jFile.get< double >("Friction", *it);
			pDef->restitution = jFile.get< double >("Restitution", *it);
			
			tShapes[jFile.get<std::string>("ID", *it)] = pDef;
		}
	}
	
	
	arr = jFile.get<json::grammar<char>::array>("Layers");
	for (i = 0, it = arr.begin(); it != arr.end(); ++it, ++i) {
		SpriteLayer tLayer;
		tLayer.scale = jFile.get< double >("Scale", *it);
		tLayer.layer = jFile.get< int >("Layer", *it);
		tLayer.ID = jFile.get<std::string>("ID", *it);
		
		arr2 = jFile.get<json::grammar<char>::array>("StaticSprites", *it);
		for (j = 0, it2 = arr2.begin(); it2 != arr2.end(); ++it2, ++j) {
			Sprite tSprite;	
			tSprite.setImage( m_Engine->graphics(), Gosu::resourcePrefix() + L"Images/" + Gosu::widen( jFile.get<std::string>("Image", *it2) ) );
			
			tSprite.setPosition( jFile.get< double >("Position[0]", *it2) * tLayer.scale, jFile.get< double >("Position[1]", *it2) * tLayer.scale, tLayer.layer);
			tSprite.setRotation( jFile.get< double >("Rotation", *it2) );
			
			tSprite.setColorMod( 
					Gosu::Color( jFile.get< int >("ColorMod[0]", *it2), 
								 jFile.get< int >("ColorMod[1]", *it2), 
								 jFile.get< int >("ColorMod[2]", *it2), 
								 jFile.get< int >("ColorMod[3]", *it2) ) );
			tSprite.setScaling( jFile.get< double >("xScale", *it2), jFile.get< double >("yScale", *it2) );
			tSprite.setWindowScale( m_Scale );
			tLayer.sprites.push_back( tSprite );
		}
		// Dynamic Sprites
		// Same as above, but then tie into some logic
		arr2 = jFile.get<json::grammar<char>::array>("DynamicSprites", *it);
		for (j = 0, it2 = arr2.begin(); it2 != arr2.end(); ++it2, ++j) {
			Sprite tSprite;	
			tSprite.setImage( m_Engine->graphics(), Gosu::resourcePrefix() + L"Images/" + Gosu::widen( jFile.get<std::string>("Image", *it2) ) );
			
			tSprite.setPosition( jFile.get< double >("Position[0]", *it2) * tLayer.scale, jFile.get< double >("Position[1]", *it2) * tLayer.scale, tLayer.layer);
			tSprite.setRotation( jFile.get< double >("Rotation", *it2) );
			
			tSprite.setColorMod( 
					Gosu::Color( jFile.get< int >("ColorMod[0]", *it2), 
								 jFile.get< int >("ColorMod[1]", *it2), 
								 jFile.get< int >("ColorMod[2]", *it2), 
								 jFile.get< int >("ColorMod[3]", *it2) ) );
			tSprite.setScaling( jFile.get< double >("xScale", *it2), jFile.get< double >("yScale", *it2) );
			tSprite.setWindowScale( m_Scale );
			tLayer.sprites.push_back( tSprite );
			
			// Physics!
		}
		m_Layers[jFile.get<std::string>("ID", *it)] = tLayer;
	}
	
	
	/*
	arr = jFile.get<json::grammar<char>::array>("GameObjects");
	for (i = 0, it = arr.begin(); it != arr.end(); ++it, ++i) {
		//jFile.get<int>("x", *it);
		//jFile.get<int>("y", *it);
		//jFile.get<std::string>("type", *it);
		//jFile.get<int>("amount", *it);
	}
	*/
	m_Music.reset( new Gosu::Song( m_Engine->audio(), Gosu::resourcePrefix() + L"Sound/Colugo-Fantastic_face.ogg"));
	m_Music->play(true);
	
}

void Environment::tellPlayer( Player *_player )
{
	m_Player = _player;
	m_Player->setWindowScale( m_Scale );
	m_Player->setPhysics( m_PlayerPos.x, m_PlayerPos.y, m_World);
	m_Player->setLayer( 4 );
}

void Environment::update()
{
	// Step physics simulation
	//m_Worldp->Step(m_TimeStep, m_Iterations);
	
	m_Player->update(m_Engine->input());
	b2Vec2 m_PlayerPos = m_Player->getPosition();
	
	// Test Zoom
	if (m_Engine->input().down(Gosu::kbUp)) {
		m_Zoom += 0.01;
	}
	if (m_Engine->input().down(Gosu::kbDown)) {
		m_Zoom -= 0.01;
	}
	
	// We need to know where to draw (temporary, this will be done via script)
	m_Focus[0] = m_PlayerPos.x;
	m_Focus[1] = m_PlayerPos.y - 4.0;
	m_Offset[0] = m_Focus[0]*m_Scale*m_Zoom - m_Width/2;
	m_Offset[1] = m_Height - m_Focus[1]*m_Scale*m_Zoom - m_Height/2;
	
	// Update scene objects
	// Do callbacks for areas
	/*
	b2AABB space;
	space.lowerBound = b2Vec2(m_Center.x - m_OuterRegion.x, m_Center.y - m_OuterRegion.y);
	space.upperBound = b2Vec2(m_Center.x + m_OuterRegion.x, m_Center.y + m_OuterRegion.y);
	
	std::map< std::string, SpriteLayer >::iterator itL;
	for (itL = m_Layers.begin(); itL != m_Layers.end(); ++itL) {
		std::vector< SceneArea >::iterator it;
		for (it = itL->second.areas.begin(); it != itL->second.areas.end(); it++) {
			if ((*it)->overLap(space)) {
				if (!(*it)->inCamera()) {
					(*it)->onEnterCamera();
				}
			}else{
				if ((*it)->inCamera()) {
					(*it)->onLeaveCamera();
				}
			}
			
			// This ties the camera focus to the player
			// defeating any choreography systems
			if ((*it)->pointIn(focus)) {
				if (!(*it)->playerIn()) {
					(*it)->onPlayerEnter();
				}
			}else{
				if ((*it)->playerIn()) {
					(*it)->onPlayerLeave();
				}
			}
		}
		// update sceneobjects
		// re-order by Y value if desired
	}
*/	
}

void Environment::draw() const
{
	// Canvas color
	m_Engine->graphics().drawQuad( 0, 0, m_canvasColor, 
		m_Width, 0, m_canvasColor,
		0, m_Height, m_canvasColor,
		m_Width, m_Height, m_canvasColor, 0);
	
	// Render all sprites
	double scale, zoom;
	std::map< std::string, SpriteLayer >::const_iterator itL;
	std::list< Sprite >::const_iterator itS;
	for (itL = m_Layers.begin(); itL != m_Layers.end(); ++itL) {
		scale = 1.0/itL->second.scale;
		zoom = 1.0 + scale * (m_Zoom - 1.0);
		for (itS = itL->second.sprites.begin(); itS != itL->second.sprites.end(); ++itS) {
			itS->drawZoom( m_Focus[0], m_Focus[1], scale, zoom, m_Width/2, m_Height/2);
		}
	}
	
	// Render dynamic objects
	m_Player->drawZoom( m_Focus[0], m_Focus[1], 1.0, m_Zoom, m_Width/2, m_Height/2);
}
