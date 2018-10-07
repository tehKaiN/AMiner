/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _TEXT_BOB_H_
#define _TEXT_BOB_H_

#include "bob_new.h"
#include <ace/utils/font.h>

typedef struct _tTextBob {
	const tFont *pFont;
	char szText[100];
	tBobNew sBob;
	tTextBitMap *pTextBitmap;
	UWORD uwWidth;
	UWORD uwDestPosY;
	UBYTE isUpdateRequired;
	UBYTE ubColor;
} tTextBob;

void textBobCreate(tTextBob *pTextBob, const tFont *pFont);

void textBobDestroy(tTextBob *pTextBob);

void textBobSet(
	tTextBob *pTextBob, const char *szText, UBYTE ubColor,
	UWORD uwPosX, UWORD uwPosY, UWORD uwDestPosY
);

void textBobUpdate(tTextBob *pTextBob);

void textBobAnimate(tTextBob *pTextBob);

#endif // _TEXT_BOB_H_