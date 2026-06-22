/* MIT License
 *
 * Copyright (c) 2026 Yaron Dayan
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/



#pragma once 
#include <cstdint>

// byteswapper (endianity swap), especially useful for versions older than C++20, where byteswap is unavailable

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