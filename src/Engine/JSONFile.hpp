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
     void readFile(std::string fileName);

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
     template <class T>
     T get(std::string name, 
	    const boost::shared_ptr<boost::any> object = boost::shared_ptr<boost::any>());

     
     
private:
     json::grammar<char>::variant variant;
     json::grammar<char>::object config; 
     std::string fileName;
     std::string fileContent;
};

#endif
