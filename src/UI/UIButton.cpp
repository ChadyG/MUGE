/*
 *  UIButton.cpp
 *  Destructible
 *
 *  Created by Chad on 7/1/09.
 *  Copyright 2009 University of Missouri Columbia. All rights reserved.
 *
 */

#include "GUIObjects.h"

UIButton::UIButton(buttonDef &_def, Gosu::Graphics &_graphics, Gosu::Input &_input)
: m_Graphics(_graphics), m_Input(_input)
{
	m_visible = true;
	m_X = _def.x;
	m_Y = _def.y;
	m_Height = _def.height;
	m_Width = _def.width;
	m_State = btnNormal;
	m_hasImage = false;
}

void UIButton::setImage(std::wstring &_file)
{
	m_hasImage = true;
	m_Image.setImage(m_Graphics, _file, m_Width, m_Height);
}

void UIButton::onMouseIn()
{
	m_State = btnHover;
}

void UIButton::onMouseOut()
{
	m_State = btnNormal;
}

void UIButton::onMouseDown()
{
	m_State = btnPress;
}

void UIButton::onMouseUp()
{
	m_State = btnNormal;
}

void UIButton::onMouseHeld()
{
	m_State = btnPress;
}

void UIButton::takeFocus()
{
}

void UIButton::update()
{
}

void UIButton::draw(int _x, int _y, int _layer) const
{
	if (m_visible) {
		int x = _x + m_X;
		int y = _y + m_Y;
		if (m_hasImage) {
			m_Image.getFrame(m_State).draw( x, y, _layer);	
		}else{
			m_Graphics.drawQuad( x - 1, m_Y - 1, 0xFF0F276E, x + m_Width + 1, y - 1, 0xFF0F276E,
								x + m_Width + 1, y + m_Height + 1, 0xFF0F276E, x - 1, y + m_Height + 1, 0xFF0F276E, _layer);
			
			m_Graphics.drawQuad( x, y, 0xFFD3DCF8, y + m_Width, y, 0xFFD3DCF8,
								x + m_Width, y + m_Height, 0xFFD3DCF8, x, y + m_Height, 0xFFD3DCF8, _layer);
		}
	}
}

bool UIButton::pointIn(int _x, int _y)
{
	if (_x > m_X && _x < (m_X + m_Width) &&
		_y > m_Y && _y < (m_Y + m_Height)) 
		return true;
	return false;
}

UIButton::buttonState UIButton::getState() const
{
	return m_State;
}
