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


#define NEG -1
#define PADDING 2

typedef std::vector<std::string> string_vec;

// TODO: reference additional headers your program requires here

#include "usrexp.h"
#include "base.h"
#include "func.h"
#include <boost/algorithm/string/trim_all.hpp>
