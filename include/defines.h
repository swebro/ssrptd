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

#ifndef DEFINES_H
#define DEFINES_H

#ifndef CONFIG
#define CONFIG "/etc/conf.d/ssrptd"
#endif

#ifndef CONFIG_LEN
#define CONFIG_LEN 64
#endif

#define CONFIG_FILE_NOT_FOUND -2
#define INIT_CONF_FAIL -3

#define ISPC_SIGLEN	4
#define ISPC_CALLEN	8
#define ISPC_RPTRIDLEN	8
#define ISPC_TXFREQLEN	9
#define	ISPC_TXFREQLEN	9
#define ISPC_RXPWRLEN	2
#define ISPC_CC_LEN	2
#define ISPC_LATLEN	8
#define	ISPC_LONGLEN	9
#define ISPC_HEIGHTLEN	3
#define ISPC_LOCLEN	20
#define	ISPC_DESCLEN	20
#define ISPC_URLLEN	124
#define ISPC_SIDLEN	40
#define ISPC_PIDLEN	40

#define COM_FILELEN	64

#endif
