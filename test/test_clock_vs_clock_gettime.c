/*
 *  This source code is part of Instrument library.
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
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#define N 100000

int func_1(void)
{
	INST_START

	int i, k, j = 0;
	for (k = 0; k < 100; ++k) {
		for (i = 0; i < N; ++i)
			j += i;
		for (i = 0; i < N; ++i)
			j -= i;
	}

	INST_END

	return j;
}


int func_2(void)
{
	INST_START

	int i, j = 0;
	for (i = 0; i < N; ++i)
		j += i;
	for (i = 0; i < N; ++i)
		j -= i;

	INST_END

	return j;
}


int main(void)
{
	INST_START

	int sol;
	clock_t start1, end1;
	struct timespec start2, end2;

	start1 = clock();
	clock_gettime(CLOCK_MONOTONIC, &start2);

	sol = func_1();
	sol = func_2();
	sol = func_1();
	sol = func_2();
	sol = func_2();
	sol = func_2();

	int calls_func_1 = get_total_calls(0);
	assert(calls_func_1 == 2);

	int calls_func_2 = get_total_calls(1);
	assert(calls_func_2 == 4);

	INST_END

	int calls_main = get_total_calls(2);
	assert(calls_main == 1);

	end1 = clock();
	clock_gettime(CLOCK_MONOTONIC, &end2);

	printf("\n");
	printf("Time with clock()         = %lf Seg\n", (double) (end1 - start1) / ((double) CLOCKS_PER_SEC));
	printf("Time with clock_gettime() = %lf Seg\n", (end2.tv_nsec - start2.tv_nsec) * 1.0e-9);
	printf("\n");

	INST_PRINT
}
