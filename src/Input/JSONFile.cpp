/*
  JSONFile.cpp
  Mizzou Game Engine
  
  
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
 
#include "JSONFile.hpp"
#include <fstream>


JSONFile::JSONFile(std::string fileName)
{
     this->readFile(fileName);
     this->parseContent();
}

void JSONFile::readFile(const std::string& fileName)
{
     this->fileName = fileName;
     std::ifstream in(fileName.c_str());
     
     getline(in, this->fileContent, (char)EOF);
     in.close();	    
}

void JSONFile::parseContent()
{
     this->variant = json::parse(this->fileContent.begin(), this->fileContent.end());

     // If this.variant is not of the type
     // json::grammar< char >::object, then there was an error.
     if(this->variant->type() != typeid(json::grammar< char >::object)) 
     {
	  // do something to handle the error
	  return; 		
     }
	
     this->config = boost::any_cast< json::grammar< char >::object >(*this->variant);	
}

bool JSONFile::isAnArray(const std::string& name)
{
     return checkType< json::grammar<char>::array >(name);
}
