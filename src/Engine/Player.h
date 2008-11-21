
class Animation;
class SceneObject;

/**
* Our Player class
* yeah..
* 
*/
class Player : public SceneObject {
	//Physics data
	
	// Image Data
	Animation root;
	Animation walk;
	Animation jump;
	
public:
	Player(int,int,b2World*,Gosu::Graphics&);
	
	void update(Gosu::Input&);
	void draw() const;
};