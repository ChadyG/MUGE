/*
 *  InputManager.cpp
 *  Destructible
 *
 *  Created by Zach Conn on 02/12/09.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */

#include "InputManager.hpp"
#include <boost/algorithm/string.hpp>
#include <cassert>

InputManager::InputManager() : bindingFile(JSONFile("Data/commandBindings.json")), currentContext("Default")
{
	buildButtonGosuValueTable();
}

InputManager::~InputManager()
{
	buttonGosuValueAssociations.clear();
	contexts.clear();
}

void InputManager::hookIntoCommand(const std::string& command, const CommandSignalType::slot_type& slot)
{
     // -- Break "context.commandName:direction" into "context.commandName" and "direction"

     std::vector< std::string > tokens;
     boost::split(tokens, command, boost::is_any_of(":"));

     std::string commandName = tokens.at(0);
     std::string key = bindingFile.get< std::string >(commandName);

     std::string direction = boost::to_lower_copy(tokens.at(1));

     // -- Extract "context" from "context.name"

     boost::split(tokens, commandName, boost::is_any_of("."));

     std::string contextName = tokens.at(0);

     // -- Create a new context if necessary

     if (contexts.find(contextName) == contexts.end())
     {
	  contexts[contextName] = std::pair< CommandTable, CommandTable >(CommandTable(), CommandTable());
     }

     // -- Find the appropriate command table based on "direction"

     CommandTable& table = (direction == "down") ? contexts[contextName].first : contexts[contextName].second;

     // -- Create a new signal if necessary

     Gosu::ButtonName buttonName = buttonGosuValueAssociations[key];

     if (table.find(buttonName) == table.end())
     {
	  table[buttonName] = CommandSignalPtr(new CommandSignalType());
     }

     CommandSignalPtr sig = table[buttonName];

     // -- Finally, connect the slot to the signal

     sig->connect(slot);
}

void InputManager::setCurrentContext(const std::string& newContext)
{
     currentContext = newContext;
}

InputManager::CommandTable& InputManager::getDownCommandTableForCurrentContext()
{
     return contexts[currentContext].first;
}

InputManager::CommandTable& InputManager::getUpCommandTableForCurrentContext()
{
     return contexts[currentContext].second;
}

void InputManager::checkCommandTableForUpdates(InputManager::CommandTable& table, Gosu::Button button)
{
     for (CommandTable::const_iterator it = table.begin(); it != table.end(); ++it)
     {
	  if (button == (*it).first)
	  {
	       CommandSignalPtr sig = (*it).second;
	       (*sig)();
	  }
     }
}

void InputManager::buttonDownHandler(Gosu::Button button)
{
     CommandTable& downCommands = getDownCommandTableForCurrentContext();
     checkCommandTableForUpdates(downCommands, button);
}

void InputManager::buttonUpHandler(Gosu::Button button)
{
     CommandTable& upCommands = getUpCommandTableForCurrentContext();
     checkCommandTableForUpdates(upCommands, button);
}

void InputManager::buildButtonGosuValueTable()
{
#define USE_INPUT_KEY(enumValue, textValue) \
	buttonGosuValueAssociations[textValue] = enumValue;
	#include "ButtonGosuValueTable.hpp"
#undef USE_INPUT_KEY
}
