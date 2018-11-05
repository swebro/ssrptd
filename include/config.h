/* Copyright (C) 2018 Kristoffer Forss <kristoffer.forss@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <defines.h>

struct Ispc{
	char signature[ISPC_SIGLEN];
	char callsign[ISPC_CALLEN];
	char rptrId[ISPC_RPTRIDLEN];
	char txFreq[ISPC_TXFREQLEN];
	char rxFreq[ISPC_TXFREQLEN];
	char rxPwr[ISPC_RXPWRLEN];
	char ColorCode[ISPC_CC_LEN];
	char latitude[ISPC_LATLEN];
	char longitude[ISPC_LONGLEN];
	char height[ISPC_HEIGHTLEN];
	char loc[ISPC_LOCLEN];
	char desc[ISPC_DESCLEN];
	char URL[ISPC_URLLEN];
	char sId[ISPC_SIDLEN];
	char pId[ISPC_PIDLEN];
};

struct Com{
	char rxCOM[COM_FILELEN];
	char txCOM[COM_FILELEN];
	char simplexCOM[COM_FILELEN];

	int rxBaud;
	int txBaud;
	int simplexBaud;
};

struct Config{
	int daemon;
	int verbose;
	char file[CONFIG_LEN];

	struct Ispc ispc;
	struct Com com;
};

int init_config(struct Config*);
int read_config(struct Config*);

#endif
