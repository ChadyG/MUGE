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

#include "Animation.h"

class Player;

#define CAT_WEAPON 0xF000
#define CAT_ITEM 0x0F00
#define CAT_BLOCK 0x00F0
#define CAT_PLAYER 0x000F

/**
* Base classes for scene object
* SceneObject describes all renderable entities
* i.e. player, enemies, items. etc.
* SceneArea describes all logical nodes
* i.e. spawn nodes, triggers
*
* Every object will have a physics body
*  non-dynamic objects will be set as sensors
*  this is to allow queries through box2D
*/
class SceneObject
{
protected:
	//Basic physics data (body + position)
	b2Body* m_Bodyp;
	b2World* m_Worldp;
	b2Vec2 m_Pos;
	uint16 m_Category;
	t_Image m_Icon;

public:
	SceneObject();
	
	virtual void update() = 0;
	virtual void draw() = 0;
	
	virtual void onGrab(Player &player) = 0;
	virtual void onHit(SceneObject &other, unsigned strength) = 0;
	
	virtual void onUse(Player &player) = 0;
	virtual const t_Image getIcon() const = 0;
	
	// Used for manipulation
	void setPosition( double x, double y, Gosu::ZPos z);
	void setRotation( double ang );
	
	// Used for run time type information during gameplay
	const uint16 getCategory() const;
};

inline const uint16 SceneObject::getCategory() const
{
	return m_Category;
}

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

#endif
