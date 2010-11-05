/*
 SceneObject.cpp
 My Unnamed Game Engine
 
 Created by Chad Godsey on 1/9/08.
 
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

#include "SceneObject.h"
#include "SceneGraph.h"
#include "../Graphics/RenderManager.h"
#include "../Graphics/Sprite.h"
#include "../Graphics/SpriteSheet.h"

std::map<std::string, Component_maker*> *Component_maker::s_makerMap = 0;

Component* Component_maker::makeComponent(std::string _name, SceneObject *_obj)
{
	try {
		// find the appropriate factory in the map of factories...
		Component_maker *maker = (*s_makerMap->find(_name)).second;
		// use that factory to construct the net_message derivative
		return maker->makeComponent(_obj);
	} catch(...) {
		throw std::runtime_error("Component type unknown: " + _name);
	}
	return(NULL);
}

bool Component_maker::ComponentExists(std::string _name)
{
	return (s_makerMap->find(_name) != s_makerMap->end());
}

//===================================================================
//	Physics Component

PhysComponent::PhysComponent(SceneObject *_obj)
	: Component(_obj), m_Body(NULL), m_Frozen(false)
{
}

void PhysComponent::update()
{
	if (m_Obj->hasComponent("Transform")) {
		TransformComponent* tc = (TransformComponent*)m_Obj->getComponent("Transform");
		b2Vec2 pos = m_Body->GetWorldCenter();
		tc->setPosition((double)pos.x, (double)pos.y);
	}
}

void PhysComponent::encodeWith(Json::Value *_val)
{
	if (m_Body->GetType() == b2BodyType::b2_staticBody) {
		(*_val)["Type"] = Json::Value("Static");
	}
	if (m_Body->GetType() == b2BodyType::b2_dynamicBody) {
		(*_val)["Type"] = Json::Value("Dynamic");
	}
	if (m_Body->GetType() == b2BodyType::b2_kinematicBody) {
		(*_val)["Type"] = Json::Value("Kinetic");
	}
	(*_val)["Rotation"] = Json::Value((double)m_Body->GetAngle());
	(*_val)["Filter"] = Json::Value(m_Body->GetFixtureList()->GetFilterData().categoryBits);
	(*_val)["Position"][0u] = Json::Value(m_Body->GetPosition().x);
	(*_val)["Position"][1u] = Json::Value(m_Body->GetPosition().y);

	b2Fixture *fixs = m_Body->GetFixtureList();
	for (b2Fixture* f = fixs; f; f = f->GetNext()){
		Json::Value jVal;
		b2Shape* shape = f->GetShape();
		//jVal["Position"][0u] = 
		switch (f->GetType()) {
		case b2Shape::e_circle:
			break;
		case b2Shape::e_polygon:
			if (isRectangle((b2PolygonShape*)f->GetShape())) {

			}else{

			}
			break;
		}
	}
}

void PhysComponent::initWith(Json::Value _val)
{
	//Create our body
	b2BodyDef bDef;
	bDef.userData = m_Obj;
	bDef.position.Set( (float32)_val["Position"].get(0u, 0.0).asDouble(), (float32)_val["Position"].get(1u, 0.0).asDouble() );
	bDef.angle = (float32)_val.get("Angle", 0.0).asDouble();
	//Extract body type, default to Dynamic
	std::string tString = _val.get("Type", "").asString();
	bDef.type = b2_dynamicBody;
	if (tString == "Static") {
		bDef.type = b2_staticBody;
	}
	if (tString == "Kinematic") {
		bDef.type = b2_kinematicBody;
	}
	m_Body = SceneGraph::getCurrentContext()->getPhysics()->CreateBody(&bDef);

	//Create our fixtures
	uint16 filter = _val.get("Filter", 0).asInt();
	if (!_val["Shapes"].isArray())
		return;
	for (unsigned int i = 0; i < _val.size(); ++i) {
		tString = _val["Shapes"][i].get("Type", "").asString();
		if (tString == "Rectangle") {
			b2PolygonShape poly;
			poly.SetAsBox((float32)_val["Shapes"][i].get("Width", 0.0).asDouble(), (float32)_val["Shapes"][i].get("Height", 0.0).asDouble(),
						b2Vec2((float32)_val["Shapes"][i]["Position"].get(0u, 0.0).asDouble(), (float32)_val["Shapes"][i]["Position"].get(1u, 0.0).asDouble()), 0.f);
			
			b2FixtureDef fix;
			fix.density = (float32)_val["Shapes"][i].get("Density", 1.0).asDouble();
			fix.friction = (float32)_val["Shapes"][i].get("Friction", 1.0).asDouble();
			fix.restitution = (float32)_val["Shapes"][i].get("Restitution", 1.0).asDouble();
			fix.filter.categoryBits = filter;
			fix.shape = &poly;
			
			m_Body->CreateFixture(&fix);
		}
		if (tString == "Circle") {
			b2CircleShape circle;
			circle.m_radius = (float32)_val["Shapes"][i].get("Radius", 1.0).asDouble();
			circle.m_p.Set((float32)_val["Shapes"][i]["Position"].get(0u, 0.0).asDouble(), (float32)_val["Shapes"][i]["Position"].get(1u, 0.0).asDouble());
			
			b2FixtureDef fix;
			fix.density = (float32)_val["Shapes"][i].get("Density", 1.0).asDouble();
			fix.friction = (float32)_val["Shapes"][i].get("Friction", 1.0).asDouble();
			fix.restitution = (float32)_val["Shapes"][i].get("Restitution", 1.0).asDouble();
			fix.filter.categoryBits = filter;
			fix.shape = &circle;
			
			m_Body->CreateFixture(&fix);
		}
		if (tString == "Polygon") {
			b2PolygonShape poly;
			poly.m_vertexCount = 0;
			b2Vec2 vertices[b2_maxPolygonVertices];
			// Verts
			Json::Value jArr = _val["Shapes"][i]["Vertices"];
			for (unsigned int j = 0; j < jArr.size(); ++j) {
				vertices[poly.m_vertexCount++].Set(
					(float32)jArr[j].get(0u, 0.0).asDouble(),
					(float32)jArr[j].get(1u, 0.0).asDouble());
			}
			poly.Set(vertices, poly.m_vertexCount);
			
			b2FixtureDef fix;
			fix.density = (float32)_val["Shapes"][i].get("Density", 1.0).asDouble();
			fix.friction = (float32)_val["Shapes"][i].get("Friction", 1.0).asDouble();
			fix.restitution = (float32)_val["Shapes"][i].get("Restitution", 1.0).asDouble();
			fix.filter.categoryBits = filter;
			fix.shape = &poly;
			
			m_Body->CreateFixture(&fix);
		}
	}
}

bool PhysComponent::isRectangle( b2PolygonShape *_poly )
{
	if (_poly->GetVertexCount() != 4)
		return false;
	//Gosu::angle();
	return true;
}

Physcom_maker Physcom_maker::s_RegisteredMaker;

Component* Physcom_maker::makeComponent(SceneObject *_obj)
{
	return new PhysComponent(_obj);
}


//===================================================================
//	Transform Component

TransformComponent::TransformComponent(SceneObject *_obj)
	: Component(_obj), m_Rotation(0.0), m_PosX(0.0), m_PosY(0.0)
{
}

void TransformComponent::encodeWith(Json::Value *_val)
{
	(*_val)["Position"][0u] = m_PosX;
	(*_val)["Position"][1u] = m_PosY;
	(*_val)["Rotation"] = m_Rotation;
}

void TransformComponent::initWith(Json::Value _val)
{
	m_PosX = _val["Position"].get(0u, 0.0).asDouble();
	m_PosY = _val["Position"].get(1u, 0.0).asDouble();
	m_Rotation = _val.get("Rotation", 0.0).asDouble();
}

Transcom_maker Transcom_maker::s_RegisteredMaker;

Component* Transcom_maker::makeComponent(SceneObject *_obj)
{
	return new TransformComponent(_obj);
}


//===================================================================
//	Render Component

RenderComponent::RenderComponent(SceneObject *_obj)
	: Component(_obj), m_Sprite(NULL), m_SpriteSheet(NULL), m_hidden(true)
{
}

void RenderComponent::update()
{
	//hmm
	if (m_Sprite) {
		if (m_Obj->hasComponent("Transform")) {
			TransformComponent* tc = (TransformComponent*)m_Obj->getComponent("Transform");
			m_Sprite->setX(tc->getPositionX());
			m_Sprite->setY(tc->getPositionY());
		}
	}
	if (m_SpriteSheet) {
		m_SpriteSheet->update();
		if (m_Obj->hasComponent("Transform")) {
			TransformComponent* tc = (TransformComponent*)m_Obj->getComponent("Transform");
			m_SpriteSheet->setX(tc->getPositionX());
			m_SpriteSheet->setY(tc->getPositionY());
		}
	}
}

void RenderComponent::encodeWith(Json::Value *_val)
{
	if (m_Sprite) {
		(*_val)["Type"] = Json::Value("Sprite");
		(*_val)["Image"] = Json::Value(Gosu::narrow(m_Sprite->fileName()));
		(*_val)["Layer"] = Json::Value(m_Sprite->layer());
		
		(*_val)["xScale"] = Json::Value(m_Sprite->factX());
		(*_val)["yScale"] = Json::Value(m_Sprite->factY());
		(*_val)["CenterX"] = Json::Value(m_Sprite->centerX());
		(*_val)["CenterY"] = Json::Value(m_Sprite->centerY());
		
		(*_val)["Position"][0u] = Json::Value(m_Sprite->posX());
		(*_val)["Position"][1u] = Json::Value(m_Sprite->posY());
		
		Gosu::Color color = m_Sprite->colorMod();
		(*_val)["ColorMod"][0u] = Json::Value(color.alpha());
		(*_val)["ColorMod"][1u] = Json::Value(color.red());
		(*_val)["ColorMod"][2u] = Json::Value(color.green());
		(*_val)["ColorMod"][3u] = Json::Value(color.blue());
	}else{
		(*_val)["Type"] = Json::Value("SpriteSheet");
		(*_val)["Image"] = Json::Value(Gosu::narrow(m_SpriteSheet->fileName()));
		(*_val)["Layer"] = Json::Value(m_SpriteSheet->layer());
		(*_val)["Width"] = Json::Value(m_SpriteSheet->width());
		(*_val)["Height"] = Json::Value(m_SpriteSheet->height());
		(*_val)["Duration"] = Json::Value(m_SpriteSheet->delay());
		
		(*_val)["xScale"] = Json::Value(m_SpriteSheet->factX());
		(*_val)["yScale"] = Json::Value(m_SpriteSheet->factY());
		(*_val)["CenterX"] = Json::Value(m_SpriteSheet->centerX());
		(*_val)["CenterY"] = Json::Value(m_SpriteSheet->centerY());
		
		(*_val)["Position"][0u] = Json::Value(m_SpriteSheet->posX());
		(*_val)["Position"][1u] = Json::Value(m_SpriteSheet->posY());
		
		Gosu::Color color = m_SpriteSheet->colorMod();
		(*_val)["ColorMod"][0u] = Json::Value(color.alpha());
		(*_val)["ColorMod"][1u] = Json::Value(color.red());
		(*_val)["ColorMod"][2u] = Json::Value(color.green());
		(*_val)["ColorMod"][3u] = Json::Value(color.blue());
	//int layer = _val.get("Layer", 0).asInt();
	//int width = _val.get("Width", 0 ).asInt();
	//int height = _val.get("Height", 0 ).asInt();
	//int duration = _val.get("Duration", 20 ).asInt();
	}
	//std::wstring file = Gosu::resourcePrefix() + L"Images/" + Gosu::widen( _val.get("Image", "defaultimg.png").asString() );
	//int layer = _val.get("Layer", 0).asInt();
	//m_Sprite = RenderManager::getCurrentContext()->createSprite( layer, file);

	//m_Sprite->setX(_val["Position"].get(0u, 0.0 ).asDouble());
	//m_Sprite->setY(_val["Position"].get(1u, 0.0 ).asDouble());
	//m_Sprite->setAngle(_val.get("Rotation", 0.0 ).asDouble());
	//m_Sprite->setColorMod(
	//	Gosu::Color( _val["ColorMod"].get(0u, 255 ).asInt(), 
	//				_val["ColorMod"].get(1u, 255 ).asInt(), 
	//				_val["ColorMod"].get(2u, 255 ).asInt(), 
	//				_val["ColorMod"].get(3u, 255 ).asInt() ) );
	//m_Sprite->setScaling( 
	//	_val.get("xScale", 1.0).asDouble(), 
	//	_val.get("yScale", 1.0).asDouble() );

	//std::string tString = _val.get("AlphaMode", "amDefault" ).asString();
}

void RenderComponent::initWith(Json::Value _val)
{
	std::string tstr = _val["Type"].asString();
	if (tstr == "Sprite")
		initSprite( _val);
	if (tstr == "SpriteSheet")
		initSpriteSheet( _val);
	//Other
}

void RenderComponent::initSprite(Json::Value _val)
{
	std::wstring file = Gosu::resourcePrefix() + L"Images/" + Gosu::widen( _val.get("Image", "defaultimg.png").asString() );
	int layer = _val.get("Layer", 0).asInt();
	m_Sprite = RenderManager::getCurrentContext()->createSprite( layer, file);

	m_Sprite->setFileName( Gosu::widen( _val.get("Image", "defaultimg.png").asString() ) );

	m_Sprite->setX(_val["Position"].get(0u, 0.0 ).asDouble());
	m_Sprite->setY(_val["Position"].get(1u, 0.0 ).asDouble());
	m_Sprite->setAngle(_val.get("Rotation", 0.0 ).asDouble());
	m_Sprite->setColorMod(
		Gosu::Color( _val["ColorMod"].get(0u, 255 ).asInt(), 
					_val["ColorMod"].get(1u, 255 ).asInt(), 
					_val["ColorMod"].get(2u, 255 ).asInt(), 
					_val["ColorMod"].get(3u, 255 ).asInt() ) );
	m_Sprite->setScaling( 
		_val.get("xScale", 1.0).asDouble(), 
		_val.get("yScale", 1.0).asDouble() );
	m_Sprite->setCenter( 
		_val.get("CenterX", 0.5).asDouble(), 
		_val.get("CenterY", 0.5).asDouble() );

	std::string tString = _val.get("AlphaMode", "amDefault" ).asString();
	if (tString == "amAdditive") {
		m_Sprite->setAlphaMode(Gosu::amAdditive);
	}else{
		m_Sprite->setAlphaMode(Gosu::amDefault);
	}
}

void RenderComponent::initSpriteSheet(Json::Value _val)
{
	std::wstring filename = Gosu::resourcePrefix() + L"Images/" + Gosu::widen( _val.get("Image", "defaultimg.png").asString() );

	int layer = _val.get("Layer", 0).asInt();
	int width = _val.get("Width", 0 ).asInt();
	int height = _val.get("Height", 0 ).asInt();
	int duration = _val.get("Duration", 20 ).asInt();

	m_SpriteSheet = RenderManager::getCurrentContext()->createSpriteSheet( layer, filename, width, height, duration);
	
	m_SpriteSheet->setFileName( Gosu::widen( _val.get("Image", "defaultimg.png").asString() ) );
	m_SpriteSheet->setX(_val["Position"].get(0u, 0.0 ).asDouble());
	m_SpriteSheet->setY(_val["Position"].get(1u, 0.0 ).asDouble());
	m_SpriteSheet->setAngle(_val.get("Rotation", 0.0 ).asDouble());
	m_SpriteSheet->setColorMod(
		Gosu::Color( _val["ColorMod"].get(0u, 255 ).asInt(), 
					_val["ColorMod"].get(1u, 255 ).asInt(), 
					_val["ColorMod"].get(2u, 255 ).asInt(), 
					_val["ColorMod"].get(3u, 255 ).asInt() ) );
	m_SpriteSheet->setScaling( 
		_val.get("xScale", 1.0).asDouble(), 
		_val.get("yScale", 1.0).asDouble() );
	m_SpriteSheet->setCenter( 
		_val.get("CenterX", 0.5).asDouble(), 
		_val.get("CenterY", 0.5).asDouble() );

	std::string tString = _val.get("AlphaMode", "amDefault" ).asString();
	if (tString == "amAdditive") {
		m_SpriteSheet->setAlphaMode(Gosu::amAdditive);
	}else{
		m_SpriteSheet->setAlphaMode(Gosu::amDefault);
	}
}

/// Disable rendering for this object 
void RenderComponent::hide() 
{
	m_hidden = false;
	if (m_Sprite)
		m_Sprite->setVisible(false);
	if (m_SpriteSheet)
		m_SpriteSheet->setVisible(false);
}

/// Enable rendering for this object 
void RenderComponent::show() 
{
	m_hidden = true;
	if (m_Sprite)
		m_Sprite->setVisible(true);
	if (m_SpriteSheet)
		m_SpriteSheet->setVisible(true);
}

void RenderComponent::setSprite( Sprite *_sprite )
{
	m_Sprite = _sprite;
	m_SpriteSheet = NULL;
}

void RenderComponent::setSpriteSheet( SpriteSheet *_anim)
{
	m_SpriteSheet = _anim;
	m_Sprite = NULL;
}

Rendcom_maker Rendcom_maker::s_RegisteredMaker;

Component* Rendcom_maker::makeComponent(SceneObject *_obj)
{
	return new RenderComponent(_obj);
}


//===================================================================
//	Group Component

GroupComponent::GroupComponent(SceneObject *_obj)
	: Component(_obj)
{
}

void GroupComponent::update()
{
	std::map< int, SceneObject* >::iterator it;
	for (it = m_Objects.begin(); it != m_Objects.end(); it++) {
		it->second->update();
	}
}

void GroupComponent::onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
	std::map< int, SceneObject* >::iterator it;
	for (it = m_Objects.begin(); it != m_Objects.end(); it++) {
		it->second->onColStart(_fix, _other, _manifold);
	}
}
void GroupComponent::onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
	std::map< int, SceneObject* >::iterator it;
	for (it = m_Objects.begin(); it != m_Objects.end(); it++) {
		it->second->onColFinish(_fix, _other, _manifold);
	}
}

	/// Message passing
void GroupComponent::onMessage(std::string _message)
{
	std::map< int, SceneObject* >::iterator it;
	for (it = m_Objects.begin(); it != m_Objects.end(); it++) {
		it->second->onMessage(_message);
	}
}

void GroupComponent::encodeWith(Json::Value *_val)
{
	std::map< std::string, SceneObject* >::iterator it;
	int i;
	for (i = 0, it = m_ObjectMap.begin(); it != m_ObjectMap.end(); it++, i++) {
		if (it->second->hasComponent("SceneGroup")) {
			Json::Value jVal, jChildren;
			jVal["GroupName"] = Json::Value(it->first);
			it->second->encodeWith( &jChildren );
			jVal["Children"] = jChildren;
			_val->append( jVal );
		}else{
			Json::Value jVal;
			jVal["Name"] = Json::Value(it->first);
			it->second->encodeWith( &jVal );
			_val->append( jVal );
		}
	}
}

void GroupComponent::initWith(Json::Value _val)
{
}

SceneObject* GroupComponent::operator[](std::string _name)
{
	if (m_ObjectMap.find(_name) != m_ObjectMap.end())
		return m_ObjectMap[_name];
	std::map< int, SceneObject* >::iterator it;
	for (it = m_Objects.begin(); it != m_Objects.end(); it++) {
		if (it->second->hasComponent("SceneGroup")) {
			SceneObject* obj = ((GroupComponent*)(it->second->getComponent("SceneGroup")))->operator[](_name);
			if (obj != NULL)
				return obj;
		}
	}
	return NULL;
}

SceneObject* GroupComponent::operator[](int _id)
{
	std::map< int, SceneObject* >::iterator it;
	for (it = m_Objects.begin(); it != m_Objects.end(); it++) {
		if (it->second->ID() == _id) {
			return it->second;
		}
		if (it->second->hasComponent("SceneGroup")) {
			SceneObject* obj = ((GroupComponent*)(it->second->getComponent("SceneGroup")))->operator[](_id);
			if (obj != NULL)
				return obj;
		}
	}
	return NULL;
}

void GroupComponent::assign(std::string _name, SceneObject* _obj)
{
	m_ObjectMap[_name] = _obj;
	m_Objects[_obj->ID()] = _obj;
}

bool GroupComponent::deleteObject(SceneObject* _object)
{
	bool found = false;
	std::map<int, SceneObject*>::iterator erase;
	std::map< int, SceneObject* >::iterator it = m_Objects.begin();
	while (it != m_Objects.end()) {
		if (it->second == _object) {
			m_ObjectMap.erase(it->second->Name());
			erase = it;
			it++;
			m_Objects.erase(erase);
			continue;
		}else
			it++;
	}
	return found;
}

Groupcom_maker Groupcom_maker::s_RegisteredMaker;

Component* Groupcom_maker::makeComponent(SceneObject *_obj)
{
	return new GroupComponent(_obj);
}


//===================================================================
//	Trigger Component

TriggerComponent::TriggerComponent(SceneObject *_obj)
: Component(_obj)
{
}

void TriggerComponent::update()
{
	//Query b2World for intersecting objects
	//Maintain a list of intersecting, entering and exiting
}

void TriggerComponent::onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
	
}
void TriggerComponent::onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold)
{
	
}

/// Message passing
void TriggerComponent::onMessage(std::string _message)
{
}

void TriggerComponent::setExtents( double _top, double _left, double _bottom, double _right)
{
	m_Box.upperBound.Set( (float32)_left, (float32)_top );
	m_Box.lowerBound.Set( (float32)_right, (float32)_bottom );
}

bool TriggerComponent::pointIn(b2Vec2 &_point)
{
	return ( _point.x > m_Box.upperBound.x && _point.x < m_Box.lowerBound.x) &&
	(_point.y > m_Box.lowerBound.y && _point.y < m_Box.upperBound.y);
}
/*
void TriggerComponent::onEnterCamera()
{
	m_inCamera = true;
}

void TriggerComponent::onLeaveCamera()
{
	m_inCamera = false;
}

void TriggerComponent::onPlayerEnter()
{
	m_inPlayer = true;
}

void TriggerComponent::onPlayerLeave()
{
	m_inPlayer = false;
}
}
*/

