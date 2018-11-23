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


extern unsigned char *com_keywords[];
extern unsigned char com_maxlen[];

extern unsigned char *ispc_keywords[];
extern unsigned char ispc_maxlen[];
extern unsigned char ispc_minlen[];

enum ispc_enum{signature, callsign, repeater_id, rx_frequency, tx_frequency,
			tx_power, color_code, latitude, longitude, height,
			location, description, URL, software_id, package_id};

enum com_enum{rx_device, rx_baudrate, tx_device, tx_baudrate, simplex_device,
		simplex_baudrate};

struct Config{
	int daemon;
	int verbose;
	unsigned char file[CONFIG_LEN];

	unsigned char *ispc[ISPC_NUM_CONF];
	unsigned char *com[COM_NUM_CONF];
};

int init_config(struct Config*);
int cleanup_config(struct Config*);
int read_config(struct Config*);

#endif
