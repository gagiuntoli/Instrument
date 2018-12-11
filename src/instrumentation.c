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


fnode_t *fhead = NULL;


tnode_t *create_time_stamp(void)
{
	tnode_t *tp = malloc(sizeof(tnode_t));

	tp->next = NULL;
	tp->tstart = clock();
	tp->dtime = -1;

	return tp;
}


fnode_t *create_function(int func_id, const char *fname)
{
	fnode_t *fp = malloc(sizeof(fnode_t));

	strcpy(fp->name, fname);
	fp->id = func_id;
	fp->next = NULL;

	// Create first element in the time list
	fp->thead = create_time_stamp();

	return fp;
}


int instrument_start(int func_id, const char *fname)
{
	if (fhead == NULL) {

		// Create and initialize list
		fhead = create_function(func_id, fname);
		return func_id;
	}

	fnode_t *fp = fhead;
	while (fp->next != NULL && fp->id != func_id)
		fp = fp->next;

	if (fp->id == func_id) {

		// The function exist, only add time stamp
		tnode_t *tp = fp->thead;
		while (tp->next != NULL)
			tp = tp->next;
		// Add element in the time list
		tp->next = create_time_stamp();
		return func_id;

	} else {

		// The function doesn't exist, create and add to list
		fp->next = create_function(func_id, fname);
		return func_id;
	}

	return -1;
}


void instrument_end(int func_id)
{
	// Search for <func_id>
	fnode_t *fp = fhead;
	while (fp->next != NULL && fp->id != func_id)
		fp = fp->next;

	if (fp == NULL) {

		return;

	} else {

		tnode_t *tp = fp->thead;
		while (tp->next != NULL)
			tp = tp->next;

		tp->dtime = clock() - tp->tstart;
	}

	return;
}


clock_t get_total_time(fnode_t *fp)
{
	clock_t total = 0;

	tnode_t *tp = fp->thead;
	while (tp != NULL) {
		total += tp->dtime;
		tp = tp->next;
	}
	return total;
}


int get_total_calls(fnode_t *fp)
{
	int calls = 0;

	tnode_t *tp = fp->thead;
	while (tp != NULL) {
		calls ++;
		tp = tp->next;
	}
	return calls;
}

void instrument_print(void)
{
	fnode_t *fp = fhead;
	printf("Function\tTime\tCalls\n");
	while (fp != NULL) {
		clock_t time = get_total_time(fp);
		int calls = get_total_calls(fp);
		printf("%s :\t%d\t%d\n", fp->name, time, calls);

		fp = fp->next;
	}
	return;
}
