#ifndef _LISTBOX_H_
#define _LISTBOX_H_

#include "OwnerDrawControl.h"

#include <string>
#include <vector>

using namespace std;

// The listbox styles
// only strings are contained
const static int LST_STRS		= 1;
// contains bitmaps too. In this case the user should provide a draw function
const static int LST_BMPS		= 2;

class CListBox;

// this is a callback style for a function which draws a list item
typedef bool (*LISTITEMDRAW)(wstring, LPDRAWITEMSTRUCT, CListBox*);

class CWindow;

/**
 * Creates a list box, with multiple selections or none, texts or bitmaps can be displayed.
 * If the type is bitmap then the passed in strings will be considered as being bitmaps
 */
class CListBox : public COwnerDrawControl
{
public:
    /**
     * Creates a new list box.
     * @param _parent - the window to which this toolbar is attached
     * @param _x - the x positino of the list box
     * @param _y - the y position of the list box
     * @param _w - the width of the list box
     * @param _h - the height of the list box
     * @param _id - the unique ID of the list box
     * @param _multiSelect - specifies whether the list box can be a multiselect list box or no. By 
     *                       default, it cannot be.
     * @param _style - specifies if the list box contains only strings (LST_STRS) or contains bitmap items (LST_BMPS)
     */
	CListBox(CWindow* _parent, int _x, int _y, int _w, int _h, DWORD _id, bool _multiSelect = false, int _style = LST_STRS, LISTITEMDRAW _drawMethod = NULL, int _itemHeight = -1);

    /**
     * Destroys an existing listbox
     */
	virtual ~CListBox();

	/**
	 * Adds the string to the items of this listbox
	 */
	void addString(const wchar_t*);

	/**
	 * Adds the string to the items of this listbox
	 */
	void addString(std::wstring s);

	/**
	 * Checks if the list box has the given item
	 */
	bool hasItem(std::wstring s);

	/**
	 * Retrieves the item count
	 */
	int getItemCount();

	/**
	 * Retrievs the item with index as _idx
	 */
	wstring getItem(int _idx);

	/**
	 * Clears the content of the list box (ie. removes all the entries).
	 */
	void clear();

	/**
	 * Returns the selected index
	 */
	int getSelectedIndex();

	/**
	 * Returns the selected item
	 */
	wstring getSelectedItem();

	/**
	 * Returns the selected items (if more than one is selected)
	 */
	vector <int> getSelectedIndexes();

    /**
     * This method is the one responsible for actually creating the window. Done like this because
     * every CControl derived class can have its own way to specify the flags for CreateWindow
	 * such as different class names, etc...
     */
	virtual bool createWindow();

	/**
	 * Draws the owner drawn list box, using the 'dis' structure
	 */
	virtual bool draw(LPDRAWITEMSTRUCT dis);

	/**
	 * Returns the item data associated with the given index
	 */
	void* getItemData(int _idx);

	/**
	 * Sets the item data at the given index
	 */
	void setItemData(int _idx, void* _data);

	/**
	 * Adds a string to the listbox, it specifies the item data too
	 */
	void addString(const wchar_t* _text, void* _data);

	/**
	 * Selects the item with the given index
	 */
	void select(int _idx);

	/**
	 * Selects the item with the given string
	 */
	void select(wstring);

private:

	// this is the method that will draw the list items
	LISTITEMDRAW drawer;

	// this is the height of one item. At this stage only fixed height items are supported
	int itemHeight;
};

#endif 
