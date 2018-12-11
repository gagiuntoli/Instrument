
set(CMAKE_BUILD_TYPE Debug)

# The test files I care only (create a list)
set(testsources
	test_1.c
	test_2.c
	test_3.c
	test_clock_vs_clock_gettime.c
	)

# Iterate over the list above
foreach (testfile ${testsources})
	# Delete File extensions (test_i.cpp -> test_i)
	get_filename_component(testname ${testfile} NAME_WE)

	# Create the executable test_i and link.
	add_executable(${testname} ${testfile})
	target_link_libraries(${testname} instrument)
endforeach ()

# Add a test that requires arguments
add_test(NAME test_1 COMMAND test_1)
add_test(NAME test_2 COMMAND test_2)
add_test(NAME test_3 COMMAND test_3)
add_test(NAME test_clock_vs_clock_gettime COMMAND test_clock_vs_clock_gettime)
