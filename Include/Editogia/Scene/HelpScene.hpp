// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_HELPSCENE_HPP
#define EDITOGIA_HELPSCENE_HPP

#include "Editogia/Scene/IScene.hpp"

namespace Editogia
{

	class HelpScene : public IScene
	{

	public:

		void draw() override;

		void update() override;

		TypeScene handle() override;

	};

}

#endif //EDITOGIA_HELPSCENE_HPP
