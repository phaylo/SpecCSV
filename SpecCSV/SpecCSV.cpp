/**
 * SpecCSV
 * SpecCSV.cpp
 *
 * Command-line CSV editor and inspector
 * @author Humam O.
 */

#include "stdafx.h"

using namespace boost::algorithm;

// global
Frame csvfile;      // CSV file
Observation meta;   // meta info
string_vec args;    // command args

int main(void)
{

	/* initialize the program */

	// 1. input file name
	std::string filename;
	std::cout << "Please input the appropriate filename\n";

	filename = getValidFileName();

	if (filename == "debug")
	{
		// for lol
		std::cout << "Delete System32 then try again...\n";
	}
	else
	{
		/* open the appropriate file */

		csvfile.open(filename);
		meta = csvfile.getMeta();

		/* program usage and actions */

		// 2. display usage
		displayUsage();

		// 3. handling differint commands
		std::string _cmd;   // for storing the string command
		int cmd;            // for hashing the command as integer
		
		do
		{
			std::cout << ">> ";
			std::getline(std::cin, _cmd);			

			args = parseCmd(_cmd);
			cmd = hashCmd(args[0]);

			try {
				switch (cmd)
				{

				case -1:  // quit
					if (args.size() != 1)
						throw ArgumentException("simply type 'quit'");

					goto quit;

					// -------------

				case 0:  // invalid
					std::cout << "ERROR: Invalid command, retry\n";
					continue;

					// -------------

				case 1:  // append
					append();
					break;

					// -------------

				case 2:  /* reserved */
					throw NotImplementedException();
					break;

					// -------------

				case 3:  // showmeta
					showMeta();
					break;

					// -------------

				case 4:  // showinfo
					showInfo();
					break;

					// -------------

				case 5:  // refresh
					if (args.size() != 1)
						throw ArgumentException("usage: refresh");

					csvfile.refresh();

					throw DeprecatedException();
					break;

					// -------------

				case 6:  // open
					openFile();
					break;

					// -------------

				case 7:  // showdetailed
					showDetailed();
					break;

					// -------------

				case 8:  // sum
					calcSum();
					break;

					// -------------

				case 9:  // delete
					deleteLine();
					break;

					// -------------

				case 10:  /* reserved */
					throw NotImplementedException();
					break;

					// -------------

				case 11:  // edit
					edit();
					break;

					// -------------

				case 12:  /* reserved */
					editField();
					break;

					// -------------

				case 13:  // insert
					insert();
					break;

					// -------------

				case 999:  // help
					if (args.size() != 1)
						throw ArgumentException("usage: help");

					displayUsage();
					break;
				}
			}

			catch (const ArgumentException& err)
			{
				std::cout << args[0] << ", " << err.what() << std::endl;
			}

			catch (const BehaviourException& err)
			{
				std::cout << args[0] <<  ", " << err.what() << std::endl;
			}

			catch (const DeprecatedException& err)
			{
				std::cout << err.what() << std::endl;
			}

			catch (const NotImplementedException& err)
			{
				std::cout << err.what() << std::endl;
			}

			catch (std::exception)
			{
				std::cout << "unhandled exception" << std::endl;
				throw;
			}

		} while (true);
	}

	quit:
		std::cout << "\nQuitting..";

	return 0;
}

