/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/files/kernelfile.h"

#include "ek2/util/error.h"

#include <algorithm>
#include <array>
#include <cerrno>
#include <cstdint>

extern "C"
{
#	include <sys/types.h>
#	include <fcntl.h>
#	include <unistd.h>
};

KernelFile::KernelFile(RelativePath&& path, std::string&& version)
	: File(std::move(path)), version_(std::move(version))
{
}

const std::string& KernelFile::version() const
{
	return version_;
}

std::shared_ptr<File> KernelFile::try_construct(RelativePath& path)
{
	const std::string kernel_magic{"HdrS"};
	std::string version;

	std::array<char, 0x100> buf;
	int fd = path.file_fd(O_RDONLY);

	// TODO: error reporting
	off_t pos = lseek(fd, 0x200, SEEK_SET);
	if (pos == -1)
		throw IOError("Seeking failed on " + path.filename(), errno);
	else if (pos != 0x200) // short file
		return nullptr;

	ssize_t rd = read(fd, buf.data(), 0x10);
	if (rd == -1)
		throw IOError("Reading failed on " + path.filename(), errno);
	else if (rd != 0x10)
		return nullptr;

	if (!std::equal(kernel_magic.begin(), kernel_magic.end(), buf.begin() + 2))
		return nullptr;

	uint16_t offset = buf[0x0f] << 8 | buf[0x0e];
	if (offset != 0)
	{
		offset += 0x200;
		pos = lseek(fd, offset, SEEK_SET);
		if (pos == -1)
			throw IOError("Seeking failed on " + path.filename(), errno);
		else if (pos != offset) // erraneous data?
			return nullptr;

		while (true)
		{
			rd = read(fd, buf.data(), buf.size());
			if (rd == -1)
				throw IOError("Reading failed on " + path.filename(), errno);
			if (rd == 0)
				return nullptr;

			// split on first space
			char* del = std::find(buf.begin(), buf.begin() + rd, ' ');
			// no space? let's copy all and try next read
			if (del == buf.begin() + rd)
				version.append(buf.data(), rd);
			else
			{
				version.append(buf.begin(), del);
				break;
			}
		}
	}

	return std::unique_ptr<File>{static_cast<File*>(
			new KernelFile(std::move(path), std::move(version)))};
}
