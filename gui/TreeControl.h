#pragma once

#include "Control.h"
#include "TreeItem.h"
#include "TreeEntry.h"

#include <vector>

using namespace std;

class CContainer;
class CImageList;

class CTreeControl;

// this is the callback that will be used in tree item notification controls
typedef bool (*TREECONTROL_NOTIFY_CALLBACK)(CTreeControl*, CTreeItem*);

/**
 * The classic Tree Control
 */
class CTreeControl : public CControl
{
public:

	/**
	 * Creates a new TreeControl...
	 */
	CTreeControl(CContainer* _parent, int _x, int _y, int _w, int _h, DWORD _id);

	/**
	 * Destructor
	 */
	virtual ~CTreeControl(void);

	/**
	 * Sets the images
	 */
	void setImages(CImageList*);

	/**
	 * Adds the item
	 */
	void addItem(CTreeItem* item);

	/**
	 * Deletes the item from the tree
	 */
	void removeItem(CTreeItem* item);

	/**
	 * Makes sure the item is visible
	 */
	void ensureVisible(CTreeItem* _item);

	/**
	 * Selectes the item.
	 */
	void selectItem(CTreeItem* _item)
	{
		TreeView_SelectItem(hwnd, _item->handle);
	}

	/**
	 * Returns all the checked items from this control
	 */
	vector<CTreeItem*> getCheckedItems();

    /**
     * This method is the one responsible for actually creating the window. Done like this because
     * every CControl derived class can have its own way to specify the flags for CreateWindow
	 * such as different class names, etc...
     */
	virtual bool createWindow();	

	/**
	 * Deletes all the items...
	 */
	void deleteAllItems();

	/**
	 * Deletes only the given item
	 */
	void deleteItem(CTreeItem*);

	/**
	 * Returns the tree item at the point
	 */
	CTreeItem* itemAt(int, int);

	/**
	 * Called when the selection is about to cahnge
	 */
	LPARAM selChanging(LPNMTREEVIEW);

	/**
	 * Called when the selection has been changed
	 */
	LPARAM selChanged(LPNMTREEVIEW);

	/**
	 * Called when the label edit of this item is about to be starting
	 */
	LPARAM endLabelEdit(LPNMTREEVIEW);

	/**
	 * Called when the label edit of this item is about to be starting
	 */
	LPARAM startLabelEdit(LPNMTREEVIEW);

	/**
	 * Returns the item if the two coordinate items are on the checkbox of it
	 */
	CTreeItem* clickOnCheckBox(int, int);

	/**
	 * Returns the CTreeItem for the given handle
	 */
	CTreeItem* getItem(HTREEITEM);

	/**
	 * Returns the selected item
	 */
	CTreeItem* getSelectedItem();

public:

	// the callbacks of the tree control
	TREECONTROL_NOTIFY_CALLBACK onSelChanging;
	TREECONTROL_NOTIFY_CALLBACK onSelChanged;
	TREECONTROL_NOTIFY_CALLBACK onFinishEditItemsLabel;
	TREECONTROL_NOTIFY_CALLBACK onStartEditItemsLabel;

private:

	// the images of the tree control
	CImageList* imgs;

	// contains all the tree items... not very fancy method, but will work
	vector<CTreeItem*> items;
};
