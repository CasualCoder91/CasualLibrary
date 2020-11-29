#include "Address.hpp"

Address::operator uintptr_t(void) const noexcept
{
	return ptr;
}

Address::operator void* (void) const noexcept
{
	return reinterpret_cast<void*>(ptr);
}

bool Address::isValid(void) const noexcept
{
	return reinterpret_cast<void*>(ptr) != nullptr;
}

Address Address::addOffset(const uint32_t offset) noexcept
{
	ptr += offset;
	return *this;
}
