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
#include <map>

#include "../Sprite/Animation.h"

/**
* Abstract base class for all UI Objects
**/

class UIObject
{
public:
	/// Called from UIContainer when mouse enters object (bounding box)
	virtual void onMouseIn() = 0;

	/// Called from UIContainer when mouse leaves object
	virtual void onMouseOut() = 0;

	/// Called from UIContainer when left mouse clicks inside object
	virtual void onMouseDown() = 0;

	/// Called from UIContainer when left mouse button is released inside object
	virtual void onMouseUp() = 0;

	/// Called from UIContainer when left mouse button is held over object
	virtual void onMouseHeld() = 0;

	/// Hide this object
	void hide() { m_visible = false; }

	/// Show this object (default)
	void show() { m_visible = true; }

	/// Say whether object is visible or not
	bool isVisible() { return m_visible; }
	
	/// Used primarily for text boxes (stops collecting keyboard input)
	virtual void takeFocus() = 0;

	/// Propogate clock ticks
	virtual void update() = 0;

	/// Render this object
	virtual void draw(int _x, int _y, int _layer) const = 0;

	/// Containment check (bounding box etc.)
	virtual bool pointIn(int _x, int _y) = 0;

protected:
	/// Whether or not to display this object
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
 * Text Area
 * simiar to text box, but does not take in input
 **/
struct texAreaDef
{
	int x;
	int y;
	int width;
	int justify;
};

class UITextArea : public UIObject
{
public:
	enum Justify {
		Left = 0,
		Center,
		Right
	};

	UITextArea(texAreaDef&, Gosu::Graphics&, Gosu::Input& );

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

	int m_X, m_Y, m_Length, m_maxSize, m_Height, m_Width, m_Offset;
	bool m_inFocus;
	Justify m_Justify;
	Gosu::Graphics& m_Graphics;
	Gosu::Font* m_Font;
	std::wstring m_Text;
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

	int m_X, m_Y, m_Min, m_Max, m_Height, m_Width;
	double m_Scale;
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

	/// Creates a text box using the provided definition and puts 
	/// it into the page designated by string
	UITextBox* createTextBox(texDef &_def);
	UITextBox* createTextBox(texDef &_def, std::string &_page);

	/// Creates a text area using the provided definition and puts 
	/// it into the page designated by string
	UITextArea* createTextArea(texAreaDef &_def);
	UITextArea* createTextArea(texAreaDef &_def, std::string &_page);

	/// Creates a slider control using the provided definition and puts 
	/// it into the page designated by string
	UISliderControl* createSlider(slideDef &_def);
	UISliderControl* createSlider(slideDef &_def, std::string &_page);

	/// Creates a button using the provided definition and puts 
	/// it into the page designated by string
	/// note: buttons must be given an image!
	UIButton* createButton(buttonDef &_def);
	UIButton* createButton(buttonDef &_def, std::string &_page);

	/// Switches context of container to the given page
	/// does not error check (as of yet)
	void setContext( std::string &_page );

	/// Call after update, whether or not a left mouse click hit an UI object
	bool didClick() const;

	void update();
	void draw(int _layer) const;
	
protected:
	typedef std::list< boost::shared_ptr<UIObject> > ObjectList;
	std::map< std::string, ObjectList > m_Pages;
	ObjectList *m_currentPage;

	std::list< boost::shared_ptr<UIObject> >::iterator m_FocusObject;
	std::list< boost::shared_ptr<UIObject> >::iterator m_MouseInObject;

	bool m_hasFocus;
	bool m_mouseHeld;
	bool m_mouseDown;
	bool m_mouseUp;
	bool m_hitObj;

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

	/// Adds the close button to the current page
	/// be careful to switch pages correctly
	void addCloseToPage();

	/// UI Window must send MouseUp actions on hide
	void hide();

	void giveFocus() { m_closed = false; }
	void takeFocus() { m_dragging = false; }
	
	bool pointIn(int _x, int _y);
	bool isClosed();

	void update();
	void draw(int _x, int _y, int _layer) const;

private:

	boost::shared_ptr<UIButton> m_CloseButton;
	//boost::scoped_ptr<Gosu::Image> m_TitleBar;
	Animation m_TitleBar;

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
	UISheet(Gosu::Graphics &_graphics, Gosu::Input &_input);

	/// Creates a window using the provided definition and puts 
	/// it into the page designated by string
	UIWindow* createWindow(windowDef &_def);
	UIWindow* createWindow(windowDef &_def, std::string &_page);

private:

};

#endif