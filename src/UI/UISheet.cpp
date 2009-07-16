/*
   UISheet.cpp
   Mizzou Game Engine
 
   Created by Chad on 6/09/09.
  
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

UISheet::UISheet(Gosu::Graphics &_graphics, Gosu::Input &_input)
: UIContainer(_graphics, _input)
{
}

UIWindow* UISheet::createWindow(windowDef &_def)
{
	//m_Objects
	m_currentPage->push_back( new UIWindow(_def, m_Graphics, m_Input) );
	return reinterpret_cast<UIWindow*>(m_currentPage->back());
}

UIWindow* UISheet::createWindow(windowDef &_def, std::string &_page)
{
	//m_Objects
	m_Pages[ _page ].push_back( new UIWindow(_def, m_Graphics, m_Input) );
	return reinterpret_cast<UIWindow*>(m_Pages[ _page ].back());
}