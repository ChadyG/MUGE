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
#include <list>
#include "../Graphics/Camera.h"

/**
*	Provide interface for audio resource management
*	-stereo output based on scene/camera
*	
*/
class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	/// Set parameters for window
	/// @param _w screen width
	/// @param _h screen height
	/// @param _s screen scaling factor - pixels per world unit
	void setScreen( int _w, int _h, int _s)
	{
		m_screenW = _w; m_screenH = _h; m_screenScale = _s;
	};

	/// Update camera parameters
	/// @param _x world space X coordinate of camera 
	/// @param _y world space Y coordinate of camera 
	/// @param _zoom zoom factor of camera 
	/// @param _rot angle of camera (degrees)
	/// This should be called on each game tick
	void setCamera( double _x, double _y, double _zoom = 1.f, double _rot = 0.f )
	{
		m_camX = _x; m_camY = _y; m_camZoom = _zoom; m_camRot = _rot;
	};

	/// Set camera object to user defined type
	/// This camera takes cares of the world to screen transformation.  
	/// The default camera does a direct transformation (1:1 linear) from world space to screen space.
	/// If this does not suit your needs, create a child class of the Camera type to perform your own transformation.
	/// @param _cam A camera object created by the caller (RenderManager will not automatically initialize your Camera)
	void setCamera( Camera *_cam )
	{
		m_Camera = _cam;
	};

	/// Run all pending operations
	/// should be called every game tick
	void update();

	/// create a sample
	/// @param _filename file to be loaded
	/// @param _name used to reference sample
	void createSample(std::wstring _filename, std::string _name);
	/// create a song
	/// @param _filename file to be loaded
	/// @param _name used to reference song
	void createSong(std::wstring _filename, std::string _name);

	/// Play the given song
	/// @param _name the song to be played
	/// @param _loop whether or not to loop the song
	/// @param _volume volume modifier (0.0 to 1.0)
	void playSong( std::string _name, bool _loop = false, double _volume = 1.0 );
	/// Play the given Sample ambiently
	/// @param _name the sample to be played
	/// @param _volume volume modifier (0.0 to 1.0)
	/// @param _speed speed modifier (0.0 to 1.0)
	void playAmbientSample( std::string _name, bool _loop = false, double _volume = 1.0, double _speed = 1.0 );
	/// Play the given Sample in world coordinates
	/// @param _name the sample to be played
	/// @param _x world X to play from
	/// @param _y world Y to play from
	/// @param _volume volume modifier (0.0 to 1.0)
	/// @param _speed speed modifier (0.0 to 1.0)
	void playStereoSample( std::string _name, double _x, double _y, bool _loop = false, double _volume = 1.0, double _speed = 1.0 );

	/// Ask whether or not the given song is currently playing
	bool songPlaying( std::string _name ) { return m_Songs[_name]->playing(); }
	/// Ask whether or not the given song is paused
	bool songPaused( std::string _name ) { return m_Songs[_name]->paused(); }

	/// Stop the song
	void stopSong( std::string _name );
	/// Pause the song
	void pauseSong( std::string _name );

	/// Stop all songs
	void stopSongs();

	//fadeToSong( std::string _name, float _time = 1.f);

	/// Static accessor to current render manager
	static AudioManager* getCurrentContext() { return s_CurrentContext; }

	/// Static setter for current render manager
	static void setCurrentContext(AudioManager* _context) { s_CurrentContext = _context; }

protected:
	struct SamplePlay
	{
		std::string name;
		boost::optional<Gosu::SampleInstance> instance;
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

	static AudioManager* s_CurrentContext;

	std::map< std::string, Gosu::Sample* > m_Samples;
	std::map< std::string, Gosu::Song* > m_Songs;
	std::map< std::string, Gosu::Song* >::iterator m_curSong;

	std::list< SamplePlay > m_SampleQ;
	//std::queue< SongPlay > m_SongQ;

	Camera *m_Camera;

	double m_camX, m_camY, m_camZoom, m_camRot;
	int m_screenW, m_screenH, m_screenScale;
};


#endif