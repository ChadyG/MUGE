/*
	Player.h
	My Unnamed Game Engine
 
	Created by Chad on 11/12/08.
	Copyright 2009 BlitThis! studios. All rights reserved.

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
#ifndef PLAYER_H
#define PLAYER_H

#include <Gosu/Gosu.hpp>
#include <Box2D.h>
#include <map>
#include "SceneObject.h"
 
class SpriteSheet;

/**
* Our Player class
* to detect if the player is on the ground (jumping)
* we will attach two shapes to the physics body, one for
* the torso and one for the feet.  We can then check the
* contact points on the feet and see if the player is 
* on the ground or touching a wall etc. by examining the
* collision normal.
* 
*/
class Player : public SceneObject
{
public:
	Player() {}

	//void setID(int _id) { m_Object.setID(_id); }
	virtual void init() {}
	
	virtual void update() {}

	///Serialization/Deserialization
	virtual void encodeWith(Json::Value *_val) {}
	virtual void initWith(Json::Value _val) {}
	
protected:
	//SceneObject m_Object;
};

/**
* This class will serve as the basis for all AI
* 
*/
class AIPlayer : public Player {
public:
	AIPlayer();
	virtual void init() {}
	
	virtual void update() {}

	///Serialization/Deserialization
	void encodeWith(Json::Value *_val) {}
	void initWith(Json::Value _val) {}
private:
	
	//AI data
	int m_State;
};

#endif