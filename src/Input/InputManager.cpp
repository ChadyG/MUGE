/*
   InputManager.cpp
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

#include "InputManager.h"

InputManager* InputManager::s_CurrentContext;

void InputManager::buttonDown(Gosu::Button _button)
{
	//Update chords
	std::map<std::string, chord>::iterator cit;
	for (cit = m_chords.begin(); cit != m_chords.end(); cit++) {
		std::list<btnVald>::iterator bit;
		for (bit = cit->second.buttons.begin(); bit != cit->second.buttons.end(); bit++) {
			if (bit->button == _button.getId()) {
				bit->valid = true;
				if (cit->second.state == actnIdle) {
					cit->second.timer = 10;
					cit->second.state = actnProcess;
				}else{
					//Check Start state
					std::list<btnVald>::iterator bit2;
					bool valid = true;
					for (bit2 = cit->second.buttons.begin(); bit2 != cit->second.buttons.end(); bit2++) {
						if (bit2->valid == false)
							valid = false;
					}
					if (valid)
						cit->second.state = actnBegin;
				}
			}
		}
	}
	//Update sequences with start state
	std::list<sequencebuff>::iterator sit;
	for (sit = m_seqbuff.begin();sit != m_seqbuff.end(); sit++) {
		if (sit->index == (sit->sequence->buttons.size()-1) &&_button.getId() == sit->sequence->buttons[sit->index]) {
			sit->sequence->state = actnBegin;
		}
	}
	//Update actions
	std::map<std::string, action>::iterator ait;
	for (ait = m_actions.begin();ait != m_actions.end(); ait++) {
		std::list<Gosu::ButtonName>::iterator bit;
		for (bit = ait->second.buttons.begin(); bit != ait->second.buttons.end(); bit++) {
			if ((*bit) == _button.getId()) {
				ait->second.state = actnBegin;
			}
		}
	}
}
void InputManager::buttonUp(Gosu::Button _button)
{
	//Invalidate current sequences
	//Update sequences
	std::list<sequencebuff>::iterator sit = m_seqbuff.begin();
	while (sit != m_seqbuff.end()) {
		if (_button.getId() != sit->sequence->buttons[sit->index]) {
			if (sit->sequence->state != actnActive) {
				sit->sequence->state = actnIdle;
				sit = m_seqbuff.erase(sit);
				continue;
			}
		}else{
			sit->timer = 10;
			sit->index++;
			if (sit->index == sit->sequence->buttons.size()) {
				sit->sequence->state = actnFinish;
				sit = m_seqbuff.erase(sit);
				continue;
			}
		}
		sit++;
	}
	//Start new sequences
	std::map<std::string, sequence>::iterator ssit;
	for (ssit = m_sequences.begin(); ssit != m_sequences.end(); ssit++) {
		if (_button.getId() == ssit->second.buttons.front()) {
			sequencebuff sb;
			sb.index = 1;
			sb.sequence = &(ssit->second);
			sb.timer = 10;
			sb.valid = true;
			m_seqbuff.push_back(sb);
		}
	}
	//Invalidate chords
	std::map<std::string, chord>::iterator cit;
	for (cit = m_chords.begin(); cit != m_chords.end(); cit++) {
		std::list<btnVald>::iterator bit;
		for (bit = cit->second.buttons.begin(); bit != cit->second.buttons.end(); bit++) {
			if (bit->button == _button.getId()) {
				if(cit->second.state == actnProcess)
					cit->second.state = actnIdle;
				else
					cit->second.state = actnFinish;
				cit->second.timer = -1;
				std::list<btnVald>::iterator bit2;
				for (bit2 = cit->second.buttons.begin(); bit2 != cit->second.buttons.end(); bit2++) {
					bit2->valid = false;
				}
			}
		}
	}
	//Update actions
	std::map<std::string, action>::iterator ait;
	for (ait = m_actions.begin();ait != m_actions.end(); ait++) {
		std::list<Gosu::ButtonName>::iterator bit;
		for (bit = ait->second.buttons.begin(); bit != ait->second.buttons.end(); bit++) {
			if ((*bit) == _button.getId()) {
				ait->second.state = actnFinish;
			}
		}
	}
}

void InputManager::update()
{
	//Update chords
	//Invalidate old chords
	std::map<std::string, chord>::iterator cit;
	for (cit = m_chords.begin(); cit != m_chords.end(); cit++) {
		if(cit->second.state == actnBegin) {
			cit->second.state = actnActive;
		}else
		if(cit->second.state == actnFinish) {
			cit->second.state = actnIdle;
		}else
		if (cit->second.state == actnProcess && cit->second.timer <= 0) {
			cit->second.state = actnIdle;
			std::list<btnVald>::iterator bit;
			for (bit = cit->second.buttons.begin(); bit != cit->second.buttons.end(); bit++) {
				bit->valid = false;
			}
		}else{
			cit->second.timer--;
		}
	}
	//Invalidate old sequences
	std::list<sequencebuff>::iterator sit = m_seqbuff.begin();
	while (sit != m_seqbuff.end()) {
		if (sit->timer <= 0 && sit->sequence->state != actnActive) {
			sit = m_seqbuff.erase(sit);
			continue;
		}else{
			sit->timer--;
		}
		sit++;
	}
	std::map<std::string, sequence>::iterator ssit;
	for (ssit = m_sequences.begin(); ssit != m_sequences.end(); ssit++) {
		if (ssit->second.state == actnBegin) {
			ssit->second.state = actnActive;
		}else
		if (ssit->second.state == actnFinish) {
			ssit->second.state = actnIdle;
		}
	}
	//Update actions
	std::map<std::string, action>::iterator ait;
	for (ait = m_actions.begin();ait != m_actions.end(); ait++) {
		if (ait->second.state == actnBegin) {
			ait->second.state = actnActive;
		}
		if (ait->second.state == actnFinish) {
			ait->second.state = actnIdle;
		}
	}
}

InputManager::actionState InputManager::query(std::string _action)
{
	if (m_sequences.find(_action) != m_sequences.end()) {
		return m_sequences.find(_action)->second.state;
	}else
	if (m_chords.find(_action) != m_chords.end()) {
		return m_chords.find(_action)->second.state;
	}else
	if (m_actions.find(_action) != m_actions.end()) {
		return m_actions.find(_action)->second.state;
	}else
		return actnInvalid;
}

void InputManager::createAction(std::string _name)
{
	m_actions[_name].state = actnIdle;
}
void InputManager::createSequence(std::string _name, int _threshhold)
{
	m_sequences[_name].state = actnIdle;
	m_sequences[_name].threshold = _threshhold;
}
void InputManager::createChord(std::string _name)
{
	m_chords[_name].state = actnIdle;
	m_chords[_name].timer = -1;
}

void InputManager::bindAction(std::string _name, Gosu::ButtonName _button)
{
	m_actions[_name].buttons.push_back(_button);
}
void InputManager::clearAction(std::string _name)
{
	m_actions[_name].buttons.clear();
}

void InputManager::pushSequence(std::string _name, Gosu::ButtonName _button)
{
	m_sequences[_name].buttons.push_back(_button);
}
void InputManager::clearSequence(std::string _name)
{
	m_sequences[_name].buttons.clear();
}

void InputManager::addChord(std::string _name, Gosu::ButtonName _button)
{
	btnVald bv;
	bv.button = _button;
	bv.valid = false;
	m_chords[_name].buttons.push_back(bv);
}
void InputManager::clearChord(std::string _name)
{
	m_chords[_name].buttons.clear();
}