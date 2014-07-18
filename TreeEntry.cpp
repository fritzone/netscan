#include "TreeEntry.h"
#include "TreeItem.h"

#include "leaker.h"

/**
 * Constructor
 */
CTreeEntry::CTreeEntry(int _type)
{
	treeEntryType = _type;
	treeItem = NULL;
}

/**
 * Returns the tree item
 */
CTreeItem* CTreeEntry::getTreeItem() 
{
	return treeItem;
}

/**
 * Sets the tree item
 */
void CTreeEntry::setTreeItem(CTreeItem* _item)
{
	treeItem = _item;
	_item->param = this;
}

/**
 * Returns the type
 */
int CTreeEntry::getType() const
{
	return treeEntryType;
}
