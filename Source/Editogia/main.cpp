#include "Editogia/Entity/Org/player_city.h"
#include "Editogia/Entity/Org/kingdom.h"
#include "Editogia/Render/View/window.h"
#include "Editogia/Scene/SceneManager.hpp"
#include "Editogia/game.h"
#include "Editogia/init.h"
#include "Editogia/globals.h"

#include <Levin/Levin.hpp>

int main()
{
	// Configure the logger
	Levin::Log::SetNewLogger(std::make_unique<Levin::FileLogger>("Editogia.log"));

	// seed the RNG.
	//srand(time(NULL));

	// Init the game data!
	if (!init_all_data())
	{
		Levin::Log::Emergency("Failed to init data!");
		return 1;
	}

	if (!set_directories())
	{
		Levin::Log::Emergency("Failed to set directories!");
		return 1;
	}

	// Set up a game.
	GAME = new Game;

	SceneManager interface{};

	while (interface.isRunning())
	{
		interface.draw();
		interface.update();
		interface.handle();
	}

	endwin();
	return 0;
}

