/*
 *  This source code is part of the Instrument library.
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
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#define MAX_FUNC 15 // Maximum number of functions to instrument
#define STRING_L 64 // Max String Length 

#define INST_START int magic_1945 = instrument_start(__FUNCTION__);
#define INST_END   instrument_end(magic_1945);
#define INST_PRINT instrument_print();


struct tnode_t_ {

	struct tnode_t_ *next;
	struct timespec start;
	struct timespec end;

};

typedef struct tnode_t_ tnode_t;


typedef struct {

	char *name;
	tnode_t *thead;
	tnode_t *ttail;

} function_t;


int instrument_start(const char *fname);
void instrument_end(int func_id);
void instrument_print(void);

tnode_t *create_time_stamp(void);
int create_function(const char *fname);

void free_tlist(tnode_t *thead);

double get_total_time(int func_id);
int get_total_calls(int func_id);
double get_standard_deviation(int func_id, double mean);


#endif
