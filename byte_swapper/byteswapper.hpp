#pragma once 
#include <cstdint>


static constexpr uint8_t byteswapper(uint8_t v) {
	return v;
}

static constexpr uint16_t byteswapper(uint16_t v) {
	return static_cast<uint16_t>((v << 8) | (v >> 8));
}

static constexpr uint32_t byteswapper(uint32_t v) {
	uint32_t lo = byteswapper(static_cast<uint16_t>(v));
	uint32_t hi = byteswapper(static_cast<uint16_t>(v >> 16));
	return (lo << 16) | hi;
}

static constexpr uint64_t byteswapper(uint64_t v) {
	uint64_t lo = byteswapper(static_cast<uint32_t>(v));
	uint64_t hi = byteswapper(static_cast<uint32_t>(v >> 32));
	return (lo << 32) | hi;
}