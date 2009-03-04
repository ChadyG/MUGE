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

class MyContactListener;
class SceneGraph;
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
* Gameplay class, loads a level and handles common
* gameplay elements
*/
class Environment
{
	// Game Data
	//boost::scoped_ptr< InputFilter > m_Inputp;
	//boost::scoped_ptr< SceneGraph > m_SceneGraphp;
	
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

public:
	Environment(std::wstring, Gosu::Graphics &);
	void update(const Gosu::Window &, int, int);
	void draw(bool) const;
};
