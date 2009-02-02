/*
 *  AIManager.h
 *  Destructible
 *
 *  Created by Chad on 1/30/09.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 *	Need a way to manage mapping controllers to sceheobjects and 
 *	respectiveknowledge base
 */

class AIController;

struct AI_Map
{
	SceneObject *m_Obj;
	AIController *m_Controller;
};

class AIManager
{
	std::vector< AIController *> m_AIControllers;
	std::vector< AI_Map *> m_Mappings;
public:
	AIManager();
	virtual void addAI(SceneObject *);
	virtual void updateAI();
};
