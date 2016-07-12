/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/files/kernelfile.h"

#include "ek2/util/error.h"

#include <cerrno>

extern "C"
{
#	include <sys/types.h>
#	include <fcntl.h>
#	include <unistd.h>
};

KernelFile::KernelFile(RelativePath&& path)
	: File(std::move(path))
{
}

std::shared_ptr<File> KernelFile::try_construct(RelativePath& path)
{
	char buf[4];
	int fd = path.file_fd(O_RDONLY);

	// TODO: error reporting
	off_t pos = lseek(fd, 0x202, SEEK_SET);
	if (pos == -1)
		throw IOError("Seeking failed on " + path.filename(), errno);
	else if (pos != 0x202) // short file
		return nullptr;

	ssize_t rd = read(fd, buf, sizeof(buf));
	if (rd == -1)
		throw IOError("Reading failed on " + path.filename(), errno);
	else if (rd != sizeof(buf))
		return nullptr;

	if (std::string(buf, sizeof(buf)) != "HdrS")
		return nullptr;

	return std::unique_ptr<File>{static_cast<File*>(
			new KernelFile(std::move(path)))};
}
