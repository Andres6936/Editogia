// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_BINDING_HPP
#define EDITOGIA_BINDING_HPP

#include "Cuss/ActionID.hpp"
#include <string>

namespace cuss
{
	struct binding
	{
		action_id act;
		std::string target;
		int a, b;

		binding(action_id ACT = ACT_NULL, std::string T = "", int A = 0, int B = 0) :
				act(ACT), target(T), a(A), b(B)
		{
		};

		std::string save_data();

		void load_data(std::istream& datastream);
	};
}

#endif //EDITOGIA_BINDING_HPP
