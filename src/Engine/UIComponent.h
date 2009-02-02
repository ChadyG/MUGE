/*
 *  UIComponent.h
 *  Destructible
 *
 *  Created by Chad on 2/1/09.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
 
 /**
 * Base class for all UI
 *  Each UI component lives in a hierarchy
 *  The constructor is given the parent
 *  the update function is given the current draw layer to use (or something else)
 */
 class UIComponent
 {
	std::vector< UIComponent* > m_Children;
	UIComponent *m_Parent;
public:
	UIComponent( UIComponent&, Gosu::Graphics & ) { }
	bool addChild( UIComponent& ) = 0;
	void update( const Gosu::Input& ) = 0;
	void draw( int ) const = 0;
 };