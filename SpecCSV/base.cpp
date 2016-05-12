/**
 * SpecCSV
 * base.cpp
 *
 * Declaration of Observation and Frame class members. also for the helper function translate
 * @author Humam O.
 */

#include "stdafx.h"


 //////////////////////////////////////////////////
 // Observation class members
 //////////////////////////////////////////////////


/* Public */


//////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////

Observation::Observation(std::string line)
{
	_Init = translate(line, '"');  // the raw line in the file
	_Numvals = _Nums();            // number of values
	_Vals = _Split();              // vector of values (as std::string)
}

//////////////////////////////////////////////////
// Accessors
//////////////////////////////////////////////////

unsigned    Observation::getNumValues()  const
{
	return _Numvals;
}
std::string Observation::getRaw()        const
{
	return _Init;
}

//////////////////////////////////////////////////
// Find functionality
//////////////////////////////////////////////////

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

//////////////////////////////////////////////////
// Observation edit functionality
//////////////////////////////////////////////////

void        Observation::alter(size_t index, std::string edit)
{
	if (index >= _Vals.size())
		throw std::out_of_range("nof field exist with such index");

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

//////////////////////////////////////////////////
// Indexer
//////////////////////////////////////////////////

std::string Observation::at(size_t index)  const
{
	// don't touch this for now
	return _Vals.at(index);
}


/* Private */


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


//////////////////////////////////////////////////
// Frame class members
//////////////////////////////////////////////////


/* Public */


//////////////////////////////////////////////////
// Constructor and destructor
//////////////////////////////////////////////////

Frame::Frame(std::string filename)
{
	// open the file for reading and writing, and set the out pointer at the end of file 
	_CSV.open(filename, std::ios::in | std::ios::out | std::ios::app);
	_Fname = filename;              // save the filename for using with other members
	_Break = _Linebreak();          // the line break

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

//////////////////////////////////////////////////
// Accessors
//////////////////////////////////////////////////

Observation  Frame::getMeta()              const
{
	return _Meta;
}
Observation* Frame::getInfo()              const
{
	return _Info;
}
unsigned     Frame::getTotalLinesNum()     const
{
	return _Totallines;
}
unsigned     Frame::getLinesNum()          const
{
	return _Lines;
}
char         Frame::getLineBreak()         const
{
	return _Break;
}
std::string  Frame::getLineBreakDetailed() const
{
	return _Breakstring;
}
bool         Frame::fileGood()             const
{
	if (_CSV.good())
		return true;
	else
		return false;
}

//////////////////////////////////////////////////
// Refresh and open files
//////////////////////////////////////////////////

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
	_Break = _Linebreak();

	_Totallines = _Linenum();
	_Lines = _Totallines - 1;

	_Meta = _Initmeta();
	_Info = _Initinfo();
}
void Frame::open(const std::string& fname)
{
	_CSV.open(fname, std::ios::in | std::ios::out | std::ios::app);
	_Fname = fname;
	_Break = _Linebreak();

	_Totallines = _Linenum();
	_Lines = _Totallines - 1;

	_Meta = _Initmeta();
	_Info = _Initinfo();
}

//////////////////////////////////////////////////
// Append functionality
//////////////////////////////////////////////////

void Frame::writeInline(const std::string& obs)
{
	//_CSV.close();
	//_CSV.open(_Fname, std::ios::in | std::ios::out | std::ios::app);
	_CSV << obs << _Break;
}

//////////////////////////////////////////////////
// Delete functionality
//////////////////////////////////////////////////

void Frame::deleteLine()
{
	_CSV.close();
	_CSV.open(_Fname, std::ios::out);

	_CSV << _Meta.getRaw() << _Break;
	for (int i = 0; i < _Lines - 1; i++)
	{
		_CSV << _Info[i].getRaw() << _Break;
	}

	_CSV.close();
	_CSV.open(_Fname, std::ios::in | std::ios::out | std::ios::app);
}
void Frame::deleteLine(size_t line)
{
	if (line >= _Lines)
		throw std::out_of_range("line doesn't exist");

	_CSV.close();
	_CSV.open(_Fname, std::ios::out);

	_CSV << _Meta.getRaw() << _Break;
	for (int i = 0; i < _Lines; i++)
	{
		if (i == line)
			continue;

		_CSV << _Info[i].getRaw() << _Break;
	}

	_CSV.close();
	_CSV.open(_Fname, std::ios::in | std::ios::out | std::ios::app);
}

//////////////////////////////////////////////////
// Edit functionality
//////////////////////////////////////////////////

void Frame::editLine(const Observation& edited)
{
	_CSV.close();
	_CSV.open(_Fname, std::ios::out);

	_CSV << _Meta.getRaw() << _Break;
	for (int i = 0; i < _Lines - 1; i++)
	{
		_CSV << _Info[i].getRaw() << _Break;
	}

	// write the edited line into the file
	_CSV << edited.getRaw() << _Break;

	_CSV.close();
	_CSV.open(_Fname, std::ios::in | std::ios::out | std::ios::app);
}
void Frame::editLine(size_t line, const Observation& edited)
{
	if (line >= _Lines)
		throw std::out_of_range("line doesn't exist");

	_CSV.close();
	_CSV.open(_Fname, std::ios::out);

	_CSV << _Meta.getRaw() << _Break;
	for (int i = 0; i < _Lines; i++)
	{
		if (i == line)
		{
			_CSV << edited.getRaw() << _Break;
			continue;
		}

		_CSV << _Info[i].getRaw() << _Break;
	}

	_CSV.close();
	_CSV.open(_Fname, std::ios::in | std::ios::out | std::ios::app);
}

//////////////////////////////////////////////////
// Insert functionality
//////////////////////////////////////////////////

void Frame::insertLine(size_t line, const Observation& obs)
{
	if (line >= _Lines)
		throw std::out_of_range("line doesn't exist");

	_CSV.close();
	_CSV.open(_Fname, std::ios::out);

	_CSV << _Meta.getRaw() << _Break;
	for (int i = 0; i < _Lines; i++)
	{
		// insert the new line
		if (i == line)
			_CSV << obs.getRaw() << _Break;

		_CSV << _Info[i].getRaw() << _Break;
	}

	_CSV.close();
	_CSV.open(_Fname, std::ios::in | std::ios::out | std::ios::app);
}


/* Private */


// number of lines in the file
unsigned        Frame::_Linenum()
{
	unsigned    result = 0;
	std::string temp;

	while (getline(_CSV, temp, _Break))
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

	std::getline(_CSV, temp, _Break);

	return Observation(temp);
}

// setup the rest lines
Observation*    Frame::_Initinfo()
{
	Observation* result = new Observation[_Lines];
	int          iter = 0;
	std::string  temp;

	while (getline(_CSV, temp, _Break))
	{
		result[iter] = Observation(temp);
		iter++;
	}

	// to reset the file cursor (see the note of seekp and seekg above)
	_CSV.close();
	_CSV.open(_Fname, std::ios::in | std::ios::out | std::ios::app);

	return result;
}

// the appropriate line break
char            Frame::_Linebreak()
{
	int countCR = 0;
	int countLF = 0;
	char buff;

	_CSV.close();
	_CSV.open(_Fname, std::ios::in | std::ios::binary);

	while (_CSV.read(&buff, 1))
	{
		if (buff == '\r')
			countCR++;
		else if (buff == '\n')
			countLF++;
	}

	_CSV.close();
	_CSV.open(_Fname, std::ios::in | std::ios::out | std::ios::app);

	// all line breaks are CRLF (windows)
	if (countCR == countLF)
	{
		_Breakstring = "CRLF, Windows";
		return LINE_FEED;
	}

	// or all line breaks are LF (unix)
	else if (countCR == 0 && countLF != 0)
	{
		_Breakstring = "LF, Unix";
		return LINE_FEED;
	}

	// all line breaks ar CR (Mac old)
	else if (countLF == 0 && countCR != 0)
	{
		_Breakstring = "CR, Mac (old)";
		return CARRIAGE_RETURN;
	}

	// mixed
	else
	{
		throw ParseException("mixed line breaks. can't parse file");
	}
}


//////////////////////////////////////////////////
// Helper function
//////////////////////////////////////////////////


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
