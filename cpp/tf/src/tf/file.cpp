#include "tf/file.h"

#include <sys/stat.h>

unsigned char FileType(const std::string& filename){
	struct stat Status;
	int Dateityp;
	stat(filename.data(), &Status);
	Dateityp = Status.st_mode & S_IFMT;
	switch (Dateityp) {
		case S_IFREG: return 1; break;
		case S_IFDIR: return 2; break;
		default: return 0;
   }
}

long long FileSize(const std::string& filename){
	struct stat Status;
	stat(filename.data(), &Status);
	return Status.st_size;
}
