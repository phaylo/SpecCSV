// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <filesystem>

#include <exception>

#include <string>
#include <vector>

// for the find method in Frame class
#define NEG -1

// for use in SpecCSV.cpp
#define PADDING 2
#define SHOWLIMIT 50
#define SHOWLASTNUM 10

// for use in private members of Frame class
#define LF '\n'
#define CR '\r'

typedef std::vector<std::string> string_vec;

// TODO: reference additional headers your program requires here

#include "usrexp.h"
#include "base.h"
#include "func.h"
#include <boost/algorithm/string/trim_all.hpp>
