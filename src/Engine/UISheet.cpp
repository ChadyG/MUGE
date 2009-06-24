/*
 *  UISheet.cpp
 *  Sensor Simulator
 *
 *  Created by Chad on 6/09/09.
 *  Copyright 2009 Mizzou. All rights reserved.
 *
 */

#include "GUIObjects.h"

UISheet::UISheet(Gosu::Graphics &_graphics, Gosu::Input &_input)
: m_hasFocus(false), m_Graphics(_graphics), m_Input(_input), m_mouseDown(false), m_mouseHeld(false), m_mouseUp(true)
{
	m_TextFocus = m_TextBoxes.end();
	m_MouseInText = m_TextBoxes.end();
	m_MouseInSlide = m_SliderControls.end();
}

void UISheet::giveFocus()
{
	m_hasFocus = true;
}

void UISheet::takeFocus()
{
	m_hasFocus = false;
}

UITextBox* UISheet::createTextBox(texDef &_def)
{
	m_TextBoxes.push_back( UITextBox(_def, m_Graphics, m_Input) );
	return &(m_TextBoxes.back());
}

UISliderControl* UISheet::createSlider(slideDef &_def)
{
	m_SliderControls.push_back( UISliderControl(_def, m_Graphics, m_Input) );
	return &(m_SliderControls.back());
}

void UISheet::update()
{
	if (m_hasFocus) {
		std::list< UITextBox >::iterator itText;
		std::list< UISliderControl >::iterator itSlide;
		bool mouseIn = false;

		// General mouse state detection
		if (m_Input.down(Gosu::msLeft)) {
			if (m_mouseDown) 
				m_mouseHeld = true;
			m_mouseDown = true;
			m_mouseUp = false;
		}else{
			m_mouseDown = false;
			m_mouseHeld = false;
		}

		// Determine events for text boxes
		bool hitObj = false;
		for (itText = m_TextBoxes.begin(); itText != m_TextBoxes.end(); itText++) {
			itText->update();
			mouseIn = itText->pointIn( m_Input.mouseX(), m_Input.mouseY());

			if (mouseIn & m_mouseDown)
				hitObj = true;

			if (m_MouseInText == itText) {
				if (mouseIn) {
					if (m_mouseHeld) 
						itText->onMouseHeld();
					else
					if (m_mouseDown) {
						if (m_TextFocus != m_TextBoxes.end() && m_TextFocus != itText)
							m_TextFocus->takeFocus();
						m_TextFocus = itText;
						itText->onMouseDown();
					}
				}else{
					itText->onMouseOut();
					m_MouseInText = m_TextBoxes.end();
				}
			}else{
				if (mouseIn) {
					itText->onMouseIn();
					m_MouseInText = itText;
				}
			}
		}

		// Remove focus if clicked outside of boxes
		if (m_mouseDown && m_TextFocus != m_TextBoxes.end() && !hitObj) {
			m_TextFocus->takeFocus();
			m_TextFocus = m_TextBoxes.end();
		}


		// Determine events for slider controls
		for (itSlide = m_SliderControls.begin(); itSlide != m_SliderControls.end(); itSlide++) {
			mouseIn = itSlide->pointIn( m_Input.mouseX(), m_Input.mouseY());

			if (m_MouseInSlide == itSlide) {
				if (mouseIn) {
					if (m_mouseHeld) 
						itSlide->onMouseHeld();
					else
					if (m_mouseDown) {
						m_MouseInSlide = itSlide;
						itSlide->onMouseDown();
					}
				}else{
					itSlide->onMouseOut();
					m_MouseInSlide = m_SliderControls.end();
				}
			}else{
				if (mouseIn) {
					itSlide->onMouseIn();
					m_MouseInSlide = itSlide;
				}
			}
		}

		// Mouse up events for any object (only sent when released over object)
		if (!m_mouseDown && !m_mouseUp) {
			if (m_MouseInText != m_TextBoxes.end()) {
				m_MouseInText->onMouseUp();
				m_MouseInText = m_TextBoxes.end();
			}
			if (m_MouseInSlide != m_SliderControls.end()) {
				m_MouseInSlide->onMouseUp();
				m_MouseInSlide = m_SliderControls.end();
			}
			m_mouseUp = true;
		}

		
	}
}

void UISheet::draw() const
{
	std::list< UITextBox >::const_iterator itText;
	std::list< UISliderControl >::const_iterator itSlide;
	for (itText = m_TextBoxes.begin(); itText != m_TextBoxes.end(); itText++) {
		itText->draw();
	}

	for (itSlide = m_SliderControls.begin(); itSlide != m_SliderControls.end(); itSlide++) {
		itSlide->draw();
	}
}