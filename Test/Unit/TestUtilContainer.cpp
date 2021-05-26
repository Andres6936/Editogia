// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include "Editogia/Util/Container.hpp"

template<typename Container>
bool equalsContainer(const Container& left, const Container& right)
{
	if (left.size() not_eq right.size())
	{
		return false;
	}

	for(std::size_t index = 0; index < left.size(); ++index)
	{
		if (left[index] not_eq right[index])
		{
			return false;
		}
	}

	return true;
}

TEST_CASE("Verify that the method resetAt set all the values of container to value send for parameter")
{
	std::vector vector {1, 2, 3, 4, 5};
	std::array array{1, 2, 3, 4, 5};

	CHECK(vector.size() == 5);
	CHECK(array.size() == 5);

	CHECK(equalsContainer(vector, {1, 2, 3, 4, 5}));
	CHECK(equalsContainer(array, {1, 2, 3, 4, 5}));

	resetAt(vector, 0);
	resetAt(array, 0);

	CHECK(vector.size() == 5);
	CHECK(array.size() == 5);

	CHECK(equalsContainer(vector, { 0, 0, 0, 0, 0 }));
	CHECK(equalsContainer(array, { 0, 0, 0, 0, 0 }));
}

void add(int& value)
{
	value *= 2;
}

TEST_CASE("Verify the transmutation of container with function add(ref value)")
{
	std::vector vector{ 1, 2, 3, 4, 5 };

	CHECK(vector.size() == 5);
	CHECK(equalsContainer(vector, { 1, 2, 3, 4, 5 }));

	applyAt(vector, add);

	CHECK(vector.size() == 5);
	CHECK(equalsContainer(vector, { 2, 4, 6, 8, 10 }));
}