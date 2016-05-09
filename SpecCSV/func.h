/**
 * SpecCSV
 * func.h
 *
 * Define some functions to be used in SpecCSV.cpp
 * @author Humam O.
 */

#pragma once

#include "stdafx.h"

#define PADDING 2       // used to align text gently (for showdetailed command)
#define SHOWLIMIT 50    // the upper limit of max observations to show freely (for showinfo command)
#define SHOWLASTNUM 10  // number of observations to show if a Frame has many (for showinfo command)

typedef std::vector<std::string> string_vec;


//////////////////////////////////////////////////
// Prototypes
//////////////////////////////////////////////////


 // misc

/**
 * Trim leading and trailing spaces from string
 *
 * @param the string to be trimmed
 */
void trim(std::string& str);

/**
 * Return an integer to check which command is being used
 *
 * @param command the command to be checked for (the first arg)
 * @return integer
 */
int hashCmd(const std::string& command);

/**
 * Parse a string and return a vectore of the command arguments
 * any words enclosed with ' ' (single quote character) is considered one argument
 *
 * @param command the command to be parsed
 * @return a vector of string, contains the arguments. (" " is the token)
 */
string_vec parseCmd(std::string command);

/**
 * Ask the user for filename till they provide a valid one
 *
 * @return the filename string
 */
std::string getValidFileName();

/**
 * Display the help text
 */
void displayUsage();

/**
 * Open a file
 * (processed info are from csvfile, meta and args global variables
 */
void openFile();

/**
 * Display the line break type used in the file
 * (processed info are from csvfile, meta and args global variables
 */
void displayLineEnd();

// display info

/**
 * Display the meta info of the file (the first line)
 * (processed info are from csvfile, meta and args global variables
 */

inline void showMeta();

/**
 * Display all lines of the file (excluding meta)
 * (processed info are from csvfile, meta and args global variables
 */
inline void showInfo();

/**
 * Disaply a detailed info about  a specific observation
 * (processed info are from csvfile, meta and args global variables
 */
void showDetailed();

// edit file

/**
 * Detect appending method, inline or detailed,
 * and call the appropriate function
 * (processed info are from csvfile, meta and args global variables
 */
void append();

/**
 * Append new observation
 * the user must provide the raw info by theirselves
 * (processed info are from csvfile, meta and args global variables
 */
void appInline();

/**
 * Append new observation
 * the application will help the user what value is being entered
 * (processed info are from csvfile, meta and args global variables
 */
void appDetailed();

/**
 * Delete an observation
 * (processed info are from csvfile, meta and args global variables
 */
void deleteLine();

/**
 * Detect editing method, inline or detailed,
 * and call the appropriate function
 * (processed info are from csvfile, meta and args global variables
 */
void edit();

/**
 * Edit an observation
 * the user must provide the raw info by theirselves
 * (processed info are from csvfile, meta and args global variables
 */
void editInline();

/**
 * Edit an observation
 * the application will help the user what value is being entered
 * (processed info are from csvfile, meta and args global variables
 */
void editDetailed();

/**
 * edit a field in an observation
 * given the name of the value
 * (processed info are from csvfile, meta and args global variables
 */
void editField();

/**
 * Detect inserting method, inline or detailed,
 * and call the appropriate function
 * (processed info are from csvfile, meta and args global variables
 */
void insert();

/**
 * Insert new observation
 * the user must provide the raw info by theirselves
 * (processed info are from csvfile, meta and args global variables
 */
void insertInline();

/**
 * Insert new observation
 * the application will help the user what value is being entered
 * (processed info are from csvfile, meta and args global variables
 */
void insertDetailed();

// data analysis

/**
 * Display the sum of a specific field from all observations
 * which field is known via the given value name
 * (processed info are from csvfile, meta and args global variables
 */
void calcSum();

/**
 * Display the mean of a specific field from all observations
 * which field is known via the given value name
 * (processed info are from csvfile, meta and args global variables
 */
 void calcMean();
 
/**
 * Display the min of a specific field from all observations
 * which field is known via the given value name
 * (processed info are from csvfile, meta and args global variables
 */
 void calcMin();
 
/**
 * Display the max of a specific field from all observations
 * which field is known via the given value name
 * (processed info are from csvfile, meta and args global variables
 */
 void calcMax();
