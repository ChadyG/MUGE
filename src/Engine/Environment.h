#include <Gosu/Gosu.hpp>
#include <Box2d.h>

class MyContactListener;
class SceneObject;
class SceneArea;
class Camera;

/**
* Gameplay class, loads a level and handles common
* gameplay elements
*/
class Environment
{
	// Game Data
	Camera* m_Camerap;
	std::vector< SceneObject* > m_Objects;
	std::vector< SceneArea* > m_Areas;
	boost::scoped_ptr< Gosu::Image > m_Background;
	boost::scoped_ptr< Gosu::Image > m_Midground;
	boost::scoped_ptr< Gosu::Image > m_Ground;
	boost::scoped_ptr< Gosu::Image > m_Foreground;
	
	// Physics data
	
	//	Pixel transformation data
	int m_aOrigin[2];
	int m_aExtents[2];
	int m_Units;

public:
	Environment(std::wstring, Gosu::Graphics &);
	void update(const Gosu::Window &, int, int);
	void draw(bool) const;
};