#ifndef _IMAGELIST_H_
#define _IMAGELIST_H_

#include "wind_upg.h"
#include <WTypes.h>
#include <CommCtrl.h>

class CWindow;

/**
 * This class represents an image list, used in TreeControls and other common controls
 */
class CImageList
{
public:

	friend class CTreeControl;

	/**
	 * Creates a new ImageList, by specifying the width and the height
	 */
	CImageList(int _width, int _height, CWindow* parent);

	/**
	 * Adds an iamge to the list
	 */
	bool addImage(LPCWSTR image);
	
	/**
	 * Adds an image from teh resource repository
	 */
	bool addResourceImage(DWORD);

private:

	// the windows handle
	HIMAGELIST imageList;

	// the width of the images
	int cx;

	// the height of the images
	int cy;

	// the handle ofthe parent
	HWND hParent;

};

#endif
