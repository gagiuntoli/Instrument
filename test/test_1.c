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

#define N 1000000

int func_1(void)
{
	INST_START

	int i, j;
	for (i = 0; i < N; ++i)
		j =+ i;

	INST_END

	return j;
}

int main(void)
{
	int sol;

	sol = func_1();
	sol = func_1();
	sol = func_1();


	INST_PRINT
}
