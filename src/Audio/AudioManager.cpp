/*
   AudioManager.cpp
   My Unnamed Game Engine
 
   Created by Chad Godsey on 2/5/10.
  
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

#include <box2d.h>
#include "AudioManager.h"
#include "../Core/Core.h"

AudioManager* AudioManager::s_CurrentContext;

AudioManager::AudioManager()
: m_camX(0), m_camY(0), m_camZoom(0), m_camRot(0) 
{
	m_curSong = m_Songs.end();
}

AudioManager::~AudioManager()
{
	if (m_curSong != m_Songs.end())
		m_curSong->second->stop();
	std::map< std::string, Gosu::Sample* >::iterator itSample;
	std::map< std::string, Gosu::Song* >::iterator itSong;
	for (itSample = m_Samples.begin(); itSample != m_Samples.end(); ++itSample) {
		delete itSample->second;
	}
	for (itSong = m_Songs.begin(); itSong != m_Songs.end(); ++itSong) {
		delete itSong->second;
	}
}

void AudioManager::doPlay()
{
	std::list< SamplePlay >::iterator it;
	for (it = m_SampleQ.begin(); it != m_SampleQ.end(); ++it) {
		double scale = 1.0;
		double zoom = 1.0 + scale * (m_camZoom - 1.0);

		b2Vec2 newPos((it->x - m_camX) * scale * m_screenScale * zoom, 
					  (it->y - m_camY) * scale * m_screenScale * zoom);
		b2Transform trans;
		trans.R.Set( m_camRot*(Gosu::pi/180.0) );
		trans.position = b2Mul( trans.R, newPos);

		if (it->loop && !it->instance->playing()) 
			it->instance.reset( m_Samples[it->name]->playPan( Gosu::clamp<double>(trans.position.x/(m_screenW/2), -1.0, 1.0), it->volume, it->speed ) );
		
		if (it->instance->playing())
			it->instance->changePan(Gosu::clamp<double>(trans.position.x/(m_screenW/2), -1.0, 1.0));
	}
}

/// create a sample
void AudioManager::createSample(std::wstring _filename, std::string _name)
{
	if (m_Samples.find(_name) != m_Samples.end())
		return;
		//throw std::runtime_error("Sample name already in use: " + _name);

	m_Samples[_name] = new Gosu::Sample(Core::getCurrentContext()->audio(), _filename);
}
/// create a song
void AudioManager::createSong(std::wstring _filename, std::string _name)
{
	if (m_Songs.find(_name) != m_Songs.end())
		return;
		//throw std::runtime_error("Song name already in use: " + _name);

	m_Songs[_name] = new Gosu::Song(Core::getCurrentContext()->audio(), _filename);
}

void AudioManager::playSong( std::string _name, bool _loop, double _volume )
{
	m_curSong = m_Songs.find(_name);
	m_Songs[_name]->play( _loop );
	m_Songs[_name]->changeVolume(_volume);
}
void AudioManager::playAmbientSample( std::string _name, bool _loop, double _volume, double _speed )
{
	m_Samples[_name]->play( _volume, _speed );
}
void AudioManager::playStereoSample( std::string _name, double _x, double _y, bool _loop, double _volume, double _speed )
{
	double scale = 1.0;
	double zoom = 1.0 + scale * (m_camZoom - 1.0);

	b2Vec2 newPos((_x - m_camX) * scale * m_screenScale * zoom, 
				  (_y - m_camY) * scale * m_screenScale * zoom);
	b2Transform trans;
	trans.R.Set( m_camRot*(Gosu::pi/180.0) );
	trans.position = b2Mul( trans.R, newPos);

	SamplePlay splay;
	splay.name = _name;
	splay.volume = _volume;
	splay.speed = _speed;
	splay.loop = _loop;
	splay.ambient = false;
	splay.x = _x;
	splay.y = _y;

	splay.instance.reset( m_Samples[_name]->playPan( Gosu::clamp<double>(trans.position.x/(m_screenW/2), -1.0, 1.0), _volume, _speed ));
	m_SampleQ.push_back( splay );
}

void AudioManager::stopSong( std::string _name )
{
	m_Songs[_name]->stop();
}
void AudioManager::pauseSong( std::string _name )
{
	m_Songs[_name]->pause();
}

void AudioManager::stopSongs()
{
	if (m_curSong != m_Songs.end())
		m_curSong->second->stop();
	m_curSong = m_Songs.end();
}