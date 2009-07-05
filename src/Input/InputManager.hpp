/*
 *  InputManager.hpp
 *  Destructible
 *
 *  Created by Zach Conn on 02/12/09.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <map>
#include <Gosu/Gosu.hpp>
#include <boost/signals.hpp>
#include <boost/shared_ptr.hpp>
#include "JSONFile.hpp"


/**
 * InputManager - loads a table of command bindings from a JSON file and provides a signal/slot interface
 * so clients can hook into command events
 **/
class InputManager 
{

public:

	typedef std::map< std::string, Gosu::ButtonName > ButtonGosuValueTable;

	typedef boost::signal< void () > CommandSignalType;
	typedef boost::shared_ptr< CommandSignalType > CommandSignalPtr;

	typedef std::map< Gosu::ButtonName, CommandSignalPtr > CommandTable;
	typedef std::map< std::string, std::pair< CommandTable, CommandTable > > ContextTable;

	InputManager();
	~InputManager();
		
     /**
      * hookIntoCommand - uses Boost.Signals to register a function as a receiver of a certain input event
      * @command: this should be in the form "context.commandName:direction," where context specifies an
      * object in the JSON binding file with a variable named commandName and direction is either "Up" or "Down".
      * @slot: the function that should be registered as a listener
      **/
	void hookIntoCommand(const std::string& command, const CommandSignalType::slot_type& slot);

     /**
      * buttonDownHandler - fires any signals associated with this key being pressed down
      **/
	void buttonDownHandler(Gosu::Button button);

     /**
      * buttonUpHandler - fires any signals associated with this key being lifted
      **/
	void buttonUpHandler(Gosu::Button button);

     /**
      * setCurrentContext - changes the current context in which the input manager is operating
      **/
	void setCurrentContext(const std::string& newContext);
		
private:
     /**
      * buildButtonGosuValueTable - uses a macro trick to convert a specific subset of 
      * the Gosu::ButtonName enumeration into a string table, which is used to convert
      * strings from the JSON binding file into ButtonName values
      **/
	void buildButtonGosuValueTable();

     /**
      * getDownCommandTableForCurrentContext - gets the table of commands that respond to key presses in the current context
      **/
	CommandTable& getDownCommandTableForCurrentContext();

     /**
      * getUpCommandTableForCurrentContext - gets the table of commands that respond to key releases in the current context
      **/
	CommandTable& getUpCommandTableForCurrentContext();

     /**
      * checkCommandTableForUpdates - given a command table, this checks to see if any of its commands correspond to the provided button
      * and sends out appropriate signals if so. This assumes that the button was pressed/released.
      **/
	void checkCommandTableForUpdates(CommandTable& table, Gosu::Button button);

     /**
      * hookSignalToSlot - this is a helper method for hookIntoCommand
      **/
	void hookSignalToSlot(const std::string& key, CommandTable& table, const CommandSignalType::slot_type& slot);
		
private:
	ButtonGosuValueTable buttonGosuValueAssociations;
	ContextTable contexts;
	std::string currentContext;
	JSONFile bindingFile;
};

#endif
