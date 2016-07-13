/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/layouts/std.h"

#include "ek2/files/genericfile.h"
#include "ek2/files/kernelfile.h"
#include "ek2/files/modulesdir.h"
#include "ek2/util/error.h"

#include <cassert>
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
	const std::string modules_path{"/lib/modules"};

	std::unordered_map<std::string, FileSet> file_map;
	typedef std::unordered_map<std::string, std::shared_ptr<File>>
		module_map_type;
	module_map_type module_map;

	// collect all moduledirs first
	modules_dir_.open(modules_path);
	while (modules_dir_.read())
	{
		// skip ., .. and all hidden files
		if (modules_dir_.filename()[0] == '.')
			continue;
		// skip non-directories and symlinks
		if (!modules_dir_.is_regular_directory())
			continue;

		RelativePath rpath{modules_dir_.relative_path()};
		std::shared_ptr<File> f;
		f = ModulesDir::try_construct(rpath);

		if (f)
			module_map[f->filename()] = f;
	}
	modules_dir_.close();

	// collect all kernel files from /boot
	boot_dir_.open(boot_path);
	while (boot_dir_.read())
	{
		// skip ., .. and all hidden files
		if (boot_dir_.filename()[0] == '.')
			continue;
		// skip directories, symlinks etc.
		if (!boot_dir_.is_regular_file())
			continue;

		RelativePath rpath{boot_dir_.relative_path()};
		std::shared_ptr<File> f;
		f = KernelFile::try_construct(rpath);

		// no good magic? try the filename
		if (!f)
		{
			std::string fn{boot_dir_.filename()};

			for (const std::string& pfx : supported_prefixes)
			{
				if (has_prefix(fn, pfx))
				{
					f = GenericFile::try_construct(rpath);
					break;
				}
			}
		}

		if (f)
		{
			// apparent version from the filename
			std::string apparent_ver{find_version(boot_dir_.filename())};
			const std::string& internal_ver{f->version()};

			if (apparent_ver.empty())
				continue; // TODO

			FileSet& fset = file_map[apparent_ver];
			fset.files().push_back(std::move(f));

			// if we're on a kernel, we should have internal version
			if (!internal_ver.empty())
			{
				// if this is a first file with internal version...
				if (fset.internal_version().empty())
				{
					// set the internal version on FileSet
					fset.internal_version(internal_ver);

					// associate the module dir
					module_map_type::iterator mi
						= module_map.find(internal_ver);
					if (mi != module_map.end())
						fset.files().push_back(mi->second);
				}
				// otherwise, check if it matches the previous one
				else if (fset.internal_version() != internal_ver)
				{
					// no match? search for the previous source
					// (for more informative output)
					for (const std::shared_ptr<File>& f : fset.files())
					{
						if (!f->version().empty())
							throw Error("Found two files with the same apparent version ("
									+ apparent_ver + ") yet different internal version: "
									+ f->filename() + " [" + fset.internal_version() + "] "
									+ " and " + fset.files().back()->filename() + " ["
									+ internal_ver + "]");
					}
					assert(0 && "Internal version in FileSet but no File has internal version");
				}
			}
		}
	}
	boot_dir_.close();

	// post-process collected /boot FileSets and move them to the final list
	kernels_.clear();
	for (std::pair<const std::string, FileSet>& kf : file_map)
	{
		// set apparent version here, we do not need it earlier
		// and it needs to be done only once
		assert(!kf.first.empty());
		kf.second.apparent_version(kf.first);

		// leave only unused moduledirs in modules map
		// others should have been copied into kernels already
		module_map.erase(kf.second.internal_version());

		// finally, move the FileSet to kernels
		kernels_.push_back(std::move(kf.second));
	}

	// add orphaned moduledirs to the final list
	for (std::pair<const std::string, std::shared_ptr<File>>& km : module_map)
	{
		FileSet module_set;
		module_set.internal_version(km.first);
		module_set.files().push_back(std::move(km.second));
		kernels_.push_back(std::move(module_set));
	}

	return true;
}

std::vector<FileSet>& StdLayout::kernels()
{
	return kernels_;
}

const std::vector<FileSet>& StdLayout::kernels() const
{
	return kernels_;
}
