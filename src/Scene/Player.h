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
#include <Gosu/Gosu.hpp>
#include <Box2D.h>
#include <map>
#include "SceneObject.h"
 
class SpriteSheet;
class RenderManager;

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
	Player();
	
	void addSpriteSheet(std::string _name, SpriteSheet* _anim);
	
	void setLayer( Gosu::ZPos _z );
	
	void setPhysics( double _x, double _y, b2World* _world);
	b2Vec2 getPosition() { return m_Pos; }
	
	void setGravity( b2Vec2 _gravity );
	
	void onColStart(SceneObject *other, b2ContactPoint point);
	void onColPersist(SceneObject *other, b2ContactPoint point);
	void onColFinish(SceneObject *other, b2ContactPoint point);
	void onMessage(std::string _message);
	
	void update();
	
protected:
	//Image Data
	std::map< std::string, SpriteSheet*> m_Anims;
	SpriteSheet* m_AnimState;
	Gosu::ZPos m_Layer;
	
	b2Body *m_Body;
	b2World *m_World;
	b2Vec2 m_Pos;
	b2Vec2 m_Gravity;
	
	// something about serialization into sqlite...
	// something about lua bindings..
};

/**
* This class will serve as the basis for all AI
* 
*/
class AIPlayer : public Player {
public:
	AIPlayer(int _x, int _y, b2World* _world, Gosu::Graphics& _graphics);
	
	void update(Gosu::Input& _input);
	void draw() const;
private:
	
	//AI data
	int m_State;
};