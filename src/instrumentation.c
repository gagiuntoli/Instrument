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

#include "instrumentation.h"


static function_t *fun_array = NULL;


tnode_t *create_time_stamp(void)
{
	tnode_t *tp = malloc(sizeof(tnode_t));

	tp->next = NULL;
	clock_gettime(CLOCK_MONOTONIC, &tp->start);

	return tp;
}


void create_function(int func_id, const char *fname)
{
	if (func_id < 0 || func_id >= MAX_FUNC)
		return;

	fun_array[func_id].name = strdup(fname);
	fun_array[func_id].thead = create_time_stamp();
	fun_array[func_id].ttail = fun_array[func_id].thead;

	return;
}


int instrument_start(int func_id, const char *fname)
{
	if (func_id < 0 || func_id >= MAX_FUNC)
		return -1;

	if (fun_array == NULL) {

		// Only the First time
		fun_array = malloc(MAX_FUNC * sizeof(function_t));
		int i;
		for (i = 0; i < MAX_FUNC; ++i) {
			fun_array[i].name = NULL;
			fun_array[func_id].thead = NULL;
			fun_array[func_id].ttail = NULL;
		}
	}

	if (fun_array[func_id].name != NULL) {

		// The function exist, only add time stamp
		tnode_t *tp = fun_array[func_id].ttail;
		tp->next = create_time_stamp();
		fun_array[func_id].ttail = tp->next;

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

	tnode_t *tp = fun_array[func_id].ttail;
	clock_gettime(CLOCK_MONOTONIC, &tp->end);

	return;
}


double get_total_time(int func_id)
{
	if (func_id < 0 || func_id >= MAX_FUNC)
		return -1;

	double total = 0.0;

	tnode_t *tp = fun_array[func_id].thead;
	while (tp != NULL) {
		total += (double) (tp->end.tv_nsec - tp->start.tv_nsec);
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


double get_standard_deviation(int func_id, double mean)
{
	if (func_id < 0 || func_id >= MAX_FUNC)
		return -1;

	double total = 0.0;

	tnode_t *tp = fun_array[func_id].thead;
	while (tp != NULL) {
		const double dt = ((double)(tp->end.tv_nsec - tp->start.tv_nsec)) * 1.0e-9;
		total += (mean - dt) * (mean - dt);
		tp = tp->next;
	}
	return sqrt(total);
}


void free_tlist(tnode_t *thead)
{
	while (thead != NULL) {
		tnode_t *ptr_aux = thead->next;
		free(thead);
		thead = ptr_aux;
	}
	return;
}


void instrument_print(void)
{
	printf("\n");
	printf("------------------------------------------------------------\n"
	       "Instrument\n"
	       "------------------------------------------------------------\n");
	printf("Function         \tTime\t\tCalls\t\tMean\t\tStdev[\%]\n");

	int id = 0;
	while (fun_array[id].name != NULL) {
		double time = ((double)get_total_time(id)) * 1.0e-9;
		int calls = get_total_calls(id);
		double mean = time / calls;
		double stdev = get_standard_deviation(id, mean);
		printf("%-16s :\t%lf\t%d\t\t%lf\t%lf\n", fun_array[id].name,
		       time, calls, mean, stdev * 100);
		id ++;
	}
	printf("\n");

	// Free all memory
	for (id = 0; id < MAX_FUNC; ++id) {
		free (fun_array[id].name);
		free_tlist(fun_array[id].thead);
	}
	return;
}
