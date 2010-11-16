/*
 PlanetObj.h
 Mizzou Game Engine
 
 Created by Chad Godsey on 7/12/09.
 
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
 
#include "../../src/Scene/SceneObject.h"
/* 
class PlanetObj : public SceneObject
{
public:
	PlanetObj();
	
	//void setSprite(Sprite *_sprite);
	// CALL SET POSITION FIRST
	void setPhysics(double _radius, b2Body *_body, b2World *_world);
	void setPhysics(double _radius, b2World *_world);

	double getMass() { return m_Mass; }
	
	//void update();
	
	void onColStart(SceneObject *other, b2ContactPoint point);
	void onColPersist(SceneObject *other, b2ContactPoint point);
	void onColFinish(SceneObject *other, b2ContactPoint point);
	/*
	// Used for manipulation
	b2Vec2 getPosition();
	void setPosition( double _x, double _y);
	void setRotation( double ang );
	* /
	// Query stuff
	double getGravity( double x, double y);
	
private:
	Sprite *m_Sprite;
	
	b2Body *m_Body;
	b2World *m_World;
	//b2Vec2 m_Pos;
	double m_Mass;

};
*/
class PlanetComp : public Component
{
public:
	PlanetComp(SceneObject *_obj);
	
	//Returns the name of this component type
	std::string name() { return std::string("Planet"); }
	
	//logic update callback
	void update();

	/// Physics callback
	void onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold);
	void onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold);

	/// Message passing
	void onMessage(std::string _message);

	//Serialization/Deserialization
	void encodeWith(Json::Value *_val);
	void initWith(Json::Value _val);


	double getMass() { return m_Mass; }
	
	// Query stuff
	double getGravity( double x, double y);
	
private:
	double m_Mass;

};

/// Builder for Trigger component
class Planetcom_maker : public Component_maker
{
public:
	Planetcom_maker() : Component_maker("Planet") {}
protected:
	Component* makeComponent(SceneObject *_obj);
	static Planetcom_maker s_RegisteredMaker;
};

/*
class PlanetObj
{
public:
	PlanetObj();
	
	// CALL SET POSITION FIRST
	void setPhysics(double _radius, b2Body *_body, b2World *_world);
	void setPhysics(double _radius, b2World *_world);

	double getMass() { return m_Mass; }
	
	void update();
	
	// Query stuff
	double getGravity( double x, double y);
	
private:
	SceneObject m_Obj;
	RenderComponent m_Rend;
	TransformComponent m_Trans;
	PhysComponent m_Phys;

	double m_Mass;

};
*/