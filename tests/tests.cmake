macro(make_test NAME WILL_FAIL)
    add_executable(test.unit.${NAME} ${SOURCES} ${COMMON_TESTS_SOURCES} tests/${NAME}.cpp)
    add_test(unit.${NAME} test.unit.${NAME})
    set_tests_properties(unit.${NAME} PROPERTIES WILL_FAIL ${WILL_FAIL})
endmacro()

make_test(utils.load_dll.correct_usage OFF)
make_test(utils.load_dll.empty_path OFF)
make_test(utils.load_dll.not_a_file OFF)
make_test(utils.load_dll.missing_dll_file OFF)
make_test(utils.win32_strerror OFF)