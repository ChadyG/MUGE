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

sruct SamplePlay
{
	int sampleID;
	double volume;
	double speed;
	bool loop;
	bool ambient;
	float x, y;
};

sruct SongPlay
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
	AudioManager() {}

	void setCamera( float _x, float _y, float _zoom = 1.f, float _rot = 0.f )
	{
		m_camX = _x; m_camY = _y, m_camZoom = _zoom; m_camRot = _rot;
	};

	void doPlay();

	/// create a sample
	int createSample(std::wstring _filename, std::string _name);
	/// create a song
	int createSong(std::wstring _filename, std::string _name);

	/// Get a sample by name
	int getSampleByName( std::string _name );
	/// Get a song by name
	int getSongByName( std::string _name );

	void playSong( int _id, double _volume = 1.0, double _speed = 1.0 );
	void playAmbientSample( int _id, double _volume = 1.0, double _speed = 1.0 );
	void playStereoSample( int _id, double _x, double _y, double _volume = 1.0, double _speed = 1.0 );

	void stopSong( int _id );
	void pauseSong( int _id );

	/// Static accessor to current render manager
	static boost::shared_ptr< AudioManager > getCurrentContext() { return s_CurrentContext; }

	/// Static setter for current render manager
	static void setCurrentContext(AudioManager* _context) { s_CurrentContext.reset(_context); }

protected:

	static boost::shared_ptr< AudioManager > s_CurrentContext;

	std::map< int, boost::shared_ptr<Gosu::Sample> > m_Samples;
	std::map< std::string, int > m_SampleMap;
	
	std::map< int, boost::shared_ptr<Gosu::Song> > m_Songs;
	std::map< std::string, int > m_SongMap;

	std::queue< SamplePlay > m_SampleQ;
	std::queue< SongPlay > m_SongQ;
	
	int m_curSampleID;
	int m_curSongID;

	float m_camX, m_camY, m_camZoom, m_camRot;
};


#endif