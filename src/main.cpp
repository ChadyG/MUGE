#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <Gosu/Window.hpp>

#include <boost/scoped_ptr.hpp> // Used throughout Gosu and this tutorial.
#include <boost/shared_ptr.hpp> // Learn them, they're moving into standard C++!
#include <boost/lexical_cast.hpp> // Could also use <sstream>, just for int <-> string conversion

#include "Engine/MUGE.h"
#include "TitleState.h"


int main(int argc, char* argv[])
{
    MUGE win;
    win.pushState( TitleState::instance() );
    win.show();
    return 0;
}