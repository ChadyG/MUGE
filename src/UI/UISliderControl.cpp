/*
   UISliderControl.cpp
   Mizzou Game Engine
 
   Created by Chad Godsey on 6/09/09.
  
 Copyright 2009 Mizzou Game Design.

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
 */

#include "boost/lexical_cast.hpp"
#include "GUIObjects.h"

UISliderControl::UISliderControl(slideDef &_def, Gosu::Graphics &_graphics, Gosu::Input &_input)
: m_Graphics(_graphics), m_Input(_input)
{
	m_visible = true;
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

void UISliderControl::takeFocus()
{
}

void UISliderControl::update()
{
}

void UISliderControl::draw(int _x, int _y, int _layer) const
{
	if (m_visible) {
		int x = _x + m_X;
		int y = _y + m_Y;
		m_Graphics.drawQuad( x - 12, y - 2, 0xFF0F276E, x + m_Width + 12, y - 2, 0xFF0F276E, 
			x + m_Width + 12, y + m_Height + 2, 0xFF0F276E, x - 12, y + m_Height + 2, 0xFF0F276E, _layer);
		
		m_Graphics.drawQuad( x - 10, y, 0xFFD3DCF8, x + m_Width + 10, y, 0xFFD3DCF8, 
			x + m_Width + 10, y + m_Height, 0xFFD3DCF8, x - 10, y + m_Height, 0xFFD3DCF8, _layer);


		m_Graphics.drawTriangle( x + m_Value * m_Scale - m_Min * m_Scale, y + m_Height - 2, 0xFFD3DCF8,
			x + m_Value * m_Scale - m_Min * m_Scale + 12, y + m_Height + 16, 0xFF0F276E,
			x + m_Value * m_Scale - m_Min * m_Scale - 12, y + m_Height + 16, 0xFF0F276E, _layer);

		m_Graphics.drawTriangle( x + m_Value * m_Scale - m_Min * m_Scale, y + m_Height, 0xFF0F276E,
			x + m_Value * m_Scale - m_Min * m_Scale + 10, y + m_Height + 15, 0xFFD3DCF8,
			x + m_Value * m_Scale - m_Min * m_Scale - 10, y + m_Height + 15, 0xFFD3DCF8, _layer);

		m_Text->drawRel( L"0", x + m_Width/2, y, _layer, 0.5, 0.0, 1.0, 1.0, Gosu::Colors::black);
		m_Text->drawRel( boost::lexical_cast<std::wstring>(m_Min), x, y, _layer, 0.5, 0.0, 1.0, 1.0, Gosu::Colors::black);
		m_Text->drawRel( boost::lexical_cast<std::wstring>(m_Max), x + m_Width, y, _layer, 0.5, 0.0, 1.0, 1.0, Gosu::Colors::black);
	}
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