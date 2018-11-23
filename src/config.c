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

unsigned char* ispc_keywords[] = {"signature", "callsign", "repeater_id",
			"rx_frequency", "tx_frequency", "tx_power",
			"color_code", "latitude", "longitude",
			"height", "location", "description", "URL",
			"software_id", "package_id"};
unsigned char ispc_maxlen[]  = {4, 8, 4, 9, 9, 2, 2, 8, 9, 3, 20, 20, 124, 40, 40};
unsigned char ispc_minlen[]  = {4, 1, 4, 9, 9, 2, 2, 8, 9, 1,  1,  0,   0,  0,  0};

unsigned char *com_keywords[] = {"rx_device", "rx_baudrate", "tx_device",
					"tx_baudrate", "simplex_device",
					"simplex_baudrate"};
unsigned char com_maxlen[] = {64, 64, 64, 64, 64, 64};

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
	for (int i = 0; i < COM_NUM_CONF; i++){
		free(conf->com[i]);
	}
}

int parseISPC(unsigned char *, unsigned char *, int, int);
int parseCOM(unsigned char *, unsigned char *, int, int);
int init_software_id(unsigned char *);
int init_package_id(unsigned char *);

int read_config(struct Config *conf){
	/* Setting upp the header */
	for (int i = 0; i < ISPC_NUM_CONF; i++){
		free(conf->ispc[i]);
		conf->ispc[i] = malloc(sizeof(unsigned char) * ispc_maxlen[i]);
		memset(conf->ispc[i], 0, ispc_maxlen[i]);
	}

	/* Allocating COM config */
	for (int i = 0; i < COM_NUM_CONF; i++){
		free(conf->com[i]);
		conf->com[i] = malloc(sizeof(unsigned char) * com_maxlen[i]);
		memset(conf->com[i], 0, com_maxlen[i]);
	}

	/* Callsign entry must be filled up with spaces */
	memset(conf->ispc[1], ' ', ispc_maxlen[1]);

	/* time to parse configuration file */
	FILE *fp;
	char *line = 0;
	unsigned char *token = 0;
	unsigned char *buf;
	size_t len = 0;
	ssize_t read;
	char invalid_conf = 0;
	int invalid_conf_index = 0;

	fp = fopen(conf->file, "r");
	if (!fp) return COULD_NOT_OPEN_FILE;

	while ((read = getline(&line, &len, fp)) != -1){
		/* buf need to be \0 terminated */
		buf = malloc(sizeof(unsigned char) * (read + 1));
		memset(buf, 0, read + 1);

		/* Remove all comments */
		for (int i = 0; i < read; i++){
			if (line[i] == '#') break;
			buf[i] = line[i];
		}

		/* Split into tokens around '=' */
		token = strtok(buf, "=");

		if (token && (invalid_conf == 0)){
			/* Process ISPC configurations */
			for (int i = 0; i < ISPC_NUM_CONF; i++){
				if (!strcmp(token, ispc_keywords[i])){
					token = strtok(0, "=");
					token[strlen(token) - 1] = 0;
					if(parseISPC(conf->ispc[i], token, strlen(token), i)){
						invalid_conf_index = i;
						invalid_conf = 1;
					}
					break;
				}
			}
			/* Process COM configurations */
			for (int i = 0; i < COM_NUM_CONF; i++){
				if (!strcmp(token, com_keywords[i])){
					token = strtok(0, "=");
					token[strlen(token) - 1] = 0;
					if(parseCOM(conf->com[i], token, strlen(token), i)){
						invalid_conf_index = i;
						invalid_conf = 2;
					}
					break;
				}
			}
		}
		free(buf);
	}


	/* software_id and package_id set by build */
	if (init_software_id(conf->ispc[13])) return COULD_NOT_SET_SOFTWARE_ID;
	if (init_package_id(conf->ispc[14])) return COULD_NOT_SET_PACKAGE_ID;

	if (invalid_conf == 1){
		/* Remove newline */
		if (conf->ispc[invalid_conf_index][strlen(conf->ispc[invalid_conf_index])] == '\n')
			conf->ispc[invalid_conf_index][strlen(conf->ispc[invalid_conf_index]) - 1] = 0;
		/* Because reapeater id is a special snowflake */
		if (invalid_conf_index == 2)
			printf("ERROR: invalid configuration \n\"%s\"\n", ispc_keywords[invalid_conf_index]);
		else
			printf("ERROR: invalid configuration \n\"%s=%s\"\n", ispc_keywords[invalid_conf_index], conf->ispc[invalid_conf_index]);
		return invalid_conf_index;
	}
	for (int i = 0; i < ISPC_NUM_CONF; i++){
		/* Because reapeater id is a special snowflake */
		if (i == repeater_id){
			printf("%s   \t0x",ispc_keywords[repeater_id]);
			for (int j = 0; j < 4; j++){
				if ((j + 1) % 2 && j != 0) printf(" ");
				if (conf->ispc[repeater_id][j] < 0x10)
					printf("0%x", conf->ispc[repeater_id][j]);
				else
					printf("%x", conf->ispc[repeater_id][j]);
			}
			printf("\n");
		}
		else
			printf("%s   \t%s\n",ispc_keywords[i], conf->ispc[i]);
	}

	fclose(fp);
	free(line);

	return 0;
}

int parseISPC(unsigned char *ispc, unsigned char *arg, int arg_len, int index){
	/* repeater_id needs to be converted from hex to dec */
	if (index == repeater_id){
		if (arg_len == 2*ispc_maxlen[repeater_id]){
			unsigned char *c = arg;
			unsigned char step = sizeof(unsigned char);
			for(int i = 0; i < ispc_maxlen[repeater_id]; i++){
				unsigned char s[repeater_id];
				memcpy(s, c + i*2*step, 2);
				ispc[i] = (unsigned char) strtol(s, 0, 16);
			}
			return 0;
		}

		/* Wrong arglen, returning error */
		return -1;
	}

	if ((arg_len <= ispc_maxlen[index]) && (arg_len >= ispc_minlen[index])){
		memcpy(ispc, arg, arg_len);
	}
	else{
		memcpy(ispc, arg, ispc_maxlen[index]);
		return -1;
	}
	return 0;
}

int parseCOM(unsigned char *com, unsigned char *arg, int arg_len, int index){
	if (arg_len <= com_maxlen[index]){
		memcpy(com, arg, arg_len);
	}
	else{
		memcpy(com, arg, com_maxlen[index]);
		return -1;
	}
	return 0;
}

/* Because software id is a special snowflake */
int init_software_id(unsigned char *conf){
	if (sprintf(conf, OS) < 0) return 1;
	if (sprintf(conf + strlen(conf), ":") < 0) return 1;
	if (sprintf(conf + strlen(conf), PROJECT) < 0) return 1;
	if (sprintf(conf + strlen(conf), "-") < 0) return 1;
	if (sprintf(conf + strlen(conf), VERSION) < 0) return 1;
	return 0;
}

/* And so are package id */
int init_package_id(unsigned char *conf){
	if (sprintf(conf, PACKET_MANAGER) < 0) return -1;
	if (sprintf(conf + strlen(conf), ":") < 0) return 1;
	if (sprintf(conf + strlen(conf), PROJECT) < 0) return 1;
	if (sprintf(conf + strlen(conf), "-") < 0) return 1;
	if (sprintf(conf + strlen(conf), VERSION) < 0) return 1;
	if (sprintf(conf + strlen(conf), "-") < 0) return 1;
	if (sprintf(conf + strlen(conf), ARCH) < 0) return 1;
	return 0;
}
