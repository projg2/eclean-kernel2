/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/files/kernelfile.h"

extern "C"
{
#	include <fcntl.h>
#	include <unistd.h>
};

KernelFile::KernelFile(RelativePath&& path)
	: File(std::move(path))
{
}

std::shared_ptr<File> KernelFile::construct(RelativePath& path)
{
	char buf[4];
	int fd = path.file_fd(O_RDONLY);

	// TODO: error reporting
	if (lseek(fd, 0x202, SEEK_SET) != 0x202)
		return nullptr;
	if (read(fd, buf, sizeof(buf)) != sizeof(buf))
		return nullptr;
	if (std::string(buf, sizeof(buf)) != "HdrS")
		return nullptr;

	return std::unique_ptr<File>{static_cast<File*>(
			new KernelFile(std::move(path)))};
}
