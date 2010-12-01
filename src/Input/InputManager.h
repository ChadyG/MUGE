/*
   InputManager.h
   My Unnamed Game Engine
 
   Created by Chad Godsey on 2/20/10.
  
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

#ifndef INMAN_H
#define INMAN_H

#include <Gosu/Gosu.hpp>
#include <Box2D/Box2D.h>
#include <json/json.h>
#include <map>
#include <list>
#include <vector>

//TODO: do I want to use this as the prototype for listening to input events on core?
class InputListener
{
public:
	InputListener() : m_prev(0), m_next(0) {}

	virtual void buttonDown(Gosu::Button _button) = 0;
	virtual void buttonUp(Gosu::Button _button) = 0;
private:
	friend class InputManager;
	InputListener *m_prev, *m_next;
};

/**
*	Intended services 
*		* Button mapping to game actions
*		* Detection of several types of high level input
*			* Chords
*			* Sequences
*			* Simple Actions
*		* Allow disabling of input
*		* Provide a query based interface for action states
*/
class InputManager
{
public:
	enum actionState {
		actnInvalid = -1,
		actnIdle = 0,
		actnProcess,
		actnBegin,
		actnActive,
		actnFinish
	};

	InputManager() : m_listeners(0) {}
	~InputManager();

	/// Initialize inputs via json config
	/// @param _jval json array of configurations
	void initWith(Json::Value _jval);

	/// Set parameters for window
	/// @param _w screen width
	/// @param _h screen height
	/// @param _s screen scaling factor - pixels per world unit
	void setScreen( int _w, int _h, int _s)
	{
		m_screenW = _w; m_screenH = _h; m_screenScale = _s;
	};

	/// Update camera parameters
	/// @param _x world space X coordinate of camera 
	/// @param _y world space Y coordinate of camera 
	/// @param _zoom zoom factor of camera 
	/// @param _rot angle of camera (degrees)
	/// This should be called on each game tick
	void setCamera( double _x, double _y, double _zoom = 1.f, double _rot = 0.f )
	{
		m_camX = _x; m_camY = _y, m_camZoom = _zoom; m_camRot = _rot;
	};

	/// Send button down callback from Gosu here
	void buttonDown(Gosu::Button _button);
	/// Send button up callback from Gosu here
	void buttonUp(Gosu::Button _button);

	/// Update the system
	/// call this once per tick
	void update();

	/// Register an input listener
	void registerListener(InputListener* _listen);

	/// Remove a registered listener from this manager
	void removeListener(InputListener* _listen);

	/// Reset all inputs to idle
	void resetInputs();

	/// Query the state of an action, sequence or chord
	/// if duplicate names exist, the precedence is action, sequence then chord for queries
	actionState query(std::string _action);

	/// Get the world coordinates of the mouse
	b2Vec2 getMouse();

	/// Create an Action with the given name
	void createAction(std::string _name);
	/// Create a Sequence with the given name and threshold
	/// @param _threshold the number of legal ticks between button presses
	void createSequence(std::string _name, int _threshold);
	/// Create a Chord with the given name
	void createChord(std::string _name, int _threshold = 10);

	/// Bind the button to the action
	void bindAction(std::string _name, Gosu::ButtonName _button);
	/// Clear all buttons from the action
	void clearAction(std::string _name);
	/// Remove all actions
	void clearActions() { m_actions.clear(); }

	/// Add a button to a sequence
	void pushSequence(std::string _name, Gosu::ButtonName _button);
	/// Remove all buttons from a sequence
	void clearSequence(std::string _name);
	/// Remove all sequences
	void clearSequences() { m_sequences.clear(); }

	/// Add a button to a chord
	void addChord(std::string _name, Gosu::ButtonName _button);
	/// Clear buttons from a chord
	void clearChord(std::string _name);
	/// Remove all chords
	void clearChords() { m_chords.clear(); }

	/// Temporarily disable input collection
	/// Causes all queries to return Idle state
	void disable() { m_enabled = false; }
	/// Re-enable input collection
	void enable() { m_enabled = true; }

	/// Static accessor to current render manager
	static InputManager* getCurrentContext() { return s_CurrentContext; }

	/// Static setter for current render manager
	static void setCurrentContext(InputManager* _context) { s_CurrentContext = _context; }

	/// Helper function to map string names onto Gosu::ButtonNames
	static Gosu::ButtonName strToButton(std::string _name);

private:
	static InputManager* s_CurrentContext;

	struct btnVald
	{
		Gosu::ButtonName button;
		bool valid;
	};
	struct sequence
	{
		std::vector<Gosu::ButtonName> buttons;
		int threshold;
		actionState state;
	};
	struct sequencebuff
	{
		InputManager::sequence* sequence;
		unsigned long timer;
		int index;
		bool valid;
	};
	struct chord
	{
		std::list<btnVald> buttons;
		actionState state;
		unsigned long timer;
		int threshold;
	};
	struct action
	{
		std::list<Gosu::ButtonName> buttons;
		actionState state;
	};
	std::map<std::string, sequence> m_sequences;
	std::list<sequencebuff> m_seqbuff;
	std::map<std::string, chord> m_chords;
	std::map<std::string, action> m_actions;

	bool m_enabled;

	double m_camX, m_camY, m_camZoom, m_camRot;
	int m_screenW, m_screenH, m_screenScale;

	InputListener* m_listeners;
};

#endif