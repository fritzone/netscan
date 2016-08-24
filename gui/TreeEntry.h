#ifndef _TREEENTRY_H_
#define _TREEENTRY_H_

class CTreeItem;

static const int TREENTRY_COMPUTER = 1;

/**
 * The class holds the 'tag' that can be attached to a tree item. Usually you'll derive
 * a class from this when you want to use it
 */
class CTreeEntry
{
public:

	/**
	 * Creates a new entry
	 */
	CTreeEntry(int);

	/**
	 * Returns the tree item of this entry
	 */
	CTreeItem* getTreeItem();

	/**
	 * Initializes the tree view of this group
	 */
	void setTreeItem(CTreeItem* _item);

	/**
	 * Returns the type of this entry
	 */
	int getType() const;

protected:
	// the tree-item which is associate with this
	CTreeItem* treeItem;

	// the type of the entry... just a number for your convenience
	int treeEntryType;
};

#endif
