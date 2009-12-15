/*
	Scene.h
	My Unnamed Game Engine
 
	Created by Chad Godsey on 11/12/08.
	Copyright 2009 BlitThis! studios.

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
 
#include <Gosu/Gosu.hpp>
#include <Box2D.h>
#include <map>
#include <list>
#include "SceneObject.h"
#include "../Input/JSONFile.hpp"
#include "../Sprite/Sprite.h"
#include "../Sprite/SpriteSheet.h"
#include "../Sprite/TranslationModules.h"
#include "../Physics/ContactListener.h"

class Core;
class Player;

struct SpriteLayer
{
	std::list< Sprite > sprites;
	std::list< SceneObject > objects;
	std::list< Trigger > triggers;
	float scale;
	int layer;
	std::string ID;
};

/**
* Scene class
* This class serves as a central hub for all gameplay objects.
* This makes communication between classes easier and in some cases actions can 
* defer up to this class.
* Also handles level loading.
*/
class Scene : public TransMod
{
public:
	Scene(std::wstring _config );
	
	void tellPlayer( Player *_player );

	void addObject( SceneObject &_object, Gosu::ZPos _layer );
	
	void addTrigger( Trigger &_trigger, Gosu::ZPos _layer );
	
	b2XForm worldToScreen( float _x, float _y, Gosu::ZPos _layer );
	
	void update();
	void draw() const;
	
protected:
	
	void evalJSON( json::grammar<char>::array _array, int _layer );
	void evalSprite( json::grammar<char>::array::const_iterator _it, int _layer );
	void evalTrigger( json::grammar<char>::array::const_iterator _it, int _layer );
	void evalWall( json::grammar<char>::array::const_iterator _it, int _layer );
	void evalKey( json::grammar<char>::array::const_iterator _it, int _layer );
	
	// Game Data
	boost::shared_ptr<Core> m_Engine;
	
	Gosu::Color m_canvasColor;
	
	boost::scoped_ptr<JSONFile> m_jFile;
	
	std::map< Gosu::ZPos, SpriteLayer > m_Layers;
	std::map< std::string, Gosu::ZPos > m_LayerNames;
	
	// Scene stuff
	boost::scoped_ptr< Gosu::Song > m_Music;
	Player *m_Player;
	b2Vec2 m_PlayerPos;
	
	// Physics data
	AdventureListener m_ContactListener;
	b2World *m_World;
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
	double m_Rot, m_Orientation;
	int m_Scale;
	int m_Width, m_Height;
	
};
