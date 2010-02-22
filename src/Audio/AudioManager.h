/*
   AudioManager.h
   My Unnamed Game Engine
 
   Created by Chad Godsey on 12/15/09.
  
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

#ifndef AUDMAN_H
#define AUDMAN_H

#include <Gosu/Gosu.hpp>
#include <map>
#include <queue>

struct SamplePlay
{
	int sampleID;
	double volume;
	double speed;
	bool loop;
	bool ambient;
	float x, y;
};

struct SongPlay
{
	int songID;
	double volume;
	double speed;
	bool loop;
};

/**
*	Provide interface for audio resource management
*	-stereo output based on scene/camera
*	
*/
class AudioManager
{
public:
	AudioManager() : m_camX(0), m_camY(0), m_camZoom(0), m_camRot(0) {}

	void setCamera( float _x, float _y, float _zoom = 1.f, float _rot = 0.f )
	{
		m_camX = _x; m_camY = _y, m_camZoom = _zoom; m_camRot = _rot;
	};

	void doPlay();

	/// create a sample
	void createSample(std::wstring _filename, std::string _name);
	/// create a song
	void createSong(std::wstring _filename, std::string _name);

	void playSong( std::string _name, double _volume = 1.0, double _speed = 1.0 );
	void playAmbientSample( std::string _name, double _volume = 1.0, double _speed = 1.0 );
	void playStereoSample( std::string _name, double _x, double _y, double _volume = 1.0, double _speed = 1.0 );

	bool songPlaying( std::string _name ) { return m_Songs[_name]->playing(); }
	bool songPaused( std::string _name ) { return m_Songs[_name]->paused(); }

	void stopSong( std::string _name );
	void pauseSong( std::string _name );

	/// Static accessor to current render manager
	static AudioManager* getCurrentContext() { return s_CurrentContext; }

	/// Static setter for current render manager
	static void setCurrentContext(AudioManager* _context) { s_CurrentContext = _context; }

protected:

	static AudioManager* s_CurrentContext;

	std::map< std::string, Gosu::Sample* > m_Samples;
	std::map< std::string, Gosu::Song* > m_Songs;

	std::queue< SamplePlay > m_SampleQ;
	std::queue< SongPlay > m_SongQ;

	float m_camX, m_camY, m_camZoom, m_camRot;
};


#endif