/**
 * SpecCSV
 * base.cpp
 *
 * @author Humam O.
 */

#include "stdafx.h"

/*** Observation class members ***/

/** public **/

// constructors
Observation::Observation(std::string line)
{
	_Init = translate(line, '"');  // the raw line in the file
	_Numvals = _Nums();               // number of values
	_Vals = _Split();              // vector of values (as std::string)
}

// accessors
unsigned    Observation::getNumValues()  const
{
	return _Numvals;
}
std::string Observation::getRaw()        const
{
	return _Init;
}

// find functionality
int         Observation::find(const std::string& value) const
{
	int result = 0;

	for (std::string val : _Vals)
	{
		if (val == value)
		{
			return result;
		}
		result++;
	}

	return NEG;
}
size_t      Observation::findLongest()                  const
{
	size_t result;

	int iter = 0;
	size_t maxlen = 0;

	for (std::string value : _Vals)
	{
		if (value == "<empty>")
		{
			iter++;
			continue;
		}

		result = value.length() > maxlen ? iter : result;
		maxlen = value.length() > maxlen ? value.length() : maxlen;

		iter++;
	}

	return result;
}
std::string Observation::getLongest()                   const
{
	size_t result;

	int iter = 0;
	size_t maxlen = 0;

	for (std::string value : _Vals)
	{
		if (value == "<empty>")
		{
			iter++;
			continue;
		}

		result = value.length() > maxlen ? iter : result;
		maxlen = value.length() > maxlen ? value.length() : maxlen;

		iter++;
	}

	return _Vals[result];
}

// observation edit functionality
void        Observation::alter(size_t index, std::string edit)
{
	std::string temp;

	// mindf*ck!
	for (unsigned i = 0, len = _Vals.size(); i < len; i++)
	{
		if (i == index)  // apply edit
		{
			if (i == len - 1)  // at the end
			{
				if (edit == "<empty>")  // empty string
				{
					;
				}
				else  // not empty string
				{
					temp += edit;
				}
			}
			else  // at the mid
			{
				if (edit == "<empty>")  // empty string
				{
					temp += ",";
				}
				else  // not empty string
				{
					temp += edit + ",";
				}
			}
		}

		else  // the rest still the same
		{
			if (i == len - 1)  // at the end
			{
				if (_Vals[i] == "<empty>")  // empty string
				{
					;
				}
				else  // not empty string
				{
					temp += _Vals[i];
				}
			}
			else  // at the mid
			{
				if (_Vals[i] == "<empty>")  // empty string
				{
					temp += ",";
				}
				else  // not empty string
				{
					temp += _Vals[i] + ",";
				}
			}
		}
	}

	_Init = temp;
	_Vals = _Split();
}

// overloaded [] operator (indexer)
std::string Observation::operator[](size_t index)  const
{
	// don't touch this for now
	return _Vals.at(index);
}

/** private **/

// calculate number of fields
unsigned    Observation::_Nums()
{
	unsigned result = 0;
	for (char c : _Init)
	{
		if (c == ',')
		{
			result++;
		}
	}
	return ++result;
}

// vector of values
string_vec  Observation::_Split()
{
	std::string  temp = "";
	string_vec result;

	unsigned iter = 0;

	for (char c : _Init)
	{
		if (c != ',')
		{
			temp += c;
		}
		else
		{
			if (temp == "")
			{
				result.push_back("<empty>");
			}
			else
			{
				result.push_back(temp);
			}

			temp = "";
			iter++;
		}
	}

	// fix for the last value in an observation
	result.push_back(temp);

	return result;
}


// ============


/*** Frame class members ***/

/** public **/

// constructor and destructor
Frame::Frame(std::string filename)
{
	// open the file for reading and writing, and set the out pointer at the end of file 
	_CSV.open(filename, std::ios::in | std::ios::out | std::ios::app);
	_Fname = filename;              // save the filename for using with other members

	_Totallines = _Linenum();       // calculate number of observations (meta adds)
	_Lines = _Totallines - 1;       // calculate number of observations

	_Meta = _Initmeta();            // initialize the meta info
	_Info = _Initinfo();            // initialize the observations info
}
Frame::~Frame()
{
	delete[] _Info;
	_CSV.close();
}

// accessors
Observation  Frame::getMeta()          const
{
	return _Meta;
}
Observation* Frame::getInfo()          const
{
	return _Info;
}
unsigned     Frame::getTotalLinesNum() const
{
	return _Totallines;
}
unsigned     Frame::getLinesNum()      const
{
	return _Lines;
}
bool         Frame::fileGood()         const
{
	if (_CSV.good())
		return true;
	else
		return false;
}

