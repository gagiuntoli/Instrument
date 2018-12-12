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

#include "instrument.h"


static function_t *fun_array = NULL;
static int fun_total = 0;


tnode_t *create_time_stamp(void)
{
	tnode_t *tp = malloc(sizeof(tnode_t));
	clock_gettime(CLOCK_MONOTONIC, &tp->start);
	tp->next = NULL;

	return tp;
}


int create_function(const char *fname)
{
	if (fun_total >= MAX_FUNC)
		return -1;

	fun_array[fun_total].name = strndup(fname, 64);
	fun_array[fun_total].thead = create_time_stamp();
	fun_array[fun_total].ttail = fun_array[fun_total].thead;

	fun_total ++;

	return fun_total - 1;
}


int get_func_id(const char *fname)
{
	int id;
	for (id = 0; id < fun_total; ++id)
		if (!strncmp(fun_array[id].name, fname, STRING_L * sizeof(char)))
			return id;
	return -1;
}


int instrument_start(const char *fname)
{

	if (fun_array == NULL) {

		// Only the First time
		fun_array = malloc(MAX_FUNC * sizeof(function_t));
		int i;
		for (i = 0; i < MAX_FUNC; ++i) {
			fun_array[i].name = NULL;
			fun_array[i].thead = NULL;
			fun_array[i].ttail = NULL;
		}
	}

	int func_id = get_func_id(fname);

	if (func_id < 0) {

		return create_function(fname);

	} else {

		// The function exist, only add time stamp
		tnode_t *tp = fun_array[func_id].ttail;
		tp->next = create_time_stamp();
		fun_array[func_id].ttail = tp->next;

		return func_id;
	}

	return -1;
}


void instrument_end(int func_id)
{
	if (func_id < 0 || func_id >= fun_total)
		return;

	tnode_t *tp = fun_array[func_id].ttail;
	clock_gettime(CLOCK_MONOTONIC, &tp->end);

	return;
}


double get_total_time(int func_id)
{
	if (func_id < 0 || func_id >= fun_total)
		return -1;

	double total = 0.0;

	tnode_t *tp = fun_array[func_id].thead;
	while (tp != NULL) {
		const double dt = \
				  (tp->end.tv_sec * 1e9 + tp->end.tv_nsec)* 1.0e-9 - \
				  (tp->start.tv_sec * 1e9 + tp->start.tv_nsec)* 1.0e-9 ;
		total += dt;
		tp = tp->next;
	}
	return total;
}


int print_time_list(int func_id)
{
	if (func_id < 0 || func_id >= fun_total)
		return -1;

	printf("func = %s ", fun_array[func_id].name);
	tnode_t *tp = fun_array[func_id].thead;
	while (tp != NULL) {
		printf("start = %lf end = %lf ",
		       (tp->start.tv_sec * 1.0e9 + tp->start.tv_nsec) * 1.0e-9,
		       (tp->end.tv_sec * 1.0e9 + tp->end.tv_nsec) * 1.0e-9);
		tp = tp->next;
	}
	printf("\n");
	return 0;
}


int get_total_calls(int func_id)
{
	if (func_id < 0 || func_id >= fun_total)
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
	if (func_id < 0 || func_id >= fun_total)
		return -1;

	double total = 0.0;

	tnode_t *tp = fun_array[func_id].thead;
	while (tp != NULL) {
		const double dt = (\
				   (tp->end.tv_sec * 1e9 + tp->end.tv_nsec) - \
				   (tp->start.tv_sec * 1e9 + tp->start.tv_nsec)) * 1.0e-9;
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
	printf("Function         \t\tTime\t\tCalls\t\tMean\t\tStdev[\%]\n");

	int id;
	for (id = 0; id < fun_total; ++id) {
		double time = get_total_time(id);
		int calls = get_total_calls(id);
		double mean = time / calls;
		double stdev = get_standard_deviation(id, mean);
		printf("%-24s :\t%lf\t%d\t\t%lf\t%lf\n", fun_array[id].name,
		       time, calls, mean, stdev * 100);
	}
	printf("\n");
//	for (id = 0; id < fun_total; ++id)
//		print_time_list(id);

	// Free all memory
	for (id = 0; id < fun_total; ++id) {
		free(fun_array[id].name);
		free_tlist(fun_array[id].thead);
	}
	free(fun_array);

	return;
}
