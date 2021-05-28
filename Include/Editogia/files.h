#ifndef _FILES_H_
#define _FILES_H_

#include <vector>
#include <string>
#include <istream>

bool directory_exists(std::string name);

bool create_directory(std::string name);

bool file_exists(std::string name);

std::vector<std::string> files_in(std::string dir, std::string suffix = "");

#endif
