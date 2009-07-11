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
public:
	Player(int _x, int _y, b2World* _world, Gosu::Graphics& _graphics);
	
	void addAnimation(std::string _name, Animation &_anim);
	
	void onHit(SceneObject &other, b2ContactPoint &point);
	
	void update(Gosu::Input& _input);
	void draw() const;
protected:
	//Image Data
	std::map<Animation*> m_Anims;
	
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