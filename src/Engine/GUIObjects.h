/*
 *  GUIObjects.h
 *  Sensor Simulator
 *
 *  Created by Chad on 6/09/09.
 *  Copyright 2009 Mizzou. All rights reserved.
 *
 */

#ifndef GUIOBJECTS_H
#define GUIOBJECTS_H

#include <Gosu/Gosu.hpp>
#include <list>


struct texDef
{
	int x;
	int y;
	int width;
	int maxsize;
};

class UITextBox
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
	void draw() const;

	bool pointIn(int, int);

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


struct slideDef
{
	int width;
	int x;
	int y;
	int min;
	int max;
	int defValue;
};

class UISliderControl 
{
public:
	UISliderControl(slideDef&, Gosu::Graphics&, Gosu::Input&);

	void onMouseIn();
	void onMouseOut();
	void onMouseDown();
	void onMouseUp();
	void onMouseHeld();

	void draw() const;

	bool pointIn(int, int);

	double getValue();

private:

	int m_X, m_Y, m_Min, m_Max, m_Height, m_Width, m_Scale;
	double m_Value;
	Gosu::Graphics& m_Graphics;
	Gosu::Input& m_Input;
	Gosu::Font* m_Text;
};


class UISheet
{
public:
	UISheet(Gosu::Graphics&, Gosu::Input&);

	void giveFocus();
	void takeFocus();

	UITextBox* createTextBox(texDef&);
	UISliderControl* createSlider(slideDef&);

	void update();
	void draw() const;

private:
	std::list< UITextBox > m_TextBoxes;
	std::list< UISliderControl > m_SliderControls;

	std::list< UITextBox >::iterator m_MouseInText;
	std::list< UITextBox >::iterator m_TextFocus;
	std::list< UISliderControl >::iterator m_MouseInSlide;

	bool m_hasFocus;
	bool m_mouseHeld;
	bool m_mouseDown;
	bool m_mouseUp;

	Gosu::Graphics& m_Graphics;
	Gosu::Input& m_Input;

};

#endif