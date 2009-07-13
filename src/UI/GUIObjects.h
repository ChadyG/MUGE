/*
   GUIObjects.h
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

#ifndef GUIOBJECTS_H
#define GUIOBJECTS_H

#include <Gosu/Gosu.hpp>
#include <list>

#include "../Sprite/Animation.h"

/**
* Abstract base class for all UI Objects
**/

class UIObject
{
public:
	virtual void onMouseIn() = 0;
	virtual void onMouseOut() = 0;
	virtual void onMouseDown() = 0;
	virtual void onMouseUp() = 0;
	virtual void onMouseHeld() = 0;

	void hide() { m_visible = false; }
	void show() { m_visible = true; }
	bool isVisible() { return m_visible; }
	
	virtual void takeFocus() = 0;

	virtual void update() = 0;
	virtual void draw(int _x, int _y,int _layer) const = 0;

	virtual bool pointIn(int _x, int _y) = 0;
protected:
	bool m_visible;
};

/**
 * Text Box
 * builds from Gosu::TextInput
 * takes care of drawing and defining UI callbacks
 **/
struct texDef
{
	int x;
	int y;
	int width;
	int maxsize;
};

class UITextBox : public UIObject
{
public:
	UITextBox(texDef&, Gosu::Graphics&, Gosu::Input&);

	void onMouseIn();
	void onMouseOut();
	void onMouseDown();
	void onMouseUp();
	void onMouseHeld();

	void takeFocus();

	void update();
	void draw(int _x, int _y, int _layer) const;

	bool pointIn(int _x, int _y);

	std::wstring getText();
	void setText(const std::wstring&);

private:

	int m_X, m_Y, m_Length, m_maxSize, m_Height, m_Width;
	int m_Caret, m_Offset, m_Timer;
	bool m_inFocus;
	Gosu::Graphics& m_Graphics;
	Gosu::Input& m_Input;
	Gosu::TextInput* m_TextInput;
	Gosu::Font* m_Text;
};

/**
 * Slider Control
 * allows mouse control of a control on a horizontal bar
 * provides accessors to read from the bar's value
 **/
struct slideDef
{
	int width;
	int x;
	int y;
	int min;
	int max;
	int defValue;
};

class UISliderControl : public UIObject
{
public:
	UISliderControl(slideDef&, Gosu::Graphics&, Gosu::Input&);

	void onMouseIn();
	void onMouseOut();
	void onMouseDown();
	void onMouseUp();
	void onMouseHeld();
	
	void takeFocus();

	void update();
	void draw(int _x, int _y, int _layer) const;

	bool pointIn(int _x, int _y);

	double getValue();

private:

	int m_X, m_Y, m_Min, m_Max, m_Height, m_Width, m_Scale;
	double m_Value;
	Gosu::Graphics& m_Graphics;
	Gosu::Input& m_Input;
	Gosu::Font* m_Text;
};

/**
 * Button
 * encapsulates simple button logic and callbacks
 * uses an image map for button states:
 *	normal, hover, press
 **/
struct buttonDef
{
	int x;
	int y;
	int width;
	int height;
};

class UIButton : public UIObject
{
public:
	
	enum buttonState {
		btnNormal = 0,
		btnHover,
		btnPress
	};

	UIButton(buttonDef&, Gosu::Graphics&, Gosu::Input&);
	
	void setImage(std::wstring &);
	
	void onMouseIn();
	void onMouseOut();
	void onMouseDown();
	void onMouseUp();
	void onMouseHeld();
	
	void takeFocus();
	
	void update();
	void draw(int _x, int _y, int _layer) const;
	
	bool pointIn(int _x, int _y);
	buttonState getState() const;
	
private:
	
	int m_X, m_Y, m_Height, m_Width;
	bool m_hasImage;
	buttonState m_State;
	Gosu::Graphics& m_Graphics;
	Gosu::Input& m_Input;
	Animation m_Image;
};

/**
* Begin Containers
*	Two types: Sheet and Window
*		Sheet is top container for Gosu window, uiwindow is an inner window
*		both manage inner objects, sending event info etc.
*
*	All UIObjects should be created using the provided createX methods on containers.
**/


class UIContainer
{
public:
	UIContainer( Gosu::Graphics&, Gosu::Input& );
	
	void giveFocus() { m_hasFocus = true; }
	void takeFocus() { m_hasFocus = false; }

	UITextBox* createTextBox(texDef&);
	UISliderControl* createSlider(slideDef&);
	UIButton* createButton(buttonDef&);

	void update();
	void draw(int _layer) const;
	
protected:
	std::list< UIObject* > m_Objects;

	std::list< UIObject* >::iterator m_FocusObject;
	std::list< UIObject* >::iterator m_MouseInObject;

	bool m_hasFocus;
	bool m_mouseHeld;
	bool m_mouseDown;
	bool m_mouseUp;

	Gosu::Graphics& m_Graphics;
	Gosu::Input& m_Input;
};

/**
* UIWindow - similar to Sheet, but only takes up part of the screen
* implement to be movable, closable(optional), and minimizable
* 
* Use Gosu::quickLoadBitmap(const std::wstring& filename) and Bitmap
* to dynamically create sized title bar (no resizing)
* Three images, top left corner, top right corner, 1pixel wide fill
*  - could use tiled bitmap
*
* Maybe have UISheet manage windows? (selective input with layers)
*/
struct windowDef
{
	int width;
	int height;
	int layer;
};

class UIWindow : public UIContainer, public UIObject
{
public:
	UIWindow(windowDef& _def, Gosu::Graphics&, Gosu::Input&);

	void onMouseIn() {}
	void onMouseOut() {}
	void onMouseDown() {}
	void onMouseUp() {}
	void onMouseHeld() {}

	void giveFocus() { m_closed = false; }
	void takeFocus() { m_dragging = false; }
	
	bool pointIn(int _x, int _y);
	bool isClosed();

	void update();
	void draw(int _x, int _y, int _layer) const;

private:

	UIButton* m_CloseButton;
	boost::scoped_ptr<Gosu::Image> m_TitleBar;

	bool m_closed;
	bool m_dragging;

	int m_Width;
	int m_Height;
	int m_X;
	int m_Y;
	int m_mouseOffX, m_mouseOffY;

};


class UISheet : public UIContainer
{
public:
	UISheet(Gosu::Graphics&, Gosu::Input&);

	UIWindow* createWindow(windowDef&);

private:

};

#endif