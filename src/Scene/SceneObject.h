/*
   SceneObject.h
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
 
#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <Gosu/Gosu.hpp>
#include <json/json.h>
#include <Box2D.h>
#include <list>

class SpriteSheet;
class Sprite;
class SceneObject;

/* TODO:
*	add component system to game objects
*	registering a component gives it callbacks during the object's lifetime
*	use for rendering, physics, etc.
*	stored in a map<string, component> where string is the name of the component type
*		This naming allows easy access to component types to determine what an object can do/is
*
*	Components must also implement serialization routines that will be called by SceneObject
*
*	create serialization 'mixin' (inherit)
*	virtual functions for encode(coder)/decode(coder) (or just use JSONFile directly)
*/	
class Component
{
public:
	Component() {}
	Component(SceneObject *_obj) { m_Obj = _obj; }

	//Returns the name of this component type
	virtual std::string name() = 0;

	//logic update callback
	virtual void update() = 0;

	/// Physics callback
	virtual void onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold) = 0;
	virtual void onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold) = 0;

	/// Message passing
	virtual void onMessage(std::string _message) = 0;

	//Serialization/Deserialization
	virtual void encodeWith(Json::Value *_val) = 0;
	virtual void initWith(Json::Value _val) = 0;
protected:
	SceneObject* m_Obj;
};

class Component_maker
{
public:
	Component_maker(std::string _name) {
		if (s_makerMap == 0)
			s_makerMap = new std::map<std::string, Component_maker*>();

		s_makerMap->insert(std::make_pair(_name, this));
	}
	/// Static component registration
	static Component* makeComponent(std::string _name, SceneObject *_obj);
	static bool ComponentExists(std::string _name);
protected:
	static std::map<std::string, Component_maker*> *s_makerMap;
	virtual Component* makeComponent(SceneObject *_obj) = 0;
};

class PhysComponent : public Component
{
public:
	PhysComponent(SceneObject *_obj);

	//Returns the name of this component type
	std::string name() {return std::string("Physics"); }

	//logic update callback
	void update();

	/// Physics callback
	void onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold){}
	void onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold){}

	/// Message passing
	void onMessage(std::string _message){}

	//Serialization/Deserialization
	void encodeWith(Json::Value *_val);
	void initWith(Json::Value _val);
	
	/// Inhibit physics response
	void Freeze() { m_Frozen = true; }
	/// Resume physics operations
	void Thaw() { m_Frozen = false; }

private:
	bool isRectangle( b2PolygonShape *_poly );

	b2Body *m_Body;
	bool m_Frozen;
};

/// Builder for Physics component
class Physcom_maker : public Component_maker
{
public:
	Physcom_maker() : Component_maker("Physics") {}
protected:
	Component* makeComponent(SceneObject *_obj);
	static Physcom_maker s_RegisteredMaker;
};



class TransformComponent : public Component
{
public:
	TransformComponent(SceneObject *_obj);

	//Returns the name of this component type
	std::string name() {return std::string("Transform"); }

	//logic update callback
	void update(){}

	/// Physics callback
	void onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold){}
	void onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold){}

	/// Message passing
	void onMessage(std::string _message){}

	//Serialization/Deserialization
	void encodeWith(Json::Value *_val);
	void initWith(Json::Value _val);

	/// setRotation - sets world Rotation
	void setRotation( double _angle ) { m_Rotation = _angle; }
	/// setPosition - sets world position
	void setPosition( double _x, double _y ) { m_PosX = _x; m_PosY = _y; }
	
	/// getRotation - returns world rotation
	double getRotation() { return m_Rotation; }
	/// getPosition - returns world position
	double getPositionX() { return m_PosX; }
	/// getPosition - returns world position
	double getPositionY() { return m_PosY; }

private:
	double m_PosX, m_PosY, m_Rotation;
};

/// Builder for Transform component
class Transcom_maker : public Component_maker
{
public:
	Transcom_maker() : Component_maker("Transform") {}
protected:
	Component* makeComponent(SceneObject *_obj);
	static Transcom_maker s_RegisteredMaker;
};



class RenderComponent : public Component
{
public:
	RenderComponent(SceneObject *_obj);

	//Returns the name of this component type
	std::string name() { return std::string("Renderable"); }
	
	//logic update callback
	void update();

	/// Physics callback
	void onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold){}
	void onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold){}

	/// Message passing
	void onMessage(std::string _message){}

	//Serialization/Deserialization
	void encodeWith(Json::Value *_val);
	void initWith(Json::Value _val);

	/// Set this object to render as the given sprite
	void setSprite( Sprite *_sprite );
	/// Set this object to render as the given SpriteSheet
	void setSpriteSheet( SpriteSheet *_anim);
	/// Disable rendering for this object 
	void hide();
	/// Enable rendering for this object 
	void show();

	bool hasSprite() { return m_Sprite != NULL; }
	bool hasSpriteSheet() { return m_SpriteSheet != NULL; }

	Sprite* getSprite() { return m_Sprite; }
	SpriteSheet* getSpriteSheet() { return m_SpriteSheet; }

private:
	void initSprite(Json::Value _val);
	void initSpriteSheet(Json::Value _val);

	Sprite *m_Sprite;
	SpriteSheet *m_SpriteSheet;
	bool m_hidden;
};

/// Builder for Render component
class Rendcom_maker : public Component_maker
{
public:
	Rendcom_maker() : Component_maker("Renderable") {}
protected:
	Component* makeComponent(SceneObject *_obj);
	static Rendcom_maker s_RegisteredMaker;
};



class GroupComponent : public Component
{
public:
	GroupComponent(SceneObject *_obj);

	//Returns the name of this component type
	std::string name() { return std::string("SceneGroup"); }
	
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

