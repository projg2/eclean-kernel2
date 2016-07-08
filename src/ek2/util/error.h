/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_UTIL_ERROR_H
#define EK2_UTIL_ERROR_H 1

#include <string>

// A plain error class with string-typed message
class Error
{
	std::string msg_;

public:
	// construct using given message
	Error(std::string&& msg);

	// get the error message
	const std::string& what() const;
};

class IOError : public Error
{
	int err_;

public:
	IOError(std::string&& msg, int errno_val);

	int err() const;
};

#endif /*EK2_UTIL_ERROR_H*/
