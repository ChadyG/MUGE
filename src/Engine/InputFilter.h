#include <Gosu/Gosu.hpp>
/**
*
*/
class InputFilter
{
	unsigned int m_Input;
public:
	InputFilter(const std::wstring &configFile);
	void reset();
	void setDown();
	void setUp();
	void setRight();
	void setLeft();
	
	bool Down();
	bool Up();
	bool Right();
	bool Left();
};