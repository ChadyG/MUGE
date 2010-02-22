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

#include "AudioManager.h"
#include "../Core/Core.h"

AudioManager* AudioManager::s_CurrentContext;

//AudioManager::AudioManager() {}

void AudioManager::doPlay()
{
}

/// create a sample
void AudioManager::createSample(std::wstring _filename, std::string _name)
{
	if (m_Samples.find(_name) == m_Samples.end())
		throw std::runtime_error("Sample name already in use: " + _name);

	m_Samples[_name] = new Gosu::Sample(Core::getCurrentContext()->audio(), _filename);
}
/// create a song
void AudioManager::createSong(std::wstring _filename, std::string _name)
{
	if (m_Songs.find(_name) == m_Songs.end())
		throw std::runtime_error("Song name already in use: " + _name);

	m_Songs[_name] = new Gosu::Song(Core::getCurrentContext()->audio(), _filename);
}

void AudioManager::playSong( std::string _name, double _volume, double _speed )
{
}
void AudioManager::playAmbientSample( std::string _name, double _volume, double _speed )
{
}
void AudioManager::playStereoSample( std::string _name, double _x, double _y, double _volume, double _speed )
{
}

void AudioManager::stopSong( std::string _name )
{
}
void AudioManager::pauseSong( std::string _name )
{
}