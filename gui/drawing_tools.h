#ifndef _DRAWINGTOOLS_H_
#define _DRAWINGTOOLS_H_

// the name of the font used on the controls
const static wchar_t* fontName = L"Verdana";

// the fonts used
static HFONT sysFont = CreateFont(13, 0, 0, 0, FW_THIN, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
                                             PROOF_QUALITY, VARIABLE_PITCH|FF_ROMAN, (LPCWSTR)fontName);
;


#endif
