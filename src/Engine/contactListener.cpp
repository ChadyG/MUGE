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

void MyContactListener::Add(const b2ContactPoint* point)
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
	copy->normal = point->normal;
	m_Contacts.push_back( *copy );
}

void MyContactListener::Persist(const b2ContactPoint* point)
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
	copy->normal = point->normal;
	m_Contacts.push_back( *copy );
}

void MyContactListener::Remove(const b2ContactPoint* point)
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

void MyContactListener::Result(const b2ContactResult* point)
{

}

void MyContactListener::UpdateCallbacks()
{
	std::vector< b2ContactPoint >::iterator ic;
	b2FilterData filter;
	for (ic = m_Contacts.begin(); ic != m_Contacts.end(); ++ic) {
		//stuff goes here
	}
}