	/// Array and hash access to inner objects
	SceneObject* operator[](std::string _name);
	SceneObject* operator[](int _id);

	bool deleteObject(SceneObject* _object);

	int size() { return m_Objects.size(); }

	void assign(std::string _name, SceneObject* _obj);

private:
	std::map< std::string, SceneObject* > m_ObjectMap;
	std::map< int, SceneObject* > m_Objects;
};

/// Builder for Group component
class Groupcom_maker : public Component_maker
{
public:
	Groupcom_maker() : Component_maker("SceneGroup") {}
protected:
	Component* makeComponent(SceneObject *_obj);
	static Groupcom_maker s_RegisteredMaker;
};




class TriggerComponent : public Component
{
public:
	TriggerComponent(SceneObject *_obj);

	//Returns the name of this component type
	std::string name() { return std::string("Trigger"); }
	
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



	/// setExtents - sets the dimensions for rectangular trigger areas
	void setExtents( double _top, double _left, double _bottom, double _right);
	
	/// overlap - test if bounding box overlaps this
	bool overlap(b2AABB &_other) { return b2TestOverlap( m_Box, _other ); }
	
	/// pointIn - test if point is within this rectangular box
	bool pointIn(b2Vec2 &_point);
	
	/// inCamera - returns the state of camera focus
	bool inCamera() { return m_inCamera; }
	/*
	/// onEnterCamera - callback for entering focus
	void onEnterCamera();
	
	/// onLeaveCamera - callback for leaving focus
	void onLeaveCamera();
	
	/// playerIn - returns the state of the player with regard to this area
	bool playerIn() { return m_inPlayer; }
	
	/// onPlayerEnter - callback for player entering this area
	void onPlayerEnter();
	
	/// onPlayerLeave - callback for player leaving this area
	void onPlayerLeave();
	*/
private:
	//structure holding extent information
	b2AABB m_Box;
	//booleans for current state of trigger
	bool m_inCamera;
	bool m_inPlayer;
	//lists!
	// at the minimum I need an array of IDs
};


/// Builder for Trigger component
class Triggcom_maker : public Component_maker
{
public:
	Triggcom_maker() : Component_maker("Trigger") {}
protected:
	Component* makeComponent(SceneObject *_obj);
	static Triggcom_maker s_RegisteredMaker;
};



/**
 Base classes for scene object
 
 Performs hierarchical transform
	all scene classes must inherit to be placed
	in scene hierarchy and be included in transform updates.
 
 SceneObject describes all renderable entities
 i.e. player, enemies, items. etc.
 SceneArea describes all logical nodes
 i.e. spawn nodes, triggers
*/
class SceneObject
{
public:
	SceneObject(int _id);
	SceneObject() :m_ID(0) {}

	virtual void update();

	std::string Name() { return m_Name; }
	void setName(std::string _name) { m_Name = _name; }
	int ID() { return m_ID; }
	void setID(int _id) { m_ID = _id; }

	/// Physics callback
	//TODO: do I want to abstract this from box2d?
	virtual void onColStart(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold);
	virtual void onColFinish(b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold);

	/// Message passing
	virtual void onMessage(std::string _message);

	///Serialization/Deserialization
	void encodeWith(Json::Value *_val);
	void initWith(Json::Value _val);

	///Check to see if SceneObject has a certain component installed.
	///@param _name string identifyer of component type
	///@return whether or not component is installed
	bool hasComponent(std::string _name);

	///Will return component of given name if installed, null otherwise
	///@param _name Identifyer of component type to return
	///@return corresponding component if type is installed, null otherwise(BE CAREFUL)
	Component* getComponent(std::string _name);

	///Register a component
	/// This should be a complete object
	///@param _com Component to add
	///@return whether or not component was added, if a component of the same type is already registered,
	/// the component will not be added
	bool addComponent(Component* _com);
	
protected:
	unsigned m_ID;
	std::string m_Name;

	std::map< std::string, Component* > m_Components;
};

/**
* Trigger descendants must implement the accessors and mutators below
*	Tests
*		overLap	-	given a b2AABB test to see if there is a collision with own AABB
*		pointIn	-	given a b2Vec2 test to see if the point lies within own AABB
*	Events
*		onEnterCamera	-	entered camera zone
*		onLeaveCamera	-	left camera zone
*		onPlayerEnter	-	player entered area
*		onPlayerLeave	-	player left area
*	State Tests
*		inCamera	-	are we in the camera? 
*		playerIn	-	is the player inside?
*/
//TODO: make a component?
/*
class Trigger : public SceneObject
{
public:
	Trigger();
	
	/// setExtents - sets the dimensions for rectangular trigger areas
	void setExtents( double _top, double _left, double _bottom, double _right);
	
	/// overlap - test if bounding box overlaps this
	bool overlap(b2AABB &_other) { return b2TestOverlap( m_Box, _other ); }
	
	/// pointIn - test if point is within this rectangular box
	bool pointIn(b2Vec2 &_point);
	
	/// inCamera - returns the state of camera focus
	bool inCamera() { return m_inCamera; }
	
	/// onEnterCamera - callback for entering focus
	void onEnterCamera();
	
	/// onLeaveCamera - callback for leaving focus
	void onLeaveCamera();
	
	/// playerIn - returns the state of the player with regard to this area
	bool playerIn() { return m_inPlayer; }
	
	/// onPlayerEnter - callback for player entering this area
	void onPlayerEnter();
	
	/// onPlayerLeave - callback for player leaving this area
	void onPlayerLeave();

protected:
	//structure holding extent information
	b2AABB m_Box;
	//booleans for current state of trigger
	bool m_inCamera;
	bool m_inPlayer;
	
};
*/
#endif
