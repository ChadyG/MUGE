/*
   UITextBox.cpp
   My Unnamed Game Engine
 
   Created by Chad Godsey on 6/09/09.
  
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

UITextBox::UITextBox(texDef &_def, Gosu::Graphics &_graphics, Gosu::Input &_input)
: m_Graphics(_graphics), m_Input(_input), m_inFocus(false)
{
	m_visible = true;
	m_maxSize = _def.maxsize;
	m_X = _def.x;
	m_Y = _def.y;
	m_Height = 20;
	m_Width = _def.width;

	m_Timer = 50;
	m_Offset = 0;
	m_Caret = 0;

	m_TextInput = new Gosu::TextInput();
	m_Text = new Gosu::Font(m_Graphics, Gosu::defaultFontName(), 20);
}

void UITextBox::onMouseIn()
{
}

void UITextBox::onMouseOut()
{
}

void UITextBox::onMouseDown()
{
	m_inFocus = true;
	m_Input.setTextInput(m_TextInput);
}

void UITextBox::onMouseUp()
{
}

void UITextBox::onMouseHeld()
{
}

void UITextBox::takeFocus()
{
	m_inFocus = false;
	m_Input.setTextInput(NULL);
}

void UITextBox::update()
{
	--m_Timer;
	if (m_Timer <= 0)
		m_Timer = 50;

	m_Caret = m_TextInput->caretPos();
	int chars = m_TextInput->text().length();
	if (m_Offset > chars)
		m_Offset = 0;
	m_Length = chars - m_Offset;
	while ( m_Text->textWidth( m_TextInput->text().substr(m_Offset, m_Length) ) >= m_Width ) {
		--m_Length;
	}
	if (m_Caret > m_maxSize)
		m_TextInput->setText( m_TextInput->text().substr(0, m_maxSize) );

	if (m_Caret < m_Offset) 
		m_Offset = 0;

	while ( m_Text->textWidth( m_TextInput->text().substr(m_Offset, m_Caret) ) >= m_Width ) {
		++m_Offset;
	}
}


void UITextBox::draw(int _x, int _y, int _layer) const
{
	if (m_visible) {
		int x = _x + m_X;
		int y = _y + m_Y;
		m_Graphics.drawQuad( x - 1, y - 1, 0xFF0F276E, x + m_Width + 1, y - 1, 0xFF0F276E,
			x + m_Width + 1, y + m_Height + 1, 0xFF0F276E, x - 1, y + m_Height + 1, 0xFF0F276E, _layer);

		m_Graphics.drawQuad( x, y, 0xFFD3DCF8, x + m_Width, y, 0xFFD3DCF8,
			x + m_Width, y + m_Height, 0xFFD3DCF8, x, y + m_Height, 0xFFD3DCF8, _layer);

		m_Text->draw( m_TextInput->text().substr(m_Offset, m_Length), x, y, _layer, 1.0, 1.0, Gosu::Colors::black);

		if (m_inFocus && m_Timer > 25) {
			double textWidth = m_Text->textWidth( m_TextInput->text().substr(m_Offset, m_Caret - m_Offset) );
			m_Graphics.drawLine(x + textWidth, y, Gosu::Colors::black, x + textWidth, y + m_Height, Gosu::Colors::black, _layer);
		}
	}
}

bool UITextBox::pointIn(int _x, int _y)
{
	if (_x > m_X && _x < (m_X + m_Width) &&
		_y > m_Y && _y < (m_Y + m_Height)) 
		return true;
	return false;
}

std::wstring UITextBox::getText()
{
	return m_TextInput->text();
}

void UITextBox::setText(const std::wstring &_text)
{
	m_TextInput->setText(_text);
	m_Offset = 0;
	m_Length = 0;
}