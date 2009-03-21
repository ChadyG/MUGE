/*
 *  Player.h
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
class Animation;
class SceneObject;

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
class Player : public SceneObject {
	//Physics data
	b2Body* m_Bodyp;
	
	//Image Data
	Animation m_Root;
	Animation m_Walk;
	Animation m_Jump;
	
public:
	Player(int,int,b2World*,Gosu::Graphics&);
	
	void update(Gosu::Input&);
	void draw() const;
};

/**
* This class will serve as the basis for all AI
* 
*/
class AIPlayer : public Player {
	//Physics data
	b2Body* m_Bodyp;
	
	//AI data
	int m_State;
	
	//Image Data
	Animation m_Root;
	Animation m_Walk;
	Animation m_Jump;
	
public:
	AIPlayer(int,int,b2World*,Gosu::Graphics&);
	
	void update(Gosu::Input&);
	void draw() const;
};