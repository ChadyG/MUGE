#include "Environment.h"
#include "tinyjson/tinyjson.hpp"
#include "ContactListener.h"
#include "Camera.h"
#include <iostream>
#include <fstream>

Environment::Environment(std::wstring levelFile, Gosu::Graphics &graphics)
{
	b2AABB worldAABB;
	worldAABB.lowerBound.Set( -50.0f, -50.0f);
	worldAABB.upperBound.Set( 50.0f, 50.0f);
	b2Vec2 gravity( 0.0f, 0.0f);
	bool do_sleep = true;
	m_ContactListenerp = new MyContactListener();
	m_Worldp = new b2World(worldAABB, gravity, do_sleep);
	m_Worldp->SetContactListener( m_ContactListenerp );

	m_Iterations = 10;
	m_TimeStep = 1.0f / 60.0f;

	//
	// Read in JSON encoded file
	// TinyJSON library will perform lexing and parsing
	// pull out data using std::map and casting from boost::any
	//
/*
	std::ifstream inputStream;
	inputStream.open(levelFile.c_str());
	if( !inputStream ) {
		throw 2;
	}

	std::string strInput;
	getline(inputStream, strInput, (char)EOF);

	json::grammar<std::string>::variant v
		= json::parse(strInput.begin(), strInput.end());
 
	if(v->type() != typeid(json::grammar<char>::object))
	{
		throw 3;
	}

	//
	//	Begin data extraction
	//

	json::grammar<char>::object level = boost::any_cast< json::grammar<char>::object >(*v);
	json::grammar<char>::array arr = boost::any_cast<json::grammar<char>::array>(*level["extents"]);
	json::grammar<char>::array::iterator it;
	json::grammar<char>::object o;

	m_aExtents[0] = boost::any_cast< int >(*arr[0]);
	m_aExtents[1] = boost::any_cast< int >(*arr[1]);

	m_iUnits =  boost::any_cast< int >(*level["unitpixel"]);

	arr = boost::any_cast<json::grammar<char>::array>(*level["person"]);
	m_Person = new Person(graphics, *m_Worldp, b2Vec2(boost::any_cast< double >(*arr[0]), boost::any_cast< double >(*arr[1])), 10.0);

	//
	//	Wall Definitions
	//

	arr = boost::any_cast<json::grammar<char>::array>(*level["walls"]);
	b2Vec2 s, e;
	json::grammar<char>::array arr2;
	for (it = arr.begin(); it != arr.end(); ++it) {
		o = boost::any_cast< json::grammar<char>::object >(**it);

		arr2 = boost::any_cast<json::grammar<char>::array>(*o["start"]);
		s = b2Vec2(boost::any_cast< double >(*arr2[0]), boost::any_cast< double >(*arr2[1]));

		arr2 = boost::any_cast<json::grammar<char>::array>(*o["end"]);
		e = b2Vec2(boost::any_cast< double >(*arr2[0]), boost::any_cast< double >(*arr2[1]));

		wallDef wDef = {&graphics, m_Worldp, s, e, m_iUnits};
		m_vWalls.push_back( Wall(wDef) );
	}

	//
	//	Sensor Definitions (must be after walls)
	//

	arr = boost::any_cast<json::grammar<char>::array>(*level["sensors"]);
	for (it = arr.begin(); it != arr.end(); ++it) {
		o = boost::any_cast< json::grammar<char>::object >(**it);

		arr2 = boost::any_cast<json::grammar<char>::array>(*o["position"]);
		s = b2Vec2(boost::any_cast< double >(*arr2[0]), boost::any_cast< double >(*arr2[1]));

		sensorDef sDef = {&graphics, m_Worldp, s,
			boost::any_cast< double >(*o["cone"]),
			boost::any_cast< double >(*o["direction"]),
			boost::any_cast< double >(*o["falloff"]),
			m_iUnits};
		m_vSensors.push_back( Sensor(sDef) );
	}

	//
	//	End JSON
	//

	std::vector<Sensor>::iterator is;
	for (is = m_vSensors.begin(); is != m_vSensors.end(); ++is) {
		is->updateBodyUD();
		is->playerIn( false );
		is->update();
	}
*/
}

void Environment::update(const Gosu::Window &window, int offX, int offY)
{
	m_Worldp->Step(m_TimeStep, m_Iterations);
	
	m_aOrigin[0] = offX + window.graphics().width()/2;
	m_aOrigin[1] = offY + window.graphics().height()/2;

}

void Environment::draw(bool lines) const
{

}
