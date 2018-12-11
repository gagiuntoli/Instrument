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

#include "instrumentation.h"


func_list_t func_list = { 0, NULL, NULL };


void take_time_1(int func_id, const char *fname)
{
	fnode_t *f_ptr;

	if (func_id >= func_list.nfunc) {

		// add a new node in the tail
		func_list.tail = malloc(sizeof(fnode_t));

		f_ptr = func_list.tail;

		f_ptr->next = NULL;
		if (func_list.nfunc == 0)
			func_list.head = func_list.tail;

		func_list.nfunc ++;

		// Catch the initial function time
		f_ptr->thead = malloc(sizeof(tnode_t));
		f_ptr->ttail = f_ptr->thead;
		tnode_t *t_ptr = f_ptr->ttail;
		t_ptr->time_s = clock();

		return;
	}

	int id;
	f_ptr = func_list.head;

	for (id = 0; id < func_id; ++id)
		f_ptr = f_ptr->next;


	return;
}

void take_time_2(int func_id)
{
	return;
}
