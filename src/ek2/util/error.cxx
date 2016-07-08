/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/util/error.h"

Error::Error(std::string&& msg)
	: msg_(std::move(msg))
{
}

const std::string& Error::what() const
{
	return msg_;
}
