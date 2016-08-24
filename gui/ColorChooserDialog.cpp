#include "ColorChooserDialog.h"

COLORREF ColorChooserDialog::custColors[16];

/**
 * Constructor
 */
ColorChooserDialog::ColorChooserDialog()
{
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = NULL;
	cc.hInstance = NULL;
	cc.rgbResult = 0;
	cc.Flags = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT ;
	cc.lCustData = NULL;
	cc.lpCustColors = custColors;
	cc.lpfnHook = NULL;
	cc.lpTemplateName = NULL;
}

/**
 * Opens the dialog
 */
COLORREF ColorChooserDialog::selectColor()
{
	ChooseColor(&cc);
	return cc.rgbResult;
}