/*
   SceneObject.h
   Mizzou Game Engine
 
   Created by Chad Godsey on 1/9/08.
  
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
 
#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <Gosu/Gosu.hpp>
#include <Box2D.h>
#include <list>

class Animation;
class Sprite;
class Scene;

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
	
	/// Register this object in a scene, used for world to screen transformations
	void registerScene( Scene *_scene );
	
	/// Set this object to render as the given sprite
	void setSprite( Sprite *_sprite );
	/// Set this object to render as the given animation
	void setAnimation( Animation *_anim);
	/// Disable rendering for this object 
	void hide();
	/// Enable rendering for this object 
	void show();
	
	void setPhysics( b2Body *_body ) { m_Body = _body; }
	
	/// Inhibit physics response
	void Freeze() { m_Frozen = true; }
	/// Resume physics operations
	void Thaw() { m_Frozen = false; }
	
	/// setOrientation - sets local rotation (relative to parent)
	//void setOrientation( double _angle ) { m_Orientation = _angle; }
	/// setTranslation - sets local translation (relative to parent)
	//void setTranslation( b2Vec2 _trans ) { m_Translation = _trans; }
	
	/// setRotation - sets world Rotation
	void setRotation( double _angle ) { /*m_Orientation += _angle - m_Rotation;*/ m_Rotation = _angle; }
	/// setPosition - sets world position
	void setPosition( b2Vec2 _pos ) { /*m_Translation += _pos - m_Position;*/ m_Position = _pos; }
	
	
	/// getOrientation - returns local rotation
	//double getOrientation() { return m_Orientation; }
	/// getTranslation - returns local translation
	//b2Vec2 getTranslation() { return m_Translation; }
	
	/// getRotation - returns world rotation
	double getRotation() { return m_Rotation; }
	/// getPosition - returns world position
	b2Vec2 getPosition() { return m_Position; }
	
	void update();// double _rotate, b2Vec2 _translate);
	
	void draw(double _x, double _y, Gosu::ZPos _layer, double _zoom = 1.0, double _angle = 0.0) const;

	/// Physics callback
	virtual void onHit(SceneObject &other, b2ContactPoint &point);
	
	//void addChild( SceneObject* _child) { m_Children.push_back( _child ); }
	
protected:

	//double m_Orientation;
	//b2Vec2 m_Translation;
	
	b2Vec2 m_Position;
	double m_Rotation;
		
	std::list< SceneObject*> m_Children;
	Scene *m_Scene;
	
	Sprite *m_Sprite;
	Animation *m_Animation;
	
	b2Body *m_Body;
	bool m_Frozen;
	
	bool m_hidden;
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
