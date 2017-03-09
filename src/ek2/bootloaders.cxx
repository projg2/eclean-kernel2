/* eclean-kernel2
 * (c) 2017 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/bootloaders.h"

#include "ek2/bootloaders/grub2.h"

#include <cassert>
#include <functional>
#include <memory>
#include <vector>

struct bootloader_info
{
	std::string name;
	std::function<std::unique_ptr<BootLoader>(const Options& opts)> construct;
};

typedef std::vector<bootloader_info> bootloader_list_type;

static const bootloader_list_type bootloader_list = {
	{ "grub2", GRUB2::construct },
};

std::vector<std::unique_ptr<BootLoader>> get_bootloaders(const Options& opts)
{
	std::vector<std::unique_ptr<BootLoader>> ret;

	for (const bootloader_info& bi : bootloader_list)
	{
		std::unique_ptr<BootLoader> b = bi.construct(opts);
		if (b)
			ret.push_back(std::move(b));
	}

	return ret;
}
