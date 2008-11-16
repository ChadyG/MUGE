#include "Camera.h"
#include "SceneObject.h"

#define SCALEFACTOR 4.0

Camera::Camera(camDef& def)
	: m_State(e_pan)
{
	m_Center = def.origin;
	m_ViewExtents = def.extents;
	m_OuterRegion = def.safe;
	m_PlayerZone = def.player;
	m_PlayerDesire = def.desire;
}


void Camera::update(std::vector< SceneObject* >& objList, std::vector< SceneArea* >& areaList, b2Vec2& focus)
{
	float32 xDist = focus.x - m_Center.x;
	float32 yDist = focus.y - m_Center.y;
	if (m_State == e_pan) {
		// Move towards focus point by scaling factor
		m_Center.x += xDist / SCALEFACTOR;
		m_Center.y += yDist / SCALEFACTOR;
		
		// Change to rest when player is in desired area
		if (fabs(xDist) < m_PlayerDesire.x && fabs(yDist) < m_PlayerDesire.y)
			m_State = e_rest;
	}else{
		// Change to pan when player leaves player area
		if (fabs(xDist) >= m_PlayerZone.x && fabs(yDist) >= m_PlayerZone.y)
			m_State = e_pan;
	}
	
	// Do callbacks for areas
	b2AABB space;
	space.lowerBound = b2Vec2(m_Center.x - m_OuterRegion.x, m_Center.y - m_OuterRegion.y);
	space.upperBound = b2Vec2(m_Center.x + m_OuterRegion.x, m_Center.y + m_OuterRegion.y);
	std::vector< SceneArea* >::iterator it;
	for (it = areaList.begin(); it != areaList.end(); it++) {
		if ((*it)->overLap(space)) {
			if (!(*it)->inCamera()) {
				(*it)->onEnterCamera();
			}
		}else{
			if ((*it)->inCamera()) {
				(*it)->onLeaveCamera();
			}
		}
		
		if ((*it)->pointIn(focus)) {
			if (!(*it)->playerIn()) {
				(*it)->onPlayerEnter();
			}
		}else{
			if ((*it)->playerIn()) {
				(*it)->onPlayerLeave();
			}
		}
	}

	std::vector< SceneObject* >::iterator itObj;
	for (itObj = objList.begin(); itObj != objList.end(); itObj++) {
		
	}
}