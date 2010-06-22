/*
	Scene.cpp
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
 
#include <Core/Core.h>

#include "../Physics/ContactListener.h"
#include "Scene.h"
#include "PlatPlayer.h"

Scene::Scene(std::wstring _config)
{
	m_Engine = Core::getCurrentContext();
	
	m_TimeStep = 1.0f / 50.0f;
	m_Iterations = 10.0f;
	
	m_Width = m_Engine->graphics().width();
	m_Height = m_Engine->graphics().height();
	m_Zoom = 1.0;
	m_Rot = 0.0;
	m_Orientation = 0.0;
	
	// box2D stuff
	b2Vec2 gravity( 0.0f, 0.0f);
	bool do_sleep = true;

	m_World = new b2World(gravity, do_sleep);
	//m_World->SetContactListener( &m_ContactListener );

	m_Iterations = 10;
	
	//
	// Read in JSON encoded file
	// TinyJSON library will perform lexing and parsing
	// pull out data using JSONFile class
	// 
	// Todo: move to Adventure State?
	
	m_jFile.reset( new JSONFile(Gosu::narrow(Gosu::resourcePrefix() + L"Data/" + _config + L".json")) );
	json::grammar<char>::array::const_iterator it, it2;
	json::grammar<char>::array arr, arr2;
	json::grammar<char>::object o;
	int i, layer;
	std::string tString;

	// Testing stuff
	m_jFile->setInt("loltest", 1);
	m_jFile->setInt("Physics", 0);
	m_jFile->setBoolean("testbool", false);
	m_jFile->setDouble("PlayerSpawn[0]", 1.0);
	m_jFile->writeFile("test.json");
	// End testing
	
	// Player spawn
	m_PlayerPos.Set(m_jFile->get< double >("PlayerSpawn[0]"), m_jFile->get< double >("PlayerSpawn[1]"));
	
	// Background color
	arr = m_jFile->get<json::grammar<char>::array>("CanvasColor");
	m_canvasColor = Gosu::Color( 
							m_jFile->get< int >("CanvasColor[0]"), 
							m_jFile->get< int >("CanvasColor[1]"),
							m_jFile->get< int >("CanvasColor[2]"),
							m_jFile->get< int >("CanvasColor[3]"));
	
	// Coordinate transform stuff for world -> screen
	m_Scale = m_jFile->get< int >("Scale");

	// Configure render/audio manager for screen transformation
	m_rendMan.setCamera( &m_Camera );
	m_rendMan.setScreen( m_Width, m_Height, m_Scale );
	m_audMan.setScreen( m_Width, m_Height, m_Scale );
	
	// Dig down to the actual scene objects
	arr = m_jFile->get<json::grammar<char>::array>("Layers");
	for (i = 0, it = arr.begin(); it != arr.end(); ++it, ++i) {
		layer = m_jFile->get< int >("Layer", *it);
		m_Layers[layer].scale = m_jFile->get< double >("Scale", *it);
		m_Layers[layer].layer = m_jFile->get< int >("Layer", *it);
		m_Layers[layer].ID = m_jFile->get<std::string>("ID", *it);

		//m_rendMan.setLayerScale( m_Layers[layer].layer, m_Layers[layer].scale);
		m_Camera.addLayer( m_Layers[layer].layer, m_Layers[layer].scale );
		
		// String access map
		m_LayerNames[m_Layers[layer].ID] = layer;
		
		// Recursive evaluation of layer hierarchy
		arr2 = m_jFile->get<json::grammar<char>::array>("Objects", *it);
		evalJSON(arr2, layer);
	}
	
	//Pass off song creation/management to manager
	//  this way anyone can pause the music if needed
	m_audMan.createSong(
		Gosu::resourcePrefix() + L"Sound/" + Gosu::widen( m_jFile->get<std::string>("Music")), 
		"Background");
	m_audMan.playSong("Background", true);
}

//void Scene::addSprite( SceneObject &_object, Gosu::ZPos _layer )
//{
//	m_Layers[_layer].objects.push_back( _object );
//}

void Scene::addTrigger( Trigger &_trigger, Gosu::ZPos _layer)
{
	m_Layers[_layer].triggers.push_back( _trigger );
}

void Scene::evalJSON( json::grammar<char>::array _array, int _layer )
{
	std::string type;
	json::grammar<char>::array::const_iterator it;
	for (it = _array.begin(); it != _array.end(); ++it) {
		type = m_jFile->get<std::string>("Type", *it);
		if (type == "Sprite") {
			evalSprite( it, _layer );
		}
		if (type == "Trigger" ) {
			evalTrigger( it, _layer );
		}
		if (type == "Key") {
			evalKey( it, _layer );
		}
		if (type == "Wall") {
			evalWall( it, _layer );
		}
		// String, functor pairs for others? meh
	}
}

void Scene::evalSprite( json::grammar<char>::array::const_iterator _it, int _layer )
{
	Sprite* tSprite = m_rendMan.createSprite(_layer,
		Gosu::resourcePrefix() + L"Images/" + Gosu::widen( m_jFile->get<std::string>("Image", *_it) ));

	std::string tString = m_jFile->get<std::string>("Name", *_it);
	
	tSprite->setX(m_jFile->get< double >("Position[0]", *_it) * m_Layers[_layer].scale);
	tSprite->setY(m_jFile->get< double >("Position[1]", *_it) * m_Layers[_layer].scale);
	tSprite->setAngle(m_jFile->get< double >("Rotation", *_it));
	tSprite->setColorMod(
		Gosu::Color( m_jFile->get< int >("ColorMod[0]", *_it), 
					m_jFile->get< int >("ColorMod[1]", *_it), 
					m_jFile->get< int >("ColorMod[2]", *_it), 
					m_jFile->get< int >("ColorMod[3]", *_it) ) );
	tSprite->setScaling( 
		m_jFile->get< double >("xScale", *_it), 
		m_jFile->get< double >("yScale", *_it) );
	//Store sprite if we care
	// I don't...
}

void Scene::evalTrigger( json::grammar<char>::array::const_iterator _it, int _layer )
{
	Trigger tTrigger;	
	
	tTrigger.setExtents( m_jFile->get<double>("Position[0]", *_it),
						m_jFile->get<double>("Position[1]", *_it),
						m_jFile->get<double>("Extents[0]", *_it),
						m_jFile->get<double>("Extents[1]", *_it));
	//Store trigger if we care
	// I don't...
}

void Scene::evalWall( json::grammar<char>::array::const_iterator _it, int _layer )
{
	//Wall tWall;
	std::string tString = m_jFile->get<std::string>("Class", *_it);
	
	//m_Layers[_layer].walls.push_back( tWall );
}

void Scene::evalKey( json::grammar<char>::array::const_iterator _it, int _layer )
{
	//Key tKey;	
	
	//m_Layers[_layer].keys.push_back( tKey );
}

void Scene::tellPlayer( Player *_player )
{
	m_Player = _player;
	m_Player->setPhysics( m_PlayerPos.x, m_PlayerPos.y, m_World);
	m_Player->setLayer( 0 );
}

void Scene::update()
{
	// Step physics simulation
	//m_Worldp->Step(m_TimeStep, m_Iterations);
	//m_Orientation += 0.01;
	
	m_Player->update();
	b2Vec2 m_PlayerPos = m_Player->getPosition();

	m_rendMan.updateSpriteSheets();
	InputManager* input = InputManager::getCurrentContext();
	
	// Test Zoom
	if (m_Engine->input().down(Gosu::kbUp) && m_Zoom < 2.0) {
		m_Zoom += 0.01;
	}
	if (m_Engine->input().down(Gosu::kbDown) && m_Zoom > 1.0) {
		m_Zoom -= 0.01;
	}
	
	// Test Rotation
	if (m_Engine->input().down(Gosu::kbQ)) {
		m_Rot += 0.5;
	}
	if (m_Engine->input().down(Gosu::kbE)) {
		m_Rot -= 0.5;
	}
	
	// We need to know where to draw (temporary, this will be done via script)
	m_Focus[0] = m_PlayerPos.x;
	m_Focus[1] = m_PlayerPos.y - 4.0;
	
	// Set screen offset from world focus point
	m_Offset[0] = m_Focus[0]*m_Scale*m_Zoom - m_Width/2;
	m_Offset[1] = m_Height - m_Focus[1]*m_Scale*m_Zoom - m_Height/2;

	m_rendMan.setCamera( m_Focus[0], m_Focus[1], m_Zoom, m_Rot);
	m_audMan.setCamera( m_Focus[0], m_Focus[1], m_Zoom, m_Rot);
	
	// Update scene objects
	// Do callbacks for areas
	
	b2AABB space;
	space.lowerBound = b2Vec2(m_Focus[0] - m_Width/2, m_Focus[1] - m_Height/2);
	space.upperBound = b2Vec2(m_Focus[0] + m_Width/2, m_Focus[1] + m_Height/2);
	
	std::map< Gosu::ZPos, SpriteLayer >::iterator itL;
	for (itL = m_Layers.begin(); itL != m_Layers.end(); ++itL) {
		std::list< Trigger >::iterator it;
		for (it = itL->second.triggers.begin(); it != itL->second.triggers.end(); it++) {
			if (it->overlap(space)) {
				if (!it->inCamera()) {
					it->onEnterCamera();
				}
			}else{
				if (it->inCamera()) {
					it->onLeaveCamera();
				}
			}
			
			if (it->pointIn(m_PlayerPos)) {
				if (!it->playerIn()) {
					it->onPlayerEnter();
				}
			}else{
				if (it->playerIn()) {
					it->onPlayerLeave();
				}
			}
		}
		// update sceneobjects
		// re-order by Y value if desired
	}
}

void Scene::draw() const
{
	// Canvas color
	m_Engine->graphics().drawQuad( 0, 0, m_canvasColor, 
		m_Width, 0, m_canvasColor,
		0, m_Height, m_canvasColor,
		m_Width, m_Height, m_canvasColor, -10);
	
	// Render all sprites
	m_rendMan.doRender();
}
