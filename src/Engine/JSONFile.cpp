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
     // json::grammar<char>::object, then there was an error.
     if(this->variant->type() != typeid(json::grammar<char>::object)) 
     {
	  // do something to handle the error
	  return; 		
     }
	
     this->config = boost::any_cast<json::grammar<char>::object>(*this->variant);	
}
/*
template <class T>
T JSONFile::get(std::string name, 
		const boost::shared_ptr<boost::any> object)
{
     int index = name.find(".", 0);
     
     json::grammar<char>::object o = object ? boost::any_cast<json::grammar<char>::object>(*object) : this->config;
  
     if (index != std::string::npos)
     {
	  return get<T>(name.substr(index+1), o[name.substr(0, index)]);    
     } 
     else 
     {
	  if(o[name])
	       return boost::any_cast<T>(*o[name]);
     }
  
     return T();
}
*/
