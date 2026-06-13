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

#include <cstdint>

#include "byteswapper.hpp"

// --- Unit Tests for byteswapper (endianity swap)---

// 8-bit tests
static_assert(byteswapper(static_cast<uint8_t>(0x00)) == 0x00, "8-bit zero failed");
static_assert(byteswapper(static_cast<uint8_t>(0x12)) == 0x12, "8-bit identity failed");
static_assert(byteswapper(static_cast<uint8_t>(0xFF)) == 0xFF, "8-bit max failed");

// 16-bit tests
static_assert(byteswapper(static_cast<uint16_t>(0x0000)) == 0x0000, "16-bit zero failed");
static_assert(byteswapper(static_cast<uint16_t>(0x1234)) == 0x3412, "16-bit standard failed");
static_assert(byteswapper(static_cast<uint16_t>(0xFFFF)) == 0xFFFF, "16-bit max failed");

// 32-bit tests
static_assert(byteswapper(static_cast<uint32_t>(0x00000000)) == 0x00000000, "32-bit zero failed");
static_assert(byteswapper(static_cast<uint32_t>(0x12345678)) == 0x78563412, "32-bit standard failed");
static_assert(byteswapper(static_cast<uint32_t>(0xFFFFFFFF)) == 0xFFFFFFFF, "32-bit max failed");

// 64-bit tests
static_assert(byteswapper(static_cast<uint64_t>(0x0000000000000000ULL)) == 0x0000000000000000ULL, "64-bit zero failed");
static_assert(byteswapper(static_cast<uint64_t>(0x123456789ABCDEF0ULL)) == 0xF0DEBC9A78563412ULL, "64-bit standard failed");
static_assert(byteswapper(static_cast<uint64_t>(0xFFFFFFFFFFFFFFFFULL)) == 0xFFFFFFFFFFFFFFFFULL, "64-bit max failed");

int main() {
    return 0;
}