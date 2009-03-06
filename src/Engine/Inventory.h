/*
 *  Inventory.h
 *  Destructible
 *
 *  Created by Chad on 2/17/09.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */

#include "Animation.h"
#include "SceneObject.h"

class InventoryWeapon;
class InventoryItem;

class Inventory
{
public:
	Inventory();
	
	void draw() const;
	void update();
	
	void addObject( SceneObject* object);
	
private:
	std::vector< InventoryWeapon* > m_Weapons;
	std::vector< InventoryItem* > m_Items;
	unsigned m_WeaponSlots;
	unsigned m_ItemSlots;
};

/**
 * Base class for all Items
 *
 * Items can exist in the scene and be picked up to be placed
 * in the player's inventory.  They are then available for use.
 * 
 */
class InventoryItem : SceneObject
{
public:
	InventoryItem() { }

	const int getAmount() const;
	
protected:
	t_Image m_Icon;
	int m_Amount;
};

/**
 *	Base class for all weapons
 *
 *	Weapons are inventory items, which can be equipped (in inventory),
 *	and used.  The inventory and player handle drawing of the weapons.  Player
 *	gets access to current weapon from inventory.
 *	When used, the weapon can interact with the world to do melee damage, spawn
 *	objects (projectiles, bombs).  
 */
class InventoryWeapon : SceneObject
{	
public:
	InventoryWeapon() { }
	
	const Animation& getSprites() const;
	
	const unsigned getLevel() const;
	const t_Image getIcon() const;

protected:
	unsigned m_Level;
	t_Image m_Icon;
	Animation* m_Animation;
};
