// created by zugyonozz
#pragma once
#include <iostream>
#include <cstdint>

using Uint8 = uint8_t;

namespace DES_TABLE {
// IP Table
constexpr int IP[64] = {
    58, 50, 42, 34, 26, 18, 10, 2, 
    60, 52, 44, 36, 28, 20, 12, 4, 
    62, 54, 46, 38, 30, 22, 14, 6, 
    64, 56, 48, 40, 32, 24, 16, 8, 
    57, 49, 41, 33, 25, 17, 9, 1, 
    59, 51, 43, 35, 27, 19, 11, 3, 
    61, 53, 45, 37, 29, 21, 13, 5, 
    63, 55, 47, 39, 31, 23, 15, 7
};

// Inverse IP Table
constexpr int IP_INV[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

// PC 1 Table
constexpr int PC1[56] = {
    57,49,41,33,25,17,9, 
    1,58,50,42,34,26,18, 
    10,2,59,51,43,35,27, 
    19,11,3,60,52,44,36,         
    63,55,47,39,31,23,15, 
    7,62,54,46,38,30,22, 
    14,6,61,53,45,37,29, 
    21,13,5,28,20,12,4 
};

// PC 2 Table
constexpr int PC2[48] = { 
    14,17,11,24,1,5, 
    3,28,15,6,21,10, 
    23,19,12,4,26,8, 
    16,7,27,20,13,2, 
    41,52,31,37,47,55, 
    30,40,51,45,33,48, 
    44,49,39,56,34,53, 
    46,42,50,36,29,32 
}; 

// SHIFTS Table
constexpr int SHIFTS[16] = {
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1
};

// EBOX Table
constexpr int EBOX[48] = {
    32,1,2,3,4,5,
    4,5,6,7,8,9,
    8,9,10,11,12,13,
    12,13,14,15,16,17,
    16,17,18,19,20,21,
    20,21,22,23,24,25,
    24,25,26,27,28,29,
    28,29,30,31,32,1
};

// PBOX Table
constexpr int PBOX[32] = {
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
};

// Correct S-box tables
constexpr int SBOX_1[4][16] = {
    {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
    {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
    {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
    {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
};

constexpr int SBOX_2[4][16] = {
    {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
    {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
    {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
    {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
};

constexpr int SBOX_3[4][16] = {
    {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
    {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
    {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
    {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
};

constexpr int SBOX_4[4][16] = {
    {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
    {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
    {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
    {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
};

constexpr int SBOX_5[4][16] = {
    {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
    {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
    {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
    {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
};

constexpr int SBOX_6[4][16] = {
    {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
    {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
    {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
    {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
};

constexpr int SBOX_7[4][16] = {
    {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
    {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
    {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
    {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
};

constexpr int SBOX_8[4][16] = {
    {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
    {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
    {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
    {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
};

// SBOXES array
constexpr const int (*SBOXES[8])[4][16] = { 
    &SBOX_1, &SBOX_2, &SBOX_3, &SBOX_4, 
    &SBOX_5, &SBOX_6, &SBOX_7, &SBOX_8 
};
}

// Forward declarations
template <size_t N> struct BIN;
template <size_t N> struct EXTBIN;
template <size_t COL, size_t ROW> struct BIN2D;
template <size_t COL, size_t ROW> struct EXTBIN2D;

// Convert string to binary
template <size_t N> constexpr void Binerize(const std::string& text, bool (&dst)[N]) {
    for(decltype(N) i = 0; i < 8 && i < text.length(); i++) {
        for(int j = 0; j < 8; j++) {
            dst[i * 8 + j] = (text[i] >> (7 - j)) & 1;
        }
    }
}
template <size_t N> constexpr void unbinerize(const bool(&src)[N], std::string& dst) {
    dst.clear();
    for(size_t byte_idx = 0; byte_idx < N / 8; byte_idx++) {
        Uint8 character = 0;
        for(int bit_idx = 0; bit_idx < 8; bit_idx++) if(src[byte_idx * 8 + bit_idx]) character |= (1 << (7 - bit_idx));
        dst += char(character);
    }
}

inline void showHex(const std::string& dst) {
    for(char c : dst) printf("%02x ", (unsigned char)c);
    printf("\n") ;
}

// Permute binary with table
template <size_t N1, size_t N2> constexpr void permute(const bool (&src)[N1], const int (&ref)[N2], bool (&dst)[N2]) {
    for(size_t i = 0; i < N2; i++) {
        dst[i] = src[ref[i] - 1];
    }
}

// Rotate binary based on SHIFTS table (fixed cumulative rotation)
template <size_t N1, size_t N2> constexpr void rotate(bool (&src)[N1], BIN2D<N2, N1>& dst) {
    // First round rotation
    for (size_t i = 0; i < N1; ++i) {
        dst.bin[0].bin[i] = src[(i + DES_TABLE::SHIFTS[0]) % N1];
    }
    
    // Subsequent rounds - cumulative rotation
    for (size_t j = 1; j < N2; ++j) {
        for (size_t i = 0; i < N1; ++i) {
            dst.bin[j].bin[i] = dst.bin[j-1].bin[(i + DES_TABLE::SHIFTS[j]) % N1];
        }
    }
}

// Reallocate binary array
template <size_t N> constexpr void reallocate(const bool (&src)[N], bool (&dst)[N]) {
    for(size_t i = 0; i < N; i++) {
        dst[i] = src[i];
    }
}

// Split binary to extended binary
template <size_t N> constexpr void split(const BIN<N>& src, EXTBIN<N>& dst) {
    static_assert(N % 2 == 0, "N must be even");
    for(size_t i = 0; i < N; i++) {
        if(i < (N / 2)) {
            dst.L.bin[i] = src.bin[i];
        } else {
            dst.R.bin[i % (N / 2)] = src.bin[i];
        }
    }
}

// Combine extended binary to binary
template <size_t N> void combine(const EXTBIN<N>& src, BIN<N>& dst) {
    for(size_t i = 0; i < N; i++) {
        dst.bin[i] = (i < (N / 2)) ? src.L.bin[i] : src.R.bin[i - (N / 2)];
    }
}

// S-box substitution
template <size_t N = 48> constexpr void SboxSubtitute(bool (&src)[N], bool (&dst)[N*2/3]) {
    static_assert(N == 48, "Input must be 48 bits for DES S-box");
    for(size_t i = 0; i < 8; i++) {
        int id = i * 6;
        int row = (src[id] << 1) | src[id + 5];
        int col = (src[id + 1] << 3) | (src[id + 2] << 2) | (src[id + 3] << 1) | src[id + 4];
        Uint8 sbox_v = (*DES_TABLE::SBOXES[i])[row][col];
        int outputStart = i * 4;
        for(int j = 0; j < 4; j++) {
            dst[outputStart + j] = (sbox_v >> (3 - j)) & 1;
        }
    }
}

// Print binary functions
template <size_t N> inline void printBin(const std::string& label, BIN<N> bin) noexcept {
    auto gap = N / 8;
    std::cout << "\n------ " << label << " ------\n";
    for(size_t i = 0; i < N; i++) {
        if(i % gap == 0 && i != 0) std::cout << " ";
        std::cout << bin.bin[i];
    }
    std::cout << "\n";
}

template <size_t N> inline void printBin(const std::string& label, EXTBIN<N> bin) {
    std::cout << "\n------ " << label << " ------\n";
    printBin("L", bin.L);
    printBin("R", bin.R);
}

// Binary structures
template <size_t N> struct BIN { 
    bool bin[N]; 
    BIN() = default;
    BIN(const EXTBIN<N>& src) { combine(src, *this); }
    BIN(const BIN<N>& other) { reallocate(other.bin, this->bin); }
    operator bool*() { return this->bin; }
};

template <size_t N> struct EXTBIN { 
    BIN<N/2> L;
    BIN<N/2> R; 
    EXTBIN() = default;
    EXTBIN(const BIN<N>& src) { split(src, *this); }
    EXTBIN(const EXTBIN<N>& other) {
        reallocate(other.L.bin, this->L.bin);
        reallocate(other.R.bin, this->R.bin);
    }
};

template <size_t COL, size_t ROW> struct BIN2D { 
    BIN<ROW> bin[COL]; 
};

template <size_t COL, size_t ROW> struct EXTBIN2D { 
    EXTBIN<ROW> bin[COL]; 
};