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

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/main.h"
#include "../include/serial.h"
#include "../include/config.h"
#include "../include/defines.h"
#include "../include/daemon.h"

int main(int argc, char **argv){
	int ret;

	static struct Config conf;
	ret = init_config(&conf);
	if (ret != 0){
		printf("ERROR, cannot init configuration");
		exit(INIT_CONF_FAIL);
	}


	int c;
	int option_index;
	static struct option long_options[] =
        {
          /* These options set a flag. */
          {"verbose", no_argument,       &(conf.verbose),   1},
          {"brief",   no_argument,       &(conf.verbose),   0},
          {"no-daemon",no_argument,      &(conf.daemon),    0},
          /* These options don’t set a flag.
             We distinguish them by their indices. */
          {"add",     no_argument,       0, 'a'},
          {"append",  no_argument,       0, 'b'},
          {"delete",  required_argument, 0, 'd'},
          {"conf.file",  required_argument, 0, 'c'},
          {"file",    required_argument, 0, 'f'},
          {0, 0, 0, 0}
        };

	while(1){
		c = getopt_long(argc, argv, "abc:d:f:", long_options, &option_index);
		if(c == -1)
			break;
		switch (c){
			case 0:
				/* If this option set a flag, do nothing else now. */
					break;
			case 'c':
				if (strlen(optarg) <= CONFIG_LEN){
					if (access(optarg, F_OK) == -1){
						printf("ERROR: config file not found, exiting\n");
						exit(CONFIG_FILE_NOT_FOUND);
					}
					memset(conf.file, 0, CONFIG_LEN);
					memcpy(conf.file, optarg, strlen(optarg));
					printf("Using config file: '%s'\n", conf.file);
				}
				else
					printf("ERROR: config file name too long\nUsing default config: %s\nConsider compiling with CONFIG_LEN=[required number of chars]\n", conf.file);
				break;
			case '?':
				printUse();
				break;
			default:
				abort();
		}
	}

	read_config(&conf);

	daemonize(&conf);

	cleanup_config(&conf);

	return 0;
}

void printUse(){
	printf("Here a usage message should appear\n");
}
