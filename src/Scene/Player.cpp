/*
   MUGE.h
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
 
#include <Box2D.h>
#include "Player.h"
#include "../Sprite/Animation.h"

Player::Player()
{

}

Player::Player(int _x, int _y, b2World* _world)
{
/*
	m_World = _world;
	b2CircleDef cDef;
	cDef.radius = 1.0;
	cDef.density = 1.0;
	cDef.friction = 1.3;
	cDef.restitution = 0.3;
	b2BodyDef bDef;
	bDef.position.Set( _x, _y );
	m_Body = m_World->CreateBody( &bDef );
	m_Body->CreateShape( &cDef );
	m_Body->SetMassFromShapes();
	*/
}

void Player::setPhysics( double _x, double _y, b2World* _world)
{
	m_Pos.x = _x;
	m_Pos.y = _y;
	setPosition( m_Pos );
/*
	m_World = _world;
	b2CircleDef cDef;
	cDef.radius = 1.0;
	cDef.density = 1.0;
	cDef.friction = 1.3;
	cDef.restitution = 0.3;
	b2BodyDef bDef;
	bDef.position.Set( _x, _y );
	bDef.linearDamping = 0.5f;
	bDef.angularDamping = 0.5f;
	//bDef.fixedRotation = true;
	m_Body = m_World->CreateBody( &bDef );
	m_Body->CreateShape( &cDef );
	m_Body->SetMassFromShapes();
	*/
}

void Player::addAnimation(std::string _name, Animation* _anim)
{
	m_Anims[_name] = _anim;
	m_AnimState = m_Anims[_name];
	//addChild( (SceneObject*)(m_Anims[_name]) );
}

void Player::setLayer(Gosu::ZPos _z)
{
	m_Layer = _z;
}

void Player::setGravity( b2Vec2 _gravity )
{
	m_Gravity = _gravity;
}

void Player::onHit(SceneObject &other, b2ContactPoint &point)
{

}

void Player::update(Gosu::Input& _input)
{
	// Either use physics or scene stuff
	m_Pos = m_Position;//m_Body->GetPosition();
	
	// Temporary shit
	if (_input.down(Gosu::kbLeft)) {
		m_AnimState = m_Anims["WalkLeft"];
		m_Pos.x -= 0.1;
		if (_input.down(Gosu::kbLeftShift)) 
			m_Pos.x -= 0.1;
	}
	if (_input.down(Gosu::kbRight)) {
		m_AnimState = m_Anims["Walk"];
		m_Pos.x += 0.1;
		if (_input.down(Gosu::kbLeftShift)) 
			m_Pos.x += 0.1;
	}
	setPosition( m_Pos );
	// end temporary movement of the bowel

	m_AnimState->update();
}

void Player::draw(double _x, double _y, double _zoom, double _angle) const
{
	m_AnimState->draw(  _x - m_Pos.x, _y - m_Pos.y, m_Layer, _zoom, _angle);
}