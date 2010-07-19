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

SceneGraph::SceneGraph()
	: m_CurIndex(1), m_SceneRoot(1), m_GroupRoot( &m_SceneRoot )
{
	m_SceneRoot.addComponent( &m_GroupRoot );
}

SceneGraph::~SceneGraph()
{
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
		}
	}
}

//Recursive group creation
void SceneGraph::createGroup(Json::Value _val, GroupComponent *_grp)
{
	SceneObject *obj = new SceneObject(++m_CurIndex);
	GroupComponent *grp = new GroupComponent(obj);
	obj->addComponent( grp );
	_grp->assign(_val.get("GroupName", "").asString(), obj);
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
}

SceneObject* SceneGraph::operator[](std::string _name)
{
	return m_GroupRoot[_name];
}

SceneObject* SceneGraph::operator[](int _id)
{
	return m_GroupRoot[_id];
}