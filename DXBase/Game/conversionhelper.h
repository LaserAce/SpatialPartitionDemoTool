#ifndef _CONVERSION_HELPER_H_
#define _CONVERSION_HELPER_H_

#include <string>
#include <vector>
#include "SimpleMath.h"
#include <fstream>

using std::string;
using std::vector;
using std::to_string;
using std::ofstream;

using namespace DirectX;
using namespace SimpleMath;

struct ConversionHelper
{
	static bool LoadFunction(vector<char>* details, string variableName, bool returnType)
	{
		string stringToConvert;
		int stringPosition = 0;
		int vectorElement = 0;

		FindVariableName(details, variableName, stringPosition);

		stringToConvert = GetVariableData(details, stringPosition);

		return static_cast<bool>(atoi(stringToConvert.c_str()));
	}

	static int LoadFunction(vector<char>* details, string variableName, int returnType)
	{
		string stringToConvert;
		int stringPosition = 0;
		int vectorElement = 0;
		
		FindVariableName(details, variableName, stringPosition);
		
		stringToConvert = GetVariableData(details, stringPosition);

		return atoi(stringToConvert.c_str());
	}

	static float LoadFunction(vector<char>* details, string variableName, float returnType)
	{
		string stringToConvert;
		int stringPosition = 0;
		int vectorElement = 0;

		FindVariableName(details, variableName, stringPosition);

		stringToConvert = GetVariableData(details, stringPosition);

	    return std::stof(stringToConvert.c_str());
	}

	static Vector3 LoadFunction(vector<char>* details, string variableName, Vector3 returnType)
	{
		Vector3 returnValue;
		string stringToConvert;
		int stringPosition = 0;
		int vectorElement = 0;

		//The <X> <Y> <Z> should be in that order in the file
		FindVariableName(details, variableName, stringPosition);

		FindVariableName(details, "<X>", stringPosition);
		stringToConvert = GetVariableData(details, stringPosition);
		returnValue.x = std::stof(stringToConvert.c_str());

		FindVariableName(details, "<Y>", stringPosition);
		stringToConvert = GetVariableData(details, stringPosition);
		returnValue.y = std::stof(stringToConvert.c_str());

		FindVariableName(details, "<Z>", stringPosition);
		stringToConvert = GetVariableData(details, stringPosition);
		returnValue.z = std::stof(stringToConvert.c_str());

		return returnValue;
	}

	static Color LoadFunction(vector<char>* details, string variableName, Color returnType)
	{
		Color returnValue;
		string stringToConvert;
		int stringPosition = 0;
		int vectorElement = 0;

		//The <X> <Y> <Z> should be in that order in the file
		FindVariableName(details, variableName, stringPosition);

		FindVariableName(details, "<R>", stringPosition);
		stringToConvert = GetVariableData(details, stringPosition);
		returnValue.R(std::stof(stringToConvert.c_str()));

		FindVariableName(details, "<G>", stringPosition);
		stringToConvert = GetVariableData(details, stringPosition);
		returnValue.G(std::stof(stringToConvert.c_str()));

		FindVariableName(details, "<B>", stringPosition);
		stringToConvert = GetVariableData(details, stringPosition);
		returnValue.B(std::stof(stringToConvert.c_str()));

		FindVariableName(details, "<A>", stringPosition);
		stringToConvert = GetVariableData(details, stringPosition);
		returnValue.A(std::stof(stringToConvert.c_str()));

		return returnValue;
	}

	//The string that gets read will have no spaces!
	static string LoadFunction(vector<char>* details, string variableName, string returnType)
	{
		int stringPosition = 0;
		int vectorElement = 0;

		FindVariableName(details, variableName, stringPosition);

		return GetVariableData(details, stringPosition);
	}

	static string SaveFunction(string variableName, bool saveType)
	{
		string returnString;

		returnString += FrameName(variableName);

		returnString += to_string(static_cast<int>(saveType));

		returnString += FrameName(variableName);

		//	returnString.push_back('\n');

		return returnString;
	}
	//This may have to be changed to not be a char pointer but a vector of chars
	static string SaveFunction(string variableName, int saveType)
	{
		string returnString;

		returnString += FrameName(variableName);

		returnString += to_string(saveType);

		returnString += FrameName(variableName);

	//	returnString.push_back('\n');

		return returnString;
	}

	static string SaveFunction(string variableName, float saveType)
	{
		string returnString;

		returnString += FrameName(variableName);

		returnString += to_string(saveType);

		returnString += FrameName(variableName);

		//returnString.push_back('\n');

		return returnString;
	}

	static string SaveFunction(string variableName, Vector3 saveType)
	{
		string returnString;

		returnString += FrameName(variableName);

		returnString += SaveFunction("X", to_string(saveType.x));

		returnString += SaveFunction("Y", to_string(saveType.y));

		returnString += SaveFunction("Z", to_string(saveType.z));

		returnString += FrameName(variableName);

	//	returnString.push_back('\n');

		return returnString;
	}

	static string SaveFunction(string variableName, Color saveType)
	{
		string returnString;

		returnString += FrameName(variableName);

		returnString += SaveFunction("R", to_string(saveType.R()));

		returnString += SaveFunction("G", to_string(saveType.G()));

		returnString += SaveFunction("B", to_string(saveType.B()));

		returnString += SaveFunction("A", to_string(saveType.A()));

		returnString += FrameName(variableName);

		//returnString.push_back('\n');

		return returnString;
	}

	static string SaveFunction(string variableName, string saveType)
	{
		string returnString;

		returnString += FrameName(variableName);

		returnString += saveType;

		returnString += FrameName(variableName);

		//returnString.push_back('\n');

		return returnString;
	}

	//Will return \n or a string in the format "<...>"
	static string GetStrings(vector<char>* dataLine, int& dataLinePosition)
	{
		string returnString;
		bool readString = false;
		while (true) //Yes I did just do that, this process shouldn't stop till it realised it is finished
		{
			//If I should be saving characters to the return string
			if (readString)
			{
				returnString.push_back((*dataLine)[dataLinePosition]);
				//I have reached the end of what I want to copy
				if ((*dataLine)[dataLinePosition] == '>')
				{
					//Increase dataLinePosition ready for next function call then break out
					dataLinePosition++;
					break;
				}
			}
			//Only start adding characters to the return string after hitting this
			if ((*dataLine)[dataLinePosition] == '<')
			{
				returnString.push_back((*dataLine)[dataLinePosition]);
				readString = true;
			}
			dataLinePosition++;
		}
		return returnString;
	}

	static void FindVariableName(vector<char>* details, string variableName, int& stringPosition)
	{
		string stringComparison;
		
		while (true)   //Yes...I know what i'm doing...
		{
			stringComparison = GetStrings(details, stringPosition);
			
			if (stringComparison.compare(variableName) == 0)
			{
				break;
			}
		}
	}
	
	static string GetVariableData(vector<char>* details, int& stringPosition)
	{
		string variableString;
		while (true)
		{
			if ((*details)[stringPosition] == '<')
			{
				break;
			}
			variableString.push_back((*details)[stringPosition]);
			stringPosition++;
		}
		return variableString;
	}

	static string FrameName(string name)
	{
		string framedName = "<";
		framedName += name;
		framedName += ">";
		return framedName;
	}

	static void SaveString(string save, ofstream* saveFile)
	{
		for (int i = 0; i < save.size(); i++)
		{
			saveFile->put(save[i]);
		}
	}

};


#endif