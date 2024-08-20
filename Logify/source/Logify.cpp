
#include "Logify/Logify.h"
#include "Logify_internal.h"

std::string Logify::getVersion() {
	return	internal::internalGetVersion();
}