/*
   InputManager.cpp
   Mizzou Game Engine
 
   Created by Zach Conn on 02/12/09.
  
  
 Copyright 2009 Mizzou Game Design.

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

#include "InputManager.hpp"
#include <boost/algorithm/string.hpp>
#include <cassert>
#include <iostream>

InputManager::InputManager() : bindingFile(JSONFile(Gosu::narrow(Gosu::resourcePrefix()) + "Data/commandBindings.json")), currentContext("Default")
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
     // -- Break "context.command:direction" into "context.command" and "direction"

     std::vector< std::string > tokens;
     boost::split(tokens, command, boost::is_any_of(":"));

     std::string commandName = tokens.at(0);
     //std::string key = bindingFile.get< std::string >(commandName);

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

     // -- We need to figure out if this is a single string or an array

     if (bindingFile.isAnArray(commandName))
     {
	  // Loop through all the keys associated with this command and for each create a signal that hooks into the provided slot
	  std::string key;
	  int index = 0;

	  while ((key = bindingFile.get< std::string >(commandName + "[" + boost::lexical_cast<std::string>(index) + "]")) != std::string())
	  {
	       hookSignalToSlot(key, table, slot);
	       ++index;
	  }
     }
     else
     {
	  std::string key = bindingFile.get< std::string >(commandName);
	  hookSignalToSlot(key, table, slot);
     }
}

void InputManager::hookSignalToSlot(const std::string& key, CommandTable& table, const CommandSignalType::slot_type& slot)
{
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

void InputManager::checkCommandTableForUpdates(CommandTable& table, Gosu::Button button)
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
