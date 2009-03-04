/*
 *  Weapons.h
 *  Destructible
 *
 *  Created by Chad on 2/17/09.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */

#include "Inventory.h"

/**
 *	Blade weapons
 *
 */
class CutterObject : public InventoryWeapon
{
public:
	CutterObject(unsigned type, unsigned sharpness);
	
	void onGrab(Player &player);
	void onHit(SceneObject &other, unsigned strength);
	
	void onUse(b2World &world, b2Vec2 pos);
	
private:
	unsigned m_Sharpness;
	Animation* m_Animation;
};

/**
 *	Clubs, hammers, etc.
 *
 */
class BludgeonObject : public InventoryWeapon
{
public:
	BludgeonObject(unsigned type, unsigned weight);
	
	void onGrab(Player &player);
	void onHit(SceneObject &other, unsigned strength);
	
	void onUse(b2World &world, b2Vec2 pos);
	
private:
	unsigned m_Weight;
	Animation* m_Animation;
};

/**
 *	Projectile shooters
 *
 */
class ShotObject : public InventoryWeapon
{
public:
	ShotObject(unsigned type, unsigned speed);
	
	void onGrab(Player &player);
	void onHit(SceneObject &other, unsigned strength);
	
	void onUse(b2World &world, b2Vec2 pos);
	
private:
	unsigned m_Speed;
	Animation* m_Animation;
};

/**
 *	Explosives
 *
 */
class KBombObject : public InventoryWeapon
{
public:
	KBombObject(unsigned type, unsigned radius, unsigned power);
	
	void onGrab(Player &player);
	void onHit(SceneObject &other, unsigned strength);
	
	void onUse(b2World &world, b2Vec2 pos);
	
private:
	unsigned m_Radius;
	unsigned m_Power;
	Animation* m_Animation;
};