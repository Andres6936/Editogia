#include "Editogia/IO/File/files.h"
#include "Editogia/Render/View/window.h" // For debugmsg()
#include "Editogia/Render/View/help.h"
#include "Editogia/globals.h"

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
