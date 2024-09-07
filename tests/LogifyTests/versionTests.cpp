#include <catch2/catch.hpp>
#include <Logify/Logify.h>


TEST_CASE("Logify Version", "[getVersion]") {
	REQUIRE(Logify::getVersion() == "0.1.2");
}