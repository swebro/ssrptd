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

#include <string.h>
#include <stdio.h>
#include "../include/config.h"

int init_config(struct Config *conf){
	if (!conf) return -1;
	conf->daemon = -1;
	conf->verbose = -1;
	char foo[] = CONFIG;
	memset(conf->file, 0, CONFIG_LEN);
	memcpy(conf->file, CONFIG, strlen(foo));
	return 0;
}

int read_config(struct Config *conf){



	return 0;
}
