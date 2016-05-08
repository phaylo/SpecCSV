/**
* SpecCSV
* usrexp.h
*
* Some user-defined exceptions to be used with other src files
* @author Humam O.
*/

#pragma once

#include "stdafx.h"

// thrown if a feature is yet to be implemented
class NotImplementedException : public std::logic_error
{
public:
	NotImplementedException(std::string msg = "not implemented") : logic_error(msg) {}
};

// thrown if a feature is deprecated and might be removed (or replaced) in the future
class DeprecatedException : public std::logic_error
{
public:
	DeprecatedException(std::string msg = "deprecated, please avoid using") : logic_error(msg) {}
};

// thrown when a function face something unexpeceted that might result in an wanted result
class BehaviourException : public std::runtime_error
{
public:
	BehaviourException(std::string msg) : runtime_error(msg) {}
};

// thrown if a command given invalid argumnts (the in-app command)
class ArgumentException : public std::runtime_error
{
public:
	ArgumentException(std::string msg) : runtime_error(msg) {}
};

// thrown when a file (in text mode) or string can't be parsed for some reason
class ParseException : public std::runtime_error
{
public:
	ParseException(std::string msg) : runtime_error(msg) {}
};