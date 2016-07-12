/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/layouts/std.h"

#include "ek2/files.h"
#include "ek2/files/genericfile.h"

#include <iostream>
#include <memory>

std::unique_ptr<Layout> StdLayout::construct()
{
	return std::unique_ptr<Layout>{static_cast<Layout*>(new StdLayout())};
}

// perform case-insensitive comparison of filename and prefix
// we can assume ASCII rules here since prefixes are well-defined
static bool ascii_iequal(char pfx_c, char fn_c)
{
	return fn_c == pfx_c
		|| (pfx_c >= 'a' && pfx_c <= 'z' && fn_c == (pfx_c &~0x20));
}

// check if the filename starts with given prefix, and has at least
// one more character following it (i.e. 'foo-1' matches 'foo-' prefix,
// 'foo-' does not)
static bool has_prefix(const std::string& fn, const std::string& pfx)
{
	return fn.size() > pfx.size() && std::equal(pfx.begin(), pfx.end(),
			fn.begin(), ascii_iequal);
}

// lowercase supported filename prefixes
static std::array<std::string, 4> supported_prefixes{
	"config-",
	"system.map-",
	"initramfs-",
	"initrd-",
};

static std::string find_version(const std::string& fn)
{
	std::string::size_type sep_pos = fn.find('-');

	if (sep_pos != fn.npos)
		return fn.substr(sep_pos+1);
	return {};
}

bool StdLayout::find_kernels()
{
	const std::string boot_path{"/boot"};

	dir_.open(boot_path);
	while (dir_.read())
	{
		// skip ., .. and all hidden files
		if (dir_.filename()[0] == '.')
			continue;
		// skip directories, symlinks etc.
		if (!dir_.is_regular_file())
			continue;

		std::shared_ptr<File> f;
		f = get_file_by_magic(dir_.relative_path());

		// no good magic? try the filename
		if (!f)
		{
			std::string fn{dir_.filename()};

			for (const std::string& pfx : supported_prefixes)
			{
				if (has_prefix(fn, pfx))
				{
					f = std::shared_ptr<File>(static_cast<File*>(
							new GenericFile(dir_.relative_path())));
					break;
				}
			}
		}

		if (f)
		{
			// apparent version from the filename
			std::string apparent_ver{find_version(dir_.filename())};

			if (apparent_ver.empty())
				continue; // TODO

			file_map_[apparent_ver].files().push_back(std::move(f));
		}
	}
	dir_.close();

	for (const std::pair<std::string, FileSet>& kf : file_map_)
	{
		std::cerr << kf.first << ":\n";
		for (const std::shared_ptr<File>& f : kf.second.files())
			std::cerr << "- " << f->filename() << "\n";
	}

	return true;
}
