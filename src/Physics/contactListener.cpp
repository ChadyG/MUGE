/*
 *  contactListener.cpp
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#include "ContactListener.h"
#include <Box2D.h>

// We only care about new contacts
void MenuListener::Add(const b2ContactPoint* point)
{
	std::vector< b2ContactPoint >::iterator ic;
	std::vector< b2ContactPoint >::iterator iDel;
	bool found = false;
	for (ic = m_Contacts.begin(); ic != m_Contacts.end(); ++ic) {
		if ((point->shape1 == ic->shape1 && point->shape2 == ic->shape2) ||
			(point->shape2 == ic->shape1 && point->shape1 == ic->shape2)) {
			iDel = ic;
			found = true;
		}
	}
	if ( found )
		m_Contacts.erase( iDel );

	b2ContactPoint *copy = new b2ContactPoint();
	copy->shape1 = point->shape1;
	copy->shape2 = point->shape2;
	copy->position = point->position;
	copy->velocity = point->velocity;
	copy->normal = point->normal;
	m_Contacts.push_back( *copy );
}

void MenuListener::Persist(const b2ContactPoint* point)
{
	std::vector< b2ContactPoint >::iterator ic;
	std::vector< b2ContactPoint >::iterator iDel;
	bool found = false;
	for (ic = m_Contacts.begin(); ic != m_Contacts.end(); ++ic) {
		if ((point->shape1 == ic->shape1 && point->shape2 == ic->shape2) ||
			(point->shape2 == ic->shape1 && point->shape1 == ic->shape2)) {
			iDel = ic;
			found = true;
		}
	}
	if ( found )
		m_Contacts.erase( iDel );

}

void MenuListener::Remove(const b2ContactPoint* point)
{
	std::vector< b2ContactPoint >::iterator ic;
	std::vector< b2ContactPoint >::iterator iDel;
	bool found = false;
	for (ic = m_Contacts.begin(); ic != m_Contacts.end(); ++ic) {
		if ((point->shape1 == ic->shape1 && point->shape2 == ic->shape2) ||
			(point->shape2 == ic->shape1 && point->shape1 == ic->shape2)) {
			iDel = ic;
			found = true;
		}
	}
	if ( found )
		m_Contacts.erase( iDel );
}

void MenuListener::Result(const b2ContactResult* point)
{

}

int MenuListener::Update()
{
	std::vector< b2ContactPoint >::iterator ic;
	b2FilterData filter;
	int small = 0, large = 0;
	for (ic = m_Contacts.begin(); ic != m_Contacts.end(); ++ic) {
		//stuff goes here
		if ( ic->velocity.Length() > 75.0f) {
			large = 2;
		}else
		if ( ic->velocity.Length() > 20.0f) {
			small = 1;
		}
	}
	return small + large;
}




// Contact Listener for Adventure mode

void AdventureListener::Add(const b2ContactPoint* point)
{
	std::vector< b2ContactPoint >::iterator ic;
	std::vector< b2ContactPoint >::iterator iDel;
	bool found = false;
	for (ic = m_Contacts.begin(); ic != m_Contacts.end(); ++ic) {
		if ((point->shape1 == ic->shape1 && point->shape2 == ic->shape2) ||
			(point->shape2 == ic->shape1 && point->shape1 == ic->shape2)) {
			iDel = ic;
			found = true;
		}
	}
	if ( found )
		m_Contacts.erase( iDel );

	b2ContactPoint *copy = new b2ContactPoint();
	copy->shape1 = point->shape1;
	copy->shape2 = point->shape2;
	copy->position = point->position;
	copy->velocity = point->velocity;
	copy->normal = point->normal;
	m_Contacts.push_back( *copy );
}

void AdventureListener::Persist(const b2ContactPoint* point)
{
	std::vector< b2ContactPoint >::iterator ic;
	std::vector< b2ContactPoint >::iterator iDel;
	bool found = false;
	for (ic = m_Contacts.begin(); ic != m_Contacts.end(); ++ic) {
		if ((point->shape1 == ic->shape1 && point->shape2 == ic->shape2) ||
			(point->shape2 == ic->shape1 && point->shape1 == ic->shape2)) {
			iDel = ic;
			found = true;
		}
	}
	if ( found )
		m_Contacts.erase( iDel );

}

void AdventureListener::Remove(const b2ContactPoint* point)
{
	std::vector< b2ContactPoint >::iterator ic;
	std::vector< b2ContactPoint >::iterator iDel;
	bool found = false;
	for (ic = m_Contacts.begin(); ic != m_Contacts.end(); ++ic) {
		if ((point->shape1 == ic->shape1 && point->shape2 == ic->shape2) ||
			(point->shape2 == ic->shape1 && point->shape1 == ic->shape2)) {
			iDel = ic;
			found = true;
		}
	}
	if ( found )
		m_Contacts.erase( iDel );
}

void AdventureListener::Result(const b2ContactResult* point)
{

}

int AdventureListener::Update()
{
	std::vector< b2ContactPoint >::iterator ic;
	b2FilterData filter;
	int small = 0, large = 0;
	for (ic = m_Contacts.begin(); ic != m_Contacts.end(); ++ic) {
		//stuff goes here
		if ( ic->velocity.Length() > 75.0f) {
			large = 2;
		}else
		if ( ic->velocity.Length() > 20.0f) {
			small = 1;
		}
	}
	return small + large;
}