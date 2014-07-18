#include "utils.h"
#include "Debugger.h"
#include "GarbageBin.h"

#include "leaker.h"

/**
 * Returns true if string a begins with string b
 */
bool beginsWith(string a, string b)
{
unsigned int i = 0;
int inside = false;
	while( i < b.length() && i < a.length() )
	{
		inside = true;
		if(a[i] != b[i])
		{
			return false;
		}
		i ++;
	}
	if(inside)
	{
		return true;
	}
	return false;
}

/**
 * Returns the trimmed version of the string
 */
string trim(string a, char c='\0')
{
unsigned int i = 0;
string result = "";
	while( i<a.length() && (a[i] == ' ' || a[i] == '\t' || a[i] == '\n' || a[i] == c))
	{
		i ++;
	}
	if( i == a.length() )
	{
		return result;
	}

size_t j = a.length()>0?a.length() - 1:0;
	while(j>0 && (a[j] == ' ' || a[j] == '\t' || a[j] == '\n' || a[j] == c) )
	{
		j --;
	}
	j ++;
	while(i != j)
	{
		result += a[i];
		i ++;
	}
	return result;
}

/**
 * Returns the index of string from string a
 */
size_t indexOf(string a, string b)
{
const char *pa = a.c_str();
const char *pb = b.c_str();
	if(strstr(pa,pb) == NULL)
	{
		return -1;
	}
	return strstr(pa,pb) - pa;
}

/**
 * Returns from where to load the rpg files
 */
string getInstallDirectory()
{
	return ".\\Objects\\";
}

/**
 * Skips the whitespaces at the beginning of a string, modifies parameter as a new index
 */
void skipWhitespace(size_t& i, string ts)
{
 	 while(iswspace(ts[i]) && i<ts.length())
 	 {
	 	i++;
	 }
}

/**
 * Returns whether a character can be used as a character that can build a name
 */
bool isNameChar(char a)
{
	return (a >= 'a' && a <= 'z')  || (a >= 'A' && a <= 'Z' ) || ( a >= '0' && a <= '9' ) || ( a == '_' );
}

/**
 * From a string removes the leading/tailing non_name characters 
 * (ie, leaves: 'a..z', 'A..Z', '0..9', '_')
 */
string pureString(string s)
{
string result = "";
int i=0;
	while(!isNameChar(s[i]))
	{
		i++;
	}
	while(isNameChar(s[i]))
	{
		result += s[i];
		i++;
	}
	
	return result;
}


/**
 * Returns the substring from the parameter, startIndx, length
 */
string substring(string s, size_t st, size_t l)
{
string result = "";
size_t i=0;
size_t toTakeL = (l==-1?
			   s.length() - st
			   :l);

	while(i <= toTakeL)
	{
		result += s[i + st];
		i++;
	}

	return result;
}

/**
 * Checks whether the parameter is a number or not.
 */
bool isNumber(string s)
{
size_t i = 0;
size_t sl = strlen(s.c_str());
	while (i<sl)
	{
	char si = s[i];
	bool isMinus = (si == '-');
	bool isDot = (si == '.');
		if( !isMinus && !isDot && !isdigit(si) )
		{
			return false;
		}
		i++;
	}
	return true;
}

/**
 * Returns true if the passed parameter is a string that begins and ends with '"'
 */
bool isString(string s)
{
string ts = trim(s);
	if(ts[0] != '"')
	{
		return false;
	}
bool ending = false;
size_t i=1;
size_t sl = strlen(ts.c_str());
	while(i<sl - 1)
	{
		if (ts[i] == '"' && ts[i-1] !='\\') 
		{
			return false;
		}
		i++;
	}

	if(ts[sl-1] != '"')
	{
		return false;
	}

	return true;
}

/**
 * Returns from the given string s, which is separated using sep the cnth. element
 */
wstring getStringSep(wstring s, wchar_t sep, int cnt)
{
int curn = 0;
wstring tmp(L"");
size_t i = 0;
	while(curn < cnt && i < s.length())
	{
		if(s[i] == sep)
		{
			curn ++;
			if(curn == cnt)
			{
				return tmp;
			}
			tmp = L"";
		}
		else
		{
			tmp += s[i];
		}
		if(i == s.length() - 1 && curn+1 == cnt)
		{
			return tmp;
		}
		i ++;
	}
	return L"";
}


/**
* This method returns the first occurrence (index) of operator op in s.
* Returns -1 if nothing was found
* @param s - the string we are searching for the operator
* @param op - the operator we are searching for. Can be longer than one char
* @return - the index at which the operator was found
*/
size_t findFirstOperator(string s, string op)
{
	size_t i=0;
	if(op.length() == 0)
	{
		return -1;
	}

	while (i<s.length())
	{
		if(s[i] == op[0])
		{
			bool found = true;
			for (size_t j=0;j<op.length();j++)
			{
				if(s[i+j] != op[j])
				{
					found = false;
				}
			}
			if(found)
			{
				return i;
			}
		}

		i++;
	}

	return -1;
}

/**
* Finds the first keyword in a string (usually keyword means: first word)
* Keywords are separated by " " or "(" or "[" from the rest.
* returns first separator, in parameter the keyword
* @param s - is the string in which we are looking for the keyword
* @param keyw - is the keyword that was found
* @return - returns the separator character
*/
char findKeyword(string s, string& keyw)
{
	size_t i=0;
	keyw = "";
	while(s[i] !=' ' && s[i]!='(' && s[i]!='[' && i<s.length())
	{
		keyw += s[i];
		i++;
	}
	return s[i];
}

/**
 * Empties the different kind of Garbage Bins
 */
void emptyGarbageBins()
{
// 	CGarbageBin<CLineEntry>::getInstance()->empty();
// 	CGarbageBin<CLineEntry>::deleteInstance();
// 
// 	CGarbageBin<CObjectGroup>::getInstance()->empty();
// 	CGarbageBin<CObjectGroup>::deleteInstance();
// 
// 	CGarbageBin<CTreeItem>::getInstance()->empty();
// 	CGarbageBin<CTreeItem>::deleteInstance();
// 
// 	CGarbageBin<CCylinder>::getInstance()->empty();
// 	CGarbageBin<CCylinder>::deleteInstance();
// 
// 	CGarbageBin<CCone>::getInstance()->empty();
// 	CGarbageBin<CCone>::deleteInstance();
// 
// 	CGarbageBin<CSphere>::getInstance()->empty();
// 	CGarbageBin<CSphere>::deleteInstance();

}


string getValueFrom(const char* s, const char* _start)
{
const char *xpos = strstr(s, _start);
	if(xpos == NULL)
	{
		return "";
	}

	xpos += strlen(_start);	
string XValue = "";

	while(xpos && *xpos != ' ' && (*xpos) > 32)
	{
		XValue += *xpos ++;
	}
	return XValue;
}


string doubleToString(long double f, int precision)
{
char frmt[1024], s[1024];
	sprintf(frmt,"%%.%df", precision);
	sprintf(s,frmt,f);
	return string(s);

}

string intToString(int theNumber)
{
char s[1024];
	sprintf(s,"%d", theNumber);
	return string(s);

}

string intAsHex(unsigned long _v)
{
char s[1024];
	sprintf(s, "%X", _v);
	return string(s);
}

long hexToInt(string _s)
{
long tmp;
	sscanf(_s.c_str(), "%X", &tmp);

	return tmp;
}


string deliverName(string base, int ctr)
{
char s[256];
	sprintf(s,"_%d", ctr);
	string result = base + s;
	return result;
}