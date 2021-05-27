#include "Editogia/player_city.h"
#include "Editogia/window.h"
#include "Editogia/interface.h"
#include "Editogia/game.h"
#include "Editogia/init.h"
#include "Editogia/kingdom.h"  // For init_kingdoms()
#include "Editogia/globals.h"

#include <Levin/Levin.hpp>

int main()
{
	// Configure the logger
	Levin::Log::SetNewLogger(std::make_unique<Levin::FileLogger>("Editogia.log"));

	// seed the RNG.
	//srand(time(NULL));

	// Start up our display (e.g. ncurses)
	init_display();

	// Init the game data!
	if (!init_all_data())
	{
		debugmsg("Failed to init data!");
		return 1;
	}

	if (!set_directories())
	{
		debugmsg("Failed to set directories!");
		return 1;
	}

	// Set up a game.
	GAME = new Game;

	Interface interface;
	interface.init();

	if (interface.starting_screen())
	{
		interface.main_loop();
	}

	endwin();
	return 0;
}

