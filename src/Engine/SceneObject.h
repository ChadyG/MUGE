/*
 *  SceneObject.h
 *  Destructible
 *
 *  Created by Chad on 1/9/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#include <Gosu/Gosu.hpp>
#include <Box2D.h>
/**
* Base classes for scene object
* SceneObject describes all renderable entities
* i.e. player, enemies, items. etc.
* SceneArea describes all logical nodes
* i.e. spawn nodes, triggers
*/
class SceneObject
{
protected:
	//Basic physics data (body + position)
	b2Body* m_Bodyp;
	b2Vec2 m_Pos;

public:
	SceneObject();
	
	virtual void update() = 0;
	virtual void draw() = 0;
	
	virtual int onAction() = 0;
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
	//structure holding extent information
	b2AABB m_Box;
	//booleans for current state of trigger
	bool m_inCamera;
	bool m_inPlayer;

public:
	SceneArea();
	
	//called by camera
	virtual bool overLap(b2AABB&) = 0;
	virtual bool pointIn(b2Vec2&) = 0;
	virtual bool inCamera() = 0;
	virtual void onEnterCamera() = 0;
	virtual void onLeaveCamera() = 0;
	virtual bool playerIn() = 0;
	virtual void onPlayerEnter() = 0;
	virtual void onPlayerLeave() = 0;
};