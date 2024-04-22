//
// Created by César B. on 4/8/2024.
//

#include <iostream>
#include "tests/api/api.hpp"
#include "utils/utils.hpp"
#include "utils/exceptions.hpp"

int main()
{	test_expect_exception(utils::load_dll("path/to/some/directory"), utils::load_dll_errors::invalid_file_extension);
}
