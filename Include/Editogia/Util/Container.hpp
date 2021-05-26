// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_CONTAINER_HPP
#define EDITOGIA_CONTAINER_HPP

#include <algorithm>
#include <type_traits>

/**
 * Wrapper around of method that set all the values of a Container
 * to the value send for parameter.
 *
 * @tparam Container The parameter must be of meet the requirements
 * of Container, it is: A Container is an object used to store other
 * objects and taking care of the management of the memory used by
 * the objects it contains.
 *
 * @tparam Type The type of value that Container stored.
 *
 * @param container The container that store the values, the
 * container must meet the requirement of Iterable.
 *
 * @param value The value used to reset the container.
 */
template<typename Container, typename Type>
void resetAt(Container& container, const Type& value)
{
	// Verify that the type elements that stored the Container is the
	// same type of value that trying assign.
	static_assert(std::is_same<typename Container::value_type, Type>::value,
			"The value of parameter not is same type that stored the Container");
	std::fill(container.begin(), container.end(), value);
}

template<typename Container, typename Function>
void applyAt(Container& container, Function&& function)
{
	for (auto& item : container)
	{
		std::invoke(std::forward<Function>(function), item);
	}
}

#endif //EDITOGIA_CONTAINER_HPP
