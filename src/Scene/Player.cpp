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
* /

#include "../Core/Core.h"
#include <Box2D.h>
#include "Player.h"
#include "../Graphics/SpriteSheet.h"
#include "../Audio/AudioManager.h"
#include "../Input/InputManager.h"

Player::Player()
	: SceneObject(-1)
{
}

void Player::setPhysics( double _x, double _y, b2World* _world)
{
	m_TransCom.setPosition( _x, _y);
}

void Player::onColStart( b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold) 
{
	
}

void Player::onColFinish( b2Fixture *_fix, SceneObject *_other, b2Manifold _manifold) 
{
	
}

void Player::onMessage(std::string _message)
{

}
*/