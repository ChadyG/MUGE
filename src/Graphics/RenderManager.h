/*
   RenderManager.h
   My Unnamed Game Engine
 
   Created by Chad Godsey on 12/9/09.
  
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

#ifndef RENMAN_H
#define RENMAN_H

#include <Gosu/Gosu.hpp>
#include <map>
#include <list>
#include <json/json.h>
//#include "SpriteSheet.h"
//#include "Sprite.h"
#include "Camera.h"

class Sprite;
class SpriteSheet;
class MessageBubble;
//class Camera;
/*
struct MessageBubble
{
	Gosu::Image *text;
	std::wstring Message;
	bool isStatic;
	int width;
	int height;
	double x;
	double y;
};
*/

class Renderable
{
public:
	Renderable() : m_posX(0.0f), m_posY(0.0f), m_zoom(1.0f), m_angle(0.0f), m_layer(0.0f), m_baseColor(Gosu::Color::WHITE), m_darkColor(Gosu::Color::WHITE) {}
	
	void setBaseColor(Gosu::Color _color) { m_baseColor = _color; }
	void setDarkColor(Gosu::Color _color) { m_darkColor = _color; }
	void setAlphaMode(Gosu::AlphaMode _alpha) { m_alphaMode = _alpha; }

	virtual void draw(double _x, double _y, double _zoom = 1.0, double _angle = 0.0) const {}
	virtual void update() {}

protected:
	friend class RenderManager;
	Renderable *m_prev, *m_next;
	double m_posX, m_posY, m_zoom, m_angle;
	int m_layer;
	//apply color
	//apply light (lighten => screen, darken => multiply)
	Gosu::Color m_baseColor, m_darkColor;
	Gosu::AlphaMode m_alphaMode;
};
/**
*	Intended services 
*		* simple resource managing for Gosu::Image in Sprites
*		* state based rendering
*			- persistent rendering based on given values
*		* clean separation of world to screen transformation
*			- all given values are in world coordinates
*			- scene manager tells camera orientation + position for rendering
*/
//TODO: establish base renderable class with draw(x, y)
//		use Gosu::Translate/Rotate stuff to do coordinate transform
//		start using render groups (?)
//== Order of transforms
//	Global
//		Translate to origin
//		Rotate
//		Translate to focus
class RenderManager
{
public:
	RenderManager();
	~RenderManager();
	
	/// Set parameters for window
	/// @param _w screen width
	/// @param _h screen height
	/// @param _s screen scaling factor - pixels per world unit
	void setScreen( int _w, int _h, double _s)
	{
		m_Camera->setExtents( _w, _h);
		m_Camera->setScale( _s );
	};

	/// Update camera parameters
	/// @param _x world space X coordinate of camera 
	/// @param _y world space Y coordinate of camera 
	/// @param _zoom zoom factor of camera 
	/// @param _rot angle of camera (degrees)
	/// This should be called on each game tick
	void setCamera( double _x, double _y, double _zoom = 1.f, double _rot = 0.f )
	{
		m_Camera->setFocus( _x, _y);
		m_Camera->setZoom( _zoom );
		m_Camera->setRotation( _rot );
	};

	/// Set camera object to user defined type
	/// This camera takes cares of the world to screen transformation.  
	/// The default camera does a direct transformation (1:1 linear) from world space to screen space.
	/// If this does not suit your needs, create a child class of the Camera type to perform your own transformation.
	/// @param _cam A camera object created by the caller (RenderManager will not automatically initialize your Camera)
	void setCamera( Camera *_cam )
	{
		m_Camera = _cam;
	};

	Gosu::Font& font() { return m_Font; }

	Camera& camera() { return *m_Camera; }

	/// Updates all sprite sheets
	/// This should be called every game tick
	void update();

	/// Call to run draw operations
	void doRender() const;

	/// Define scaling factor for a layer
	void setLayerScale(int _layer, float _scale) { m_LayerScales[_layer] = _scale; }
	
	/// create a message bubble
	MessageBubble* createMessage(std::wstring _message, double _x, double _y, bool _static = true);
	/// create a sprite
	Sprite* createSprite(int _layer, std::wstring _filename);
	/// create a SpriteSheet
	SpriteSheet* createSpriteSheet(
		int _layer, std::wstring _filename, int _width, int _height, int _delay = 1);

	
	/// register a Renderable
	void registerRenderable(int _layer, Renderable* _r);

	/// register a sprite
	void registerSprite(int _layer, Sprite* _sp);
	/// register a SpriteSheet
	void registerSpriteSheet(int _layer, SpriteSheet* _ss);

	
	/// Delete a specific Renderable
	void deleteRenderable(Renderable* _r);

	/// Delete a specific sprite
	void deleteSprite(Sprite* _sprite);
	/// Delete a specific spritesheet
	void deleteSpriteSheet(SpriteSheet* _sheet);
	/// Delete a specific sprite
	void deleteMessage(MessageBubble* _message);

	/// Release all Sprites
	//void clearSprites() { m_Sprites.clear(); }
	/// Release all SpriteSheets
	//void clearSpriteSheets() { m_SpriteSheets.clear(); }
	/// Release all Message Bubbles
	//void clearMessages() { m_Messages.clear(); }
	/// Release all resources
	//void clearAll() { m_Sprites.clear(); m_SpriteSheets.clear(); m_Messages.clear(); }
	
	/// Static accessor to current render manager
	static RenderManager* getCurrentContext() { return s_CurrentContext; }

	/// Static setter for current render manager
	static void setCurrentContext(RenderManager* _context) { s_CurrentContext = _context; }

protected:
	static RenderManager* s_CurrentContext;

	//Gosu Images are a shared resource
	std::map< std::wstring, Gosu::Image* > m_Images;
	//std::map< int, Gosu::Image* > m_Images;
	//std::map< std::wstring, int > m_ImageMap;

	std::map< int, float > m_LayerScales;

	//Renderable objects
	//std::list< SpriteSheet > m_SpriteSheets;
	//std::list< Sprite > m_Sprites;
	Renderable *m_rendlist;
	//std::list< Animation > m_Animations;
	//std::list< MessageBubble > m_Messages;

	//Shared resources for message bubbles
	//SpriteSheet m_MessageCorners;
	//Gosu::Image m_MessageTip;
	Gosu::Font m_Font;
	
	int m_curImageID;

	//double m_camX, m_camY, m_camZoom, m_camRot;
	//int m_screenW, m_screenH, m_screenScale;

	Camera *m_Camera;
};


#endif

