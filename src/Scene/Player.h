/*
   Player.h
   Mizzou Game Engine
 
   Created by Chad Godsey on 11/12/08.
  
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