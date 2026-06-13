#include <iostream>

static inline uint16_t byteswap16(uint16_t v) {
	return static_cast<uint16_t>((v << 8) | (v >> 8));
}

// using explicit specialization 
template<typename T>
static inline T byteswapper(T v) {
	return static_cast<T>(v);
}

template <>
static inline uint8_t byteswapper(uint8_t v) {
	return v;
}

template <>
static inline uint16_t byteswapper<uint16_t>(uint16_t v) {
	return byteswap16(v);
}

template <>
static inline uint32_t byteswapper<uint32_t>(uint32_t v) {
	uint32_t lo = byteswapper(static_cast<uint16_t>(v));
	uint32_t hi = byteswapper(static_cast<uint16_t>(v >> 16));
	return (lo << 16) | hi;
}

template <>
static inline uint64_t byteswapper<uint64_t>(uint64_t v) {
	uint64_t lo = byteswapper(static_cast<uint32_t>(v));
	uint64_t hi = byteswapper(static_cast<uint32_t>(v >> 32));
	return (lo << 32) | hi;
}