/*
 *  UIWindow.cpp
 *  Sensor Simulator
 *
 *  Created by Chad on 7/09/09.
 *  Copyright 2009 Mizzou. All rights reserved.
 *
 */

#include "GUIObjects.h"

UIWindow::UIWindow(windowDef& _def, Gosu::Graphics &_graphics, Gosu::Input &_input)
: m_closed(false), m_dragging(false), UIContainer(_graphics, _input)
{
	m_visible = true;
	// Check width for minimum size
	m_Width = _def.width;
	m_Height = _def.height;

	m_X = 50;
	m_Y = 50;

	//m_TextFocus = m_TextBoxes.end();
	//m_MouseInText = m_TextBoxes.end();
	//m_MouseInSlide = m_SliderControls.end();
	//m_MouseInButton = m_Buttons.end();

	Gosu::Bitmap &map = Gosu::quickLoadBitmap(Gosu::resourcePrefix() + L"Images/titlebar.png");
	Gosu::Bitmap titleBar;

	titleBar.resize( m_Width, 25);

	for (int x=0; x < 25; ++x) {
		for (int y=0; y < 25; ++y) {
			titleBar.setPixel( x, y, map.getPixel( x, y) );
			titleBar.setPixel( x + m_Width - 25, y, map.getPixel( x + 50, y) );
		}
	}
	Gosu::Color c;
	for (int y=0; y < 25; ++y) {
		c = map.getPixel( 26, y);
		for (int x=0; x < m_Width - 50; ++x) {
			titleBar.setPixel( x + 25, y, c );
		}
	}

	m_TitleBar.reset( new Gosu::Image(_graphics, titleBar) );

	buttonDef def;
	def.x = m_Width - 25;
	def.y = 5;
	def.width = 20;
	def.height = 20;
	m_CloseButton = createButton( def );
	m_CloseButton->setImage( Gosu::resourcePrefix() + L"Images/closebutton.png");
}

bool UIWindow::isClosed()
{
	return m_closed;
}

bool UIWindow::pointIn(int _x, int _y)
{
	if (_x > m_X && _x < (m_X + m_Width) &&
		_y > m_Y && _y < (m_Y + m_Height)) 
		return true;
	return false;
}

void UIWindow::update()
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

		// Dragging logic
		if (m_dragging) {
			if (m_mouseHeld) {
				m_X = mouseX - m_mouseOffX;
				m_Y = mouseY - m_mouseOffY;
			}else
				m_dragging = false;
		}
		if (mouseX > m_X && mouseX < m_X + m_Width &&
			mouseY > m_Y && mouseY < m_Y + 25) {
			if (m_mouseDown && !m_mouseHeld) {
				m_mouseOffX = mouseX - m_X;
				m_mouseOffY = mouseY - m_Y;
				m_dragging = true;
			}
		}

		// Determine events for text boxes
		bool hitObj = false;
		for (itObj = m_Objects.begin(); itObj != m_Objects.end(); itObj++) {
			(*itObj)->update();
			mouseIn = (*itObj)->pointIn( mouseX - m_X, mouseY - m_Y);

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
		
		// Check status of close button
		if (m_CloseButton->getState() == UIButton::btnPress) {
			m_closed = true;
		}

		
	}
}

void UIWindow::draw(int _x, int _y, int _layer) const
{
	if (m_visible) {
		m_TitleBar->draw(m_X, m_Y, _layer);
		m_Graphics.drawQuad( m_X, m_Y + 25, Gosu::Colors::white, m_X, m_Y + m_Height, Gosu::Colors::white,
			m_X-1 + m_Width, m_Y + m_Height, Gosu::Colors::white, m_X-1 + m_Width, m_Y + 25, Gosu::Colors::white, _layer);

		m_Graphics.drawLine( m_X, m_Y + 25, Gosu::Colors::black, m_X, m_Y + m_Height, Gosu::Colors::black, _layer);
		m_Graphics.drawLine( m_X, m_Y + m_Height, Gosu::Colors::black, m_X-1 + m_Width, m_Y + m_Height, Gosu::Colors::black, _layer);
		m_Graphics.drawLine( m_X-1 + m_Width, m_Y + m_Height, Gosu::Colors::black, m_X-1 + m_Width, m_Y + 25, Gosu::Colors::black, _layer);

		std::list< UIObject* >::const_iterator itObj;
		for (itObj = m_Objects.begin(); itObj != m_Objects.end(); itObj++) {
			(*itObj)->draw(m_X, m_Y, _layer);
		}
	}
}