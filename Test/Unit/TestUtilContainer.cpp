// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include "Editogia/Util/Container.hpp"

TEST_CASE("Verify that all the properties are set to false for the construct for default")
{
	std::vector vector {1, 2, 3, 4, 5};
	std::array array{1, 2, 3, 4, 5};

	CHECK(vector.size() == 5);
	CHECK(array.size() == 5);

	CHECK(vector.at(4) == 5);
	CHECK(array.at(4) == 5);
}

