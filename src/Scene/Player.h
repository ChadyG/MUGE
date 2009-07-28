/*
 *  Player.h
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#include <Gosu/Gosu.hpp>
#include <Box2D.h>
#include <map>
#include "SceneObject.h"
 
class Animation;

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
	Player(int _x, int _y, b2World* _world);
	
	void addAnimation(std::string _name, Animation* _anim);
	
	void setLayer( Gosu::ZPos _z );
	
	void setPhysics( double _x, double _y, b2World* _world);
	//b2Vec2 getPosition();
	
	void setGravity( b2Vec2 _gravity );
	
	void onHit(SceneObject &other, b2ContactPoint &point);
	
	void update(Gosu::Input& _input);
	void draw(double _x, double _y, double _zoom = 1.0, double _angle = 0.0) const;
	
protected:
	//Image Data
	std::map< std::string, Animation*> m_Anims;
	Animation* m_AnimState;
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