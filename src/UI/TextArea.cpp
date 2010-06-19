/*
   UITextArea.cpp
   My Unnamed Game Engine
 
   Created by Chad Godsey on 7/14/09.
  
 Copyright 2009 BlitThis! studios.

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

#include "GUIObjects.h"

UITextArea::UITextArea(texAreaDef &_def, Gosu::Graphics &_graphics, Gosu::Input &_input )
: m_Graphics(_graphics), m_inFocus(false), m_Offset(0)
{
	m_visible = true;
	m_X = _def.x;
	m_Y = _def.y;
	m_Height = 20;
	m_Width = _def.width;
	m_Justify = (UITextArea::Justify)(_def.justify);

	m_Font = new Gosu::Font(m_Graphics, Gosu::defaultFontName(), 20);
}

void UITextArea::onMouseIn()
{
}

void UITextArea::onMouseOut()
{
}

void UITextArea::onMouseDown()
{
	m_inFocus = true;
}

void UITextArea::onMouseUp()
{
}

void UITextArea::onMouseHeld()
{
}

void UITextArea::takeFocus()
{
	m_inFocus = false;
}

void UITextArea::update()
{
	if (m_Justify == Left) {
		m_Offset = 0;
	}
	if (m_Justify == Center) {
		int width = m_Font->textWidth( m_Text );
		m_Offset = m_Width/2 - width/2;
	}
	if (m_Justify == Right) {
		int width = m_Font->textWidth( m_Text );
		m_Offset = m_Width - width;
	}
}


void UITextArea::draw(int _x, int _y, int _layer) const
{
	if (m_visible) {
		int x = _x + m_X;
		int y = _y + m_Y;
		m_Graphics.drawQuad( x - 1, y - 1, 0xFFbfd4f9, x + m_Width + 1, y - 1, 0xFFbfd4f9,
			x + m_Width + 1, y + m_Height + 1, 0xFFbfd4f9, x - 1, y + m_Height + 1, 0xFFbfd4f9, _layer);

		m_Graphics.drawQuad( x, y, 0xFFe6eefd, x + m_Width, y, 0xFFe6eefd,
			x + m_Width, y + m_Height, 0xFFe6eefd, x, y + m_Height, 0xFFe6eefd, _layer);

		m_Font->draw( m_Text, x + m_Offset, y, _layer, 1.0, 1.0, Gosu::Colors::black);

	}
}

bool UITextArea::pointIn(int _x, int _y)
{
	if (_x > m_X && _x < (m_X + m_Width) &&
		_y > m_Y && _y < (m_Y + m_Height)) 
		return true;
	return false;
}

std::wstring UITextArea::getText()
{
	return m_Text;
}

void UITextArea::setText(const std::wstring &_text)
{
	m_Text = _text;
}