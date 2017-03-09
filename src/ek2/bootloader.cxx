/* eclean-kernel2
 * (c) 2017 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/bootloader.h"

BootLoader::BootLoader(const Options& opts)
	: opts_(opts)
{
}

void BootLoader::postrm()
{
}
