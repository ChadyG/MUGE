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
#include <map>
#include <list>
#include "../Sprite/Sprite.h"
#include "../Sprite/Animation.h"

class MyContactListener;
class InputFilter;

struct SpriteLayer
{
	std::list< Sprite > sprites;
	//std::list< SceneArea > areas;
	float scale;
	int layer;
	std::string ID;
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
	
	Gosu::Color m_canvasColor;
	std::map< std::string, SpriteLayer > m_Layers;
	
	// Temporary stuff
	b2Vec2 m_PlayerPos;
	Animation m_PlayerImage;
	boost::scoped_ptr< Gosu::Song > m_Music;
	// end temp sprite data
	
	
	// Physics data
	boost::scoped_ptr< b2World > m_Worldp;
	float m_TimeStep;
	float m_Iterations;
	
	//	Pixel transformation data
	// Focus is the level coordinates of the center of the screen
	// Extents are the rectangular width and height of the level
	// Zoom is a zooming factor for all layers
	// Scale is the x/y scale to transform from level coordinates to screen
	// Width and Height are screen size
	double m_Focus[2];
	double m_Offset[2];
	double m_Extents[2];
	double m_Zoom;
	int m_Scale;
	int m_Width, m_Height;
	
};
