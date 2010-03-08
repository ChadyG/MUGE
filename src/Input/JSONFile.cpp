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

bool JSONFile::writeFile(std::string file)
{
	std::ofstream outputStream;
	outputStream.open(file.c_str(), std::ios_base::trunc);
	if( !outputStream ) {
		throw std::runtime_error("File could not be opened: " + file);
	}
	outputStream.close();
	return writeFile(file, this->variant, 0);
}

bool JSONFile::writeFile(std::string file, json::grammar<char>::variant& var, int depth)
{
	std::ofstream outputStream;
	outputStream.open(file.c_str(), std::ios_base::app);
	if( !outputStream ) {
		throw std::runtime_error("File could not be opened: " + file);
	}

	if(var->type() == typeid(bool)) {
		// variant is of type "bool"...
		if (boost::any_cast< bool >(*var))
			outputStream << "true";
		else
			outputStream << "false";
	}
	else if(var->type() == typeid(int)) {
		// variant is of type "int"...
		outputStream << boost::any_cast< int >(*var);
	}
	else if(var->type() == typeid(double)) {
		// variant is of type "double"...
		outputStream.setf(std::ios_base::showpoint);
		//outputStream.setf(std::ios_base::fixed);
		outputStream << boost::any_cast< double >(*var);
	}
	else if(var->type() == typeid(std::string)) {
		// variant is a string...
		outputStream << "\"" << boost::any_cast< std::string >(*var) << "\"";
	}
	else if(var->type() == typeid(json::grammar<char>::array)) {
	// variant is an array => use recursion
		json::grammar<char>::array const & a = boost::any_cast< json::grammar<char>::array >(*var);
		
		outputStream << "[";
		for(json::grammar<char>::array::const_iterator it = a.begin(); it != a.end(); ++it) {
			outputStream << "\n";
			for (int i = 0; i < depth + 1; ++i)
				outputStream << "\t";
			outputStream.flush();
			bool valid = writeFile( file, boost::any_cast< json::grammar<char>::variant >(*it), depth + 1);
			if (++it != a.end() && valid)
				outputStream << ",";
			--it;
		}
		outputStream << "\n";
		for (int i = 0; i < depth; ++i)
			outputStream << "\t";
		outputStream << "]";
	}
	else if(var->type() == typeid(json::grammar<char>::object)) {
		// variant is an object => use recursion
		json::grammar<char>::object const & o = boost::any_cast< json::grammar<char>::object >(*var);
 
		outputStream << "{";
		for(json::grammar<char>::object::const_iterator it = o.begin(); it != o.end(); ++it) {
			outputStream << "\n";
			for (int i = 0; i < depth+1; ++i)
				outputStream << "\t";
			outputStream << "\"" << (*it).first << "\" : ";
			outputStream.flush();
			writeFile( file, boost::any_cast< json::grammar<char>::variant >((*it).second), depth + 1);
			if (++it != o.end())
				outputStream << ",";
			--it;
		}
		outputStream << "\n";
		for (int i = 0; i < depth; ++i)
			outputStream << "\t";
		outputStream << "}";
	}else{
		// ERROR: unknown type...
		return false;
	}

	outputStream.close();
	return true;
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

bool JSONFile::hasKey(const std::string& key, const boost::shared_ptr< boost::any > object)
{
	json::grammar< char >::object o = object ? boost::any_cast< json::grammar< char >::object >(*object) : this->config;
    return (o.find(key) != o.end());
}

// ------------------------------------------------------------------
bool JSONFile::setInt(const std::string& name, 
	int val)
{
	json::grammar< char >::variant var(new boost::any(val));
	if (set(name, var)) {
		this->variant = boost::shared_ptr<boost::any>(new boost::any(this->config));
		return true;
	}else
		return false;
}

bool JSONFile::setDouble(const std::string& name, 
	double val)
{
	json::grammar< char >::variant var(new boost::any(val));
	if (set(name, var)) {
		this->variant = boost::shared_ptr<boost::any>(new boost::any(this->config));
		return true;
	}else
		return false;
}

bool JSONFile::setString(const std::string& name, 
	std::string val)
{
	json::grammar< char >::variant var(new boost::any(val));
	if (set(name, var)) {
		this->variant = boost::shared_ptr<boost::any>(new boost::any(this->config));
		return true;
	}else
		return false;
}

bool JSONFile::setBoolean(const std::string& name, 
	bool val)
{
	json::grammar< char >::variant var(new boost::any(val));
	if (set(name, var)) {
		this->variant = boost::shared_ptr<boost::any>(new boost::any(this->config));
		return true;
	}else
		return false;
}

bool JSONFile::set(const std::string& name, 
	json::grammar< char >::variant val, 
	const boost::shared_ptr< boost::any > object)
{
	unsigned int index = name.find(".", 0);     
	
	json::grammar< char >::object *o = object ? &boost::any_cast< json::grammar< char >::object >(*object) : &(this->config);
	
	// The variable "name" still contains an object
	if (index != std::string::npos)
	{				
		return set(name.substr(index+1), val, get< boost::shared_ptr< boost::any > >(name.substr(0, index), object));
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
			var = (*o)[name.substr(0, arrayOperatorIndex)];
		}
		else
		{
			//if(o[name])
			//{
				//if(typeid(T) == typeid(boost::shared_ptr< boost::any >)) {
					(*o)[name] = val;
					return true;
				//}else{
				//	(*o[name]) = val,;
				//	return true;
				//}
			//}
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
				boost::any_cast< json::grammar<char>::array& >(*var).at(arrayIndex) = val;
				return true;
			}
			// more []
			else
			{
				var = getIndex< boost::shared_ptr< boost::any > >(var, arrayIndex);
			}	       
		}
	}
	
	return false;
}