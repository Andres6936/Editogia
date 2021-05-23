#include "Editogia/globals.h"
#include "Editogia/files.h"
#include "Editogia/help.h"
#include "Editogia/window.h" // For debugmsg()

Game* GAME;

Help_database* HELP;

std::string SAVE_DIR;

bool set_directories()
{
	SAVE_DIR = "save/";
	if (!directory_exists(SAVE_DIR) && !create_directory(SAVE_DIR))
	{
		debugmsg("Unable to create directory '%s'.", SAVE_DIR.c_str());
		return false;
	}
	return true;
}