// misc
int hashCmd(const std::string& command)
{
	if (command == "quit")
		return -1;

	if (command == "append")
	{
		return 1;
	}
	else if (false && command == "detailed")
	{
		return 2;
	}
	else if (command == "showmeta")
	{
		return 3;
	}
	else if (command == "showinfo")
	{
		return 4;
	}
	else if (command == "refresh")
	{
		return 5;
	}
	else if (command == "open")
	{
		return 6;
	}
	else if (command == "showdetailed")
	{
		return 7;
	}
	else if (command == "sum")
	{
		return 8;
	}
	else if (command == "delete")
	{
		return 9;
	}
	else if (false && command == "delete")
	{
		return 10;
	}
	else if (command == "edit")
	{
		return 11;
	}
	else if (command == "editfield")
	{
		return 12;
	}
	else if (command == "insert")
	{
		return 13;
	}
	else if (command == "help")
	{
		return 999;
	}
	else
	{
		return 0;
	}
}
string_vec parseCmd(std::string command)
{
	// trim all leading and trailing spaces
	trim(command);

	string_vec result;
	std::string::iterator it = command.begin();

	std::string temp = "";
	bool space_flag = false;

	// we need to split string with token ' ' (1 or more spaces)
	// if the parser detect \' char we are going to treat every
	// space inside as part of the arg, till the next \' (multiple words args)
	for (; it != command.end(); it++)
	{
		if (*it == '\'')
		{
			space_flag = !space_flag;
			continue;
		}

		if (*it != ' ' || space_flag)
		{
			temp += *it;
		}
		else if (*it == ' ' && temp != "" || it == command.end() - 1 && temp != "")
		{
			result.push_back(temp);
			temp = "";
		}
		else if (*it == ' ' && temp == "")
		{
			continue;
		}
	}

	// for the last arg
	result.push_back(temp);

	return result;
}

std::string getValidFileName()
{
	std::string result;
	do
	{
		std::cout << "\nFile name: ";
		std::getline(std::cin, result);
	} while (!std::tr2::sys::exists(result) && result != "debug");

	return result;
}

void displayUsage()
{
	std::cout << "\ncommands: \n";

	std::cout << "'quit' to quit anytime\n\n";

	std::cout << "'append mode' to append new observation.\n";
	std::cout << "'insert mode linenumber' to insert new observation\n\n";

	std::cout << "'edit mode linenumber' to edit an observation\n";
	std::cout << "'delete linenumber' to delete an observation\n\n";

	std::cout << "'showmeta' to display the meta info of the current file\n";
	std::cout << "'showinfo' to display the info of the current file\n";
	std::cout << "'showdetailed linenumber' to display detailed info for a specific line\n\n";

	std::cout << "'sum field' to display total of a value\n\n";

	std::cout << "'refresh' to refresh all the info about the current file (deprecated)\n";
	std::cout << "'open' to close this file and open a new one\n\n";

	std::cout << "'help' to display this help message\n\n";

	std::cout << "mode can be either \"inline\" or \"detailed\"\n";
	std::cout << "linenumber can be \"last\", an alias for the last observation available\n\n";
	std::cout << "if an argument is consist of more than one word, enclose it with single quote character\n";
	std::cout << "e.g: sum 'Days Spent'\n\n";
}
void openFile()
{
	if (args.size() != 2)
		throw ArgumentException("usage: open filename");

	std::string filename = args[1];

	if (!std::tr2::sys::exists(filename))
		throw BehaviourException("file not found");

	csvfile.refresh(filename);
	meta = csvfile.getMeta();
}


// display info
inline void showMeta()
{
	if (args.size() != 1)
		throw ArgumentException("usage: showmeta");

	for (int i = 0, len = meta.getNumValues(); i < len; i++)
	{
		std::cout << meta[i] << std::endl;
	}
}
inline void showInfo()
{
	if (args.size() != 1)
		throw ArgumentException("usage: showinfo");

	for (int i = 0, len = csvfile.getLinesNum(); i < len; i++)
		std::cout << "[" << i + 1 << "] " << csvfile.getInfo()[i].getRaw() << std::endl;
}
void showDetailed()
{
	if (args.size() != 2)
	{
		throw ArgumentException("usage: showdetailed linenumber");
	}

	size_t linenum;

	try
	{
		if (args[1] == "last")
			linenum = csvfile.getLinesNum();
		else
			linenum = std::stoi(args[1]);
	}
	catch (std::invalid_argument)
	{
		throw ArgumentException("linenumber must be an integer");
	}

	if (linenum > csvfile.getLinesNum() || linenum == 0)
		throw BehaviourException("line doesn't exist");

	for (int i = 0, len = meta.getNumValues(); i < len; i++)
	{
		std::cout.width(meta.getLongest().length() + PADDING);

		std::cout << std::left << meta[i] << std::left << ": ";
		std::cout << std::right << csvfile.getInfo()[linenum - 1][i] << std::endl;
	}
}

