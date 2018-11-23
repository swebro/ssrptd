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


#ifndef PROJECT
#define PROJECT "ssrptd"
#endif
#ifndef VERSION
#define VERSION "9999"
#endif

#ifdef __linux__
#define OS "linux"
#elif __APPLE__
#define OS "osx"
#elif _WIN32
#define OS "windows"
#endif

#ifndef PACKET_MANAGER
#define PACKET_MANAGER "devel"
#endif

#ifdef __aarch64__
#define "arm64"
#elif __arm__ || _M_ARM || _M_ARMT
#define "arm"
#elif __amd64__ || __amd64 || __x86_64__ || __x86_64 || _M_X64 || _M_AMD64
#define ARCH "x86"
#elif i386 || __i386 || __i386__ || _M_IX86 || _X86_
#define ARCH "i386"
#else
#define ARCH "unknown"
#error("unknown arch")
#endif

#ifndef CONFIG
//#define CONFIG "/etc/conf.d/ssrptd.conf"
#define CONFIG "etc/conf.d/ssrptd.conf"
#endif

#ifndef CONFIG_LEN
#define CONFIG_LEN 64
#endif

#define CONFIG_FILE_NOT_FOUND -2
#define INIT_CONF_FAIL -3
#define COULD_NOT_OPEN_FILE -4
#define COULD_NOT_SET_SOFTWARE_ID -5
#define COULD_NOT_SET_PACKAGE_ID -6

#define ISPC_NUM_CONF 15
#define COM_NUM_CONF 6

#define COM_FILELEN	64

#endif
