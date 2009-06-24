/*
 *  UISliderControl.cpp
 *  Sensor Simulator
 *
 *  Created by Chad on 6/09/09.
 *  Copyright 2009 Mizzou. All rights reserved.
 *
 */

#include "boost/lexical_cast.hpp"
#include "GUIObjects.h"

UISliderControl::UISliderControl(slideDef &_def, Gosu::Graphics &_graphics, Gosu::Input &_input)
: m_Graphics(_graphics), m_Input(_input)
{
	m_Width = _def.width;
	m_Height = 10;
	m_Min = _def.min;
	m_Max = _def.max;
	m_Value = _def.defValue;
	m_X = _def.x;
	m_Y = _def.y;
	m_Scale = m_Width / (m_Max - m_Min);

	m_Text = new Gosu::Font(m_Graphics, Gosu::defaultFontName(), 10);
}

void UISliderControl::onMouseIn()
{

}

void UISliderControl::onMouseOut()
{
}

void UISliderControl::onMouseDown()
{

}

void UISliderControl::onMouseUp()
{
}

void UISliderControl::onMouseHeld()
{
	m_Value = Gosu::clamp<double>((m_Input.mouseX() - m_X) / m_Scale + m_Min, m_Min, m_Max);
	m_Input.setMousePosition( m_Input.mouseX(), m_Y + m_Height + 5);
}

void UISliderControl::draw() const
{
	m_Graphics.drawQuad( m_X - 12, m_Y - 2, 0xFF0F276E, m_X + m_Width + 12, m_Y - 2, 0xFF0F276E, 
		m_X + m_Width + 12, m_Y + m_Height + 2, 0xFF0F276E, m_X - 12, m_Y + m_Height + 2, 0xFF0F276E, 5);
	
	m_Graphics.drawQuad( m_X - 10, m_Y, 0xFFD3DCF8, m_X + m_Width + 10, m_Y, 0xFFD3DCF8, 
		m_X + m_Width + 10, m_Y + m_Height, 0xFFD3DCF8, m_X - 10, m_Y + m_Height, 0xFFD3DCF8, 5);


	m_Graphics.drawTriangle( m_X + m_Value * m_Scale - m_Min * m_Scale, m_Y + m_Height - 2, 0xFFD3DCF8,
		m_X + m_Value * m_Scale - m_Min * m_Scale + 12, m_Y + m_Height + 16, 0xFF0F276E,
		m_X + m_Value * m_Scale - m_Min * m_Scale - 12, m_Y + m_Height + 16, 0xFF0F276E, 5);

	m_Graphics.drawTriangle( m_X + m_Value * m_Scale - m_Min * m_Scale, m_Y + m_Height, 0xFF0F276E,
		m_X + m_Value * m_Scale - m_Min * m_Scale + 10, m_Y + m_Height + 15, 0xFFD3DCF8,
		m_X + m_Value * m_Scale - m_Min * m_Scale - 10, m_Y + m_Height + 15, 0xFFD3DCF8, 5);

	m_Text->drawRel( L"0", m_X + m_Width/2, m_Y, 5, 0.5, 0.0, 1.0, 1.0, Gosu::Colors::black);
	m_Text->drawRel( boost::lexical_cast<std::wstring>(m_Min), m_X, m_Y, 5, 0.5, 0.0, 1.0, 1.0, Gosu::Colors::black);
	m_Text->drawRel( boost::lexical_cast<std::wstring>(m_Max), m_X + m_Width, m_Y, 5, 0.5, 0.0, 1.0, 1.0, Gosu::Colors::black);
}

bool UISliderControl::pointIn(int _x, int _y)
{
	int x = m_X + m_Value * m_Scale - m_Min * m_Scale;
	if (_x > (x - 10) && _x < (x + 10) &&
		_y > (m_Y + m_Height) && _y < (m_Y + m_Height + 15)) 
		return true;
	return false;
}

double UISliderControl::getValue()
{
	return m_Value;
}