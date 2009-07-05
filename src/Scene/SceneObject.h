/*
 *  SceneObject.h
 *  Destructible
 *
 *  Created by Chad on 1/9/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
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
	
	virtual void update();
	virtual void draw();

	virtual void onHit(SceneObject &other, b2ContactPoint &point);
	
	// Used for manipulation
	void setPosition( double x, double y, Gosu::ZPos z);
	void setRotation( double ang );
	
protected:
	//Basic physics data (body + position)
	Sprite m_Sprite;
	b2Body* m_Bodyp;
	b2World* m_Worldp;
	b2Vec2 m_Pos;
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
