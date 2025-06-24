#include <iostream>
#include <cstdint>
#include <string>
using namespace std;

// Forward declarations
template <size_t N> struct BIN;
template <size_t N1, size_t N2> struct BIN2D;
enum Mode : uint8_t { ENCRYPT, DECRYPT };

// Tables (you need to include these)
uint8_t IP[64] = {
	58, 50, 42, 34, 26, 18, 10, 2, 
	60, 52, 44, 36, 28, 20, 12, 4, 
	62, 54, 46, 38, 30, 22, 14, 6, 
	64, 56, 48, 40, 32, 24, 16, 8, 
	57, 49, 41, 33, 25, 17, 9, 1, 
	59, 51, 43, 35, 27, 19, 11, 3, 
	61, 53, 45, 37, 29, 21, 13, 5, 
	63, 55, 47, 39, 31, 23, 15, 7
};

uint8_t SHIFTS[16] = {
	1, 1, 2, 2, 2, 2, 2, 2,
	1, 2, 2, 2, 2, 2, 2, 1
};

uint8_t PC1[56] = {
	57,49,41,33,25,17,9,
	1,58,50,42,34,26,18,
	10,2,59,51,43,35,27,
	19,11,3,60,52,44,36,
	63,55,47,39,31,23,15,
	7,62,54,46,38,30,22,
	14,6,61,53,45,37,29,
	21,13,5,28,20,12,4
};

uint8_t PC2[48] = {
	14,17,11,24,1,5,
	3,28,15,6,21,10,
	23,19,12,4,26,8,
	16,7,27,20,13,2,
	41,52,31,37,47,55,
	30,40,51,45,33,48,
	44,49,39,56,34,53,
	46,42,50,36,29,32
};

uint8_t E_BIT[48] = {
	32,1,2,3,4,5,
	4,5,6,7,8,9,
	8,9,10,11,12,13,
	12,13,14,15,16,17,
	16,17,18,19,20,21,
	20,21,22,23,24,25,
	24,25,26,27,28,29,
	28,29,30,31,32,1
};

uint8_t PBOX[32] = {
	16, 7, 20, 21, 29, 12, 28, 17,
	1, 15, 23, 26, 5, 18, 31, 10,
	2, 8, 24, 14, 32, 27, 3, 9,
	19, 13, 30, 6, 22, 11, 4, 25
};

