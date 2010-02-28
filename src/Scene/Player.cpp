/*
   Player.h
   My Unnamed Game Engine
 
   Created by Chad Godsey on 7/12/09.
  
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

#include "../Core/Core.h"
#include <Box2D.h>
#include "Player.h"
#include "../Graphics/SpriteSheet.h"
#include "../Audio/AudioManager.h"
#include "../Input/InputManager.h"

Player::Player()
{
	m_AnimState = NULL;
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
	bDef.userData = this;
	bDef.position.Set( _x, _y );
	bDef.linearDamping = 0.5f;
	bDef.angularDamping = 0.5f;
	//bDef.fixedRotation = true;
	m_Body = m_World->CreateBody( &bDef );
	m_Body->CreateShape( &cDef );
	m_Body->SetMassFromShapes();
*/
}

void Player::addSpriteSheet(std::string _name, SpriteSheet* _anim)
{
	m_Anims[_name] = _anim;
	if (m_AnimState)
		m_AnimState->setVisible(false);
	m_AnimState = m_Anims[_name];
	m_AnimState->setVisible(true);
}

void Player::setLayer(Gosu::ZPos _z)
{
	m_Layer = _z;
	std::map< std::string, SpriteSheet*>::iterator iAnim;
	for (iAnim = m_Anims.begin(); iAnim != m_Anims.end(); iAnim++) {
		(*iAnim).second->setLayer( m_Layer );
	}
}

void Player::setGravity( b2Vec2 _gravity )
{
	m_Gravity = _gravity;
}

void Player::onColStart( SceneObject *other, b2ContactPoint point) 
{
	
}

void Player::onColPersist( SceneObject *other, b2ContactPoint point) 
{
	
}

void Player::onColFinish( SceneObject *other, b2ContactPoint point) 
{
	
}

void Player::onMessage(std::string _message)
{

}

void Player::update()
{
	InputManager* input = InputManager::getCurrentContext();
	m_Pos = m_Body->GetPosition();
	
	if (input->query("Play.MoveLeft") == InputManager::actnActive) {
		m_AnimState->setVisible(false);
		m_AnimState = m_Anims["WalkLeft"];
		m_AnimState->setVisible(true);
		m_Pos.x -= 0.1;
		if (input->query("Play.Run") == InputManager::actnActive) 
			m_Pos.x -= 0.1;
	}
	if (input->query("Play.MoveRight") == InputManager::actnActive) {
		m_AnimState->setVisible(false);
		m_AnimState = m_Anims["Walk"];
		m_AnimState->setVisible(true);
		m_Pos.x += 0.1;
		if (input->query("Play.Run") == InputManager::actnActive)
			m_Pos.x += 0.1
	}

	m_AnimState->setX( m_Pos.x );
	m_AnimState->setY( m_Pos.y );
	m_AnimState->setAngle( Gosu::angle( m_Gravity.x, m_Gravity.y, 0.0, 0.0) );
}