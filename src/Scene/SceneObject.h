/*
   SceneObject.h
   My Unnamed Game Engine
 
   Created by Chad Godsey on 1/9/08.
  
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
 
#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <Gosu/Gosu.hpp>
#include <Box2D.h>
#include <list>

class SpriteSheet;
class Sprite;

/**
 Base classes for scene object
 
 Performs hierarchical transform
	all scene classes must inherit to be placed
	in scene hierarchy and be included in transform updates.
 
 SceneObject describes all renderable entities
 i.e. player, enemies, items. etc.
 SceneArea describes all logical nodes
 i.e. spawn nodes, triggers
*/
class SceneObject
{
public:
	SceneObject();

	/// Set this object to render as the given sprite
	void setSprite( Sprite *_sprite );
	/// Set this object to render as the given SpriteSheet
	void setSpriteSheet( SpriteSheet *_anim);
	/// Disable rendering for this object 
	void hide();
	/// Enable rendering for this object 
	void show();
	
	void setPhysics( b2Body *_body ) { m_Body = _body; }
	
	/// Inhibit physics response
	void Freeze() { m_Frozen = true; }
	/// Resume physics operations
	void Thaw() { m_Frozen = false; }
	
	/// setRotation - sets world Rotation
	void setRotation( double _angle ) { m_Rotation = _angle; }
	/// setPosition - sets world position
	void setPosition( b2Vec2 _pos ) { m_Position = _pos; }
	
	/// getRotation - returns world rotation
	double getRotation() { return m_Rotation; }
	/// getPosition - returns world position
	b2Vec2 getPosition() { return m_Position; }
	
	virtual void update();

	/// Physics callback
	virtual void onColStart(SceneObject *other, b2ContactPoint point);
	virtual void onColPersist(SceneObject *other, b2ContactPoint point);
	virtual void onColFinish(SceneObject *other, b2ContactPoint point);

	/// Message passing
	virtual void onMessage(std::string _message);
	
protected:
	b2Vec2 m_Position;
	double m_Rotation;

	Sprite *m_Sprite;
	SpriteSheet *m_SpriteSheet;
	
	b2Body *m_Body;
	bool m_Frozen;
	
	bool m_hidden;

	unsigned m_ID;
};

/**
* Trigger descendants must implement the accessors and mutators below
*	Tests
*		overLap	-	given a b2AABB test to see if there is a collision with own AABB
*		pointIn	-	given a b2Vec2 test to see if the point lies within own AABB
*	Events
*		onEnterCamera	-	entered camera zone
*		onLeaveCamera	-	left camera zone
*		onPlayerEnter	-	player entered area
*		onPlayerLeave	-	player left area
*	State Tests
*		inCamera	-	are we in the camera? 
*		playerIn	-	is the player inside?
*/
class Trigger : public SceneObject
{
public:
	Trigger();
	
	/// setExtents - sets the dimensions for rectangular trigger areas
	void setExtents( double _top, double _left, double _bottom, double _right);
	
	/// overlap - test if bounding box overlaps this
	bool overlap(b2AABB &_other) { return b2TestOverlap( m_Box, _other ); }
	
	/// pointIn - test if point is within this rectangular box
	bool pointIn(b2Vec2 &_point);
	
	/// inCamera - returns the state of camera focus
	bool inCamera() { return m_inCamera; }
	
	/// onEnterCamera - callback for entering focus
	void onEnterCamera();
	
	/// onLeaveCamera - callback for leaving focus
	void onLeaveCamera();
	
	/// playerIn - returns the state of the player with regard to this area
	bool playerIn() { return m_inPlayer; }
	
	/// onPlayerEnter - callback for player entering this area
	void onPlayerEnter();
	
	/// onPlayerLeave - callback for player leaving this area
	void onPlayerLeave();

protected:
	//structure holding extent information
	b2AABB m_Box;
	//booleans for current state of trigger
	bool m_inCamera;
	bool m_inPlayer;
	
};

#endif
