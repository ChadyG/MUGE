/*
	Core.cpp
	My Unnamed Game Engine
 
	Created by Chad Godsey on 12/12/08.
	Copyright 2009 BlitThis! studios.

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
#include "Core.h"
#include "GameState.h"

Core* Core::s_CurrentContext;

/**
* Entry point to the engine
* manages states, and sends Gosu callbacks
* to the current state
*/
Core::Core(int _width, int _height, bool _fullscreen, double _updateInterval)
: Gosu::Window(_width, _height, _fullscreen, _updateInterval)
{
	InputManager::setCurrentContext( &m_inputManager );
	m_inputManager.enable();

	m_listeners = &m_inputManager;

	m_inputManager.createAction("MouseDown");
	m_inputManager.bindAction("MouseDown", Gosu::msLeft);
	
	Json::Value jVal;
	jVal["Dimension"][0u] = Json::Value(_width);
	jVal["Dimension"][1u] = Json::Value(_height);
	jVal["Fullscreen"] = Json::Value(_fullscreen);
	jVal["UpdateInterval"] = Json::Value(_updateInterval);
	m_data["Window"] = jVal;

	//m_font = new Gosu::Font(graphics(), Gosu::defaultFontName(), 10);

	m_FPS = (int)(1000.0/_updateInterval);
	m_curFPS = 1;
	m_curTicks = 0;
	m_lastSecond = Gosu::milliseconds()/1000;
	m_stackDirty = false;

	m_UI = new UISheet(graphics(), input());
	m_UI->giveFocus();

	// Create the movable window container
	windowDef wDef;
	wDef.height = 100;
	wDef.width = 250;
	m_UIWin = m_UI->createWindow(wDef);
	m_UIWin->giveFocus();
	m_UIWin->hide();

	// Create text areas for the window (Wall editing)
	texAreaDef tADef;
	tADef.justify = UITextArea::Center;
	tADef.width = -1;
	tADef.x = 125;
	tADef.y = 30;
	m_UIDialogue = m_UIWin->createTextArea( tADef );
	m_UIDialogue->setText( L"Would you like to quit?" );
	// Create Cancel/Confirm buttons
	buttonDef bDef;
	bDef.height = 32;
	bDef.width = 70;
	bDef.x = 40;
	bDef.y = 50;
	m_UIConfirm = m_UIWin->createButton( bDef );
	m_UIConfirm->setImage( Gosu::resourcePrefix() + L"Images/ConfirmButton.png" );
	bDef.x = 140;
	m_UICancel = m_UIWin->createButton( bDef );
	m_UICancel->setImage( Gosu::resourcePrefix() + L"Images/CancelButton.png" );

	m_inDialog = false;

	m_Cursor.reset(new Gosu::Image(graphics(), Gosu::resourcePrefix() + L"Images/cursor.png", false));
	m_showCursor = false;
	m_cursorSetting = false;
}

void Core::buttonDown(Gosu::Button _button)
{
	//Pass down to listeners
	for (InputListener *il = m_listeners; il != NULL; il = il->m_next) {
		il->buttonDown(_button);
	}
}

void Core::buttonUp(Gosu::Button _button)
{
	//Pass down to listeners
	for (InputListener *il = m_listeners; il != NULL; il = il->m_next) {
		il->buttonUp(_button);
	}
}

void Core::registerListener(InputListener* _listen)
{
	_listen->m_next = m_listeners;
	if (m_listeners && m_listeners->m_prev)
		m_listeners->m_prev = _listen;
	m_listeners = _listen;
}

void Core::removeListener(InputListener* _listen)
{
	for (InputListener *il = m_listeners; il != NULL; il = il->m_next) {
		if (il == _listen) {
			if (il->m_prev)
				il->m_prev->m_next = il->m_next;
			if(il->m_next) 
				il->m_next->m_prev = il->m_prev;
			return;
		}
	}
}

void Core::changeState( GameState *state )
{
	// Deferred state changes
	m_stackDirty = true;
	// Set current state for deletion
	if (!m_States.empty()) {
		m_States.top()->setFocus(false);
		m_States.top()->setDirty();
	}
	m_NextStates.push( boost::shared_ptr<GameState>(state) );
}

void Core::pushState( GameState *state )
{
	m_stackDirty = true;
	// take focus from current state and pause (not deleting)
	if (!m_States.empty()) {
		m_States.top()->setFocus(false);
		m_States.top()->setPause(true);
		m_States.top()->pause();
	}
	m_NextStates.push( boost::shared_ptr<GameState>(state) );
}

void Core::popState()
{
	if (m_States.empty()) 
		close();
	m_stackDirty = true;
	m_States.top()->setFocus(false);
	m_States.top()->setDirty();
}

void Core::update()
{
	// Keep track of FPS for perf testing
	m_curTicks++;
	if (Gosu::milliseconds()/1000 != m_lastSecond) {
		m_curFPS = m_curTicks;
		m_curTicks = 0;
		m_lastSecond = Gosu::milliseconds()/1000;
	}
	
	// Perform deferred state changes 
	//this needs to happen outside of any actual state callbacks
	if (m_stackDirty) {
		// Look for pending deletions
		while (!m_States.empty() && m_States.top()->dirty()) {
			m_States.top()->cleanup();
			m_States.pop();
		}
		// Transfer pending states to current stack
		while (!m_NextStates.empty()) {
			m_States.push(m_NextStates.front());
			m_NextStates.front()->init();
			m_NextStates.pop();
		}
		// Give focus to current state and unpause if paused 
		if (!m_States.empty()) {
			m_States.top()->setFocus(true);
			if (m_States.top()->isPaused()) {
				m_States.top()->setPause(false);
				m_States.top()->resume();
			}
		}
		m_stackDirty = false;
	}
	
	//start close dialog
	if (m_inputManager.query("Game.Quit") == InputManager::actnBegin) {
		m_inDialog = true;
		m_showCursor = true;
		m_UIWin->show();
		m_UIWin->giveFocus();
	}

	if (m_UIConfirm->getState() == UIButton::btnPress){
		close();
	}

	if (m_UIWin->isClosed() && m_UIWin->isVisible()) {
		m_UIWin->hide();
		m_showCursor = m_cursorSetting;
		m_inDialog = false;
	}
	if (m_UICancel->getState() == UIButton::btnPress){
		m_inDialog = false;
		m_showCursor = m_cursorSetting;
		m_UIWin->hide();
	}
	m_UI->update();
	// End Close dialog

	if (!m_inDialog) {
		if (!m_States.empty()) {
			m_States.top()->update();
		}else
			close();
	}

	// Done last as to keep queries valid for each game tick
	m_inputManager.update();
}

void Core::draw()
{
	//m_font->draw(m_message, 4, 4, 15);
	m_UI->draw(20);
	if (m_showCursor)
		m_Cursor->draw(input().mouseX() - 7, input().mouseY(), 21);

	if (!m_States.empty())
		m_States.top()->draw();
}

int Core::getFPS()
{
	return m_curFPS;
}
