#ifndef _COLORCHOOSERDIALOG_H_
#define _COLORCHOOSERDIALOG_H_

#include "wind_upg.h"
#include <wtypes.h>

/**
 * Represents a system dialog box that the user can use to pick a color.
 */
class ColorChooserDialog
{
public:

	/**
	 * Creates a new object
	 */
	ColorChooserDialog();

	/**
	 * Opens the dialog, and lets the user select a color. Returns the selected color value.
	 */
	COLORREF selectColor();

private:
	// the custom colors
	static COLORREF custColors[16] ;

	// the windows structure used to init this box
	CHOOSECOLOR cc;
};

#endif
