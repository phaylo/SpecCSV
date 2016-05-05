// stdafx.cpp : source file that includes just the standard includes
// SpecCSV.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

typedef std::vector<std::string> string_vec;

// prototype
std::string translate(std::string source, char rem);

class NotImplementedException : public std::exception
{
public:
	virtual const char* what() const
	{
		return "not implemented";
	}
};
class DeprecatedException : public std::exception
{
public:
	virtual const char* what() const
	{
		return "deprecated, please avoid using it";
	}
};
class BehaviourException : public std::runtime_error
{
public:
	BehaviourException(std::string msg) : runtime_error(msg) {}
};
class ArgumentException : public std::invalid_argument
{
public:
	ArgumentException(std::string msg) : invalid_argument(msg) {}
};

class Observation
{
public:
	Observation(std::string line)
	{
		_Init    = translate(line, '"');  // the raw line in the file
		_Numvals = _Nums();               // number of values
		_Vals    = _Split();              // vector of values (as std::string)
	}
	Observation() {}

	unsigned    getNumValues()  const
	{
		return _Numvals;
	}
	std::string getRaw()        const
	{
		return _Init;
	}

	int         find(const std::string& value) const
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
	size_t      findLongest()                  const
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
	std::string getLongest()                   const
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
	void        alter(size_t index, std::string edit)
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

	std::string operator[](size_t index)  const
	{
		// don't touch this for now
		return _Vals.at(index);
	}

private:

	// calculate number of values
	unsigned    _Nums()
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
	string_vec _Split()
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

	unsigned    _Numvals;
	std::string _Init;
	string_vec  _Vals;
};

class Frame
{
public:
	Frame(std::string filename)
	{
		// open the file for reading and writing, and set the out pointer at the end of file 
		_CSV.open(filename, std::ios::in | std::ios::out | std::ios::app);
		_Fname = filename;              // save the filename for using with other members

		_Totallines = _Linenum();       // calculate number of observations (meta adds)
		_Lines      = _Totallines - 1;  // calculate number of observations

		_Meta       = _Initmeta();      // initialize the meta info
		_Info       = _Initinfo();      // initialize the observations info
	}
	Frame() {}
	~Frame()
	{
		delete[] _Info;
		_CSV.close();
	}

	Observation     getMeta()          const
	{
		return _Meta;
	}
	Observation*    getInfo()          const
	{
		return _Info;
	}
	unsigned        getTotalLinesNum() const
	{
		return _Totallines;
	}
	unsigned        getLinesNum()      const
	{
		return _Lines;
	}
	bool            fileGood()         const
	{
		if (_CSV.good())
			return true;
		else
			return false;
	}

	// different file modifiers
	// in general, try to solve the openning and closing of the file with seekp and seekg

	void writeInline(const std::string& obs)
	{
		//_CSV.close();
		//_CSV.open(_Fname, std::ios::in | std::ios::out | std::ios::app);
		_CSV << obs << '\n';
	}
	void refresh()
	{
		_CSV.close();
		_CSV.open(_Fname, std::ios::in | std::ios::out | std::ios::app);

		_Totallines = _Linenum();
		_Lines = _Totallines - 1;

		_Meta = _Initmeta();
		_Info = _Initinfo();
	}
	void refresh(const std::string& fname)
	{
		_CSV.close();
		_CSV.open(fname, std::ios::in | std::ios::out | std::ios::app);
		_Fname = fname;

		_Totallines = _Linenum();
		_Lines = _Totallines - 1;

		_Meta = _Initmeta();
		_Info = _Initinfo();
	}
	void open(const std::string& fname)
	{
		_CSV.open(fname, std::ios::in | std::ios::out | std::ios::app);
		_Fname = fname;

		_Totallines = _Linenum();
		_Lines = _Totallines - 1;

		_Meta = _Initmeta();
		_Info = _Initinfo();
	}

	void deleteLine()
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
	void deleteLine(size_t line)
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

	void editLine(const Observation& edited)
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
	void editLine(size_t line, const Observation& edited)
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

	void insertLine(size_t line, const Observation& obs)
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

private:

	// number of lines in the file
	unsigned        _Linenum()
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
	Observation     _Initmeta()
	{
		std::string temp;
		std::getline(_CSV, temp);

		return Observation(temp);
	}

	// setup the rest lines
	Observation*    _Initinfo()
	{
		Observation* result = new Observation[_Lines];
		int          iter   = 0;
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

	std::fstream        _CSV;
	int                 _Totallines;
	int                 _Lines;
	std::string         _Fname;
	Observation         _Meta;
	Observation*        _Info;

	std::streamoff*     _Lineoff;
};

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
