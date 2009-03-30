/*
 *  SceneGraph.h
 *  Destructible
 *
 *  Created by Chad on 2/20/09.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */

#include "SceneObject.h"
#include "Camera.h"
#include "Player.h"

//forward declarations

class SceneGraph
{
public:
	//Static Objects
	int aGround[128];
	
	//Dynamic Objects
	boost::scoped_ptr< Camera > Camerap;
	boost::scoped_ptr< Player > Playerp;
	std::vector< boost::scoped_ptr< SceneArea > > Areasv;
	std::vector< boost::scoped_ptr< SceneObject > > Enemiesv;
	std::vector< boost::scoped_ptr< SceneObject > > Bodiesv;
	std::vector< boost::scoped_ptr< SceneObject > > Weaponsv;
};
