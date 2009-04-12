/*
 *  Environment.cpp
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#include "MUGE.h"
#include "Environment.h"
#include "JSONFile.hpp"
#include "ContactListener.h"
#include <string>
#include <iostream>
#include <fstream>

Environment::Environment(std::wstring levelFile, MUGE* _engine)
{
	m_Engine = _engine;
	
	m_TimeStep = 1.0f / 60.0f;
	m_Iterations = 10.0f;
	
	m_Width = m_Engine->graphics().width();
	m_Height = m_Engine->graphics().height();
	
	m_PlayerPos = b2Vec2( 384, 300);

	m_SceneGraphp.reset( new SceneGraph() );
	camDef cDef;
	cDef.origin = b2Vec2( 384, 240);
	cDef.extents = b2Vec2( 384, 1024);
	cDef.safe = b2Vec2( 900, 500);
	cDef.player = b2Vec2( 200, 100);
	cDef.desire = b2Vec2( 50, 50);
	m_SceneGraphp->Camerap.reset( new Camera(cDef) );
	
	//
	// Read in JSON encoded file
	// TinyJSON library will perform lexing and parsing
	// pull out data using JSONFile class
	//
	
	JSONFile jFile(Gosu::narrow(levelFile));
	json::grammar<char>::array::const_iterator it;
	json::grammar<char>::array arr, arr2;
	json::grammar<char>::object o;
	int i;
	
	arr = jFile.get<json::grammar<char>::array>("canvasColor");
	m_canvasColor = Gosu::Color( boost::any_cast< int >(*arr[0]), boost::any_cast< int >(*arr[1]), boost::any_cast< int >(*arr[2]),  boost::any_cast< int >(*arr[3]) );
	std::wstring filename = Gosu::resourcePrefix() + L"Images/Hero_Idle.png";
	m_PlayerImagep.reset( new Gosu::Image(m_Engine->graphics(), filename, false));
	
	m_Units[0] = jFile.get< int >("xunits");
	m_Units[1] = jFile.get< int >("yunits");
	
	
	m_backgroundScale = jFile.get< double >("backgroundScale");
	m_midgroundScale = jFile.get< double >("midgroundScale");
	m_groundScale = jFile.get< double >("groundScale");
	m_foregroundScale = jFile.get< double >("foregroundScale");
	
	
	
	arr = jFile.get<json::grammar<char>::array>("background");
	for (i = 0, it = arr.begin(); it != arr.end(); ++it, ++i) {
		Sprite tSprite;
		o = boost::any_cast< json::grammar<char>::object >(**it);		
		tSprite.setImage( m_Engine->graphics(), Gosu::resourcePrefix() + L"Images/Levels/" + Gosu::widen(boost::any_cast< std::string >(*o["image"])) );
		
		arr2 = boost::any_cast<json::grammar<char>::array>(*o["position"]);
		tSprite.setPosition( boost::any_cast< double >(*arr2[0]) * m_Units[0], m_Height - boost::any_cast< double >(*arr2[1]) * m_Units[1], 1);
		tSprite.setRotation( boost::any_cast< double >(*o["rotation"]) );
		
		arr2 = boost::any_cast<json::grammar<char>::array>(*o["colormod"]);
		tSprite.setColorMod( Gosu::Color( boost::any_cast< int >(*arr2[0]), boost::any_cast< int >(*arr2[1]), boost::any_cast< int >(*arr2[2]),  boost::any_cast< int >(*arr2[3])) );
		tSprite.setScaling( boost::any_cast< double >(*o["xScale"]), boost::any_cast< double >(*o["yScale"]) );
		m_Background.push_back( tSprite );
	}
	
	arr = jFile.get<json::grammar<char>::array>("midground");
	for (i = 0, it = arr.begin(); it != arr.end(); ++it, ++i) {
		Sprite tSprite;
		o = boost::any_cast< json::grammar<char>::object >(**it);		
		tSprite.setImage( m_Engine->graphics(), Gosu::resourcePrefix() + L"Images/Levels/" + Gosu::widen(boost::any_cast< std::string >(*o["image"])) );
		
		arr2 = boost::any_cast<json::grammar<char>::array>(*o["position"]);
		tSprite.setPosition( boost::any_cast< double >(*arr2[0]) * m_Units[0], m_Height - boost::any_cast< double >(*arr2[1]) * m_Units[1], 1);
		tSprite.setRotation( boost::any_cast< double >(*o["rotation"]) );
		
		arr2 = boost::any_cast<json::grammar<char>::array>(*o["colormod"]);
		tSprite.setColorMod( Gosu::Color( boost::any_cast< int >(*arr2[0]), boost::any_cast< int >(*arr2[1]), boost::any_cast< int >(*arr2[2]),  boost::any_cast< int >(*arr2[3])) );
		tSprite.setScaling( boost::any_cast< double >(*o["xScale"]), boost::any_cast< double >(*o["yScale"]) );
		m_Midground.push_back( tSprite );
	}
	
	arr = jFile.get<json::grammar<char>::array>("ground");
	for (i = 0, it = arr.begin(); it != arr.end(); ++it, ++i) {
		Sprite tSprite;
		o = boost::any_cast< json::grammar<char>::object >(**it);		
		tSprite.setImage( m_Engine->graphics(), Gosu::resourcePrefix() + L"Images/Levels/" + Gosu::widen(boost::any_cast< std::string >(*o["image"])) );
		
		arr2 = boost::any_cast<json::grammar<char>::array>(*o["position"]);
		tSprite.setPosition( boost::any_cast< double >(*arr2[0]) * m_Units[0], m_Height - boost::any_cast< double >(*arr2[1]) * m_Units[1], 1);
		tSprite.setRotation( boost::any_cast< double >(*o["rotation"]) );
		
		arr2 = boost::any_cast<json::grammar<char>::array>(*o["colormod"]);
		tSprite.setColorMod( Gosu::Color( boost::any_cast< int >(*arr2[0]), boost::any_cast< int >(*arr2[1]), boost::any_cast< int >(*arr2[2]),  boost::any_cast< int >(*arr2[3])) );
		tSprite.setScaling( boost::any_cast< double >(*o["xScale"]), boost::any_cast< double >(*o["yScale"]) );
		m_Ground.push_back( tSprite );
	}
	
	arr = jFile.get<json::grammar<char>::array>("foreground");
	for (i = 0, it = arr.begin(); it != arr.end(); ++it, ++i) {
		Sprite tSprite;
		o = boost::any_cast< json::grammar<char>::object >(**it);		
		tSprite.setImage( m_Engine->graphics(), Gosu::resourcePrefix() + L"Images/Levels/" + Gosu::widen(boost::any_cast< std::string >(*o["image"])) );
		
		arr2 = boost::any_cast<json::grammar<char>::array>(*o["position"]);
		tSprite.setPosition( boost::any_cast< double >(*arr2[0]) * m_Units[0], m_Height - boost::any_cast< double >(*arr2[1]) * m_Units[1], 1);
		tSprite.setRotation( boost::any_cast< double >(*o["rotation"]) );
		
		arr2 = boost::any_cast<json::grammar<char>::array>(*o["colormod"]);
		tSprite.setColorMod( Gosu::Color( boost::any_cast< int >(*arr2[0]), boost::any_cast< int >(*arr2[1]), boost::any_cast< int >(*arr2[2]),  boost::any_cast< int >(*arr2[3])) );
		tSprite.setScaling( boost::any_cast< double >(*o["xScale"]), boost::any_cast< double >(*o["yScale"]) );
		m_Foreground.push_back( tSprite );
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
	
	m_Music.reset( new Gosu::Song( m_Engine->audio(), Gosu::resourcePrefix() + L"Sound/Colugo-Fantastic_face.ogg"));
	m_Music->play(true);
	
}

void Environment::update()
{
	// Step physics simulation
	//m_Worldp->Step(m_TimeStep, m_Iterations);
	
	if (m_Engine->input().down(Gosu::kbLeft)) {
		m_PlayerPos.x -= 1.0;
		if (m_Engine->input().down(Gosu::kbLeftShift)) 
			m_PlayerPos.x -= 1.0;
	}
	if (m_Engine->input().down(Gosu::kbRight)) {
		m_PlayerPos.x += 1.0;
		if (m_Engine->input().down(Gosu::kbLeftShift)) 
			m_PlayerPos.x += 1.0;
	}
	
	m_SceneGraphp->Camerap->update( m_SceneGraphp->Areasv, m_PlayerPos);
	
	b2Vec2 camPos = m_SceneGraphp->Camerap->getCenter();
	
	m_aOrigin[0] = camPos.x - m_Width/2;
	m_aOrigin[1] = camPos.y - m_Height/2;

}

void Environment::draw() const
{
	m_Engine->graphics().drawQuad( 0, 0, m_canvasColor, 
		m_Width, 0, m_canvasColor,
		0, m_Height, m_canvasColor,
		m_Width, m_Height, m_canvasColor, 0);
	
	std::vector< Sprite >::const_iterator it;
	for (it = m_Background.begin(); it != m_Background.end(); ++it) {
		it->draw( -m_aOrigin[0]*(1.0f/m_backgroundScale), 0, 1);
	}
	for (it = m_Midground.begin(); it != m_Midground.end(); ++it) {
		it->draw( -m_aOrigin[0]*(1.0f/m_midgroundScale), 0, 2);
	}
	for (it = m_Ground.begin(); it != m_Ground.end(); ++it) {
		it->draw( -m_aOrigin[0]*(1.0f/m_groundScale), 0, 3);
	}
	for (it = m_Foreground.begin(); it != m_Foreground.end(); ++it) {
		it->draw( -m_aOrigin[0]*(1.0f/m_foregroundScale), 0, 10);
	}
	
	m_PlayerImagep->draw( m_PlayerPos.x - m_aOrigin[0], m_PlayerPos.y, 4);
}
