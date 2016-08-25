#ifndef _COMPUTER_H_
#define _COMPUTER_H_

#include "TreeEntry.h"

#include <string>

using namespace std;

class CTreeItem;

/**
 * Holds the data of a computer. With time this structure will be filled to hold every computer related data.
 */
class CComputer : public CTreeEntry
{
public:

	CComputer(const wstring& _ip) : CTreeEntry(TREENTRY_COMPUTER), ip(_ip)
	{}

	std::wstring getIp() const { return ip; }
	void setIp(std::wstring val) { ip = val; }
	wstring name;
	CTreeItem* treeItem;
private:
	wstring ip;
	
};

#endif
