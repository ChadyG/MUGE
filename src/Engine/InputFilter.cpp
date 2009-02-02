#include "InputFilter.h"
#define UP 0x0001
#define DOWN 0x0002
#define LEFT 0x0004
#define RIGHT 0x0008

InputFilter::InputFilter(const std::wstring &configFile)
{
	m_Input = 0x0000;
}

void InputFilter::reset()
{
	m_Input = 0x0000;
}

void InputFilter::setUp()
{
	m_Input |= UP;
}

void InputFilter::setDown()
{
	m_Input |= DOWN;
}

void InputFilter::setLeft()
{
	m_Input |= LEFT;
}

void InputFilter::setRight()
{
	m_Input |= RIGHT;
}

/**
*	Accessors
*/

bool InputFilter::Up()
{
	return m_Input &= UP;
}

bool InputFilter::Down()
{
	return m_Input &= DOWN;
}

bool InputFilter::Right()
{
	return m_Input &= RIGHT;
}

bool InputFilter::Left()
{
	return m_Input &= LEFT;
}