/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _COMM_SHOP_H_
#define _COMM_SHOP_H_

#include <ace/types.h>

void commShopGsCreate(void);

void commShopGsLoop(void);

void commShopGsDestroy(void);

void commShopAlloc(void);

void commShopDealloc(void);

#endif // _COMM_SHOP_H_