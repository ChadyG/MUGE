/*
   UIContainer.cpp
   Mizzou Game Engine
 
   Created by Chad on 7/09/09.
  
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

#include "GUIObjects.h"

UIContainer::UIContainer(Gosu::Graphics &_graphics, Gosu::Input &_input)
: m_hasFocus(false), m_Graphics(_graphics), m_Input(_input), m_mouseDown(false), m_mouseHeld(false), m_mouseUp(true)
{
	m_FocusObject = m_Objects.end();
	m_MouseInObject = m_Objects.end();
}

UITextBox* UIContainer::createTextBox(texDef &_def)
{
	m_Objects.push_back( new UITextBox(_def, m_Graphics, m_Input) );
	return reinterpret_cast<UITextBox*>(m_Objects.back());
}

UISliderControl* UIContainer::createSlider(slideDef &_def)
{
	m_Objects.push_back( new UISliderControl(_def, m_Graphics, m_Input) );
	return reinterpret_cast<UISliderControl*>(m_Objects.back());
}

UIButton* UIContainer::createButton(buttonDef &_def)
{
	m_Objects.push_back( new UIButton(_def, m_Graphics, m_Input) );
	return reinterpret_cast<UIButton*>(m_Objects.back());
}

void UIContainer::update()
{
	if (m_hasFocus) {
		std::list< UIObject* >::iterator itObj;
		bool mouseIn = false;
		int mouseX = m_Input.mouseX(), mouseY = m_Input.mouseY();

		// General mouse state detection
		if (m_Input.down(Gosu::msLeft)) {
			if (m_mouseDown) {
				m_mouseHeld = true;
				m_mouseDown = false;
			}else
				m_mouseDown = true;
			
			m_mouseUp = false;
		}else{
			m_mouseDown = false;
			m_mouseHeld = false;
		}

		// Determine events for all objects
		// FIXME: this probably has fighting in overlapped objects
		bool hitObj = false;
		for (itObj = m_Objects.begin(); itObj != m_Objects.end(); itObj++) {
			(*itObj)->update();
			mouseIn = (*itObj)->pointIn( mouseX, mouseY);

			if (mouseIn & m_mouseDown)
				hitObj = true;

			if (m_MouseInObject == itObj) {
				if (mouseIn) {
					if (m_mouseHeld) 
						(*itObj)->onMouseHeld();
					
					if (m_mouseDown) {
						if (m_FocusObject != m_Objects.end() && m_FocusObject != itObj)
							(*m_FocusObject)->takeFocus();
						m_FocusObject = itObj;
						(*itObj)->onMouseDown();
					}
				}else{
					(*itObj)->onMouseOut();
					m_MouseInObject = m_Objects.end();
				}
			}else{
				if (mouseIn) {
					(*itObj)->onMouseIn();
					m_MouseInObject = itObj;
				}
			}
		}

		// Remove focus if clicked outside of boxes
		if (m_mouseDown && m_FocusObject != m_Objects.end() && !hitObj) {
			(*m_FocusObject)->takeFocus();
			m_FocusObject = m_Objects.end();
		}



		// Mouse up events for any object (only sent when released over object)
		if (!m_mouseHeld && !m_mouseUp) {
			if (m_MouseInObject != m_Objects.end()) {
				(*m_MouseInObject)->onMouseUp();
				m_MouseInObject = m_Objects.end();
			}
			m_mouseUp = true;
		}
		
	}
}

void UIContainer::draw(int _layer) const
{
	std::list< UIObject* >::const_iterator itObj;
	for (itObj = m_Objects.begin(); itObj != m_Objects.end(); itObj++) {
		(*itObj)->draw(0, 0, _layer);
	}
}