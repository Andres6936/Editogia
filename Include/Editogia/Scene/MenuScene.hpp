// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_MENUSCENE_HPP
#define EDITOGIA_MENUSCENE_HPP

#include "Editogia/Scene/IScene.hpp"

namespace Editogia
{

	class MenuScene : public IScene
	{

	public:

		void draw() override;

		void update() override;

		TypeScene handle() override;

	};

}

#endif //EDITOGIA_MENUSCENE_HPP
