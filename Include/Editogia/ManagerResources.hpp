// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_MANAGERRESOURCES_HPP
#define EDITOGIA_MANAGERRESOURCES_HPP

#include <map>
#include <cstdint>

#include "Editogia/resource.h"

namespace Editogia
{

	class ManagerResources : private std::map<Resource, std::uint32_t>
	{

	public:

		ManagerResources();

		void setAt(std::int32_t value, Resource type) noexcept;

	};

}

#endif //EDITOGIA_MANAGERRESOURCES_HPP
