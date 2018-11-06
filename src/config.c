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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/config.h"
#include "../include/defines.h"

char* ispc_keywords[] = {"signature", "callsign", "repeater_id",
			"rx_frequency", "tx_frequency", "tx_power",
			"color_code", "latitude", "longitude",
			"height", "location", "description", "URL",
			"software_id", "package_id"};
char ispc_len[] = {4, 8, 4, 9, 9, 2, 2, 8, 9, 3, 20, 20, 124, 40, 40};

int init_config(struct Config *conf){
	if (!conf) return -1;
	conf->daemon = -1;
	conf->verbose = -1;
	memset(conf->file, 0, CONFIG_LEN);
	memcpy(conf->file, CONFIG, strlen(CONFIG));


	return 0;
}

int cleanup_config(struct Config *conf){
	for (int i = 0; i < ISPC_NUM_CONF; i++){
		free(conf->ispc[i]);
	}
}

int read_config(struct Config *conf){
	for (int i = 0; i < ISPC_NUM_CONF; i++){
		free(conf->ispc[i]);
		conf->ispc[i] = malloc(sizeof(char) * ispc_len[i]);
		memset(conf->ispc[i], 0, ispc_len[i]);
	}

	/* Callsign entry must be filled up with spaces */
	memset(conf->ispc[1], ' ', ispc_len[1]);
	/* software_id and package_id set by build */
	sprintf(conf->ispc[13], OS);
	sprintf(conf->ispc[13] + strlen(conf->ispc[13]), ":");
	sprintf(conf->ispc[13] + strlen(conf->ispc[13]), PROJECT);
	sprintf(conf->ispc[13] + strlen(conf->ispc[13]), "-");
	sprintf(conf->ispc[13] + strlen(conf->ispc[13]), VERSION);

	sprintf(conf->ispc[14], PACKET_MANAGER);
	sprintf(conf->ispc[14] + strlen(conf->ispc[14]), ":");
	sprintf(conf->ispc[14] + strlen(conf->ispc[14]), PROJECT);
	sprintf(conf->ispc[14] + strlen(conf->ispc[14]), "-");
	sprintf(conf->ispc[14] + strlen(conf->ispc[14]), VERSION);
	sprintf(conf->ispc[14] + strlen(conf->ispc[14]), "-");
	sprintf(conf->ispc[14] + strlen(conf->ispc[14]), ARCH);


	/* time to parse configuration file */
	int sum = 0;
	for(int i = 0; i < ISPC_NUM_CONF; i++){
		sum+=ispc_len[i];
	}
	printf("%d\n",sum);

	return 0;
}
