#include "TreeItem.h"
#include "TreeEntry.h"
#include "TreeControl.h"

#include "leaker.h"

/**
 * Creates a new tree item
 */
CTreeItem::CTreeItem(wstring _text, int _imageIndex, CTreeItem* _parent, CTreeEntry* _param)
{
	text = _text;
	imageIndex = _imageIndex;
	selectedImageIndex = _imageIndex;
	parent = _parent;
	param = _param;
	if(param)
	{
		param->setTreeItem(this);
	}
	// keeping the child/parent relationship alive ... if there is a parent
	if(_parent)
	{
		_parent->childs.push_back(this);
	}
}

/**
 * Returns the parameter of this tree item
 */
CTreeEntry* CTreeItem::getParameter() const
{
	return param;
}

/**
 * Checks if the treeitem is checked or no
 */
bool CTreeItem::isChecked()
{
UINT state = TreeView_GetCheckState(tree->getHandle(), handle);
	if(state == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * Returns the tree of this item
 */
CTreeControl* CTreeItem::getTree()
{
	return tree;
}

/**
 * Checks this tree item
 */
void CTreeItem::check()
{
	TreeView_SetCheckState(tree->getHandle(), handle, TRUE);
}

/**
 * Unchecks the tree item
 */
void CTreeItem::uncheck()
{
	TreeView_SetCheckState(tree->getHandle(), handle, FALSE);
}

/**
 * Deletes this item from the tree
 */
void CTreeItem::removeFromTree(bool childrenToo)
{
	// firstly: delete all the child items of this entry
	if(childrenToo)
	{
		for(size_t i=0; i<childs.size(); i++)
		{
			childs[i]->removeFromTree(true);
		}
	}

	// and finally remove this item
	tree->deleteItem(this);
}

void CTreeItem::boldIt()
{
UINT state = TVIS_BOLD;
UINT mask = TVIF_STATE;
TVITEMEX itemex;
	itemex.mask = TVIF_STATE | TVIF_HANDLE;
	itemex.state = state;
	itemex.hItem = handle;
	itemex.stateMask = state;

	TreeView_SetItem(tree->getHandle(), &itemex);
}

void CTreeItem::unBoldIt()
{
UINT state = TVIS_BOLD;
UINT mask = TVIF_STATE;
TVITEMEX itemex;
	itemex.mask = TVIF_STATE | TVIF_HANDLE;
	itemex.state = 0;
	itemex.hItem = handle;
	itemex.stateMask = state;

	TreeView_SetItem(tree->getHandle(), &itemex);
}

void CTreeItem::setImage(int newImage)
{
	UINT mask = TVIF_IMAGE;
	TVITEMEX itemex;
	itemex.mask = mask;
	itemex.hItem = handle;
	itemex.iImage = newImage;

	TreeView_SetItem(tree->getHandle(), &itemex);
}

void CTreeItem::setSelectedImage(int newImage)
{
	UINT mask = TVIF_SELECTEDIMAGE;
	TVITEMEX itemex;
	itemex.mask = mask;
	itemex.hItem = handle;
	itemex.iSelectedImage = newImage;
	

	TreeView_SetItem(tree->getHandle(), &itemex);
}

void CTreeItem::setText(LPWSTR text)
{
	UINT mask = TVIF_TEXT;
	TVITEMEX itemex;
	itemex.mask = mask;
	itemex.hItem = handle;
	itemex.pszText = text;

	TreeView_SetItem(tree->getHandle(), &itemex);
}
