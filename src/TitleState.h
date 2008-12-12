#include <Gosu/Gosu.hpp>
#include "Engine/GameState.h"

class MUGE;

/**
* State for title screen and other middleware screens
* goes into main menu
*/
class TitleState : public GameState
{
	boost::scoped_ptr<Gosu::Image> m_TitleScreen;
	int counter;
	int step;
	
	static TitleState m_StateInstance;

protected:
	TitleState() { }
	
public:
	void init(Gosu::Graphics &graphics);
	void cleanup();
	
	void pause();
	void resume();
	
	void update(const Gosu::Input &, MUGE *);
	void draw() const;
	
	static TitleState* instance() {
		return &m_StateInstance;
	}
};