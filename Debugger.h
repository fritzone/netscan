#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

#include <string>


using namespace std;

/**
 * This singleton class represents a (quite primitive) way to log messages to a file.
 */
class CDebugger
{

public:

	/**
	 * Destructor.
	 */
	virtual ~CDebugger();

	/**
	 * Returns the instance
	 */
	static CDebugger* getInstance();

	/**
	 * Frees the instance
	 */
	static void deleteInstance();

	/**
	 * Logs the given message
	 */
	bool log(string, string);

	/**
	 * Stops the logging
	 */
	void stopLogging();

	/**
	 * Starts the logging
	 */
	void startLogging();

	/**
	 * Sets the type of the log generation ...
	 * 1: after each write close  the file
	 * 2: keep the file open
	 */
	static void setLogType(int lt);

private:
	/**
	 * Creates a new object
	 */
	CDebugger();

	// the instance
	static CDebugger* instance;

	// tells us if there is a need to log
	bool needsLog;

	string fileName;

	FILE* fp;

	static int logType;

};

// used when logging a number
static char nr[256];
static CDebugger* globalLogger = NULL;
static CDebugger* killedLogger = (CDebugger*)(-1);

/***************************************************************************************************
 *                                   Macros used for logging                                       *
 ***************************************************************************************************/

// simple logger
#define LOG(S)		

//globalLogger = CDebugger::getInstance(); 
//	if(globalLogger !=NULL && globalLogger != killedLogger) globalLogger->log("editor.log", string("[")+__FUNCTION__+"] "+S);

// error logger
#define LOG_E(S)	

//LOG(string("[ERROR]") + S)

// log the given number as long double
#define LOG_F(S,n)	

//sprintf(nr," %.4f",n); LOG(string(S) + nr);

// log the given number as int
#define LOG_N(S,n)	

//sprintf(nr," %d",n); LOG(string(S) + nr);

// log the given number as hex
#define LOG_X(S,n)	

//sprintf(nr," %x",(DWORD)n); LOG(string(S) + nr);

// logs the given windows messages as a string
#define LOG_MSG(S,n) 

//LOG(string(S) + getMessageDescription(n));
				
// logs the given string...
#define LOG_S(S,s) 

//LOG(string(S) + string(s));

// just assert and log if null found
// #define ASSERT(p)	if(! (p) ) {										\
// 						LOG(string("ASSERT FAILURE:")+string(" "#p));	\
// 					}
#endif 
