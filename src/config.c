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
char ispc_maxlen[]  = {4, 8, 4, 9, 9, 2, 2, 8, 9, 3, 20, 20, 124, 40, 40};
char ispc_minlen[]  = {4, 1, 3, 9, 9, 2, 2, 8, 9, 1,  1,  0,   0,  0,  0};
char ispc_hardlen[] = {1, 0, 1, 1, 1, 1, 1, 1, 1, 0,  0,  0,   0,  0,  0};

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

int parseISPC(char *, char *, int, int);
int init_software_id(struct Config*);
int init_package_id(struct Config*);

int read_config(struct Config *conf){
	/* Setting upp the header */
	for (int i = 0; i < ISPC_NUM_CONF; i++){
		free(conf->ispc[i]);
		conf->ispc[i] = malloc(sizeof(char) * ispc_maxlen[i]);
		memset(conf->ispc[i], 0, ispc_maxlen[i]);
	}

	/* Callsign entry must be filled up with spaces */
	memset(conf->ispc[1], ' ', ispc_maxlen[1]);

	/* time to parse configuration file */
	FILE *fp;
	char *line = 0;
	char *token = 0;
	char *buf;
	size_t len = 0;
	ssize_t read;
	int invalid_conf = -1;

	fp = fopen(conf->file, "r");
	if (!fp) return COULD_NOT_OPEN_FILE;

	while ((read = getline(&line, &len, fp)) != -1){
		/* buf need to be \0 terminated */
		buf = malloc(sizeof(char) * (read + 1));
		memset(buf, 0, read + 1);

		/* Remove all comments */
		for (int i = 0; i < read; i++){
			if (line[i] == '#') break;
			buf[i] = line[i];
		}

		/* Split into tokens around '=' */
		token = strtok(buf, "=");

		/* Process ISPC configurations */
		if (token && (invalid_conf == -1)) for (int i = 0; i < ISPC_NUM_CONF; i++){
			if (!strcmp(token, ispc_keywords[i])){
				token = strtok(0, "=");
				token[strlen(token) - 1] = 0;
				if(parseISPC(conf->ispc[i], token, strlen(token), i)){
					invalid_conf = i;
				}
				break;
			}
		}
		free(buf);
	}

	if (invalid_conf != -1){
		/* Remove newline */
		if (conf->ispc[invalid_conf][strlen(conf->ispc[invalid_conf])] == '\n')
			conf->ispc[invalid_conf][strlen(conf->ispc[invalid_conf]) - 1] = 0;
		printf("ERROR: invalid configuration \n\"%s=%s\"\n", ispc_keywords[invalid_conf], conf->ispc[invalid_conf]);
		return invalid_conf;
	}
	for (int i = 0; i < ISPC_NUM_CONF; i++){
		printf("%s   \t%s\n",ispc_keywords[i], conf->ispc[i]);
	}

	fclose(fp);
	free(line);


	/* software_id and package_id set by build */
	if (!init_software_id(conf)) return COULD_NOT_SET_SOFTWARE_ID;
	if (!init_package_id(conf)) return COULD_NOT_SET_PACKAGE_ID;

	return 0;
}


int parseISPC(char *ispc, char *arg, int arg_len, int index){
	/* repeater_id needs to be converted from hex to dec */
	if (index == 2){
		if (arg_len == 2*ispc_maxlen[2]){
			char *c = arg;
			char step = sizeof(char);
			for(int i = 0; i < ispc_maxlen[2]; i++){
				char s[2];
				memcpy(s, c + i*2*step, 2);
				ispc[i] = strtol(s, 0, 16);
			}
			return 0;
		}
		else if (arg_len == 2*ispc_minlen[2]){
			char *c = arg;
			char step = sizeof(char);
			for(int i = 0; i < ispc_minlen[2]; i++){
				char s[2];
				memcpy(s, c + i*2*step, 2);
				ispc[i + ispc_maxlen[2] - ispc_minlen[2]] = strtol(s, 0, 16);
			}
			return 0;
		}

		/* Wrong arglen, returning error */

		return -1;
	}

	if (ispc_maxlen[index] == ispc_minlen[index]){
		if (arg_len == ispc_maxlen[index]){
			memcpy(ispc, arg, arg_len);
		}
		else{
			memcpy(ispc, arg, ispc_maxlen[index]);
			return -1;
		}
	}
	else{
		if ((arg_len <= ispc_maxlen[index]) && (arg_len >= ispc_minlen[index])){
			memcpy(ispc, arg, arg_len);
		}
		else{
			memcpy(ispc, arg, ispc_maxlen[index]);
			return -1;
		}
	}
	return 0;
}

int init_software_id(struct Config *conf){
	if (sprintf(conf->ispc[13], OS) < 0) return -1;
	if (sprintf(conf->ispc[13] + strlen(conf->ispc[13]), ":") < 0) return -1;
	if (sprintf(conf->ispc[13] + strlen(conf->ispc[13]), PROJECT) < 0) return -1;
	if (sprintf(conf->ispc[13] + strlen(conf->ispc[13]), "-") < 0) return -1;
	if (sprintf(conf->ispc[13] + strlen(conf->ispc[13]), VERSION) < 0) return -1;
}

int init_package_id(struct Config *conf){
	if (sprintf(conf->ispc[14], PACKET_MANAGER) < 0) return -1;
	if (sprintf(conf->ispc[14] + strlen(conf->ispc[14]), ":") < 0) return -1;
	if (sprintf(conf->ispc[14] + strlen(conf->ispc[14]), PROJECT) < 0) return -1;
	if (sprintf(conf->ispc[14] + strlen(conf->ispc[14]), "-") < 0) return -1;
	if (sprintf(conf->ispc[14] + strlen(conf->ispc[14]), VERSION) < 0) return -1;
	if (sprintf(conf->ispc[14] + strlen(conf->ispc[14]), "-") < 0) return -1;
	if (sprintf(conf->ispc[14] + strlen(conf->ispc[14]), ARCH) < 0) return -1;
}
