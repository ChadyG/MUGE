#include "JSONFile.hpp"
#include <fstream>

JSONFile::JSONFile(std::string fileName)
{
     this->readFile(fileName);
     this->parseContent();
}

void JSONFile::readFile(std::string fileName)
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
