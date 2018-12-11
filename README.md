# Instrument

This is an C instrumentation library to profile and measure performance of C codes.
The implementation is based on linked list to store the profile of every function
 that needs to be instrumented. 


# Usage

The usage is very simple. It is needed first to include the header `instrument.h` in the program
to profile and then the macros `INST_START` and `INST_END` should be placed at the beginning and
 end of the function where the elapsed time want to be measured, i.e.: 

```c
int func_1(void)
{
	INST_START

	int i, j;
	for (i = 0; i < N; ++i)
		j =+ i;

	INST_END

	return j;
}
```


More simple and easy to understand examples of the usage of this library can be found in the `test` folder.

