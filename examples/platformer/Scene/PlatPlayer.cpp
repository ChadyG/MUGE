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

#include "Graphics/RenderManager.h"
#include "Input/InputManager.h"
#include "PlatPlayer.h"

PlatformPlayer::PlatformPlayer()
{
	m_AnimState = NULL;
}

void PlatformPlayer::setPhysics( double _x, double _y, b2World* _world)
{
	m_Position.x = _x;
	m_Position.y = _y;
}

void PlatformPlayer::init()
{
	RenderManager *rendMan = RenderManager::getCurrentContext();
	m_Anims["WalkLeft"] = rendMan->createSpriteSheet(0, 
		Gosu::resourcePrefix() + L"Images/Hero_walking_left.png", 
		85,
		128,
		20);
	m_Anims["WalkRight"] = rendMan->createSpriteSheet(0, 
		Gosu::resourcePrefix() + L"Images/Hero_walking.png", 
		85,
		128,
		20);
		
	m_AnimState = m_Anims["WalkRight"];
	m_AnimState->setVisible(true);
	 m_Anims["WalkLeft"]->setVisible(false);
}

void PlatformPlayer::setLayer(Gosu::ZPos _z)
{
	m_Layer = _z;
	std::map< std::string, SpriteSheet*>::iterator iAnim;
	for (iAnim = m_Anims.begin(); iAnim != m_Anims.end(); iAnim++) {
		(*iAnim).second->setLayer( m_Layer );
	}
}

void PlatformPlayer::setGravity( b2Vec2 _gravity )
{
	m_Gravity = _gravity;
}

void PlatformPlayer::onColStart( b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold) 
{
	
}

void PlatformPlayer::onColFinish( b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold) 
{
	
}

void PlatformPlayer::onMessage(std::string _message)
{

}

void PlatformPlayer::update()
{
	InputManager* input = InputManager::getCurrentContext();
	//m_Position = m_Body->GetPosition();
	
	if (input->query("Play.MoveLeft") == InputManager::actnActive) {
		m_AnimState->setVisible(false);
		m_AnimState = m_Anims["WalkLeft"];
		m_AnimState->setVisible(true);
		m_Position.x -= 0.1;
		if (input->query("Play.Run") == InputManager::actnActive) 
			m_Position.x -= 0.1;
	}
	if (input->query("Play.MoveRight") == InputManager::actnActive) {
		m_AnimState->setVisible(false);
		m_AnimState = m_Anims["WalkRight"];
		m_AnimState->setVisible(true);
		m_Position.x += 0.1;
		if (input->query("Play.Run") == InputManager::actnActive)
			m_Position.x += 0.1;
	}

	m_AnimState->setX( m_Position.x );
	m_AnimState->setY( m_Position.y );
}