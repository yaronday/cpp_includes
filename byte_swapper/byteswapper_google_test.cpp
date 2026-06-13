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


#pragma warning(push, 0) 
#include <gtest/gtest.h>
#pragma warning(pop)

#include <cstdint>

#include "byteswapper.hpp" 

TEST(ByteSwapperTest, Uint8Identity) {
    EXPECT_EQ(static_cast<uint8_t>(byteswapper(static_cast<uint8_t>(0x00))), 0x00);
    EXPECT_EQ(static_cast<int>(byteswapper(static_cast<uint8_t>(0x12))), 0x12);
    EXPECT_EQ(static_cast<int>(byteswapper(static_cast<uint8_t>(0xFF))), 0xFF);
}

TEST(ByteSwapperTest, Uint16Swap) {
    EXPECT_EQ(byteswapper(static_cast<uint16_t>(0x0000)), 0x0000);
    EXPECT_EQ(byteswapper(static_cast<uint16_t>(0x1234)), 0x3412);
    EXPECT_EQ(byteswapper(static_cast<uint16_t>(0xAABB)), 0xBBAA);
    EXPECT_EQ(byteswapper(static_cast<uint16_t>(0xFFFF)), 0xFFFF);
}

TEST(ByteSwapperTest, Uint32Swap) {
    EXPECT_EQ(byteswapper(static_cast<uint32_t>(0x00000000)), 0x00000000);
    EXPECT_EQ(byteswapper(static_cast<uint32_t>(0x12345678)), 0x78563412);
    EXPECT_EQ(byteswapper(static_cast<uint32_t>(0xAABBCCDD)), 0xDDCCBBAA);
    EXPECT_EQ(byteswapper(static_cast<uint32_t>(0xFFFFFFFF)), 0xFFFFFFFF);
}

TEST(ByteSwapperTest, Uint64Swap) {
    EXPECT_EQ(byteswapper(static_cast<uint64_t>(0x0000000000000000ULL)), 0x0000000000000000ULL);
    EXPECT_EQ(byteswapper(static_cast<uint64_t>(0x123456789ABCDEF0ULL)), 0xF0DEBC9A78563412ULL);
    EXPECT_EQ(byteswapper(static_cast<uint64_t>(0xFFFFFFFFFFFFFFFFULL)), 0xFFFFFFFFFFFFFFFFULL);
}

TEST(ByteSwapperTest, Uint64DoubleSwapIdentity) {
    static constexpr uint64_t original = 0x123456789ABCDEF0ULL;

    // Swapping twice should cycle the bytes back to their exact original positions
    static constexpr uint64_t once = byteswapper(original);
    static constexpr uint64_t twice = byteswapper(once);

    EXPECT_EQ(twice, original);
    EXPECT_NE(once, original); // Sanity check: Ensure the first swap actually changed the value
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}