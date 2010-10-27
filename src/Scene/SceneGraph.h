/*
   SceneGraph.h
   My Unnamed Game Engine
 
   Created by Chad Godsey on 7/2/10.
  
 Copyright 2010 BlitThis! studios.

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

#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <Gosu/Gosu.hpp>
#include <Box2D.h>
#include <map>
#include <list>
#include <json/json.h>
#include "../Core/Core.h"
#include "SceneObject.h"
#include "Player.h"

struct RendLayer
{
	float scale;
	int layer;
	std::string ID;
};
/*
struct WarpTrigger
{
	Trigger trigger;
	std::string level;
};
*/
class SceneGraph
{
public:
	SceneGraph();
	~SceneGraph();

	void setPhysics( b2World *_world ) { m_World = _world; }
	b2World* getPhysics() { return m_World; }

	void addLocalPlayer(std::string _name, Player* _player);
	//network player to come (probably doesn't need to be different)
	Player* getPlayer(std::string _name);

	SceneObject* createObject(std::string _name);
	bool deleteObject(SceneObject* _object);

	//Call setPhysics first!
	void loadFile(std::wstring _config);

	void update();

	SceneObject* operator[](std::string _name);
	SceneObject* operator[](int _id);

	/// Static accessor to current render manager
	static SceneGraph* getCurrentContext() { return s_CurrentContext; }

	/// Static setter for current render manager
	static void setCurrentContext(SceneGraph* _context) { s_CurrentContext = _context; }

private:
	static SceneGraph* s_CurrentContext;

	void createGroup(Json::Value _val, GroupComponent *_grp);
	// Game Data
	std::wstring m_config;
	Json::Value m_json;

	b2World* m_World;

	unsigned int m_CurIndex;
	SceneObject m_SceneRoot;
	GroupComponent m_GroupRoot;
	std::map< int, SceneObject* > m_Objects;

	std::map<std::string, Player*> m_Players;
	//SceneGroup m_Root;
	//std::map< std::string, SceneGroup > m_Groups;

	//If and when I made a networking layer, this is where it will go
	//The connection will manage the following
	// - incoming/outgoing messages
	// - synchronizing local objects and sending local changes
	// - keep track of what local changes are legal 
	//		(master list on server as final filter, local only to limit traffic)
	// - stuff
	//NETWORKCONNECTION
};

#endif