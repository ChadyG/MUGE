/*
   Main.cpp
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
 
#include <Gosu/Gosu.hpp>
#include "Input/JSONFile.hpp"
#include "Core/MUGE.h"
#include "TitleState.h"


int main(int argc, char* argv[])
{
	JSONFile jFile(Gosu::narrow(Gosu::resourcePrefix() + L"Data/main.json"));
	json::grammar<char>::array::const_iterator it;
	json::grammar<char>::array arr, arr2;
	json::grammar<char>::object o;
	int width = jFile.get< int >("WindowSize[0]");
	int height = jFile.get< int >("WindowSize[1]");
	double updateInterval = jFile.get< double >("UpdateInterval");
	bool fullscreen = jFile.get< bool >("FullScreen");
	
	MUGE win(width, height, fullscreen, updateInterval);
	win.pushState( TitleState::instance() );
	win.show();
	return 0;
}
