#include <Gosu/Gosu.hpp>

/**
* Base classes for scene object
* SceneObject describes all renderable entities
* i.e. player, enemies, items. etc.
* SceneArea describes all logical nodes
* i.e. spawn nodes, triggers
*/
class SceneObject
{
	b2Body* m_Bodyp;
	b2Vec2 m_Pos;

public:
	SceneObject();
	
	virtual void update() = 0;
	virtual void draw() = 0;
};


class SceneArea
{
	b2AABB m_Box;

public:
	SceneArea() = 0;
	
	bool overLap(b2AABB &area) = 0;
};