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
#include <stdint.h>
#include <stdlib.h>


#define N 100000

uint64_t func_1(void)
{
	int i;
	int *v1 = malloc(N * sizeof(int));
	int *v2 = malloc(N * sizeof(int));
	for (i = 0; i < N; ++i) {
		v1[i] = i;
		v2[i] = i;
	}
	int *v3 = malloc(N * sizeof(int));

	for (i = 0; i < N; ++i)
		v3[i] = v1[i] + v2[i];

	uint64_t sum = 0;
	for (i = 0; i < N; ++i)
		sum += v3[i];

	free(v1);
	free(v2);
	free(v3);
	return sum;
}

int main(void)
{
	INST_START

	uint64_t sol = func_1();

	INST_END
}