// S-box tables (sample - you need to include all 8)
uint8_t SBOX[8][4][16] = {
	// S1
	{
		{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
		{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
	},
	// S2-S8 (add the rest of the S-boxes here)
	// For brevity, I'll just copy S1 - replace with actual S-boxes
	{
		{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
		{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
	},
	{
		{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
		{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
	},
	{
		{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
		{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
	},
	{
		{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
		{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
	},
	{
		{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
		{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
	},
	{
		{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
		{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
	},
	{
		{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
		{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
	}
};

template <size_t N> void binerize(const string& text, bool (&dst)[N]) {
	for(int i = 0; i < 8; i++) for(int j = 0; j < 8; j++) dst[i * 8 + j] = (text[i] >> (7 - j)) & 1;
}

template <size_t N1, size_t N2> constexpr void permute(bool (&src)[N1], uint8_t (&ref)[N2], bool (&dst)[N2]) {
	for(int i= 0; i < N2; i++) dst[i] = src[ref[i] - 1];
}

template <size_t N1, size_t N2> void rotate(bool (&src)[N1], uint8_t (&shifts)[N2], bool (&dst)[N2][N1]) {
	for(int i = 0; i < N1; i++) dst[0][i] = src[(i + shifts[0]) % N1];
	for(int j = 1; j < N2; j++) for(int i = 0; i < N1; i++) dst[j][i] = dst[j-1][(i + shifts[j]) % N1];
}

template <size_t N> constexpr void reallocate(bool (&src)[N], bool (&dst)[N]) {
	for(auto i = 0; i < N; i++) dst[i] = src[i];
}

template <size_t N1, size_t N2> constexpr void reallocate(bool (&dst)[N1][N2], bool (&src)[N1][N2]) {
	for(auto i = 0; i < N1; i++) for(auto j = 0; j < N2; j++) dst[i][j] = src[i][j];
}

template <size_t N> constexpr void split(BIN<N>& src) {
	for(auto i = 0; i < N; i++) {
		if(i < (N / 2)) src.L[i] = src.Arr[i];
		else src.R[i % (N / 2)] = src.Arr[i];
	}
}

template <size_t N1, size_t N2> void split(BIN2D<N1, N2>& src) {
	for(auto i = 0; i < N1; i++)
		for(auto j = 0; j < N2; j++) {
			if(j < (N2 / 2)) src.L[i][j] = src.Arr[i][j];
			else src.R[i][j % (N2 / 2)] = src.Arr[i][j];
		}
}

template <size_t N> void combine(BIN<N>& src) {
	for(auto i = 0; i < N; i++) src.Arr[i] = (i < (N / 2)) ? src.L[i] : src.R[i - (N / 2)];
}

// FIXED: combine function for BIN2D
template <size_t N1, size_t N2> void combine(BIN2D<N1, N2>& src, int round) {
	for(auto j = 0; j < N2; j++) src.Arr[round][j] = (j < (N2 / 2)) ? src.L[round][j] : src.R[round][j - (N2 / 2)];
}

template <size_t N> void sboxSubstitute(bool (&input)[N], bool (&output)[N*2/3]) {
    static_assert(N == 48, "Input harus 48 bits untuk S-box DES");
    for(int i = 0; i < 8; i++) {
        int startIdx = i * 6;
        int row = (input[startIdx] << 1) | input[startIdx + 5];
        int col = (input[startIdx + 1] << 3) | (input[startIdx + 2] << 2) | (input[startIdx + 3] << 1) | input[startIdx + 4];
        uint8_t sboxValue = SBOX[i][row][col];
        int outputStart = i * 4;
        for(int j = 0; j < 4; j++) output[outputStart + j] = (sboxValue >> (3 - j)) & 1;
    }
}

void printSboxDetails(bool (&input)[48], bool (&output)[32]) {
    cout << "\n========= S-BOX SUBSTITUTION =========\n";
    for(int i = 0; i < 8; i++) {
        int startIdx = i * 6;
        cout << "\nS" << (i+1) << " Input:  ";
        for(int j = 0; j < 6; j++) cout << input[startIdx + j];
        int row = (input[startIdx] << 1) | input[startIdx + 5];
        int col = (input[startIdx + 1] << 3) | (input[startIdx + 2] << 2) | (input[startIdx + 3] << 1) | input[startIdx + 4];
        cout << " -> Row: " << row << ", Col: " << col;
        uint8_t sboxValue = SBOX[i][row][col];
        cout << " -> Value: " << (int)sboxValue;
        cout << " -> Output: ";
        int outputStart = i * 4;
        for(int j = 0; j < 4; j++) cout << output[outputStart + j];
    }
    cout << "\n";
}

template <size_t N> constexpr void printBin(bool (&bin)[N]) {
	auto gap = (N % 8 == 0) ? 8 : 7;
	for(auto i = 0; i < N; i++) {
		if(i % gap == 0) cout << "\n";
		cout << bin[i];
	}
	cout << "\n";
}

template <size_t N> constexpr void printBin(bool (&L)[N], bool (&R)[N]) {
	const size_t gap = (N % 8 == 0) ? 8 : 7;
	for (size_t i = 0; i < N; i += gap) {
		for (size_t j = 0; j < gap; j++) cout << L[i + j];
		cout << " | ";
		for (size_t j = 0; j < gap; j++) cout << R[i + j];
		cout << "\n";
	}
}

template <size_t N1, size_t N2> constexpr void printBin(const char* label, bool (&arr)[N1][N2]) {
	cout << label ;
	for(int i = 0; i < N1; i++) {
		cout << "\n--------- ROUND " << i + 1 << " ---------\n";
		for(int j = 0; j < N2; j++) {
			cout << arr[i][j];
			if((j + 1) % 4 == 0) cout << " " ;
		}
		cout << "\n";
	}
}

template <size_t N1, size_t N2> constexpr void printBin(bool (&L)[N1][N2], bool (&R)[N1][N2]) {
	auto gap = (N2 % 8 == 0) ? 8 : 7;
	auto lines = (N2 + gap - 1) / gap;
	for(auto i = 0; i < N1; i++) {
		cout << "\n--------- ROUND " << i + 1 << " ---------\n";
		int start = 0;
		for(int j = 0; j < lines; j++) {
			int end = start + gap;
			if(end > N2) end = N2;
			for(auto k = start; k < end; k++) cout << L[i][k];
			cout << " | ";
			for(auto k = start; k < end; k++) cout << R[i][k];
			cout << "\n";
			start += gap;
		}
	}
}

template <size_t N> struct BIN {
	bool L[N/2];
	bool R[N/2];
	bool Arr[N];

	BIN() = default;
	BIN(bool (&Arr)[N]) {
		reallocate(Arr, this->Arr);
		split(*this);
	}
};

template <size_t N1, size_t N2> struct BIN2D {
	bool L[N1][N2/2];
	bool R[N1][N2/2];
	bool Arr[N1][N2];

	BIN2D() = default;
	BIN2D(bool (&Arr)[N1][N2]) { reallocate(this->Arr, Arr); }
};

template <size_t N1, size_t N2 = N1> struct BINPACK {
	BIN<N1> plain;
	BIN<N1> chiper;
	BIN<N2> key;
};

class DES {
public:
    // Data structures for bits and rounds
    BINPACK<64> bin;
    BINPACK<64,56> permuted;
    BIN2D<16,56> subkeys;
    BIN2D<16,48> permutedKey;
    BIN2D<16,48> ExpandR;
    BIN2D<16,48> XORSubkey;
    BIN2D<16,32> sboxValue;
    BIN2D<16,32> PBoxValue;
    BIN2D<17,32> currentL;
    BIN2D<17,32> currentR;

    // Constructor: initialize and run 16 DES rounds
    DES(const string& text, const string& key, Mode mode) {
        // 1. Binerisasi plaintext/chiper dan key
        binerize(text, mode==ENCRYPT?bin.plain.Arr:bin.chiper.Arr);
        binerize(key, bin.key.Arr);
        split((mode==ENCRYPT)?bin.plain:bin.chiper);
        split(bin.key);

        // 2. Initial Permutation (IP)
        permute(bin.plain.Arr, IP, permuted.plain.Arr);
        split(permuted.plain);

        // 3. Key Permutation PC1 + split
        permute(bin.key.Arr, PC1, permuted.key.Arr);
        split(permuted.key);

        // 4. Rotate subkey halves and combine
        rotate(permuted.key.L, SHIFTS, subkeys.L);
        rotate(permuted.key.R, SHIFTS, subkeys.R);
        for(int i=0;i<16;i++) combine(subkeys, i);

        // 5. Apply PC2 on each round subkey
        for(int i=0;i<16;i++) permute(subkeys.Arr[i], PC2, permutedKey.Arr[i]);
        split(permutedKey);

        // 6. Initialize round 0 L and R from permuted plaintext
        for(int j=0;j<32;j++) {
            currentL.Arr[0][j] = permuted.plain.L[j];
            currentR.Arr[0][j] = permuted.plain.R[j];
        }

        // 7. Perform 16 Feistel rounds
        for(int i=0;i<16;i++) {
            // Expansion E
            permute(currentR.Arr[i], E_BIT, ExpandR.Arr[i]);
            // XOR with round key
            for(int j=0;j<48;j++) XORSubkey.Arr[i][j] = ExpandR.Arr[i][j] ^ permutedKey.Arr[i][j];
            // S-Box substitution
            sboxSubstitute(XORSubkey.Arr[i], sboxValue.Arr[i]);
            // P-Box permutation
            permute(sboxValue.Arr[i], PBOX, PBoxValue.Arr[i]);
            // Backup L[i]
            bool tempL[32];
            for(int j=0;j<32;j++) tempL[j] = currentL.Arr[i][j];
            // Compute next L and R
            for(int j=0;j<32;j++) currentL.Arr[i+1][j] = currentR.Arr[i][j];
            for(int j=0;j<32;j++) currentR.Arr[i+1][j] = tempL[j] ^ PBoxValue.Arr[i][j];
        }
    }

    // Print Expanded R values
    void printExpandedR(int rounds = 16) const {
        cout << "Expanded R";
        for(int i = 0; i < rounds && i < 16; i++) {
            cout << "\n--------- ROUND " << (i+1) << " ---------\n";
            for(int j = 0; j < 48; j++) {
                cout << ExpandR.Arr[i][j];
                if(j < 47) cout << ' ';
            }
            cout << "\n";
        }
    }
    // Debug a specific round: currentR and its expanded bits
    void debugRound(int r) const {
        if(r < 1 || r > 16) return;
        int i = r-1;
        cout << "--- Debug Round " << r << " ---";
        cout << "currentR["<<r<<"]: ";
        for(int j=0;j<32;j++) cout<<currentR.Arr[i][j];
        cout << "ExpandR["<<r<<"]: ";
        for(int j=0;j<48;j++) cout<<ExpandR.Arr[i][j];
        cout << " ";
    }
};

int main() {
	DES des = {"RAFIINDR", "24102004", ENCRYPT};
	cout << "\n---------- PLAIN ----------\n";
	printBin(des.bin.plain.L, des.bin.plain.R);
	cout << "\n----------- KEY -----------\n";
	printBin(des.bin.key.L, des.bin.key.R);
	cout << "\n---- PERMUTED PLAIN IP ----\n";
	printBin(des.permuted.plain.L, des.permuted.plain.R);
	cout << "\n----- PERMUTED KEY PC1 ----\n";
	printBin(des.permuted.key.L, des.permuted.key.R);
	cout << "\n------- ROTATED KEY -------\n";
	printBin(des.subkeys.L, des.subkeys.R);
	cout << "\n----- PERMUTED KEY PC2 ----\n";
	printBin(des.permutedKey.L, des.permutedKey.R);
	des.printExpandedR() ;
	printBin("\n---- XOR WITH SUBKEY ----\n", des.XORSubkey.Arr) ; 
	printBin("\n---- S-BOX OUTPUT ----\n", des.sboxValue.Arr) ; 
	printBin("\n---- P-BOX OUTPUT ----\n", des.PBoxValue.Arr) ;
	printBin("\n---- FINAL L VALUES ----\n", des.currentL.Arr) ;
	printBin("\n---- FINAL R VALUES ----\n", des.currentR.Arr) ;

	return 0;
}