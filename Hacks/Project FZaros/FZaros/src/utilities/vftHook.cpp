#include <memory>
#include "vftHook.hpp"

CVFTHook::CVFTHook(uintptr_t **base) {
	this->initialize(base);
}

void CVFTHook::initialize(uintptr_t **base) {
	this->base = base;
	this->oldVft = *this->base;

	size_t vftSize = 0;
	while (this->oldVft[vftSize++]) {}

	this->newVft = new uintptr_t[vftSize];
	memcpy_s(this->newVft, vftSize * sizeof uintptr_t, this->oldVft, vftSize * sizeof uintptr_t);

	*this->base = this->newVft;
}

void CVFTHook::release() {
	*this->base = this->oldVft;
}