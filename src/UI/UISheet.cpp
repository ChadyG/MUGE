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
: UIContainer(_graphics, _input)
{
	//m_TextFocus = m_TextBoxes.end();
	//m_MouseInText = m_TextBoxes.end();
	//m_MouseInSlide = m_SliderControls.end();
	//m_MouseInButton = m_Buttons.end();
}

UIWindow* UISheet::createWindow(windowDef &_def)
{
	m_Objects.push_back( new UIWindow(_def, m_Graphics, m_Input) );
	return reinterpret_cast<UIWindow*>(m_Objects.back());
}