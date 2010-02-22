/*
   Main.cpp
   My Unnamed Game Engine
 
   Created by Chad Godsey on 11/12/08.
  
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
 
#include <Gosu/Gosu.hpp>
#include "Input/JSONFile.hpp"
#include "Input/InputManager.h"
#include "Core/Core.h"
#include "TitleState.h"


int main(int argc, char* argv[])
{
	// Let's make the game creation a little more dynamic
	JSONFile jFile(Gosu::narrow(Gosu::resourcePrefix() + L"Data/main.json"));
	json::grammar<char>::array::iterator it, it2;
	json::grammar<char>::array arr, arr2;
	json::grammar<char>::object o;
	std::string str, str2;
	int i;
	
	// Retrieve window size parameters
	int width = jFile.get< int >("WindowSize[0]");
	int height = jFile.get< int >("WindowSize[1]");
	// Framerate and fullscreen stuff
	double updateInterval = jFile.get< double >("UpdateInterval");
	bool fullscreen = jFile.get< bool >("FullScreen");
	
	// Create the game object
    Core win(width, height, fullscreen, updateInterval);
	// Must set context otherwise subsystems will fail
	win.setCurrentContext(&win);

	// Input stuff!
	InputManager* iman = InputManager::getCurrentContext();
	arr = jFile.get<json::grammar<char>::array>("ButtonMaps");
	for (it = arr.begin(); it != arr.end(); ++it) {
		str = jFile.get< std::string >("Type", *it);
		if (str == "action") {
			str = jFile.get< std::string >("Action", *it);
			iman->createAction(str);
			if (jFile.hasKey("Key", *it)) {
				//There is only one key to be bound
				str2 = jFile.get< std::string >("Key", *it);
				iman->bindAction(str, InputManager::strToButton(str2));
			}else{
				//There are multiple keys to be bound
				arr2 = jFile.get<json::grammar<char>::array>("Keys", *it);
				for (i = 0,it2 = arr.begin(); it2 != arr2.end(); ++it2, ++i) {
					str2 = jFile.get< std::string >("Keys[" + boost::lexical_cast<std::string>(i) + "]", *it);
					iman->bindAction(str, InputManager::strToButton(str2));
				}
			}
			continue;
		}
		if (str == "chord") {
			str = jFile.get< std::string >("Action", *it);
			iman->createChord(str);
			if (jFile.hasKey("Key", *it)) {
				//There is only one key to be bound
				str2 = jFile.get< std::string >("Key", *it);
				iman->addChord(str, InputManager::strToButton(str2));
			}else{
				//There are multiple keys to be bound
				arr2 = jFile.get<json::grammar<char>::array>("Keys", *it);
				for (i = 0,it2 = arr.begin(); it2 != arr2.end(); ++it2, ++i) {
					str2 = jFile.get< std::string >("Keys[" + boost::lexical_cast<std::string>(i) + "]", *it);
					iman->addChord(str, InputManager::strToButton(str2));
				}
			}
			continue;
		}
		if (str == "sequence") {
			str = jFile.get< std::string >("Action", *it);
			i = jFile.get< int >("Threshold", *it);
			iman->createSequence(str, i);
			if (jFile.hasKey("Key", *it)) {
				//There is only one key to be bound
				str2 = jFile.get< std::string >("Key", *it);
				iman->pushSequence(str, InputManager::strToButton(str2));
			}else{
				//There are multiple keys to be bound
				arr2 = jFile.get<json::grammar<char>::array>("Keys", *it);
				for (i = 0,it2 = arr.begin(); it2 != arr2.end(); ++it2, ++i) {
					str2 = jFile.get< std::string >("Keys[" + boost::lexical_cast<std::string>(i) + "]", *it);
					iman->pushSequence(str, InputManager::strToButton(str2));
				}
			}
			continue;
		}
	}

	// Create and add a title state to our game object
    TitleState *state = new TitleState( Gosu::widen(jFile.get< std::string >("StartState")) );
    win.pushState( state );

	// Start the game!
    win.show();

	// Exit
    return 0;
}
