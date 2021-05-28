// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_ISCENE_HPP
#define EDITOGIA_ISCENE_HPP

#include "Editogia/Scene/TypeScene.hpp"

namespace Editogia
{

	class IScene
	{

	public:

		virtual void draw() = 0;

		virtual void update() = 0;

		virtual TypeScene handle() = 0;

	};

}

#endif //EDITOGIA_ISCENE_HPP
