/*
 *  UIComponent.h
 *  Destructible
 *
 *  Created by Chad on 2/1/09.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#include <Gosu/Gosu.hpp>
#include <Box2D.h>
 
 /**
 * Base class for all UI
 *  Each UI component lives in a hierarchy
 *  The constructor is given the parent
 *	update is given the current input
 *  the draw function is given the current draw layer to use (or something else)
 */
class UIComponent
{
protected:
	std::vector< UIComponent* > m_Children;
	std::wstring m_Text;
	UIComponent *m_Parent;
public:
	UIComponent() { }
	void addChild( const UIComponent* );
	virtual bool pointIn( const b2Vec2& );
	virtual void update( const Gosu::Input& );
	virtual void draw( int ) const;
};
