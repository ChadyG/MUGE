/*
   SceneGraph.cpp
   My Unnamed Game Engine
 
   Created by Chad Godsey on 7/4/10.
  
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

#include "SceneGraph.h"
#include <fstream>

SceneGraph* SceneGraph::s_CurrentContext;

SceneGraph::SceneGraph()
	: m_CurIndex(1), m_SceneRoot(1), m_GroupRoot( &m_SceneRoot )
{
	m_SceneRoot.addComponent( &m_GroupRoot );
}

SceneGraph::~SceneGraph()
{
	//iterator over m_Objects and clean up
}

void SceneGraph::addLocalPlayer(std::string _name, Player* _player)
{
	_player->setID(++m_CurIndex);
	m_Players[_name] = _player;
}

Player* SceneGraph::getPlayer(std::string _name)
{
	if (m_Players.find(_name) != m_Players.end()) {
		Player* obj = m_Players[_name];
		if (obj != NULL)
			return obj;
	}
	return NULL;
}

SceneObject* SceneGraph::createObject(std::string _name)
{
	SceneObject *obj = new SceneObject(++m_CurIndex);
	obj->setName(_name);
	m_GroupRoot.assign(_name, obj);
	m_Objects[obj->ID()] = obj;
	return obj;
}

bool SceneGraph::deleteObject(SceneObject* _object)
{
	bool found = false;
	std::map<int, SceneObject*>::iterator erase;
	std::map<int, SceneObject*>::iterator it = m_Objects.begin();
	while (it != m_Objects.end()) {
		if (it->second == _object) {
			m_GroupRoot.deleteObject(_object);
			delete it->second;
			erase = it;
			it++;
			m_Objects.erase(erase);
			found = true;
			continue;
		}else
			it++;
	}
	return found;
}

void SceneGraph::loadFile(std::wstring _config)
{
	Json::Reader reader;
	reader.parseFile( Gosu::narrow(Gosu::resourcePrefix() + L"Data/" + _config + L".json"), m_json);

	for (int i = 0; i < m_json["Objects"].size(); ++i) {
		if (m_json["Objects"][i].isMember("GroupName")) {
			//Create group
			createGroup(m_json["Objects"][i], &m_GroupRoot);
		}else{
			SceneObject *obj = new SceneObject(++m_CurIndex);
			obj->initWith(m_json["Objects"][i]);
			m_GroupRoot.assign(m_json["Objects"][i].get("Name", "").asString(), obj);
			m_Objects[obj->ID()] = obj;
		}
	}
}

void SceneGraph::writeFile(std::wstring _outfile)
{
	Json::Value jVal;
	Json::StyledWriter writer;
	m_GroupRoot.encodeWith( &jVal );
	writer.writeFile( Gosu::narrow(Gosu::resourcePrefix() + L"Data/" + _outfile + L".json"), jVal );
}

//Recursive group creation
void SceneGraph::createGroup(Json::Value _val, GroupComponent *_grp)
{
	SceneObject *obj = new SceneObject(++m_CurIndex);
	GroupComponent *grp = new GroupComponent(obj);
	obj->addComponent( grp );
	_grp->assign(_val.get("GroupName", "").asString(), obj);
	m_Objects[obj->ID()] = obj;
	for (int i = 0; i < _val["Children"].size(); ++i) {
		if (_val["Children"][i].isMember("GroupName")) {
			//create group
			createGroup(_val["Children"][i], grp);
		}else{
			//create object
			SceneObject *obj = new SceneObject(++m_CurIndex);
			obj->initWith(_val["Children"][i]);
			grp->assign(_val["Children"][i].get("Name", "").asString(), obj);
		}
	}
}

void SceneGraph::update()
{
	m_GroupRoot.update();
	/*
	b2AABB space;
	space.lowerBound = b2Vec2(m_Focus[0] - m_Width/2, m_Focus[1] - m_Height/2);
	space.upperBound = b2Vec2(m_Focus[0] + m_Width/2, m_Focus[1] + m_Height/2);
	
	std::list< Trigger >::iterator it;
	for (it = itL->m_Triggers.begin(); it != m_Triggers.end(); it++) {
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
	*/
}

SceneObject* SceneGraph::operator[](std::string _name)
{
	return m_GroupRoot[_name];
}

SceneObject* SceneGraph::operator[](int _id)
{
	return m_Objects[_id];
}