// edit file
void append()
{
	if (args.size() != 2)
	{
		throw ArgumentException("usage: append mode");
	}

	if (args[1] == "inline")
		appInline();

	else if (args[1] == "detailed")
		appDetailed();

	else
		throw ArgumentException("unknown mode. see help");
}
void appInline()
{
	std::string obs;

	std::cout << "Input your observation:\n";
	std::getline(std::cin, obs);

	if (Observation(obs).getNumValues() != meta.getNumValues())
		throw BehaviourException("number of fields doesn't match");

	csvfile.writeInline(obs);
	csvfile.refresh(); 
}
void appDetailed()
{
	std::string obs;
	std::string obs2;

	for (int i = 0, len = meta.getNumValues(); i < len; i++)
	{
		std::cout << meta[i] << ": ";
		std::getline(std::cin, obs);

		if (i != len - 1)
		{
			obs2 += obs + ",";
		}
		else
		{
			obs2 += obs;
		}
	}

	csvfile.writeInline(obs2);
	csvfile.refresh(); 
}

void deleteLine()
{
	if (args.size() != 2)
		throw ArgumentException("usage: delete linenumber");

	size_t linenum;

	try
	{
		if (args[1] == "last")
		{
			csvfile.deleteLine();
			csvfile.refresh();
			return;
		}
		else
			linenum = std::stoi(args[1]);
	}
	catch (std::invalid_argument)
	{
		throw ArgumentException("linenumber must be an integer");
	}

	if (linenum > csvfile.getLinesNum() || linenum == 0)
		throw BehaviourException("line doesn't exist");

	csvfile.deleteLine(linenum - 1);
	csvfile.refresh();
}

void edit()
{
	if (args.size() != 3)
	{
		throw ArgumentException("usage: edit mode linenumber");
	}

	if (args[1] == "inline")
		editInline();

	else if (args[1] == "detailed")
		editDetailed();

	else
		throw ArgumentException("unknown mode. see help");
}
void editInline()
{
	std::string obs;
	size_t linenum;

	try
	{
		if (args[2] == "last")
			linenum = csvfile.getLinesNum();
		else
			linenum = std::stoi(args[2]);
	}
	catch (std::invalid_argument)
	{
		throw ArgumentException("linenumber must be an integer");
	}

	if (linenum > csvfile.getLinesNum() || linenum == 0)
		throw BehaviourException("line doesn't exist");

	// get edit info
	std::cout << "Input your observation:\n";
	std::getline(std::cin, obs);

	if (Observation(obs).getNumValues() != meta.getNumValues())
		throw BehaviourException("number of fields doesn't match");

	Observation edited(obs);

	if (args[2] == "last")
		csvfile.editLine(edited);
	else
		csvfile.editLine(linenum - 1, edited);

	csvfile.refresh();
}
void editDetailed()
{
	std::string obs;
	std::string obs2;

	size_t linenum;
	Observation target;

	try
	{
		if (args[2] == "last")
			linenum = csvfile.getLinesNum();
		else
			linenum = std::stoi(args[2]);
	}
	catch (std::invalid_argument)
	{
		throw ArgumentException("linenumber must be an integer");
	}

	if (linenum > csvfile.getLinesNum() || linenum == 0)
		throw BehaviourException("line doesn't exist");

	target = csvfile.getInfo()[linenum - 1];

	// get the edited info
	for (int i = 0, len = meta.getNumValues(); i < len; i++)
	{
		std::cout << meta[i] << ": (" << target[i] << ") ";
		std::getline(std::cin, obs);

		if (i != len - 1)
		{
			obs2 += obs + ",";
		}
		else
		{
			obs2 += obs;
		}
	}

	Observation edited(obs2);

	if (args[2] == "last")
		csvfile.editLine(edited);
	else
		csvfile.editLine(linenum - 1, edited);

	csvfile.refresh();
}

