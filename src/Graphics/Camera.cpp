/*
   Camera.cpp
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

#include "Camera.h"
#include <Box2D.h>
/*

b2XForm Camera::worldToScreen( float _x, float _y, Gosu::ZPos _layer )
{
	b2XForm trans;
	trans.R.Set( 0.0 );
	trans.position.Set( _x + m_FocusX, _y + m_FocusY);
	return trans;
}


void Camera_Parallax::addLayer(int _layer, float _dist)
{
	m_LayerScales[_layer] = _dist;
}

b2XForm Camera_Parallax::worldToScreen( float _x, float _y, Gosu::ZPos _layer )
{
	double scale = 1.0/m_LayerScales[_layer];
	double zoom = 1.0 + scale * (m_Zoom - 1.0);
	b2Vec2 newPos((_x * m_Scale * scale) * zoom, 
				  (_y * m_Scale * scale) * zoom);
	b2XForm trans;
	trans.R.Set( m_Rot*(Gosu::pi/180.0) );
	trans.position = b2Mul( trans.R, newPos);
	trans.position.Set( trans.position.x + m_Width/2, trans.position.y + m_Height/2);
	return trans;
}
*/