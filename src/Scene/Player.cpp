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

b2Vec2 Player::getPosition()
{
	return m_Pos;
}

void Player::addAnimation(std::string _name, Animation* _anim)
{
	m_Anims[_name] = _anim;
	m_AnimState = m_Anims[_name];
}

void Player::setWindowScale(double _scale)
{
	std::map< std::string, Animation*>::iterator tAnim;
	for (tAnim = m_Anims.begin(); tAnim != m_Anims.end(); ++tAnim) 
		tAnim->second->setWindowScale(_scale);
}

void Player::setLayer(Gosu::ZPos _z)
{
	std::map< std::string, Animation*>::iterator tAnim;
	for (tAnim = m_Anims.begin(); tAnim != m_Anims.end(); ++tAnim) 
		tAnim->second->setPosition( 0, 0, _z);
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
	//m_Pos = m_Body->GetPosition();
	
	if (_input.down(Gosu::kbLeft)) {
		m_Pos.x -= 0.1;
		if (_input.down(Gosu::kbLeftShift)) 
			m_Pos.x -= 0.1;
	}
	if (_input.down(Gosu::kbRight)) {
		m_Pos.x += 0.1;
		if (_input.down(Gosu::kbLeftShift)) 
			m_Pos.x += 0.1;
	}

	m_AnimState->setPosition( m_Pos.x, m_Pos.y, 4);
	m_AnimState->update();
}

void Player::draw(double _x, double _y) const
{
	m_AnimState->draw( _x, _y );
}

void Player::drawZoom(double _x, double _y, double _scale, double _zoom, int _scrWidth, int _scrHeight) const
{
	m_AnimState->drawZoom(  _x, _y, _scale, _zoom, _scrWidth, _scrHeight);
}