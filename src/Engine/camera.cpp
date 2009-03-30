/*
 *  Camera.cpp
 *  Destructible
 *
 *  Created by Chad on 11/15/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#include "Camera.h"
#include "SceneObject.h"

// This needs to be close to the player's speed (pixels/second)
// since it is a scaling factor, the player can have varying speed
// and the camera will be able to adjust.
#define SCALEFACTOR 50.0

Camera::Camera(camDef& def)
	: m_State(e_pan)
{
	m_Center = def.origin;
	m_ViewExtents = def.extents;
	m_OuterRegion = def.safe;
	m_PlayerZone = def.player;
	m_PlayerDesire = def.desire;
}


void Camera::update( std::vector< boost::scoped_ptr< SceneArea > >& areaList, b2Vec2& focus)
{
	float32 xDist = focus.x - m_Center.x;
	float32 yDist = focus.y - m_Center.y;
	if (m_State == e_pan) {
		// Move towards focus point by scaling factor
		m_Center.x += xDist / SCALEFACTOR;
		m_Center.y += yDist / SCALEFACTOR;
		
		// Change to rest when player is in desired area
		if ((fabs(xDist) < m_PlayerDesire.x && fabs(yDist) < m_PlayerDesire.y) || focus.x < m_ViewExtents.x || focus.x > m_ViewExtents.y)
			m_State = e_rest;
	}else{
		// Change to pan when player leaves player area
		if (focus.x > m_ViewExtents.x && focus.x < m_ViewExtents.y && (fabs(xDist) >= m_PlayerZone.x || fabs(yDist) >= m_PlayerZone.y))
			m_State = e_pan;
	}
	
	// Do callbacks for areas
	b2AABB space;
	space.lowerBound = b2Vec2(m_Center.x - m_OuterRegion.x, m_Center.y - m_OuterRegion.y);
	space.upperBound = b2Vec2(m_Center.x + m_OuterRegion.x, m_Center.y + m_OuterRegion.y);
	std::vector< boost::scoped_ptr< SceneArea > >::iterator it;
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
		
		// This ties the camera focus to the player
		// defeating any choreography systems
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
}

b2Vec2 Camera::getCenter() const
{
	return m_Center;
}