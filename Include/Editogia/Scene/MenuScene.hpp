// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_MENUSCENE_HPP
#define EDITOGIA_MENUSCENE_HPP

#include <Cuss/Interface.hpp>
#include <Editogia/Render/View/window.h>
#include "Editogia/Scene/IScene.hpp"

namespace Editogia
{

	class MenuScene : public IScene
	{

	private:

		cuss::interface i_start;

		Window w_start{ 0, 0, 80, 24 };

	public:

		MenuScene();

		void draw() override;

		void update() override;

		TypeScene handle() override;

	};

}

#endif //EDITOGIA_MENUSCENE_HPP
