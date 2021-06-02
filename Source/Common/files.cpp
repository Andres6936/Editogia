#include "Editogia/IO/File/files.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>

bool directory_exists(std::string name)
{
	DIR* dir;
	dir = opendir(name.c_str());
	if (dir)
	{
		closedir(dir);
		return true;
	}
	return false;
}

bool create_directory(std::string name)
{
	if (directory_exists(name))
	{
		return false; // Couldn't create it because it exists...
	}
#if (defined _WIN32 || defined __WIN32__)
	mkdir(name.c_str());
#else
	mkdir(name.c_str(), 0777);
#endif
	if (!directory_exists(name))
	{  // Check to make sure we succeeded
		return false;
	}
	return true;
}

bool file_exists(std::string name)
{
	std::ifstream fin;
	fin.open(name.c_str());
	if (fin.is_open())
	{
		fin.close();
		return true;
	}
	return false;
}

std::vector<std::string> files_in(std::string dir, std::string suffix)
{
	std::vector<std::string> ret;
	DIR* dp;
	dirent* dirp;

	if ((dp = opendir(dir.c_str())) == NULL)
		return ret;

	while ((dirp = readdir(dp)) != NULL)
	{
		std::string filename(dirp->d_name);
		if (suffix == "" || filename.find(suffix) != std::string::npos)
		{
			ret.push_back(std::string(dirp->d_name));
		}
	}

	closedir(dp);
	return ret;
}

