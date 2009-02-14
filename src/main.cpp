/*
 *  Main.cpp
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
 #include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <Gosu/Window.hpp>

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include "Engine/MUGE.h"
#include "TitleState.h"


int main(int argc, char* argv[])
{
    MUGE win;
    win.pushState( TitleState::instance() );
    win.show();
    return 0;
}
