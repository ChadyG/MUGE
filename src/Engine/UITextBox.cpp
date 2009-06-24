/*
 *  UITextBox.cpp
 *  Sensor Simulator
 *
 *  Created by Chad on 6/09/09.
 *  Copyright 2009 Mizzou. All rights reserved.
 *
 */

#include "GUIObjects.h"

UITextBox::UITextBox(texDef &_def, Gosu::Graphics &_graphics, Gosu::Input &_input)
: m_Graphics(_graphics), m_Input(_input), m_inFocus(false)
{
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


void UITextBox::draw() const
{
	m_Graphics.drawQuad( m_X - 1, m_Y - 1, 0xFF0F276E, m_X + m_Width + 1, m_Y - 1, 0xFF0F276E,
		m_X + m_Width + 1, m_Y + m_Height + 1, 0xFF0F276E, m_X - 1, m_Y + m_Height + 1, 0xFF0F276E, 5);

	m_Graphics.drawQuad( m_X, m_Y, 0xFFD3DCF8, m_X + m_Width, m_Y, 0xFFD3DCF8,
		m_X + m_Width, m_Y + m_Height, 0xFFD3DCF8, m_X, m_Y + m_Height, 0xFFD3DCF8, 5);

	m_Text->draw( m_TextInput->text().substr(m_Offset, m_Length), m_X, m_Y, 5, 1.0, 1.0, Gosu::Colors::black);

	if (m_inFocus && m_Timer > 25) {
		double textWidth = m_Text->textWidth( m_TextInput->text().substr(m_Offset, m_Caret - m_Offset) );
		m_Graphics.drawLine(m_X + textWidth, m_Y, Gosu::Colors::black, m_X + textWidth, m_Y + m_Height, Gosu::Colors::black, 5);
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
}