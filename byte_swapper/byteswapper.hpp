/*
 * Copyright (C) 2026 Yaron Dayan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Attribution Notice:
 * The original author, Yaron Dayan, must be credited in all derivative works.
 */

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