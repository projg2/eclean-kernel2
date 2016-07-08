/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/util/error.h"

#include <cerrno>
#include <cstring>

Error::Error(std::string&& msg)
	: msg_(std::move(msg))
{
}

const std::string& Error::what() const
{
	return msg_;
}

IOError::IOError(std::string&& msg, int errno_val)
	: Error(std::move(msg) + ": " + strerror(errno_val)), err_(errno_val)
{
}

int IOError::err() const
{
	return err_;
}
