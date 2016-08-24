#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>

using namespace std;

/// returns whether a character can be used as a character that can build a name
bool isNameChar(char);

/// returns true if string a begins with string b
bool beginsWith(string, string);

/// return the trimmed string a
string trim(string a, char c); 

/// returns the string without leading/tailing operators 
string pureString(string);

/// returns the index of b in a, or -1 if not found
size_t indexOf(string, string);

/// returns the install directory of the product (ie. where to look for rpc and rpg files)
string getInstallDirectory();

/// skips the white space in the given string.
void skipWhitespace(size_t&, string);	

/// returns the substring from the parameter, starting from, length. length == -1, everything
string substring(string,size_t,size_t);

/// tells us whether a string passed in is a number or not
bool isNumber(string);

/// returns whether a string is a string (ie. a set of characters between " ").
bool isString(string);

/// returns the specified (int) string from the string (string) separator is char
wstring getStringSep(wstring, wchar_t, int);

/// This method returns the first occurrence (index) of operator op in s.
size_t findFirstOperator(wstring s, wstring op);

/// searches for the operator op in s to be at level level
size_t findLeveledOperator(wstring s, wstring op, int level);

/// Finds the first keyword in a string (usually keyword means: first word)
char findKeyword(wstring s, wstring& keyw);

/// empties the created garbage bins... 
void emptyGarbageBins();

/// returns the value of _start from s
string getValueFrom(const wchar_t* s, const wchar_t* _start);

// checks if v is between v1 and v2
template <class T>
bool between(T v, T v1, T v2)
{
	return (v >= v1 && v <= v2) || (v >= v2 && v <= v1);
}

// returns the string representation of the long double with the given precision
string doubleToString(long double f, int precision);

// returns the given integer as a hexadecimal number
string intAsHex(unsigned long v);

// returns the int as a string
string intToString(int theNumber);

// return the hex number in the string as an integer
long hexToInt(string _s);

// delivers the concatenation of the name with a number
string deliverName(string base, int ctr);

#endif
