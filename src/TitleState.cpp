#include "TitleState.h"
#include "Engine/MUGE.h"

/**
*
*/

TitleState TitleState::m_StateInstance;

void TitleState::init(Gosu::Graphics &graphics)
{
	std::wstring filename = Gosu::sharedResourcePrefix() + L"Images/MUGDTitle.png";
	m_TitleScreen.reset(new Gosu::Image(graphics, filename, false));
	
	counter = 256;
	step = 0;
}

void TitleState::cleanup()
{
	m_TitleScreen.reset(0);
}

void TitleState::pause()
{

}

void TitleState::resume()
{

}

void TitleState::update(const Gosu::Input &input, MUGE* engine)
{
	if (input.down(Gosu::kbReturn) || input.down(Gosu::kbSpace) || counter <= 0) {
		engine->popState();//GameMenu.instance());
	}
	--counter;
}

void TitleState::draw() const
{
	m_TitleScreen->draw(0,0,0, 1,1, Gosu::Color::fromHSV(0, 0, counter));
}