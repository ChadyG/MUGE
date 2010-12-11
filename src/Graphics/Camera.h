/*
   Camera.h
   My Unnamed Game Engine
 
   Created by Chad Godsey on 12/9/09.
  
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

#ifndef CAMERA_H
#define CAMERA_H

#include <Gosu/Gosu.hpp>
#include <map>

struct CameraTransform
{
	double x, y, rot, zoom;
};

class Camera
{
public:
	Camera() : m_FocusX(0.f), m_FocusY(640), m_Width(480), m_Scale(1.f), m_Zoom(1.f), m_Rot(0.f) {}
	
	/// Set the window size of the camera
	/// @param _w The width of the window
	/// @param _h The height
	void setExtents(int _w, int _h) { m_Width = _w; m_Height = _h; }
	/// Set the scaling factor of the window
	/// This determines the world pixel resolution to the screen (world/screen)
	/// @param _scale scaling factor
	void setScale( double _scale ) { m_Scale = _scale; }
	/// Set the zoom factor of the camera
	/// same effect as changing the scale, but meant to be dynamic
	/// @param _zoom the zoom factor (default 1)
	void setZoom( double _zoom ) { m_Zoom = _zoom; }
	/// Set the rotation angle (degrees)
	/// @param _rot angle to rotate by
	void setRotation( double _rot ) { m_Rot = _rot; }
	/// Set the world coordinates to center the camera on
	/// @param _x World X
	/// @param _y World Y
	void setFocus(double _x, double _y) { m_FocusX = _x, m_FocusY = _y; }

	double X() { return m_FocusX; }
	double Y() { return m_FocusY; }
	double Zoom() { return m_Zoom; }
	double Scale() { return m_Scale; }
	double Rotation() { return m_Rot; }
	int Width() { return m_Width; }
	int Height() { return m_Height; }
	
	/// This tells the Render Manager where on screen a world coordinate currently is.
	/// When creating your own camera, this method must be implemented.
	/// @param _x World X 
	/// @param _y World Y
	/// @param _z World Z (in Gosu layers)
	/// @return CameraTransform screen coordinates with rotation for displaying on screen
	virtual CameraTransform worldToScreen(double _x, double _y, Gosu::ZPos _z);

	/// This tells the Render Manager where on screen a world coordinate currently is.
	/// When creating your own camera, this method must be implemented.
	/// @param _x World X 
	/// @param _y World Y
	/// @param _z World Z (in Gosu layers)
	/// @return CameraTransform screen coordinates with rotation for displaying on screen
	virtual CameraTransform screenToWorld(double _x, double _y, Gosu::ZPos _z);

protected:

	double m_FocusX, m_FocusY;
	int m_Width, m_Height;
	double m_Scale, m_Zoom, m_Rot;
};

/// Parallax Camera
/// Uses Gosu layers with individual scaling factors to create a parallax effect
class Camera_Parallax : public Camera
{
public:
	Camera_Parallax() {}
	
	void addLayer(int _layer, double _dist);
	
	/// Come up with a batch mechanism
	CameraTransform worldToScreen(double _x, double _y, Gosu::ZPos _z);

	CameraTransform screenToWorld(double _x, double _y, Gosu::ZPos _z);

private:

	std::map< int, double > m_LayerScales;
};

#endif