// refresh and open files
void Frame::refresh()
{
	_CSV.close();
	_CSV.open(_Fname, std::ios::in | std::ios::out | std::ios::app);

	_Totallines = _Linenum();
	_Lines = _Totallines - 1;

	_Meta = _Initmeta();
	_Info = _Initinfo();
}
void Frame::refresh(const std::string& fname)
{
	_CSV.close();
	_CSV.open(fname, std::ios::in | std::ios::out | std::ios::app);
	_Fname = fname;

	_Totallines = _Linenum();
	_Lines = _Totallines - 1;

	_Meta = _Initmeta();
	_Info = _Initinfo();
}
void Frame::open(const std::string& fname)
{
	_CSV.open(fname, std::ios::in | std::ios::out | std::ios::app);
	_Fname = fname;

	_Totallines = _Linenum();
	_Lines = _Totallines - 1;

	_Meta = _Initmeta();
	_Info = _Initinfo();
}

// append functionality
void Frame::writeInline(const std::string& obs)
{
	//_CSV.close();
	//_CSV.open(_Fname, std::ios::in | std::ios::out | std::ios::app);
	_CSV << obs << '\n';
}

// delete functionality
void Frame::deleteLine()
{
	_CSV.close();
	_CSV.open(_Fname, std::ios::out);

	_CSV << _Meta.getRaw() << "\n";
	for (int i = 0; i < _Lines - 1; i++)
	{
		_CSV << _Info[i].getRaw() << "\n";
	}

	_CSV.close();
	_CSV.open(_Fname, std::ios::in | std::ios::out | std::ios::app);
}
void Frame::deleteLine(size_t line)
{
	_CSV.close();
	_CSV.open(_Fname, std::ios::out);

	_CSV << _Meta.getRaw() << "\n";
	for (int i = 0; i < _Lines; i++)
	{
		if (i == line)
			continue;

		_CSV << _Info[i].getRaw() << "\n";
	}

	_CSV.close();
	_CSV.open(_Fname, std::ios::in | std::ios::out | std::ios::app);
}

// edit functionality
void Frame::editLine(const Observation& edited)
{
	_CSV.close();
	_CSV.open(_Fname, std::ios::out);

	_CSV << _Meta.getRaw() << "\n";
	for (int i = 0; i < _Lines - 1; i++)
	{
		_CSV << _Info[i].getRaw() << "\n";
	}

	// write the edited line into the file
	_CSV << edited.getRaw() << "\n";

	_CSV.close();
	_CSV.open(_Fname, std::ios::in | std::ios::out | std::ios::app);
}
void Frame::editLine(size_t line, const Observation& edited)
{
	_CSV.close();
	_CSV.open(_Fname, std::ios::out);

	_CSV << _Meta.getRaw() << "\n";
	for (int i = 0; i < _Lines; i++)
	{
		if (i == line)
		{
			_CSV << edited.getRaw() << "\n";
			continue;
		}

		_CSV << _Info[i].getRaw() << "\n";
	}

	_CSV.close();
	_CSV.open(_Fname, std::ios::in | std::ios::out | std::ios::app);
}

// insert functionality
void Frame::insertLine(size_t line, const Observation& obs)
{
	_CSV.close();
	_CSV.open(_Fname, std::ios::out);

	_CSV << _Meta.getRaw() << "\n";
	for (int i = 0; i < _Lines; i++)
	{
		// insert the new line
		if (i == line)
			_CSV << obs.getRaw() << "\n";

		_CSV << _Info[i].getRaw() << "\n";
	}

	_CSV.close();
	_CSV.open(_Fname, std::ios::in | std::ios::out | std::ios::app);
}

/** private **/

// number of lines in the file
unsigned        Frame::_Linenum()
{
	unsigned    result = 0;
	std::string temp;

	std::streamoff pos = _CSV.tellg();

	while (getline(_CSV, temp))
	{
		result++;
	}

	// to reset the file cursor (see the note of seekp and seekg above)
	_CSV.close();
	_CSV.open(_Fname, std::ios::in | std::ios::out | std::ios::app);

	return result;
}

// setup the meta information (the first line)
Observation     Frame::_Initmeta()
{
	std::string temp;
	std::getline(_CSV, temp);

	return Observation(temp);
}

// setup the rest lines
Observation*    Frame::_Initinfo()
{
	Observation* result = new Observation[_Lines];
	int          iter = 0;
	std::string  temp;

	while (getline(_CSV, temp))
	{
		result[iter] = Observation(temp);
		iter++;
	}

	// to reset the file cursor (see the note of seekp and seekg above)
	_CSV.close();
	_CSV.open(_Fname, std::ios::in | std::ios::out | std::ios::app);

	return result;
}


// ============


// return new string with the removed rem from the source
std::string translate(std::string source, char rem)
{
	std::string result = "";

	for (char c : source)
	{
		if (c != rem)
		{
			result += c;
		}
	}

	return result;
}