void editField()
{
	// check for valid number of arguments
	if (args.size() != 3)
		throw ArgumentException("usage: editfield linenumber field");

	size_t linenum;
	int field = meta.find(args[2]);

	std::string editField;
	Observation edited;

	// check for a valid linenumber argument
	try
	{
		if (args[1] == "last")
			linenum = csvfile.getLinesNum();
		else
			linenum = std::stoi(args[1]);
	}
	catch (std::invalid_argument)
	{
		throw ArgumentException("linenumber must be an integer");
	}

	if (linenum > csvfile.getLinesNum() || linenum == 0)
		throw BehaviourException("line doesn't exist");

	// check for a valid field argument
	if (field == NEG)
		throw BehaviourException("field doesn't exist");

	// get edit info
	edited = csvfile.getInfo()[linenum - 1];
	std::cout << meta[field] << ": (" << edited[field] << ") ";

	std::getline(std::cin, editField);
	edited.alter(field, editField);

	// edit the file
	csvfile.editLine(linenum - 1, edited);
	csvfile.refresh();
}

void insert()
{
	if (args.size() != 3)
	{
		throw ArgumentException("usage: insert mode linenumber");
	}

	if (args[1] == "inline")
		insertInline();

	else if (args[1] == "detailed")
		insertDetailed();

	else
		throw ArgumentException("unknown mode. see help");
}
void insertInline()
{
	std::string obs;
	size_t linenum;

	try
	{
		if (args[2] == "last")
			linenum = csvfile.getLinesNum();
		else
			linenum = std::stoi(args[2]);
	}
	catch (std::invalid_argument)
	{
		throw ArgumentException("linenumber must be an integer");
	}

	if (linenum > csvfile.getLinesNum() || linenum == 0)
		throw BehaviourException("line doesn't exist");

	std::cout << "Input your observation:\n";
	std::getline(std::cin, obs);

	if (Observation(obs).getNumValues() != meta.getNumValues())
		throw BehaviourException("number of fields doesn't match");

	Observation edited(obs);

	csvfile.insertLine(linenum - 1, edited);
	csvfile.refresh();
}
void insertDetailed()
{
	std::string obs;
	std::string obs2;
	size_t linenum;

	try
	{
		if (args[2] == "last")
			linenum = csvfile.getLinesNum();
		else
			linenum = std::stoi(args[2]);
	}
	catch (std::invalid_argument)
	{
		throw ArgumentException("linenumber must be an integer");
	}

	if (linenum > csvfile.getLinesNum() || linenum == 0)
		throw BehaviourException("line doesn't exist");

	for (int i = 0, len = meta.getNumValues(); i < len; i++)
	{
		std::cout << meta[i] << ": ";
		std::getline(std::cin, obs);

		if (i != len - 1)
		{
			obs2 += obs + ",";
		}
		else
		{
			obs2 += obs;
		}
	}

	Observation edited(obs2);

	csvfile.insertLine(linenum - 1, edited);
	csvfile.refresh();
}

// data analysis
void calcSum()
{
	if (args.size() != 2)
		throw ArgumentException("usage: sum fieldname");

	int sum = 0;
	int val = meta.find(args[1]);
		
	if (val == NEG)
		throw BehaviourException("field doesn't exist");

	for (int i = 0, len = csvfile.getLinesNum(); i < len; i++)
	{
		try
		{
			sum += std::stoi(csvfile.getInfo()[i][val]);
		}
		catch (std::invalid_argument)
		{
			throw BehaviourException("one or more field is not a number");
		}
	}

	std::cout << args[1] << " total is: ";
	std::cout << sum << std::endl;
}