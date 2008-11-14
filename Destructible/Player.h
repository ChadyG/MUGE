
class Animation;
class SceneObject;

/**
* Our Player class
* yeah..
* most of the "Extra Stuff" is from another game, not sure if we need it yet
*/
class Player : public SceneObject {
	//Box2D data
	b2Body* body;
	b2Vec2 position;
	b2Vec2 origin;
	
	// Image Data
	Animation root;
	Animation walk;
	
	// Extra stuff
	float32 rotation;
	short outOfScene;
	unsigned short score;
	unsigned short canJump;
	b2Vec2 jumpFrom;
public:
	Player(int,int,b2World*,Gosu::Graphics&);
	
	void update(Gosu::Input&);
	void draw() const;
	
	unsigned getScore() const;
	void addScore();
	b2Vec2 getPosition() const;
	void ApplyImpulse(b2Vec2);
	short isOutOfScene() const;
	void setContact(const b2Vec2&);
	void collision();
};