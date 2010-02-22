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

//AudioManager::AudioManager() {}


void AudioManager::doPlay()
{
}

/// create a sample
int AudioManager::createSample(std::wstring _filename, std::string _name)
{
	if (m_SampleMap.find(_name) == m_SampleMap.end())
		throw std::runtime_error("Sample name already in use: " + _name);

	m_Samples[m_curSampleID] = new Gosu::Sample(Core::getCurrentContext()->audio(), _filename);
	m_SampleMap[_name] = m_curSampleID++;
}
/// create a song
int AudioManager::createSong(std::wstring _filename, std::string _name)
{
	if (m_SongMap.find(_name) == m_SongMap.end())
		throw std::runtime_error("Song name already in use: " + _name);
}

/// Get a sample by name
int AudioManager::getSampleByName( std::string _name )
{
	if (m_SampleMap.find(_name) == m_SampleMap.end())
		return -1;
	return m_SampleMap[_name];
}
/// Get a song by name
int AudioManager::getSongByName( std::string _name )
{
	if (m_SongMap.find(_name) == m_SongMap.end())
		return -1;
	return m_SongMap[_name];
}

void AudioManager::playSong( int _id, double _volume = 1.0, double _speed = 1.0 )
{
}
void AudioManager::playAmbientSample( int _id, double _volume = 1.0, double _speed = 1.0 )
{
}
void AudioManager::playStereoSample( int _id, double _x, double _y, double _volume = 1.0, double _speed = 1.0 )
{
}

void AudioManager::stopSong( int _id )
{
}
void AudioManager::pauseSong( int _id )
{
}