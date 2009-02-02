/*
 *  InputFilter.h
 *  Destructible
 *
 *  Created by Chad on 12/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#include <Gosu/Gosu.hpp>
/**
*
*/

class InputFilter
{
	unsigned int m_Input;
public:
	InputFilter(const std::wstring &configFile);
	void reset();
	void setDown();
	void setUp();
	void setRight();
	void setLeft();
	
	bool Down();
	bool Up();
	bool Right();
	bool Left();
};

class PlayerInput
{
	InputFilter m_input;
public:
	PlayerInput();
	virtual void config(std::string &file);
	virtual void update(Gosu::Input &);
	
};