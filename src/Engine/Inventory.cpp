/*
 *  Inventory.cpp
 *  Destructible
 *
 *  Created by Chad on 2/17/09.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */

#include "Inventory.h"


inline const Animation& InventoryWeapon::getSprites() const
{
	return *m_Animation;
}

inline const unsigned InventoryWeapon::getLevel() const
{
	return m_Level;
}
