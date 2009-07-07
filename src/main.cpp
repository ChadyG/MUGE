/*
 *  Main.cpp
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
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
