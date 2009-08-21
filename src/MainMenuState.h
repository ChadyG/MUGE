/*
   MainMenuState.h
   Mizzou Game Engine
 
   Created by Chad Godsey on 11/12/08.
  
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
 
#include "Global.h"

#include <list>
#include "Core/GameState.h"
#include "Physics/ContactListener.h"

class MUGE;
class UIContainer;
class UIObject;
class UIScreen;
class UIWindow;
class UIButton;
class UITextArea;
class UITextBox;

/**
 * State for main menu
 * central hub to move into other major game states
 *
 * This is a good place to toy around with things, as it is the first
 * piece of the game the player can interact with.
 */
class MainMenuState : public GameState
{
public:
	MainMenuState( std::wstring _config );
	
	void init( MUGE* _engine );
	void cleanup();
	
	void pause();
	void resume();
	
	void update();
	void draw() const;

	/// Screen to World coordinate transform
	/// wrapper function
	b2Vec2 worldToScreen( b2Vec2 _world, Gosu::ZPos _layer );
	
	void CursorUp();
	void CursorDown();
	
private:
	
	void evalJSON(json::grammar<char>::array _array, UIScreen *_container);
	void evalButton(json::grammar<char>::array::const_iterator _it, UIContainer *_container);
	void evalTextBox(json::grammar<char>::array::const_iterator _it, UIContainer *_container);
	void evalTextArea(json::grammar<char>::array::const_iterator _it, UIContainer *_container);
	void evalWindow(json::grammar<char>::array::const_iterator _it, UIScreen *_container);

	std::wstring m_ConfigFile;
	boost::scoped_ptr<JSONFile> m_jFile;

	boost::scoped_ptr<Gosu::Image> m_MenuScreen;
	boost::scoped_ptr<Gosu::Image> m_Cursor;
	boost::scoped_ptr<Gosu::Image> m_MouseCursor;
	boost::scoped_ptr<Gosu::Sample> m_CursorMove;
	boost::scoped_ptr<Gosu::Sample> m_CursorSelect;
	boost::scoped_ptr<Gosu::Sample> m_PhysHit;
	boost::scoped_ptr<Gosu::Sample> m_PhysBigHit;
	std::vector< Gosu::Image* > m_LettersV;
	
	int m_CursorPos, m_lastCursorPos;
	b2Vec2 m_mousePos;
	bool m_Held, m_msLeftHeld;
	
	double m_width, m_height, m_units;
	int m_Iterations;
	float m_TimeStep;
	
	boost::scoped_ptr< b2World > m_Worldp;
	MenuListener m_ContactListener;
	std::list< b2Body* > m_Bodies;
	b2MouseJoint* m_mouseJoint;
	
	
	UIScreen *m_UI;
	std::list< UIObject* > m_UIObjects;
	std::map< std::string, UIObject* > m_UINames;
	
};