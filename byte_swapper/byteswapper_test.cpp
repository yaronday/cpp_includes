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