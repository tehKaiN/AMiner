/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include <ace/types.h>
#include <fixmath/fix16.h>
#include "bob_new.h"
#include "text_bob.h"
#include "mineral.h"
#include "dynamite.h"

#define VEHICLE_WIDTH 32
#define VEHICLE_HEIGHT 24

typedef enum _tDrillDir {
	DRILL_DIR_NONE = 0,
	DRILL_DIR_H,
	DRILL_DIR_V
} tDrillDir;

typedef enum _tDrillState {
	DRILL_STATE_VERT_ANIM_IN = 0,
	DRILL_STATE_DRILLING,
	DRILL_STATE_VERT_ANIM_OUT
} tDrillState;

typedef enum _tToolState {
	TOOL_STATE_IDLE,
	TOOL_STATE_DRILL
} tToolState;

typedef enum _tVehicleState {
	VEHICLE_STATE_MOVING,
	VEHICLE_STATE_DRILLING,
	VEHICLE_STATE_EXPLODING,
	VEHICLE_STATE_SMOKING,
	VEHICLE_STATE_TELEPORTING_OUT,
	VEHICLE_STATE_TELEPORTING_WAIT_FOR_CAMERA,
	VEHICLE_STATE_TELEPORTING_IN
} tVehicleState;

typedef struct _tVehicle {
	tBCoordYX sSteer;
	tTextBob sTextBob;
	tBobNew sBobBody;
	tBobNew sBobTrack;
	tBobNew sBobJet;
	tBobNew sBobTool;
	tBobNew sBobWreck;
	tBobNew sBobSmoke;
	fix16_t fX;
	fix16_t fY;
	fix16_t fDx;
	fix16_t fDy;
	fix16_t fDestX;
	fix16_t fDestY;
	UBYTE ubVehicleState;
	UBYTE isFacingRight;
	UBYTE ubTrackFrame;
	fix16_t ubTrackAnimCnt;
	UBYTE ubBodyShakeCnt;
	UBYTE ubJetShowFrame;
	UBYTE ubJetAnimFrame;
	UBYTE ubJetAnimCnt;
	UBYTE ubToolAnimCnt;
	UBYTE ubDrillDir;
	UBYTE ubDrillVAnimCnt;
	UBYTE ubSmokeAnimFrame;
	UBYTE ubSmokeAnimCnt;
	UBYTE ubTeleportAnimFrame;
	UBYTE ubTeleportAnimCnt;
	UWORD uwTeleportX;
	UWORD uwTeleportY;
	UBYTE ubDrillState;
	// Cargo
	UBYTE ubCargoMax;
	UBYTE ubCargoCurr;
	UWORD uwCargoScore;
	UWORD pStock[MINERAL_TYPE_COUNT];
	// Score, fuel, hull
	LONG lCash;
	UWORD uwDrillCurr;
	UWORD uwDrillMax;
	UWORD wHullCurr;
	UWORD wHullMax;
	UWORD uwDrillTileX;
	UWORD uwDrillTileY;
	UBYTE ubPlayerIdx;
	UBYTE ubDestructionState;
	tDynamite sDynamite;
} tVehicle;

void vehicleBitmapsCreate(void);

void vehicleBitmapsDestroy(void);

void vehicleCreate(tVehicle *pVehicle, UBYTE ubIdx);

void vehicleDestroy(tVehicle *pVehicle);

UBYTE vehicleIsNearShop(const tVehicle *pVehicle);

void vehicleSetPos(tVehicle *pVehicle, UWORD uwX, UWORD uwY);

void vehicleResetPos(tVehicle *pVehicle);

void vehicleReset(tVehicle *pVehicle);

void vehicleMove(tVehicle *pVehicle, BYTE bDirX, BYTE bDirY);

void vehicleProcessText(void);

void vehicleProcess(tVehicle *pVehicle);

void vehicleTeleport(tVehicle *pVehicle, UWORD uwX, UWORD uwY);

uint8_t vehiclesAreClose(void);

tVehicle g_pVehicles[2];

#endif // _VEHICLE_H_
