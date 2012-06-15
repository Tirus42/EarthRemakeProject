#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <windows.h>
#include <string>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>

unsigned char FileType(const std::string& filename);

long long FileSize(const std::string& filename);

#endif // NETWORK_H_INCLUDED
