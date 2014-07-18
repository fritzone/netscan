#ifndef _TREEITEM_H_
#define _TREEITEM_H_

#include "wind_upg.h"

#include <string>
#include <vector>

using namespace std;

class CTreeEntry;
class CTreeControl;
class CTreeEntry;

/**
 * This is a tree control entry. The trees contain CTreeItem entries
 */
class CTreeItem
{
public:

	friend class CTreeControl;
	friend class CTreeEntry;

	/**
	 * Creates a new object
	 * @param _text: is the text that will be shown
	 * @param _imageIndex: is the index of the image associated with this tree entry
	 * @param _parent: the tree...
	 * @param _param: Is a user defined parameters, such as a CTreeEntry derived object
	 */
	CTreeItem(wstring _text, int _imageIndex, CTreeItem* _parent, CTreeEntry* _param);

	/**
	 * Returns whether this tree item is checked or not	
	 */
	bool isChecked();

	/**
	 * Returns the parameter of this entry
	 */
	CTreeEntry* getParameter() const;

	/**
	 * Returns the tree of this object
	 */
	CTreeControl* getTree();

	/**
	 * Checks this tree item
	 */
	void check();

	/**
	 * Unchecks the tree item
	 */
	void uncheck();

	/**
	 * Deletes this item from the tree
	 */
	void removeFromTree(bool);

	/**
	 * Returns the text of the tree item
	 */
	wstring const& getText() const
	{
		return text;
	}

	/**
	 * Bolds the tree item
	 */
	void boldIt();

	/**
	 * Un-Bolds the tree item
	 */
	void unBoldIt();

	void setImage(int newImage);

	void setSelectedImage(int newImage);

	void setText(LPWSTR text);

private:

	// the text
	wstring text;

	// the image
	int imageIndex;
	int selectedImageIndex;

	// the parent
	CTreeItem* parent;

	// application parameter... of type CTreeEntry
	CTreeEntry* param;

	// the windows handle of this entry
	HTREEITEM handle;

	// the tree control
	CTreeControl* tree;

	// the child items of this item
	vector<CTreeItem*> childs;
};

#endif
