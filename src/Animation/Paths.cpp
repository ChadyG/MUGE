/*
   Paths.cpp
   My Unnamed Game Engine
 
   Created by Chad Godsey.
  
 Copyright 2010 BlitThis! studios.

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

#include "Paths.h"
#include <math.h>

void Step_Path::addStep( Step _step )
{
	std::list< Step >::iterator loc;
	for (loc = m_Path.begin(); loc != m_Path.end() && loc->time < _step.time; loc++);
	m_Path.insert(loc,_step);
	if (m_TotalTime < _step.time)
		m_TotalTime = _step.time;
}
void Step_Path::insertStep( Step _step )
{
	std::list< Step >::iterator loc;
	for (loc = m_Path.begin(); loc != m_Path.end() && loc->time < _step.time; loc++);
	m_Path.insert(loc,_step);
	if (m_TotalTime < _step.time)
		m_TotalTime = _step.time;
}

void Step_Path::deleteStep( int _p )
{
	std::list< Step >::iterator loc;
	int i = 0;
	for (loc = m_Path.begin(); loc != m_Path.end() && i < _p; loc++, ++i) {
		m_Path.erase(loc);
	}
	m_TotalTime = m_Path.back().time;
}
void Step_Path::setStep( int _p, Step _step )
{
	std::list< Step >::iterator loc;
	int i = 0;
	for (loc = m_Path.begin(); loc != m_Path.end() && i < _p; loc++, ++i);
	if (loc != m_Path.end()) {
		loc->point.x = _step.point.x;
		loc->point.y = _step.point.y;
		loc->time = _step.time;
	}
	m_TotalTime = m_Path.back().time;
}

b2Vec2 Step_Path::getPosition( float _time ) const
{
	if (m_Repeat && _time > m_TotalTime)
		_time = fmod(_time, m_TotalTime);
	b2Vec2 pos;
	pos.x = 0.0f;
	pos.y = 0.0f;
	std::list< Step >::const_iterator it1, it2;
	float pTime = 0.0;
	for (it1 = m_Path.begin(); it1 != m_Path.end() && it1->time < _time; ++it1);

	if (it1 == m_Path.end())
		return pos;

	if (it1 != m_Path.begin() && it1->time > _time) it1--;

	return it1->point;
}





void Line_Path::addStep( Step _step )
{
	std::list< Step >::iterator loc;
	for (loc = m_Path.begin(); loc != m_Path.end() && loc->time < _step.time; loc++);
	m_Path.insert(loc,_step);
	if (m_TotalTime < _step.time)
		m_TotalTime = _step.time;
}
void Line_Path::insertStep( Step _step )
{
	std::list< Step >::iterator loc;
	for (loc = m_Path.begin(); loc != m_Path.end() && loc->time < _step.time; loc++);
	m_Path.insert(loc,_step);
	if (m_TotalTime < _step.time)
		m_TotalTime = _step.time;
}
void Line_Path::deleteStep( int _p )
{
	std::list< Step >::iterator loc;
	int i = 0;
	for (loc = m_Path.begin(); loc != m_Path.end() && i < _p; loc++, ++i) {
		m_Path.erase(loc);
	}
	m_TotalTime = m_Path.back().time;
}
void Line_Path::setStep( int _p, Step _step )
{
	std::list< Step >::iterator loc;
	int i = 0;
	for (loc = m_Path.begin(); loc != m_Path.end() && i < _p; loc++, ++i);
	if (loc != m_Path.end()) {
		loc->point.x = _step.point.x;
		loc->point.y = _step.point.y;
		loc->time = _step.time;
	}
	m_TotalTime = m_Path.back().time;
}
/// p(x) = p1 + (p2 - p1)*t
/// t = (x - x(k)) / h
/// h = x(k+1) - x(k)
b2Vec2 Line_Path::getPosition( float _time ) const
{
	if (m_Repeat && _time > m_TotalTime)
		_time = fmod(_time, m_TotalTime);
	b2Vec2 pos;
	pos.x = 0.0f;
	pos.y = 0.0f;
	std::list< Step >::const_iterator it1, it2;
	float pTime = 0.0;
	for (it2 = m_Path.begin(); it2 != m_Path.end() && it2->time <= _time; ++it2);
	if (it2 == m_Path.end())
		return pos;

	it1 = it2; --it1;
	float h = it2->time - it1->time;
	float t = (_time - it1->time) / h;
	pos = it1->point + t*(it2->point - it1->point);
	return pos;
}





void Smooth_Path::addStep( Step _step )
{
	m_Path.push_back(_step);
	if (m_TotalTime < _step.time)
		m_TotalTime = _step.time;
	b2Vec2 tan;
	int size = m_Path.size();
	if (size >= 3) {
		std::list< Step >::iterator it1, it2;
		it1 = m_Path.end();
		--it1; --it1; --it1;
		it2 = m_Path.end();
		--it2;
		tan = 0.5f * (it2->point - it1->point);
		//modify last
		m_Tangents.pop_back();
		m_Tangents.push_back( tan );
		
	}
	//new
	tan.x = 0.0f;
	tan.y = 0.0f;
	m_Tangents.push_back( tan );
}
void Smooth_Path::insertStep( Step _step )
{
	std::list< Step >::iterator loc;
	std::list< b2Vec2 >::iterator ltan = m_Tangents.begin();
	for (loc = m_Path.begin(); loc != m_Path.end() && loc->time < _step.time; loc++);
	m_Path.insert(loc,_step);
	for (loc = m_Path.begin(); loc != m_Path.end() && loc->time != _step.time; loc++, ltan++);
	if (m_TotalTime < _step.time)
		m_TotalTime = _step.time;
	b2Vec2 tan;
	int size = m_Path.size();
	if (size >= 3) {
		std::list< Step >::iterator it1, it2;
		it1 = loc;//assign to next
		--it1; --it1;//we want previous
		it2 = loc;//assign to next
		tan = 0.5f * (it2->point - it1->point);
		//modify previous tangent
		ltan--;
		ltan->x = tan.x;
		ltan->y = tan.y;
		ltan++;//return to next tangent
	}
	//new
	tan.x = 0.0f;
	tan.y = 0.0f;
	m_Tangents.insert( ltan, tan );
}
void Smooth_Path::deleteStep( int _p )
{
	std::list< Step >::iterator loc;
	std::list< b2Vec2 >::iterator ltan = m_Tangents.begin();
	int i = 0;
	for (loc = m_Path.begin(); loc != m_Path.end() && i < _p; loc++, ltan++, ++i) {
		m_Path.erase(loc);
		m_Tangents.erase(ltan);
	}
	m_TotalTime = m_Path.back().time;
}
void Smooth_Path::setStep( int _p, Step _step )
{
	std::list< Step >::iterator loc;
	int i = 0;
	for (loc = m_Path.begin(); loc != m_Path.end() && i < _p; loc++, ++i);
	if (loc != m_Path.end()) {
		loc->point.x = _step.point.x;
		loc->point.y = _step.point.y;
		loc->time = _step.time;
	}
	m_TotalTime = m_Path.back().time;
}
/// Cubic Hermite Spline using Catmull-Rom tangents
/// for interpolating x on the interval (x(k), x(k+1))
///
/// p(x) = h00(t)*p0 + h10(t)*h*m0 + h01(t)*p1 + h11(t)*h*m1
/// h = x(k+1) - x(k)
/// t = (x - x(k)) / h
///
/// h00(t) = (1 + 2*t)(1 - t)^2
/// h10(t) = t*(1 - t)^2
/// h01(t) = t^2(3 - 2*t)
/// h11(t) = t^2*(t - 1)
///
/// Tangent is found by
/// m(k) = (p(k+1) - p(k-1)) / 2
b2Vec2 Smooth_Path::getPosition( float _time ) const
{
	if (m_Repeat && _time > m_TotalTime)
		_time = fmod(_time, m_TotalTime);
	b2Vec2 pos;
	pos.x = 0.0f;
	pos.y = 0.0f;
	std::list< Step >::const_iterator p1, p2;
	std::list< b2Vec2 >::const_iterator m1, m2;
	float pTime = 0.0;
	for (p2 = m_Path.begin(), m2 = m_Tangents.begin(); p2 != m_Path.end() && p2->time <= _time; ++p2, ++m2);
	if (p2 == m_Path.end())
		return pos;

	p1 = p2; --p1;
	m1 = m2; --m1;
	float h = p2->time - p1->time;
	float t = (_time - p1->time) / h;

	float h00 = (1 + 2*t)*(1 - t)*(1 - t);
	float h10 = t*(1 - t)*(1 - t);
	float h01 = t*t*(3 - 2*t);
	float h11 = t*t*(t - 1);

	pos = h00*p1->point + h10*(*m1) + h01*p2->point + h11*(*m2);
	return pos;
}






void Bezier_Path::addStep( Step _step )
{
	std::list< Step >::iterator loc;
	std::list< b2Vec2 >::iterator ltan = m_Tangents.begin();
	m_Path.push_back(_step);
	int i = 0;
	if (m_TotalTime < _step.time)
		m_TotalTime = _step.time;
	b2Vec2 tan;
	int size = m_Path.size();
	if (size >= 3) {
		std::list< Step >::iterator it1, it2;
		it1 = m_Path.end();
		--it1; --it1; --it1;
		it2 = m_Path.end();
		--it2;
		tan = 0.75f * (it2->point - it1->point);
		//modify last
		m_Tangents.pop_back();
		m_Tangents.push_back( tan );
	}
	//new
	tan.x = 0.0f;
	tan.y = 0.0f;
	m_Tangents.push_back( tan );
}
void Bezier_Path::insertStep( Step _step )
{
	std::list< Step >::iterator loc;
	std::list< b2Vec2 >::iterator ltan = m_Tangents.begin();
	for (loc = m_Path.begin(); loc != m_Path.end() && loc->time < _step.time; loc++);
	m_Path.insert(loc,_step);
	int i = 0;
	for (loc = m_Path.begin(); loc != m_Path.end() && loc->time != _step.time; loc++, ltan++, ++i);
	if (m_TotalTime < _step.time)
		m_TotalTime = _step.time;
	b2Vec2 tan;
	int size = m_Path.size();
	if (size >= 3 && i < (size-1) && i > 1) {
		std::list< Step >::iterator it1, it2;
		it1 = loc;//assign to current
		--it1;//we want previous
		it2 = loc;//assign to current
		it2++;
		tan = 0.75f * (it2->point - it1->point);
		//modify previous tangent
		ltan--;
		ltan->x = tan.x;
		ltan->y = tan.y;
		ltan++;//return to next tangent
	}
	//new
	tan.x = 0.0f;
	tan.y = 0.0f;
	m_Tangents.insert( ltan, tan );
}
void Bezier_Path::deleteStep( int _p )
{
	std::list< Step >::iterator loc = m_Path.begin();
	std::list< b2Vec2 >::iterator ltan = m_Tangents.begin();
	int i = 0;
	for (; loc != m_Path.end() && i < _p; loc++, ltan++, ++i);
	m_Path.erase(loc);
	m_Tangents.erase(ltan);
	m_TotalTime = m_Path.back().time;
}
void Bezier_Path::setStep( int _p, Step _step )
{
	std::list< Step >::iterator loc;
	int i = 0;
	for (loc = m_Path.begin(); loc != m_Path.end() && i < _p; loc++, ++i);
	if (loc != m_Path.end()) {
		loc->point.x = _step.point.x;
		loc->point.y = _step.point.y;
		loc->time = _step.time;
	}
	m_TotalTime = m_Path.back().time;
}
/// Cubic Bezier Spline
/// Formed of four points per segment
/// p0 p1 p2 p3
/// p0 and p3 lie on path
/// p1 and p2 approximate path
/// B(t) = (1 - t)^3*p0 + 3*(1 - t)^2*t*p1 + 3*(1 - t)*t^2*p2 + t^3*p3
b2Vec2 Bezier_Path::getPosition( float _time ) const
{
	if (m_Repeat && _time > m_TotalTime)
		_time = fmod(_time, m_TotalTime);
	b2Vec2 pos;
	pos.x = 0.0f;
	pos.y = 0.0f;
	std::list< Step >::const_iterator p1, p2;
	std::list< b2Vec2 >::const_iterator m1, m2;
	float pTime = 0.0;
	for (p2 = m_Path.begin(), m2 = m_Tangents.begin(); p2 != m_Path.end() && p2->time <= _time; ++p2, ++m2);
	if (p2 == m_Path.end())
		return pos;

	p1 = p2; --p1;
	m1 = m2; --m1;
	float h = p2->time - p1->time;
	float t = (_time - p1->time) / h;
	float it = 1 - t;

	pos = it*it*it*p1->point + 3*it*it*t*(p1->point+*m1) + t*t*t*p2->point + 3*it*t*t*(p2->point-*m2);
	return pos;
}