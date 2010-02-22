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
#include <map>
#include <list>
#include <vector>

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

	InputManager() {}

	void buttonDown(Gosu::Button _button);
	void buttonUp(Gosu::Button _button);

	void update();

	actionState query(std::string _action);

	void createAction(std::string _name);
	void createSequence(std::string _name, int _threshhold);
	void createChord(std::string _name);

	void bindAction(std::string _name, Gosu::ButtonName _button);
	void clearAction(std::string _name);
	void clearActions() { m_actions.clear(); }

	void pushSequence(std::string _name, Gosu::ButtonName _button);
	void clearSequence(std::string _name);
	void clearSequences() { m_sequences.clear(); }

	void addChord(std::string _name, Gosu::ButtonName _button);
	void clearChord(std::string _name);
	void clearChords() { m_chords.clear(); }

	void disable() { m_enabled = false; }
	void enable() { m_enabled = true; }

	/// Static accessor to current render manager
	InputManager* getCurrentContext() { return s_CurrentContext; }

	/// Static setter for current render manager
	static void setCurrentContext(InputManager* _context) { s_CurrentContext = _context; }

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
		sequence* sequence;
		int timer;
		int index;
		bool valid;
	};
	struct chord
	{
		std::list<btnVald> buttons;
		actionState state;
		int timer;
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
};

#endif