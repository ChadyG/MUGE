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

#include <Box2D/Box2D.h>
#include "InputManager.h"
#include "../Core/Core.h"

InputManager* InputManager::s_CurrentContext;

InputManager::~InputManager() 
{
	std::map<std::string, chord>::iterator cit;
	for (cit = m_chords.begin(); cit != m_chords.end(); cit++) {
		cit->second.buttons.clear();
	}
	m_chords.clear();
	std::map<std::string, sequence>::iterator sit;
	for (sit = m_sequences.begin();sit != m_sequences.end(); sit++) {
		sit->second.buttons.clear();
	}
	m_sequences.clear();
	m_seqbuff.clear();
	std::map<std::string, action>::iterator ait;
	for (ait = m_actions.begin();ait != m_actions.end(); ait++) {
		ait->second.buttons.clear();
	}
	m_actions.clear();
}

//TODO: look into using Gosu::Input::idToChar/charToId
Gosu::ButtonName InputManager::strToButton(std::string _name)
{
	if (_name == "kb1") return Gosu::kb1;
	if (_name == "kb2") return Gosu::kb2;
	if (_name == "kb3") return Gosu::kb3;
	if (_name == "kb4") return Gosu::kb4;
	if (_name == "kb5") return Gosu::kb5;
	if (_name == "kb6") return Gosu::kb6;
	if (_name == "kb7") return Gosu::kb7;
	if (_name == "kb8") return Gosu::kb8;
	if (_name == "kb9") return Gosu::kb9;
	if (_name == "kb0") return Gosu::kb0;
	
	if (_name == "kbA") return Gosu::kbA;
	if (_name == "kbB") return Gosu::kbB;
	if (_name == "kbC") return Gosu::kbC;
	if (_name == "kbD") return Gosu::kbD;
	if (_name == "kbE") return Gosu::kbE;
	if (_name == "kbF") return Gosu::kbF;
	if (_name == "kbG") return Gosu::kbG;
	if (_name == "kbH") return Gosu::kbH;
	if (_name == "kbI") return Gosu::kbI;
	if (_name == "kbJ") return Gosu::kbJ;
	if (_name == "kbK") return Gosu::kbK;
	if (_name == "kbL") return Gosu::kbL;
	if (_name == "kbM") return Gosu::kbM;
	if (_name == "kbN") return Gosu::kbN;
	if (_name == "kbO") return Gosu::kbO;
	if (_name == "kbP") return Gosu::kbP;
	if (_name == "kbQ") return Gosu::kbQ;
	if (_name == "kbR") return Gosu::kbR;
	if (_name == "kbS") return Gosu::kbS;
	if (_name == "kbT") return Gosu::kbT;
	if (_name == "kbU") return Gosu::kbU;
	if (_name == "kbV") return Gosu::kbV;
	if (_name == "kbW") return Gosu::kbW;
	if (_name == "kbX") return Gosu::kbX;
	if (_name == "kbY") return Gosu::kbY;
	if (_name == "kbZ") return Gosu::kbZ;

	if (_name == "kbEscape") return Gosu::kbEscape;
	if (_name == "kbTab") return Gosu::kbTab;
	if (_name == "kbReturn") return Gosu::kbReturn;
	if (_name == "kbEnter") return Gosu::kbEnter;
	if (_name == "kbSpace") return Gosu::kbSpace;
	if (_name == "kbLeftShift") return Gosu::kbLeftShift;
	if (_name == "kbRightShift") return Gosu::kbRightShift;
	if (_name == "kbLeftControl") return Gosu::kbLeftControl;
	if (_name == "kbRightControl") return Gosu::kbRightControl;

	if (_name == "kbUp") return Gosu::kbUp;
	if (_name == "kbDown") return Gosu::kbDown;
	if (_name == "kbLeft") return Gosu::kbLeft;
	if (_name == "kbRight") return Gosu::kbRight;

	if (_name == "kbF1") return Gosu::kbF1;
	if (_name == "kbF2") return Gosu::kbF2;
	if (_name == "kbF3") return Gosu::kbF3;
	if (_name == "kbF4") return Gosu::kbF4;
	if (_name == "kbF5") return Gosu::kbF5;
	if (_name == "kbF6") return Gosu::kbF6;
	if (_name == "kbF7") return Gosu::kbF7;
	if (_name == "kbF8") return Gosu::kbF8;
	if (_name == "kbF9") return Gosu::kbF9;
	if (_name == "kbF10") return Gosu::kbF10;
	if (_name == "kbF11") return Gosu::kbF11;
	if (_name == "kbF12") return Gosu::kbF12;
	
	if (_name == "gpLeft") return Gosu::gpLeft;
	if (_name == "gpUp") return Gosu::gpUp;
	if (_name == "gpRight") return Gosu::gpRight;
	if (_name == "gpDown") return Gosu::gpDown;
	
	if (_name == "gpButton0") return Gosu::gpButton0;
	if (_name == "gpButton1") return Gosu::gpButton1;
	if (_name == "gpButton2") return Gosu::gpButton2;
	if (_name == "gpButton3") return Gosu::gpButton3;
	if (_name == "gpButton4") return Gosu::gpButton4;
	if (_name == "gpButton5") return Gosu::gpButton5;
	if (_name == "gpButton6") return Gosu::gpButton6;
	if (_name == "gpButton7") return Gosu::gpButton7;
	if (_name == "gpButton8") return Gosu::gpButton8;
	if (_name == "gpButton9") return Gosu::gpButton9;
	if (_name == "gpButton10") return Gosu::gpButton10;
	if (_name == "gpButton11") return Gosu::gpButton11;
	if (_name == "gpButton12") return Gosu::gpButton12;
	if (_name == "gpButton13") return Gosu::gpButton13;
	if (_name == "gpButton14") return Gosu::gpButton14;
	if (_name == "gpButton15") return Gosu::gpButton15;

	if (_name == "msLeft") return Gosu::msLeft;
	if (_name == "msRight") return Gosu::msRight;
	if (_name == "msMiddle") return Gosu::msMiddle;
	if (_name == "msWheelUp") return Gosu::msWheelUp;
	if (_name == "msWheelDown") return Gosu::msWheelDown;

	return Gosu::noButton;
}

