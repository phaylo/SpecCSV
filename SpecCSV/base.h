/**
 * SpecCSV
 * base.h
 *
 * Observation and Frame class, and the helper function translate 
 * @author Humam O.
 */

#pragma once

#include "stdafx.h"

/* prototypes */

/**
 * Return a string with a removed chars from it
 *
 * @param source the original string
 * @param rem the char to be removed
 * @return string with each occurrence of rem removed
 */
std::string translate(std::string source, char rem);


// ============


/* Observation class */
class Observation
{
public:

	// constructors

	/**
	 * Construct an Observation object
	 *
	 * @param line the line of string to be processed
	 */
	Observation(std::string line);
	Observation() = default;

	// accessors

	/**
	 * Get the number of fields in an observation
	 *
	 * @return number of fields
	 */
	unsigned    getNumValues()                 const;

	/**
	* Get the raw string line
	*
	* @return original processed string
	*/
	std::string getRaw()                       const;

	// find functionality

	/**
	* Return the index of a filed given its string
	*
	* @param value field to look for
	* @return the index of the field or NEG if not found
	*/
	int         find(const std::string& value) const;

	/**
	* Return the index of a field with the longest string
	*
	* @return the index of the field with longest string
	*/
	size_t      findLongest()                  const;

	/**
	* Return the string of a field with the longest string
	*
	* @return the string of the field
	*/
	std::string getLongest()                   const;

	// observation edit functionality

	/**
	* Edit the observation given a specific index and a new string
	*
	* @param index the index of the field which will be edited
	* @param edit the new string
	*/
	void        alter(size_t index, std::string edit);

	// overloaded [] operator (indexer)
	/**
	* Get a field string given its index
	*
	* @param index the index of the filed
	* @return the field at the specific index
	*/
	std::string operator[](size_t index)  const;

private:

	// calculate number of fields
	unsigned    _Nums();

	// vector of values
	string_vec _Split();

	unsigned    _Numvals;  // number of fields
	std::string _Init;     // raw line
	string_vec  _Vals;     // string vector of fields
};


// ============


/* Frame class */
class Frame
{
public:
	// constructors and destructor

	/**
	* Construct a Frame object
	*
	* @param filename the filename of the CSV file to be processed
	*/
	Frame(std::string filename);
	Frame() = default;
	~Frame();

	// accessors
	/**
	* Get the meta info of the Frame (first line)
	*
	* @return Observation object contains the data of the first line
	*/
	Observation     getMeta()          const;

	/**
	* Get the observations of the frame
	*
	* @return a pointer to Observation contains all of the frame observations
	*/
	Observation*    getInfo()          const;

	/**
	* Get the number of observations in the frame with the meta line as well
	*
	* @return total number of observations
	*/
	unsigned        getTotalLinesNum() const;

	/**
	* Get the number of observations in the frame
	*
	* @return total number of observations
	*/
	unsigned        getLinesNum()      const;

	/**
	* Get the line break character in the file
	*
	* @return total number of observations
	*/
	char           getLineBreak()      const;

	/**
	* Get the line break type
	*
	* @return a string stating what is the type of line break
	*/
	std::string    getLineBreakDetailed()      const;

	/**
	* Check if the file is in good state or not
	*
	* @return true if it's good, false otherwise
	*/
	bool            fileGood()         const;

	// refresh and open files

	/**
	* Reopen the current file and refresh all of the Frame info
	*/
	void refresh();

	/**
	* Close the current file and open another file and refresh all of the Frame info
	*
	* @param fname name of the file to be processed
	*/
	void refresh(const std::string& fname);

	/**
	* Open a file and refresh all of the Frame info
	*
	* @param fname name of the file to be processed
	*/
	void open(const std::string& fname);

	// append functionality

	/**
	* Write a raw string to the file
	*
	* @param obs the string to be written
	*/
	void writeInline(const std::string& obs);

	// delete functionality

	/**
	* Delete a line (observation) from the file (doesn't refresh Frame info)
	*
	* @param line the line number to be deleted. if not specified, the last line will be deleted
	*/
	void deleteLine();
	void deleteLine(size_t line);

	// edit functionality

	/**
	* Edit a line (observation) from the file (doesn't refresh Frame info)
	*
	* @param line the line number to be edited. if not specified, the last line will be edited
	* @param edited the new observation to be written
	*/
	void editLine(const Observation& edited);
	void editLine(size_t line, const Observation& edited);

	// insert functionality

	/**
	* insert a line (observation) from the file (doesn't refresh Frame info)
	*
	* @param line the line number where the new line will be inserted. if not specified, it's the last line
	* @param obs the new observation to be written
	*/
	void insertLine(size_t line, const Observation& obs);

private:

	// number of lines in the file
	unsigned        _Linenum();

	// setup the meta information (the first line)
	Observation     _Initmeta();

	// setup the rest lines
	Observation*    _Initinfo();

	// the appropriate line break
	char        _Linebreak();

	std::fstream        _CSV;         // fstream of the file
	char                _Break;       // line break
	std::string         _Breakstring; // line break detailed
	int                 _Totallines;  // total lines in the file (including meta)
	int                 _Lines;       // total lines in the file (excluding meta)
	std::string         _Fname;       // file name
	Observation         _Meta;        // meta info (first line)
	Observation*        _Info;        // observations info (rest of the lines)

	std::streamoff*     _Lineoff;     // store the offsets of each line at the beginning (to be used)
};