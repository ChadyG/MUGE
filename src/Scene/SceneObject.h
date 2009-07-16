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

#include "../Sprite/Sprite.h"
#include "../Sprite/Animation.h"

class Player;

/**
* Base classes for scene object
* SceneObject describes all renderable entities
* i.e. player, enemies, items. etc.
* SceneArea describes all logical nodes
* i.e. spawn nodes, triggers
*/
class SceneObject
{
public:
	SceneObject();
	
	virtual void update() = 0;

	virtual void onHit(SceneObject &other, b2ContactPoint &point);
	
	// Used for manipulation
	void setPosition( double x, double y, Gosu::ZPos z);
	void setRotation( double ang );
	
protected:

};

/**
* SceneArea descendants must implement the accessors and mutators below
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
class SceneArea
{
public:
	SceneArea();
	
	//called by camera
	virtual bool overLap(b2AABB&);
	virtual bool pointIn(b2Vec2&);
	virtual bool inCamera();
	virtual void onEnterCamera();
	virtual void onLeaveCamera();
	virtual bool playerIn();
	virtual void onPlayerEnter();
	virtual void onPlayerLeave();

protected:
	//structure holding extent information
	b2AABB m_Box;
	//booleans for current state of trigger
	bool m_inCamera;
	bool m_inPlayer;
	
};

#endif
