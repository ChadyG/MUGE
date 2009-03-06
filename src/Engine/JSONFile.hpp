#ifndef JSONFILE_H
#define JSONFILE_H

#include "tinyjson/tinyjson.hpp"
#include <string>

class JSONFile
{
public:
     JSONFile(std::string fileName);
		
     // -------------------------------------------------------------
     // The purpose of splitting up parsing a file into seperate
     // methods (readFile() and parseContent()) is to allow dynamic
     // changing of the jsonfile that this represents.  More
     // specifically, this will let implementing writing json files
     // (and using them) in this class easier, hopefully.  Writing
     // will probably be implemented by modifying this->fileContent
     // then writing it to the file.
     // -------------------------------------------------------------
		
     /**
      * readFile - read a file, but not parse it yet
      * @fileName: the path to the file to read
      *
      * This will store the file path in this->fileName and store its content in this->fileContent
      **/
     void readFile(const std::string& fileName);
		
     /**
      * parseContent - parse the content of the file using tinyjson
      *
      * This will use tinyjson to parse this->fileContent.
      **/
     void parseContent();
		
     /**
      * get - read a variable from a JSON file
      * @name: the name of the variable, including any object it belongs to (example: object1.variable)
      * @object: this is used in recursive calls to find the variable in any object
      *
      * get() will call itself if the variable is in an
      * object, passing the left-most object as the object argument
      * and the rest as the name. This permits something like
      * object1.object2.object3.variable to be passed into name.  The
      * type for the template should be the expected type of the
      * variable.
      **/
     template < class T >
     T get(const std::string& name, 
	   const boost::shared_ptr< boost::any > object = boost::shared_ptr< boost::any >());

     /**
      * checkType - checks whether the value of the specified object has the specified type
      * @name: the name of the variable, including any object it belongs to (example: object1.variable)
      **/
     template < class T >
     bool checkType(const std::string& name);

     /**
      * isAnArray - checks whether the value of the specified object is a JSON array
      * @name: the name of the variable, including any object it belongs to (example: object1.variable)
      **/
     bool isAnArray(const std::string& name);
		
private:
     json::grammar< char >::variant variant;
     json::grammar< char >::object config; 
     std::string fileName;
     std::string fileContent;
		
     template < class T >
     T getIndex(boost::shared_ptr< boost::any > a, unsigned int index);
};


// ------------------------------------------------------------------

template < class T >
bool JSONFile::checkType(const std::string& name)
{
     json::grammar<char>::variant value = get< json::grammar<char>::variant >(name);

     return (value->type() == typeid(T));
}

// ------------------------------------------------------------------
template < class T >
T JSONFile::get(const std::string& name, 
		const boost::shared_ptr< boost::any > object)
{
     unsigned int index = name.find(".", 0);     
	
     json::grammar< char >::object o = object ? boost::any_cast< json::grammar< char >::object >(*object) : this->config;
	
     // The variable "name" still contains an object
     if (index != std::string::npos)
     {				
	  return get< T >(name.substr(index+1), get< boost::shared_ptr< boost::any > >(name.substr(0, index), object));
     } 
     // "object" is now the direct parent of the variable "name"
     else 
     {
	  // check to see if the variable we want is an array by
	  // seeing if it has [] in the name	  
	  unsigned int arrayOperatorIndex = name.find("[", 0);
	  unsigned int arrayIndex = 0;
	  boost::shared_ptr< boost::any > var = boost::shared_ptr< boost::any >();
			
	  if(arrayOperatorIndex != std::string::npos)
	  {
	       var = o[name.substr(0, arrayOperatorIndex)];
	  }
	  else
	  {
	       if(o[name])
	       {
		    if(typeid(T) == typeid(boost::shared_ptr< boost::any >))
			 return boost::any_cast< T >(o[name]);
		    else
			 return boost::any_cast< T >(*o[name]);
	       }
	  }
	  
	  while(arrayOperatorIndex != std::string::npos && var)
	  {
	       arrayIndex = boost::lexical_cast< int >(name.substr(arrayOperatorIndex + 1, 
								 name.find("]", arrayOperatorIndex) - 
								 arrayOperatorIndex - 1));
			
	       arrayOperatorIndex = name.find("[", arrayOperatorIndex+1);
			
			
	       // no more []
	       if(var && arrayOperatorIndex == std::string::npos)
	       {
		    return getIndex< T >(var, arrayIndex);
	       }
	       // more []
	       else
	       {
		    var = getIndex< boost::shared_ptr< boost::any > >(var, arrayIndex);
	       }	       
	  }
     }
	
     return T();
}


// ------------------------------------------------------------------
template <class T>
T JSONFile::getIndex(boost::shared_ptr< boost::any > var, unsigned int index)
{
	
     json::grammar< char >::array const & a = boost::any_cast< json::grammar<char>::array >(*var);
	
     unsigned int i = 0;
     for(json::grammar< char >::array::const_iterator it = a.begin(); it != a.end(); ++it, ++i)
     {
		
	  if(i == index)
	  {
	       // if we want to return a boost:shared_ptr, then
	       // dereferencing it twice will screw up
	       if(typeid(T) == typeid(boost::shared_ptr< boost::any >))
		    return boost::any_cast< T >(*it);
	       else
		    return boost::any_cast< T >(**it);
	  }
     }
     return T();
}

#endif
