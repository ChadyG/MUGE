/*
 *  Environment.cpp
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#include "Environment.h"
#include "JSONFile.hpp"
#include "ContactListener.h"
#include "Gosu/Utility.hpp"
#include <string>
#include <iostream>
#include <fstream>

Environment::Environment(std::wstring levelFile, Gosu::Graphics &graphics)
{

	//m_SceneGraphp.reset(new SceneGraph());
	m_TimeStep = 1.0f / 60.0f;
	m_Iterations = 10.0f;
	
	m_Width = graphics.width();
	m_Height = graphics.height();
	//
	// Read in JSON encoded file
	// TinyJSON library will perform lexing and parsing
	// pull out data using JSONFile class
	//
	
	JSONFile jFile(Gosu::narrow(levelFile));
	json::grammar<char>::array::iterator it;
	json::grammar<char>::array arr;
	json::grammar<char>::object o;
	int i;
	
	std::string tstring = jFile.get<std::string>("background");
	std::wstring filename = Gosu::resourcePrefix() + Gosu::widen(tstring);
	m_BackgroundImagep.reset(new Gosu::Image(graphics, filename, false));
	
	tstring = jFile.get<std::string>("midground");
	filename = Gosu::resourcePrefix() + Gosu::widen(tstring);
	m_MidgroundImagep.reset(new Gosu::Image(graphics, filename, false));
	
	tstring = jFile.get<std::string>("groundimage");
	filename = Gosu::resourcePrefix() + Gosu::widen(tstring);
	m_GroundImagep.reset(new Gosu::Image(graphics, filename, false));
	
	tstring = jFile.get<std::string>("foreground");
	filename = Gosu::resourcePrefix() + Gosu::widen(tstring);
	m_ForegroundImagep.reset(new Gosu::Image(graphics, filename, false));
	
	arr = jFile.get<json::grammar<char>::array>("ground");
	
	for (i = 0, it = arr.begin(); it != arr.end(); ++it, ++i) {
		//m_SceneGraphp->aGround[i] = boost::any_cast<int>(**it);
	}
	
	arr = jFile.get<json::grammar<char>::array>("platforms");
	for (i = 0, it = arr.begin(); it != arr.end(); ++it, ++i) {
		o = boost::any_cast< json::grammar<char>::object >(**it);
		//jFile.get<int>("x",o);
		//jFile.get<int>("y",o);
		//jFile.get<std::string>("size",o);
	}
	
	arr = jFile.get<json::grammar<char>::array>("blocks");
	for (i = 0, it = arr.begin(); it != arr.end(); ++it, ++i) {
		o = boost::any_cast< json::grammar<char>::object >(**it);
		//jFile.get<int>("x",o);
		//jFile.get<int>("y",o);
		//jFile.get<int>("width",o);
		//jFile.get<int>("height",o);
		//jFile.get<json::grammar<char>::array>("shape[1]",o);
		//jFile.get<json::grammar<char>::array>("shape[2]",o);
	}
	
	arr = jFile.get<json::grammar<char>::array>("items");
	for (i = 0, it = arr.begin(); it != arr.end(); ++it, ++i) {
		o = boost::any_cast< json::grammar<char>::object >(**it);
		//jFile.get<int>("x",o);
		//jFile.get<int>("y",o);
		//jFile.get<std::string>("type",o);
		//jFile.get<int>("amount",o);
	}
	
	arr = jFile.get<json::grammar<char>::array>("ai");
	for (i = 0, it = arr.begin(); it != arr.end(); ++it, ++i) {
		o = boost::any_cast< json::grammar<char>::object >(**it);
		//jFile.get<int>("x",o);
		//jFile.get<int>("y",o);
		//jFile.get<std::string>("character",o);
		//jFile.get<int>("health",o);
	}
	
}

void Environment::update()
{
	// Step physics simulation
	m_Worldp->Step(m_TimeStep, m_Iterations);
	
	b2Vec2 camPos = m_SceneGraphp->Camerap->getCenter();
	
	m_aOrigin[0] = camPos.x + m_Width/2;
	m_aOrigin[1] = camPos.y + m_Height/2;

}

void Environment::draw() const
{

}
