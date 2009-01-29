#include <Gosu/Gosu.hpp>

/**
* The camera takes care of following player,
* identifying what needs to be drawn, and
* triggering level zones (onEnterCamera or so)
* which we can use for spawn nodes
*/

class Animation
{
	std::vector< boost::shared_ptr<Gosu::Image> > m_Sprites;
	
	unsigned int m_Frame;
	int m_numFrames;
	int m_Timer;
	int m_Count;

public:
	Animation(Gosu::Graphics&, std::wstring&, int width, int height, int count=1);
	
	Gosu::Image& operator->();
};