void TriggerComponent::encodeWith(Json::Value *_val)
{
}

void TriggerComponent::initWith(Json::Value _val)
{
}


Triggcom_maker Triggcom_maker::s_RegisteredMaker;

Component* Triggcom_maker::makeComponent(SceneObject *_obj)
{
	return new TriggerComponent(_obj);
}


//===================================================================
//	Scene Object Base

SceneObject::SceneObject(int _id)
	: m_ID(_id), m_Name("")
{
}

void SceneObject::update()
{
	std::map< std::string, Component* >::iterator cit;
	for (cit = m_Components.begin(); cit != m_Components.end(); cit++) {
		cit->second->update();
	}
}

void SceneObject::onColStart( b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold) 
{
	std::map< std::string, Component* >::iterator cit;
	for (cit = m_Components.begin(); cit != m_Components.end(); cit++) {
		cit->second->onColStart(_fix, _other, _manifold);
	}
}

void SceneObject::onColFinish( b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold) 
{
	std::map< std::string, Component* >::iterator cit;
	for (cit = m_Components.begin(); cit != m_Components.end(); cit++) {
		cit->second->onColFinish(_fix, _other, _manifold);
	}
}

void SceneObject::onMessage(std::string _message) 
{
	std::map< std::string, Component* >::iterator cit;
	for (cit = m_Components.begin(); cit != m_Components.end(); cit++) {
		cit->second->onMessage(_message);
	}
}

