/*
 *  Environment.cpp
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#include "../Core/MUGE.h"
#include "Environment.h"
#include "../Input/JSONFile.hpp"
#include "../Physics/ContactListener.h"
#include <string>
#include <iostream>
#include <fstream>

Environment::Environment(std::wstring levelFile, MUGE* _engine)
{
	m_Engine = _engine;
	
	m_TimeStep = 1.0f / 60.0f;
	m_Iterations = 10.0f;
	
	m_Width = m_Engine->graphics().width();
	m_Height = m_Engine->graphics().height();
	m_Zoom = 1.0;
	
	//
	// Read in JSON encoded file
	// TinyJSON library will perform lexing and parsing
	// pull out data using JSONFile class
	//
	
	JSONFile jFile(Gosu::narrow(Gosu::resourcePrefix() + L"Data/" + levelFile + L".json"));
	json::grammar<char>::array::const_iterator it, it2;
	json::grammar<char>::array arr, arr2;
	json::grammar<char>::object o;
	int i, j;
	
	
	// This will be pushed to level file later
	m_PlayerPos = b2Vec2( jFile.get< double >("PlayerSpawn[0]"), jFile.get< double >("PlayerSpawn[1]"));
	
	arr = jFile.get<json::grammar<char>::array>("CanvasColor");
	m_canvasColor = Gosu::Color( 
							jFile.get< int >("CanvasColor[0]"), 
							jFile.get< int >("CanvasColor[1]"),
							jFile.get< int >("CanvasColor[2]"),
							jFile.get< int >("CanvasColor[3]"));
	
	
	std::wstring filename = Gosu::resourcePrefix() + L"Images/Hero_walking.png";
	m_PlayerImage.setImage(m_Engine->graphics(), filename, 85, 128, 20);
	//m_PlayerImagep.reset( new Gosu::Image(m_Engine->graphics(), filename, false));
	
	m_Scale = jFile.get< int >("Scale");
	m_PlayerImage.setWindowScale( m_Scale );
	
	
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
		m_Layers[jFile.get<std::string>("ID", *it)] = tLayer;
	}
	
	
	
	arr = jFile.get<json::grammar<char>::array>("GameObjects");
	for (i = 0, it = arr.begin(); it != arr.end(); ++it, ++i) {
		//jFile.get<int>("x", *it);
		//jFile.get<int>("y", *it);
		//jFile.get<std::string>("type", *it);
		//jFile.get<int>("amount", *it);
	}
	
	m_Music.reset( new Gosu::Song( m_Engine->audio(), Gosu::resourcePrefix() + L"Sound/Colugo-Fantastic_face.ogg"));
	m_Music->play(true);
	
}

void Environment::update()
{
	// Step physics simulation
	//m_Worldp->Step(m_TimeStep, m_Iterations);
	
	// Quick and dirty!
	if (m_Engine->input().down(Gosu::kbLeft)) {
		m_PlayerPos.x -= 0.1;
		if (m_Engine->input().down(Gosu::kbLeftShift)) 
			m_PlayerPos.x -= 0.1;
	}
	if (m_Engine->input().down(Gosu::kbRight)) {
		m_PlayerPos.x += 0.1;
		if (m_Engine->input().down(Gosu::kbLeftShift)) 
			m_PlayerPos.x += 0.1;
	}
	m_PlayerImage.setPosition( m_PlayerPos.x, m_PlayerPos.y, 4);
	
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

	// This will be handled in player later
	m_PlayerImage.update();
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
	m_PlayerImage.drawCurFrameZoom( m_Focus[0], m_Focus[1], 1.0, m_Zoom, m_Width/2, m_Height/2);
}
