#include <Box2D.h>
#include <vector>
/**
* The camera takes care of following player,
* identifying what needs to be drawn, and
* triggering level zones (onEnterCamera or so)
* which we can use for spawn nodes
*/

class SceneArea;
class SceneObject;

enum CameraStates {
	e_pan,
	e_rest
};

struct camDef
{
	b2Vec2 origin;
	b2Vec2 extents;
	b2Vec2 safe;
	b2Vec2 player;
	b2Vec2 desire;
};

class Camera
{
	b2Vec2 m_Center;
	b2Vec2 m_ViewExtents;
	b2Vec2 m_OuterRegion;
	b2Vec2 m_PlayerZone;
	b2Vec2 m_PlayerDesire;
	
	int m_State;

public:
	Camera(camDef&);
	
	void update(std::vector< SceneObject* >&, std::vector< SceneArea* >&, b2Vec2&);
};