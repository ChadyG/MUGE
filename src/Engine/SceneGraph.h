/*
 *  SceneGraph.h
 *  Destructible
 *
 *  Created by Chad on 2/20/09.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */

//forward declarations
class Camera;
class Player;
class SceneArea;
class SceneObject;

class SceneGraph
{
public:
	//Static Objects
	int aGround[128];
	
	//Dynamic Objects
	boost::scoped_ptr< Camera > Camerap;
	//boost::scoped_ptr< Player > Playerp;
	std::vector< SceneArea* > Areasp;
	std::vector< boost::scoped_ptr< SceneObject > > Enemiesv;
	std::vector< boost::scoped_ptr< SceneObject > > Bodiesv;
	std::vector< boost::scoped_ptr< SceneObject > > Weaponsv;
};
