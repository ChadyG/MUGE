/*
   RenderManager.cpp
   My Unnamed Game Engine
 
   Created by Chad Godsey on 12/23/09.
  
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

#include <Gosu/Gosu.hpp>
#include "RenderManager.h"
#include <Box2D/Box2D.h>
#include "../Core/Core.h"
#include "MessageBubble.h"
#include "SpriteSheet.h"
#include "Sprite.h"
#include "Camera.h"

RenderManager* RenderManager::s_CurrentContext;

//save for later to do precaching of memory
RenderManager::RenderManager() 
	: m_curImageID(0), m_rendlist(0), m_Camera(0),
	m_Font(Core::getCurrentContext()->graphics(), Gosu::defaultFontName(), 12, 0)
{	
}

RenderManager::~RenderManager()
{
	Renderable *r = m_rendlist;
	while (r != NULL) {
		r = r->m_next;
		if (r && r->m_prev) delete r->m_prev;
	}
}

void RenderManager::update()
{
	Renderable *r = m_rendlist;
	while (r != NULL) {
		r->update();
		r = r->m_next;
	}
}

void RenderManager::doRender() const
{
	int camW = m_Camera->Width(), camH = m_Camera->Height();
	//m_camX = _x; m_camY = _y, 
	double camZoom = m_Camera->Zoom(), camRot = m_Camera->Rotation(), camScale = m_Camera->Scale();

	Renderable *r = m_rendlist;
	while (r != NULL) {
		CameraTransform trans = m_Camera->worldToScreen( r->m_posX, r->m_posY, r->m_layer);
		r->draw(trans.x, trans.y, trans.zoom, trans.rot);
		r = r->m_next;
	}
}

//
//	Standard creators

MessageBubble* RenderManager::createMessage(std::wstring _message, double _x, double _y, bool _static)
{
	MessageBubble *nMB = new MessageBubble(_message, _static);
	nMB->setX(_x);
	nMB->setY(_y);

	nMB->m_prev = NULL;
	nMB->m_next = m_rendlist;
	if (m_rendlist)
		m_rendlist->m_prev = nMB;
	m_rendlist = nMB;
	return nMB;
}

Sprite* RenderManager::createSprite(int _layer, std::wstring _filename)
{
	//m_Sprites[m_CurSpriteID] = new Sprite();
	Sprite *tsprite = new Sprite();
	if (m_Images.find(_filename) == m_Images.end()) {
		m_Images[_filename] = new Gosu::Image(Core::getCurrentContext()->graphics(),_filename);
	}
	tsprite->setImage(m_Images[_filename]);
	tsprite->setLayer((double)_layer);

	tsprite->m_prev = NULL;
	tsprite->m_next = m_rendlist;
	if (m_rendlist)
		m_rendlist->m_prev = tsprite;
	m_rendlist = tsprite;
	return tsprite;
}

SpriteSheet* RenderManager::createSpriteSheet(
	int _layer, std::wstring _filename, int _width, int _height, int _delay)
{
	SpriteSheet *isheet = new SpriteSheet( _filename, _width, _height, _delay );
	isheet->setLayer(_layer);

	isheet->m_prev = NULL;
	isheet->m_next = m_rendlist;
	if (m_rendlist)
		m_rendlist->m_prev = isheet;
	m_rendlist = isheet;
	return isheet;
}

//
// Asset registration

void RenderManager::registerRenderable(int _layer, Renderable* _r)
{
	_r->m_prev = NULL;
	_r->m_next = m_rendlist;
	m_rendlist->m_prev = _r;
	m_rendlist = _r;
}

void RenderManager::registerSprite(int _layer, Sprite* _sp)
{
	_sp->setLayer((double)_layer);
	_sp->m_prev = NULL;
	_sp->m_next = m_rendlist;
	m_rendlist->m_prev = _sp;
	m_rendlist = _sp;
}

void RenderManager::registerSpriteSheet(int _layer, SpriteSheet* _ss)
{
	_ss->setLayer(_layer);
	_ss->m_prev = NULL;
	_ss->m_next = m_rendlist;
	m_rendlist->m_prev = _ss;
	m_rendlist = _ss;
}

//
// Asset deletion

void RenderManager::deleteRenderable(Renderable* _r)
{ 
	if (_r->m_prev) {
		_r->m_prev->m_next = _r->m_next;
	}

	if (_r->m_next) {
		_r->m_next->m_prev = _r->m_prev;
	}

	if (_r == m_rendlist) {
		m_rendlist = _r->m_next;
	}
	delete _r;
}

void RenderManager::deleteSprite(Sprite* _sprite) 
{ 
	if (_sprite->m_prev) {
		_sprite->m_prev->m_next = _sprite->m_next;
	}

	if (_sprite->m_next) {
		_sprite->m_next->m_prev = _sprite->m_prev;
	}

	if (_sprite == m_rendlist) {
		m_rendlist = _sprite->m_next;
	}
	delete _sprite;
}
void RenderManager::deleteSpriteSheet(SpriteSheet* _sheet) 
{ 
	if (_sheet->m_prev) {
		_sheet->m_prev->m_next = _sheet->m_next;
	}

	if (_sheet->m_next) {
		_sheet->m_next->m_prev = _sheet->m_prev;
	}

	if (_sheet == m_rendlist) {
		m_rendlist = _sheet->m_next;
	}
	delete _sheet;
}
void RenderManager::deleteMessage(MessageBubble* _message)
{
	if (_message->m_prev) {
		_message->m_prev->m_next = _message->m_next;
	}

	if (_message->m_next) {
		_message->m_next->m_prev = _message->m_prev;
	}

	if (_message == m_rendlist) {
		m_rendlist = _message->m_next;
	}
	delete _message;
}
