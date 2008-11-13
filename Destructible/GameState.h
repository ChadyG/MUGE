
/**
* Virtual class to describe a game state
* all logic will go into these classes
* they use the singleton paradigm so simplify
* use
*/
class GameState
{
	GameState();

public:
	virtual void init() = 0;
	virtual void cleanup() = 0;
	
	virtual void pause() = 0;
	virtual void resume() = 0;
	
	virtual void update() = 0;
	virtual void draw() = 0;
	
	virtual void instance() = 0;
};