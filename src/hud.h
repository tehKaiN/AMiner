/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _HUD_H_
#define _HUD_H_

#include <ace/utils/extview.h>
#include <ace/utils/font.h>

typedef enum _tHudPlayer {
	PLAYER_1 = 0,
	PLAYER_2
} tHudPlayer;

void hudCreate(tView *pView, const tFont *pFont);

void hudDestroy(void);

void hudReset(UBYTE isChallenge);

void hudSetDepth(UBYTE ubPlayer, UWORD uwDepth);

void hudSetScore(UBYTE ubPlayer, ULONG ulCash);

void hudSetCargo(UBYTE ubPlayer, UBYTE ubCargo, UBYTE ubCargoMax);

void hudSetFuel(UBYTE ubPlayer, UWORD ubFuel, UWORD ubFuelMax);

void hudSetHealth(UBYTE ubPlayer, UBYTE ubHealth, UBYTE ubHealthMax);

void hudUpdate(void);

#endif // _HUD_H_
