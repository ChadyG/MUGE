/**
* The camera takes care of following player,
* identifying what needs to be drawn, and
* triggering level zones (onEnterCamera or so)
* which we can use for spawn nodes
*/
enum CameraStates {
	PAN,
	REST
};

class Camera
{
	b2AABB m_OuterRegion;
	b2AABB m_ViewRegion;
	b2AABB m_PlayerZone;
	b2AABB m_PlayerDesire;
	
	int m_State;

public:
	Camera();
	
	void update(std::vector< SceneArea* >::iterator);
};