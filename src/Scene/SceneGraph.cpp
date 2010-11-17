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

	// Physics joints must be created after objects
	//e_revoluteJoint,
	//e_prismaticJoint,
	//e_distanceJoint,
	//e_pulleyJoint,
	//e_mouseJoint,
	//e_gearJoint,
	//e_lineJoint,
    //e_weldJoint,
	//e_frictionJoint, //figure out what this one is!
	
	for (int i = 0; i < m_json["Joints"].size(); ++i) {
		if (m_json["Joints"][i]["Type"] == "Revolute") {
			b2RevoluteJointDef jDef;
			jDef.Initialize(
				&((PhysComponent*)m_GroupRoot[m_json["Joints"][i].get("ObjectA", "").asString()]->getComponent("Physics"))->getBody(),
				&((PhysComponent*)m_GroupRoot[m_json["Joints"][i].get("ObjectB", "").asString()]->getComponent("Physics"))->getBody(), 
				b2Vec2(
					m_json["Joints"][i]["Position"].get(0u, 0.0).asDouble(),
					m_json["Joints"][i]["Position"].get(1u, 0.0).asDouble()));
			jDef.upperAngle = m_json["Joints"][i].get("upperAngle", 0.0).asDouble();
			jDef.lowerAngle = m_json["Joints"][i].get("lowerAngle", 0.0).asDouble();
			jDef.enableMotor = m_json["Joints"][i].get("enableMotor", false).asBool();
			jDef.enableLimit = m_json["Joints"][i].get("enableLimit", false).asBool();
			jDef.maxMotorTorque = m_json["Joints"][i].get("maxMotorTorque", 0.0).asDouble();
			jDef.motorSpeed = m_json["Joints"][i].get("motorSpeed", 0.0).asDouble();
			m_World->CreateJoint(&jDef);
		}
		if (m_json["Joints"][i]["Type"] == "Prismatic") {
			b2PrismaticJointDef jDef;
			jDef.Initialize(
				&((PhysComponent*)m_GroupRoot[m_json["Joints"][i].get("ObjectA", "").asString()]->getComponent("Physics"))->getBody(),
				&((PhysComponent*)m_GroupRoot[m_json["Joints"][i].get("ObjectB", "").asString()]->getComponent("Physics"))->getBody(), 
				b2Vec2(
					m_json["Joints"][i]["Position"].get(0u, 0.0).asDouble(),
					m_json["Joints"][i]["Position"].get(1u, 0.0).asDouble()),
				b2Vec2(
					m_json["Joints"][i]["Axis"].get(0u, 1.0).asDouble(),
					m_json["Joints"][i]["Axis"].get(1u, 0.0).asDouble()));
			jDef.lowerTranslation = m_json["Joints"][i].get("lowerTranslation", 0.0).asDouble();
			jDef.upperTranslation = m_json["Joints"][i].get("upperTranslation", 0.0).asDouble();
			jDef.referenceAngle = m_json["Joints"][i].get("referenceAngle", 0.0).asDouble();
			jDef.enableLimit = m_json["Joints"][i].get("enableLimit", false).asBool();
			jDef.enableMotor = m_json["Joints"][i].get("enableMotor", false).asBool();
			jDef.maxMotorForce = m_json["Joints"][i].get("maxMotorForce", 0.0).asDouble();
			jDef.motorSpeed = m_json["Joints"][i].get("motorSpeed", 0.0).asDouble();
			m_World->CreateJoint(&jDef);
		}
		if (m_json["Joints"][i]["Type"] == "Distance") {
			b2DistanceJointDef jDef;
			jDef.Initialize(
				&((PhysComponent*)m_GroupRoot[m_json["Joints"][i].get("ObjectA", "").asString()]->getComponent("Physics"))->getBody(),
				&((PhysComponent*)m_GroupRoot[m_json["Joints"][i].get("ObjectB", "").asString()]->getComponent("Physics"))->getBody(), 
				b2Vec2(
					m_json["Joints"][i]["PositionA"].get(0u, 0.0).asDouble(),
					m_json["Joints"][i]["PositionA"].get(1u, 0.0).asDouble()),
				b2Vec2(
					m_json["Joints"][i]["PositionB"].get(0u, 0.0).asDouble(),
					m_json["Joints"][i]["PositionB"].get(1u, 0.0).asDouble()));
			jDef.length  = m_json["Joints"][i].get("length", 1.0).asDouble();
			jDef.frequencyHz  = m_json["Joints"][i].get("frequencyHz", 0.0).asDouble();
			jDef.dampingRatio   = m_json["Joints"][i].get("dampingRatio", 0.0).asDouble();
			m_World->CreateJoint(&jDef);
		}
		if (m_json["Joints"][i]["Type"] == "Pulley") {
			b2PulleyJointDef jDef;
			jDef.Initialize(
				&((PhysComponent*)m_GroupRoot[m_json["Joints"][i].get("ObjectA", "").asString()]->getComponent("Physics"))->getBody(),
				&((PhysComponent*)m_GroupRoot[m_json["Joints"][i].get("ObjectB", "").asString()]->getComponent("Physics"))->getBody(), 
				b2Vec2(
					m_json["Joints"][i]["GroundAnchorA"].get(0u, -1.0).asDouble(),
					m_json["Joints"][i]["GroundAnchorA"].get(1u, 1.0).asDouble()),
				b2Vec2(
					m_json["Joints"][i]["GroundAnchorB"].get(0u, 1.0).asDouble(),
					m_json["Joints"][i]["GroundAnchorB"].get(1u, 1.0).asDouble()),
				b2Vec2(
					m_json["Joints"][i]["AnchorA"].get(0u, -1.0).asDouble(),
					m_json["Joints"][i]["AnchorA"].get(1u, 0.0).asDouble()),
				b2Vec2(
					m_json["Joints"][i]["AnchorB"].get(0u, 1.0).asDouble(),
					m_json["Joints"][i]["AnchorB"].get(1u, 0.0).asDouble()),
				m_json["Joints"][i].get("ratio", 1.0).asDouble());
			jDef.lengthA  = m_json["Joints"][i].get("lengthA", 0.0).asDouble();
			jDef.maxLengthA  = m_json["Joints"][i].get("maxLengthA", 0.0).asDouble();
			jDef.lengthB  = m_json["Joints"][i].get("lengthB", 0.0).asDouble();
			jDef.maxLengthB  = m_json["Joints"][i].get("maxLengthB", 0.0).asDouble();
			jDef.collideConnected = m_json["Joints"][i].get("collideConnected", true).asBool();
			m_World->CreateJoint(&jDef);
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