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
#include "../Sprite/Sprite.h"
#include "../Sprite/SpriteSheet.h"

SceneObject::SceneObject()
{
}

void SceneObject::setSprite( Sprite *_sprite )
{
	m_Sprite = _sprite;
	m_SpriteSheet = NULL;
}

void SceneObject::setSpriteSheet( SpriteSheet *_anim)
{
	m_SpriteSheet = _anim;
	m_Sprite = NULL;
}

void SceneObject::hide()
{
	m_hidden = true;
}

void SceneObject::show()
{
	m_hidden = false;
}


void SceneObject::update()
{
}

void SceneObject::draw(double _x, double _y, Gosu::ZPos _layer, double _zoom, double _angle) const
{
	if (m_Sprite)
		m_Sprite->draw( m_Position.x, m_Position.y, _layer, _zoom, m_Rotation + _angle);
	
	if (m_SpriteSheet)
		m_SpriteSheet->draw( m_Position.x, m_Position.y, _layer, _zoom, m_Rotation + _angle);
}

void SceneObject::onHit( SceneObject &other, b2ContactPoint &point) 
{
	
}
	


Trigger::Trigger()
{
}

void Trigger::setExtents( double _top, double _left, double _bottom, double _right)
{
	m_Box.upperBound.Set( _left, _top );
	m_Box.lowerBound.Set( _right, _bottom );
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
