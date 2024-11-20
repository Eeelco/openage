// Copyright 2013-2024 the openage authors. See copying.md for legal info.

#include "file.h"

#include <cstdio>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <utility>

#include "error/error.h"
#include "log/log.h"

#include "util/filelike/native.h"
#include "util/filelike/python.h"
#include "util/fslike/directory.h"
#include "util/path.h"
#include "util/strings.h"


namespace openage::util {


File::File() = default;


// yes. i'm sorry. but cython can't enum class yet.
File::File(const std::string &path, int mode) :
	File{path, static_cast<mode_t>(mode)} {}


File::File(const std::string &path, mode_t mode) {
	this->filelike = std::make_shared<filelike::Native>(path, mode);
}


File::File(std::shared_ptr<filelike::FileLike> filelike) :
	filelike{filelike} {}


File::File(const py::Obj &filelike) {
	this->filelike = std::make_shared<filelike::Python>(filelike);
}


std::string File::read(ssize_t max) {
	return this->filelike->read(max);
}


size_t File::read_to(void *buf, ssize_t max) {
	return this->filelike->read_to(buf, max);
}


bool File::readable() {
	return this->filelike->readable();
}


void File::write(const std::string &data) {
	this->filelike->write(data);
}


bool File::writable() {
	return this->filelike->writable();
}


void File::seek(ssize_t offset, seek_t how) {
	this->filelike->seek(offset, how);
}


bool File::seekable() {
	return this->filelike->seekable();
}


size_t File::tell() {
	return this->filelike->tell();
}


void File::close() {
	this->filelike->close();
}


void File::flush() {
	this->filelike->flush();
}


ssize_t File::size() {
	return this->filelike->get_size();
}


std::vector<std::string> File::get_lines() {
	// TODO: relay the get_lines to the underlaying filelike
	//       which may do a better job in getting the lines.
	//       instead, we read everything and then split up into lines.
	std::vector<std::string> result = util::split_newline(this->read());

	return result;
}


std::shared_ptr<filelike::FileLike> File::get_fileobj() const {
	return this->filelike;
}


std::ostream &operator<<(std::ostream &stream, const File &file) {
	stream << "File(";
	file.filelike->repr(stream);
	stream << ")";

	return stream;
}

File File::get_temp_file(bool executable) {
	fslike::Directory temp_dir = fslike::Directory::get_temp_directory();
	std::string file_name = std::tmpnam(nullptr);
	std::ostringstream dir_path;
	temp_dir.repr(dir_path);

	if (executable) {
		// 0755 == rwxr-xr-x
		File file_wrapper = File(dir_path.str() + file_name, 0755);
		return file_wrapper;
	}

	// 0644 == rw-r--r--
	File file_wrapper = File(dir_path.str() + file_name, 0644);
	return file_wrapper;
}

} // namespace openage::util
