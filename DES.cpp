#include "DES.h"
using namespace std ;

int main() {
	BIN<64> plain ;
	Binerize("RAFIINDR", plain.bin) ;
	printBin("PLAIN", plain) ;

	BIN<64> key ;
	Binerize("24102004", key.bin) ;
	printBin("KEY", key) ;

	BIN<64> plainIP ;
	permute(plain.bin, DES_TABLE::IP, plainIP.bin) ;
	EXTBIN<64> plainIP_ext = plainIP ;
	printBin("IP", plainIP_ext) ;

	BIN<56> keyPC1;
	permute(key.bin, DES_TABLE::PC1, keyPC1.bin) ;
	EXTBIN<56> keyPC1_ext = keyPC1 ;
	printBin("PC 1", keyPC1_ext) ;

	BIN2D<16, 28> subkey_L ;
	rotate(keyPC1_ext.L.bin, subkey_L) ;

	BIN2D<16, 28> subkey_R ;
	rotate(keyPC1_ext.R.bin, subkey_R) ;

	EXTBIN2D<16, 56> subkeys_ext ;
	BIN2D<16, 56> subkeys ;
	BIN2D<16, 48> subkeysPC2 ;
	BIN2D<16, 48> Expanded_R ;
	BIN2D<16, 48> subkeysXOR ;
	BIN2D<16, 32> SBOXS ;
	BIN2D<16, 32> PBOXS ;
	BIN2D<16, 64> XOR ;
	for(auto i = 0; i < 16; i++) {
		cout << "\n====== ROUND " << i << " ======\n" ;
		EXTBIN<56> temp ;
		reallocate(subkey_L.bin[i].bin, temp.L.bin) ;
		reallocate(subkey_R.bin[i].bin, temp.R.bin) ;
		subkeys_ext.bin[i] = temp ;
		printBin("Subkey", subkeys_ext.bin[i]) ;
		BIN<56> tempPC2src(subkeys_ext.bin[i]) ;
		permute(tempPC2src.bin, DES_TABLE::PC2, subkeysPC2.bin[i].bin) ;
		printBin("PC2", subkeysPC2.bin[i]) ;
		permute(plainIP_ext.R.bin, DES_TABLE::EBOX, Expanded_R.bin[i].bin) ;
		printBin("EXPANDED R", Expanded_R.bin[i]) ;
		for(int j = 0; j < 48; j++) subkeysXOR.bin[i].bin[j] = Expanded_R.bin[i].bin[j] ^ subkeysPC2.bin[i].bin[j] ;
		printBin("Subkey XOR", subkeysXOR.bin[i]) ;
		SboxSubtitute(subkeysXOR.bin[i].bin, SBOXS.bin[i].bin) ;
		printBin("SBOX", SBOXS.bin[i]) ;
		permute(SBOXS.bin[i].bin, DES_TABLE::PBOX, PBOXS.bin[i].bin) ;
		printBin("PBOX", PBOXS.bin[i]) ;
		for(int j = 0; j < 64; j++) XOR.bin[i].bin[j] = PBOXS.bin[i].bin[j] ^ (i > 0) ? XOR.bin[i - 1].bin[i] : plainIP.bin[i] ;
		printBin("XOR", XOR.bin[i]) ;
	}
}