# Instrument

This is an C instrumentation library to profile and measure performance of C codes.

# Usage

The usage is very simple. The example shows the basic use:

```c
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
```

The macros `INST_START` and `INST_END` should be placed at the beginning and end of the 
function where the elapsed time want to be measured and `INST_PRINT` should be located 
where the information is going to be printed to `stdout`.

After compiling and run the code the output should be approximately:

```bash
Function        Time    Calls
func_1 :        15265   3
```

More simple and easy to understand examples of the usage of this library can be found in the `test` folder.
