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
	// Game Data
	//boost::scoped_ptr< InputFilter > m_Inputp;
	boost::scoped_ptr< SceneGraph > m_SceneGraphp;
	
	// Need to decide on a depth factor for front and back
	// maybe 1/3 slow for back and 1/3 faster for front?
	// This will decide the width of the images.
	boost::scoped_ptr< Gosu::Image > m_BackgroundImagep;
	boost::scoped_ptr< Gosu::Image > m_MidgroundImagep;
	boost::scoped_ptr< Gosu::Image > m_GroundImagep;
	boost::scoped_ptr< Gosu::Image > m_ForegroundImagep;
	
	// Physics data
	boost::scoped_ptr< b2World > m_Worldp;
	float m_TimeStep;
	float m_Iterations;
	
	//	Pixel transformation data
	int m_aOrigin[2];
	int m_aExtents[2];
	int m_Units;
	int m_Width, m_Height;
	
	// Temporary stuff
	b2Vec2 m_PlayerPos;
	Gosu::Graphics* m_Graphicsp;

public:
	Environment(std::wstring, Gosu::Graphics &);
	void update(const Gosu::Input &);
	void draw() const;
};
