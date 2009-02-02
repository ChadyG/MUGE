/*
 *  AIController.h
 *  Destructible
 *
 *  Created by Chad on 1/30/09.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */

struct AIKnowledgeBase
{
	b2Vec2 m_Playerpos;
	int m_PlayerHealth;
	int m_NumFriendlies;
	//List of percieved projectile + velocities (?)
};

class AIcontroller
{
	InputFilter m_Input;
	AIKnowledgeBase m_KB;
	
public:
	AIController() { }
	virtual void update(AIKnowledgeB &, SceneObject&) = 0;	
};