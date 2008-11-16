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
	
	// Box2D data
	b2World* m_Worldp;
	MyContactListener* m_ContactListenerp;
	float32 m_TimeStep;
	int32 m_Iterations;
	
	//	Pixel transformation data
	int m_aOrigin[2];
	int m_aExtents[2];
	int m_Units;

public:
	Environment(std::wstring, Gosu::Graphics &);
	void update(const Gosu::Window &, int, int);
	void draw(bool) const;
};