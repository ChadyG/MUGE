/*
 *  Environment.h
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#include <Gosu/Gosu.hpp>
#include <Box2D.h>
#include "Sprite.h"
#include "SceneGraph.h"

class MyContactListener;
class InputFilter;

/**
* Used later for data baking/loading
*/
struct envData
{
	//pointer to data within
	int vPointer[5];
	//data
};

/**
* Gameplay class
* This class serves as a central hub for all gameplay objects.
* This makes communication between classes easier and in some cases actions can 
* defer up to this class.
* Also handles level loading.
*/
class Environment
{
public:
	Environment(std::wstring, MUGE* _engine );
	void update();
	void draw() const;
	
protected:
	// Game Data
	MUGE* m_Engine;
	boost::scoped_ptr< SceneGraph > m_SceneGraphp;
	
	// Temporary data for sprite layers
	boost::scoped_ptr< Gosu::Image > m_BackgroundImagep;
	boost::scoped_ptr< Gosu::Image > m_MidgroundImagep;
	boost::scoped_ptr< Gosu::Image > m_GroundImagep;
	boost::scoped_ptr< Gosu::Image > m_ForegroundImagep;
	
	Gosu::Color m_canvasColor;
	std::vector< Sprite > m_Background;
	std::vector< Sprite > m_Midground;
	std::vector< Sprite > m_Ground;
	std::vector< Sprite > m_Foreground;
	
	// Parallax scrolling factors
	float m_backgroundScale;
	float m_midgroundScale;
	float m_groundScale;
	float m_foregroundScale;
	
	// Temporary stuff
	b2Vec2 m_PlayerPos;
	Animation m_PlayerImage;
	// end temp sprite data
	
	boost::scoped_ptr< Gosu::Song > m_Music;
	
	// Physics data
	boost::scoped_ptr< b2World > m_Worldp;
	float m_TimeStep;
	float m_Iterations;
	
	//	Pixel transformation data
	int m_aOrigin[2];
	int m_aExtents[2];
	int m_Units[2];
	int m_Width, m_Height;
	
};
