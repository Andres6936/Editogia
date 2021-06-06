// Joan Andrés (@Andres6936) Github.

#include <fstream>
#include "Editogia/game.h"
#include "Editogia/globals.h"
#include "Editogia/Math/rng.h"
#include "Editogia/Gea/world.h"
#include "Editogia/Scene/MenuScene.hpp"

using namespace Editogia;

MenuScene::MenuScene()
{
	if (!i_start.load_from_file("cuss/start.cuss"))
	{

	}

	std::ifstream fin;
	std::string motd;
	std::vector<std::string> art;

	fin.open("motd.txt");
	if (fin.is_open())
	{
		std::string tmp;
		while (!fin.eof())
		{
			getline(fin, tmp);
			if (tmp.empty())
			{
				motd += "\n";
			}
			else
			{
				motd += tmp;
			}
		}
		fin.close();
	}
	i_start.set_data("text_motd", motd);

	fin.open("art.txt");
	if (fin.is_open())
	{
		std::string tmpart, tmpline;
		while (!fin.eof())
		{
			getline(fin, tmpline);
			if (!tmpline.empty() && tmpline[0] == '#' &&
				(tmpline.length() == 1 || tmpline[1] == ' '))
			{
				continue; // It's a comment
			}
			else if (tmpline.length() == 1 && tmpline[0] == '%')
			{
				art.push_back(tmpart);
				tmpart = "";
			}
			else
			{
				tmpart += tmpline;
				tmpart += "\n";
				tmpline = "";
			}
		}
		if (!tmpart.empty())
		{
			tmpart = tmpart.substr(0, tmpart.length() - 1); // Strip off \n
			art.push_back(tmpart);
		}
		fin.close();
	}

	if (!art.empty())
	{
		i_start.set_data("text_art", art[rng(0, art.size() - 1)]);
	}

	if (GAME->load_world())
	{
		i_start.set_data("text_world_name", GAME->world->get_name());
		i_start.set_data("text_world_name", c_yellow);
	}
	else
	{
		i_start.set_data("text_world_name", "<c=ltred>No world exists!  "
											"Press <c=pink>G<c=ltred> to generate one.<c=/>");
	}
}

void MenuScene::draw()
{
	i_start.draw(&w_start);
	w_start.refresh();
}

void MenuScene::update()
{

}

TypeScene MenuScene::handle()
{
	const KeyCode key = w_start.getKeyEvent().getKeyCode();

	if (key == KeyCode::H)
	{
		return TypeScene::Help;
	}
	else if (key == KeyCode::L)
	{
		return TypeScene::Load;
	}
	else if (key == KeyCode::N)
	{
		return TypeScene::New;
	}
	else if (key == KeyCode::G)
	{
		return TypeScene::CreateWorld;
	}
	else if (key == KeyCode::Q)
	{
		return TypeScene::Exit;
	}

	return TypeScene::None;
}