void InputManager::buttonDown(Gosu::Button _button)
{
	//Update chords
	std::map<std::string, chord>::iterator cit;
	for (cit = m_chords.begin(); cit != m_chords.end(); cit++) {
		std::list<btnVald>::iterator bit;
		for (bit = cit->second.buttons.begin(); bit != cit->second.buttons.end(); bit++) {
			//do we match any buttons?
			if (bit->button == _button) {
				bit->valid = true;
				//start processing?
				if (cit->second.state == actnIdle) {
					cit->second.timer = Gosu::milliseconds();
					cit->second.state = actnProcess;
				}else{
					//determine if this finishes the chord
					std::list<btnVald>::iterator bit2;
					bool valid = true;
					for (bit2 = cit->second.buttons.begin(); bit2 != cit->second.buttons.end(); bit2++) {
						if (bit2->valid == false)
							valid = false;
					}
					//chord is complete
					if (valid)
						cit->second.state = actnBegin;
				}
			}
		}
	}
	//Update sequences with start state
	//Sequences finish on button down, with previous hits on up
	std::list<sequencebuff>::iterator sit;
	for (sit = m_seqbuff.begin();sit != m_seqbuff.end(); sit++) {
		if (sit->index == (sit->sequence->buttons.size()-1) && _button == sit->sequence->buttons[sit->index]) {
			sit->sequence->state = actnBegin;
		}
	}
	//Update actions
	std::map<std::string, action>::iterator ait;
	for (ait = m_actions.begin();ait != m_actions.end(); ait++) {
		std::list<Gosu::ButtonName>::iterator bit;
		for (bit = ait->second.buttons.begin(); bit != ait->second.buttons.end(); bit++) {
			if ((*bit) == _button) {
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
		if (_button != sit->sequence->buttons[sit->index]) {
			//not next button in sequence, stop watching this sequence
			if (sit->sequence->state != actnActive) {
				sit->sequence->state = actnIdle;
				sit = m_seqbuff.erase(sit);
				continue;
			}
		}else{
			//is next button, continue watching
			sit->timer = sit->sequence->threshold;
			sit->index++;
			if (sit->index == sit->sequence->buttons.size()) {
				//sequence is complete, stop watching
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
		if (_button == ssit->second.buttons.front()) {
			sequencebuff sb;
			sb.index = 1;
			sb.sequence = &(ssit->second);
			sb.timer = Gosu::milliseconds();
			sb.valid = true;
			m_seqbuff.push_back(sb);
		}
	}
	//Invalidate chords
	std::map<std::string, chord>::iterator cit;
	for (cit = m_chords.begin(); cit != m_chords.end(); cit++) {
		std::list<btnVald>::iterator bit;
		for (bit = cit->second.buttons.begin(); bit != cit->second.buttons.end(); bit++) {
			if (bit->button == _button) {
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
			if ((*bit) == _button) {
				ait->second.state = actnFinish;
			}
		}
	}
}

void InputManager::initWith(Json::Value _jval)
{
	std::string str, str2;
	for (int i = 0; i < _jval.size(); ++i) {
		str = _jval[i].get("Type", "action").asString();
		if (str == "action") {
			str = _jval[i].get("Action", "default").asString();
			createAction(str);
			if (_jval[i].isMember("Key")) {
				//There is only one key to be bound
				str2 = _jval[i].get("Key", "kbA").asString();
				bindAction(str, InputManager::strToButton(str2));
			}else{
				//There are multiple keys to be bound
				for (int j = 0; j < _jval[i]["Keys"].size(); ++j) {
					str2 = _jval[i]["Keys"].get(j,"kbA").asString();
					bindAction(str, InputManager::strToButton(str2));
				}
			}
			continue;
		}
		if (str == "chord") {
			str = _jval[i].get("Chord", "default").asString();
			int t = _jval[i].get("Threshold", 200).asInt();
			createChord(str, t);
			if (_jval[i].isMember("Key")) {
				//There is only one key to be bound
				str2 = _jval[i].get("Key", "kbA").asString();
				addChord(str, InputManager::strToButton(str2));
			}else{
				//There are multiple keys to be bound
				for (int j = 0; j < _jval[i]["Keys"].size(); ++j) {
					str2 = _jval[i]["Keys"].get(j,"kbA").asString();
					addChord(str, InputManager::strToButton(str2));
				}
			}
			continue;
		}
		if (str == "sequence") {
			str = _jval[i].get("Sequence", "default").asString();
			int t = _jval[i].get("Threshold", 400).asInt();
			createSequence(str, t);
			if (_jval[i].isMember("Key")) {
				//There is only one key to be bound
				str2 = _jval[i].get("Key", "kbA").asString();
				pushSequence(str, InputManager::strToButton(str2));
			}else{
				//There are multiple keys to be bound
				for (int j = 0; j < _jval[i]["Keys"].size(); ++j) {
					str2 = _jval[i]["Keys"].get(j,"kbA").asString();
					pushSequence(str, InputManager::strToButton(str2));
				}
			}
			continue;
		}
	}
}

void InputManager::resetInputs()
{
	//Update sequences
	m_seqbuff.clear();

	//Start new sequences
	std::map<std::string, sequence>::iterator ssit;
	for (ssit = m_sequences.begin(); ssit != m_sequences.end(); ssit++) {
		ssit->second.state = actnIdle;
	}
	//Invalidate chords
	std::map<std::string, chord>::iterator cit;
	for (cit = m_chords.begin(); cit != m_chords.end(); cit++) {
		cit->second.state = actnIdle;
	}
	//Update actions
	std::map<std::string, action>::iterator ait;
	for (ait = m_actions.begin();ait != m_actions.end(); ait++) {
		ait->second.state = actnIdle;
	}
}

void InputManager::update()
{
	unsigned long time;
	//Update chords
	//Invalidate old chords
	std::map<std::string, chord>::iterator cit;
	for (cit = m_chords.begin(); cit != m_chords.end(); cit++) {
		time = Gosu::milliseconds() - cit->second.timer;
		if(cit->second.state == actnBegin) {
			cit->second.state = actnActive;
		}else
		if(cit->second.state == actnFinish) {
			cit->second.state = actnIdle;
		}else
		if (cit->second.state == actnProcess && time >= cit->second.threshold) {
			cit->second.state = actnIdle;
			std::list<btnVald>::iterator bit;
			for (bit = cit->second.buttons.begin(); bit != cit->second.buttons.end(); bit++) {
				bit->valid = false;
			}
		}
	}
	//Invalidate old sequences
	std::list<sequencebuff>::iterator sit = m_seqbuff.begin();
	while (sit != m_seqbuff.end()) {
		time = Gosu::milliseconds() - cit->second.timer;
		if (time >= sit->sequence->threshold && sit->sequence->state != actnActive) {
			sit = m_seqbuff.erase(sit);
			continue;
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

b2Vec2 InputManager::getMouse()
{
	b2Vec2 mouse(Core::getCurrentContext()->input().mouseX() - m_screenW/2, Core::getCurrentContext()->input().mouseY() - m_screenH/2);

	double scale = 1.0;
	double zoom = 1.0 + scale * (m_camZoom - 1.0);

	b2Transform trans;
	trans.R.Set( -m_camRot*(Gosu::pi/180.0) );
	trans.position = b2Mul( trans.R, mouse);

	mouse.Set((trans.position.x) / (scale * m_screenScale * zoom) + m_camX, 
				  (trans.position.y) / (scale * m_screenScale * zoom) + m_camY);
	
	return mouse;
}

InputManager::actionState InputManager::query(std::string _action)
{
	if (!m_enabled)
		return actnIdle;

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
void InputManager::createSequence(std::string _name, int _threshold)
{
	m_sequences[_name].state = actnIdle;
	m_sequences[_name].threshold = _threshold;
}
void InputManager::createChord(std::string _name, int _threshold)
{
	m_chords[_name].state = actnIdle;
	m_chords[_name].timer = -1;
	m_chords[_name].threshold = _threshold;
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