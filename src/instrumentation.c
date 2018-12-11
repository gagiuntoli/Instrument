/*
 *  This source code is part of Instrument
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

#include "instrumentation.h"


function_t *fun_array = NULL;


tnode_t *create_time_stamp(void)
{
	tnode_t *tp = malloc(sizeof(tnode_t));

	tp->next = NULL;
	tp->tstart = clock();
	tp->dtime = -1;

	return tp;
}


void create_function(int func_id, const char *fname)
{
	if (func_id < 0 || func_id >= MAX_FUNC)
		return;

	fun_array[func_id].name = strdup(fname);
	fun_array[func_id].thead = create_time_stamp();

	return;
}


int instrument_start(int func_id, const char *fname)
{
	if (func_id < 0 || func_id >= MAX_FUNC)
		return -1;

	if (fun_array == NULL) {
		fun_array = malloc(MAX_FUNC * sizeof(function_t));
		int i;
		for (i = 0; i < MAX_FUNC; ++i) {
			fun_array[i].name = NULL;
			fun_array[func_id].thead = NULL;
		}
	}

	if (fun_array[func_id].name != NULL) {

		// The function exist, only add time stamp
		tnode_t *tp = fun_array[func_id].thead;
		while (tp->next != NULL)
			tp = tp->next;
		// Add element in the time list
		tp->next = create_time_stamp();
		return func_id;

	} else {

		// Create new function
		create_function(func_id, fname);
		return func_id;
	}

	return -1;
}


void instrument_end(int func_id)
{
	if (func_id < 0 || func_id >= MAX_FUNC)
		return;

	tnode_t *tp = fun_array[func_id].thead;
	while (tp->next != NULL)
		tp = tp->next;

	tp->dtime = clock() - tp->tstart;

	return;
}


clock_t get_total_time(int func_id)
{
	if (func_id < 0 || func_id >= MAX_FUNC)
		return -1;

	clock_t total = 0;

	tnode_t *tp = fun_array[func_id].thead;
	while (tp != NULL) {
		total += tp->dtime;
		tp = tp->next;
	}
	return total;
}


int get_total_calls(int func_id)
{
	if (func_id < 0 || func_id >= MAX_FUNC)
		return -1;

	int calls = 0;

	tnode_t *tp = fun_array[func_id].thead;
	while (tp != NULL) {
		calls ++;
		tp = tp->next;
	}
	return calls;
}


void instrument_print(void)
{
	printf("Function         \tTime\tCalls\tMean\n");

	int id = 0;
	while (fun_array[id].name != NULL) {
		clock_t time = get_total_time(id);
		int calls = get_total_calls(id);
		printf("%-16s :\t%d\t%d\n", fun_array[id].name, time, calls);
		id ++;
	}
	return;
}
