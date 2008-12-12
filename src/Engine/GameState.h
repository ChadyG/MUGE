#ifndef GAMESTATE_H
#define GAMESTATE_H

class MUGE;

/**
* Abstract class to describe a game state
* all logic will go into these classes
* they use the singleton paradigm so simplify
* use
*/
class GameState
{
protected:
	GameState() { }

public:
	virtual void init(Gosu::Graphics &) = 0;
	virtual void cleanup() = 0;
	
	virtual void pause() = 0;
	virtual void resume() = 0;
	
	virtual void update(const Gosu::Input &, MUGE*) = 0;
	virtual void draw() const = 0;
	
};

#endif