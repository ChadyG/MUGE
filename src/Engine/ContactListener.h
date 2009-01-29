#include <Box2D.h>
#include <vector>

/**
* Used by Box2D to provide physics callbacks
* derived functions are called during CCD and 
* should only be used to buffer contact lists
* as they may be called multiple times for a 
* single collision
*/
class MyContactListener : public b2ContactListener
{
	std::vector< b2ContactPoint > m_Contacts;
public:
	void Add(const b2ContactPoint* point);
	void Persist(const b2ContactPoint* point);
	void Remove(const b2ContactPoint* point);
	void Result(const b2ContactResult* point);
	void UpdateCallbacks();
};
