/*
 *  This source code is part of MicroC: a finite element code
 *  to solve microstructural problems for composite materials.
 *
 *  Copyright (C) - 2018 - Guido Giuntoli <gagiuntoli@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef INSTRUMENTATION_H
#define INSTRUMENTATION_H


#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


#define INST_START take_time_1(__COUNTER__, __FUNCTION__);
#define INST_END   take_time_2(__COUNTER__);


struct tnode_t_ {

	struct tnode_t_ *next;
	clock_t time_s;
	clock_t dtime;

};

typedef struct tnode_t_ tnode_t;


struct fnode_t_ {

	char name[128];
	struct fnode_t_ *next;

	tnode_t *thead;
	tnode_t *ttail;

};

typedef struct fnode_t_ fnode_t;


typedef struct {

	int nfunc;
	fnode_t *head;
	fnode_t *tail;

} func_list_t;


void take_time_1(int func_id, const char *fname);
void take_time_2(int func_id);

#endif
