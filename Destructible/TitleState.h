class GameState;

/**
* State for title screen and other middleware screens
* goes into main menu
*/
class TitleState : public GameState
{
	TitleState();
	
	static TitleState m_StateInstance;

public:
	void init();
	void cleanup();
	
	void pause();
	void resume();
	
	void update();
	void draw();
	
	void instance();
};