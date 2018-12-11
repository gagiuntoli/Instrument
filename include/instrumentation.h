/*
 *  This source code is part Instrument
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
#include <stdio.h>


#define INST_START int magic_1945 = instrument_start(__COUNTER__, __FUNCTION__);
#define INST_END   instrument_end(magic_1945);
#define INST_PRINT instrument_print();


struct tnode_t_ {

	struct tnode_t_ *next;
	clock_t tstart;
	clock_t dtime;

};

typedef struct tnode_t_ tnode_t;


struct fnode_t_ {

	char name[128];
	int id;
	struct fnode_t_ *next;
	tnode_t *thead;
};

typedef struct fnode_t_ fnode_t;


int instrument_start(int func_id, const char *fname);
void instrument_end(int func_id);
void instrument_print(void);

#endif