void SceneObject::encodeWith(Json::Value *_val) 
{
	std::map< std::string, Component* >::iterator cit;
	for (cit = m_Components.begin(); cit != m_Components.end(); cit++) {
		Json::Value jComp;
		cit->second->encodeWith( &jComp );
		(*_val)[cit->first] = jComp;
	}
}

void SceneObject::initWith(Json::Value _val) 
{
	Json::Value::Members mems = _val.getMemberNames();
	Json::Value::Members::iterator mit;
	for (mit = mems.begin(); mit != mems.end(); mit++) {
		if ((*mit) == "Name") {
			m_Name = _val["Name"].asString();
		}else{
			if (Component_maker::ComponentExists((*mit))) {
				addComponent(Component_maker::makeComponent((*mit), this));
				m_Components[(*mit)]->initWith(_val[(*mit)]);
			}
		}
	}
}

bool SceneObject::hasComponent(std::string _name) 
{
	if (m_Components.find(_name) != m_Components.end())
		return true;
	return false;
}

Component* SceneObject::getComponent(std::string _name) 
{
	if (m_Components.find(_name) != m_Components.end())
		return m_Components[_name];
	return NULL;
}

bool SceneObject::addComponent(Component* _com) 
{
	if (m_Components.find(_com->name()) == m_Components.end()) {
		m_Components[_com->name()] = _com;
		return true;
	}
	return false;
}

	
//===================================================================
//	Trigger Object
/*
Trigger::Trigger()
{
}

void Trigger::setExtents( double _top, double _left, double _bottom, double _right)
{
	m_Box.upperBound.Set( (float32)_left, (float32)_top );
	m_Box.lowerBound.Set( (float32)_right, (float32)_bottom );
}

bool Trigger::pointIn(b2Vec2 &_point)
{
	return ( _point.x > m_Box.upperBound.x && _point.x < m_Box.lowerBound.x) &&
		(_point.y > m_Box.lowerBound.y && _point.y < m_Box.upperBound.y);
}

void Trigger::onEnterCamera()
{
	m_inCamera = true;
}

void Trigger::onLeaveCamera()
{
	m_inCamera = false;
}

void Trigger::onPlayerEnter()
{
	m_inPlayer = true;
}

void Trigger::onPlayerLeave()
{
	m_inPlayer = false;
}
*/