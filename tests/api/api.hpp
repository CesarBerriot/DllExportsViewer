//
// Created by César B. on 4/8/2024.
//

#pragma once

/**
 * @brief ends the test with a result determined from the thrown exception
 * @details
 * performs a try-catch statement around try_code, logs some debug info and validates
 * the test depending on whether the thrown error code equals expected_error_code.
 * the test will be considered as failed if no exception is thrown
 */
#define test_expect_exception(try_code, expected_error_code) \
	do	\
	{	try	\
		{	try_code;	\
		}	\
		catch(utils::exception exception)	\
		{	std::cout << "exception from " #try_code " : \n" << exception.what() << '\n';	\
			exit(exception.error_code == expected_error_code ? EXIT_SUCCESS : EXIT_FAILURE);	\
		}	\
		std::cout << "no exception from " #try_code;	\
		exit(EXIT_FAILURE);	\
	} while(false)
