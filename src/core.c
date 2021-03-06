/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "core.h"
#include <ace/managers/game.h>
#include <ace/managers/rand.h>
#include <ace/managers/system.h>
#include <ace/utils/palette.h>
#include "menu.h"
#include "dino.h"
#include "game.h"
#include "hud.h"
#include "vehicle.h"
#include "fade.h"
#include "debug.h"
#include "base_tile.h"
#include "ground_layer.h"
#include "hi_score.h"
#include "tile.h"
#include "explosion.h"
#include "comm.h"

static tBitMap *s_pTiles;
static UWORD s_pPaletteRef[1 << GAME_BPP];
static UWORD *s_pColorBg;

static tView *s_pView;
static tVPort *s_pVpMain;
tTileBufferManager *g_pMainBuffer;

static void mainPaletteProcess(UBYTE ubFadeLevel) {
	tFadeState eState = fadeGetState();
	if(eState == FADE_STATE_IN_MORPHING || eState == FADE_STATE_OUT_MORPHING) {
		*s_pColorBg = paletteColorDim(s_pPaletteRef[0], ubFadeLevel);
		paletteDim(s_pPaletteRef, g_pCustom->color, 27, ubFadeLevel);
	}
}

void coreProcessBeforeBobs(void) {
	// Undraw all bobs
	debugColor(0x008);
	bobNewBegin();

	// Draw pending tiles
	tileBufferQueueProcess(g_pMainBuffer);

	// Draw dino bones before anything else
	dinoProcess();
}

void coreProcessAfterBobs(void) {

	// Finish bob drawing
	bobNewPushingDone();
	bobNewEnd();

	// Update HUD state machine and draw stuff
	hudUpdate();

	// Load next base tiles, if needed
	baseTileProcess();

	// Update palette for new ground layers, also take into account fade level
	fadeProcess();
	UBYTE ubFadeLevel = fadeGetLevel();
	groundLayerProcess(g_pMainBuffer->pCamera->uPos.uwY, ubFadeLevel);
	mainPaletteProcess(ubFadeLevel);

	debugColor(0x800);
	viewProcessManagers(s_pView);
	copProcessBlocks();
	debugColor(*s_pColorBg);
	vPortWaitForEnd(s_pVpMain);
}

void coreGsCreate(void) {
	hiScoreLoad();
	s_pView = viewCreate(0,
		TAG_VIEW_GLOBAL_CLUT, 1,
	TAG_END);

	g_pFont = fontCreate("data/uni54.fnt");
	textBobManagerCreate(g_pFont);
	s_pTiles = bitmapCreateFromFile("data/tiles.bm", 0);
	dinoCreate();

	hudCreate(s_pView, g_pFont);

	s_pVpMain = vPortCreate(0,
		TAG_VPORT_VIEW, s_pView,
		TAG_VPORT_BPP, GAME_BPP,
	TAG_END);
	g_pMainBuffer = tileBufferCreate(0,
		TAG_TILEBUFFER_VPORT, s_pVpMain,
		TAG_TILEBUFFER_BITMAP_FLAGS, BMF_CLEAR | BMF_INTERLEAVED,
		TAG_TILEBUFFER_BOUND_TILE_X, 11,
		TAG_TILEBUFFER_BOUND_TILE_Y, 2047,
		TAG_TILEBUFFER_IS_DBLBUF, 1,
		TAG_TILEBUFFER_TILE_SHIFT, 5,
		TAG_TILEBUFFER_REDRAW_QUEUE_LENGTH, 100,
		TAG_TILEBUFFER_TILESET, s_pTiles,
	TAG_END);

	paletteLoad("data/aminer.plt", s_pPaletteRef, 1 << GAME_BPP);
	memset(s_pVpMain->pPalette, 0, 1 << GAME_BPP);
	s_pColorBg = &s_pVpMain->pPalette[0];

	baseTileCreate(g_pMainBuffer);
	audioCreate();
	g_pSampleDrill = sampleCreateFromFile("data/sfx/drill1.raw8", 8000);
	g_pSampleOre = sampleCreateFromFile("data/sfx/ore2.raw8", 8000);
	g_pSampleTeleport = sampleCreateFromFile("data/sfx/teleport.raw8", 8000);

#ifdef GAME_DEBUG
	randInit(2184);
#else
	// Seed from beam pos Y & X
	randInit((g_pRayPos->bfPosY << 8) | g_pRayPos->bfPosX);
#endif

	tileInit(0, 1);

	bobNewManagerCreate(
		g_pMainBuffer->pScroll->pFront, g_pMainBuffer->pScroll->pBack,
		g_pMainBuffer->pScroll->uwBmAvailHeight
	);
	explosionManagerCreate();
	groundLayerCreate(s_pVpMain);
	commCreate();
	vehicleBitmapsCreate();
	vehicleCreate(&g_pVehicles[0], PLAYER_1);
	vehicleCreate(&g_pVehicles[1], PLAYER_2);

	menuPreload();
	bobNewAllocateBgBuffers();
	systemUnuse();

	g_pMainBuffer->pCamera->uPos.uwX = 32;
	// Initial background
	tileBufferRedrawAll(g_pMainBuffer);

	// Load the view
	viewLoad(s_pView);

	// Default config
	g_is2pPlaying = 0;
	g_is1pKbd = 0;
	g_is2pKbd = 1;
	g_isChallenge = 1;
	g_isAtari = 0;

	hudReset(0, 0);
	gamePushState(menuGsCreate, menuGsLoop, menuGsDestroy);
}

void coreGsLoop(void) {
	// you shouldn't be here!
	gameClose();
}

void coreGsDestroy(void) {
systemUse();

	menuUnload();
	bitmapDestroy(s_pTiles);
	dinoDestroy();
	baseTileDestroy();
	textBobManagerDestroy();
	fontDestroy(g_pFont);
	vehicleDestroy(&g_pVehicles[0]);
	vehicleDestroy(&g_pVehicles[1]);
	vehicleBitmapsDestroy();
	commDestroy();
	bobNewManagerDestroy();

	audioDestroy();
	sampleDestroy(g_pSampleDrill);
	sampleDestroy(g_pSampleOre);
	sampleDestroy(g_pSampleTeleport);

	explosionManagerDestroy();

  hudDestroy();
  viewDestroy(s_pView);
}
