#include "Debugger.h"

#pragma warning(disable : 4996)

#include "leaker.h"

CDebugger* CDebugger::instance = NULL;
int CDebugger::logType = -1;

/**
 * Constructor
 */
CDebugger::CDebugger()
{
}

/** 
 * Destructor
 */
CDebugger::~CDebugger()
{
}

/**
 * Frees the instance
 */
void CDebugger::deleteInstance()
{
}

/**
 * Returns the instance
 */
CDebugger* CDebugger::getInstance()
{
    return NULL;
}

/**
 * Logs the string a to the file f
 */
bool CDebugger::log(string f, string a)
{
	return true;
}

/**
 * Starts the logging
 */
void CDebugger::startLogging()
{
}

/**
 * Stops the logging
 */
void CDebugger::stopLogging()
{
}

void CDebugger::setLogType(int lt)
{
}