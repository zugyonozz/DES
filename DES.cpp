#include "DES.h"
using namespace std;

// Function untuk DES Encryption
BIN<64> des_encrypt(const BIN<64>& plaintext, const BIN<64>& key) {
    // Initial Permutation
    BIN<64> plainIP;
    permute(plaintext.bin, DES_TABLE::IP, plainIP.bin);
    EXTBIN<64> plainIP_ext = plainIP;
	printBin("IP", plainIP_ext) ;
    
    // Key Processing
    BIN<56> keyPC1;
    permute(key.bin, DES_TABLE::PC1, keyPC1.bin);
    EXTBIN<56> keyPC1_ext = keyPC1;
	printBin("PC 1", plainIP_ext) ;
    
    // Generate all subkeys
    BIN2D<16, 28> subkey_L, subkey_R;
    rotate(keyPC1_ext.L.bin, subkey_L);
    rotate(keyPC1_ext.R.bin, subkey_R);
	for(int i = 0; i < 16; i++) {
		cout << "\n---------------------------- ROUND " << i << " ----------------------------\n" ;
		EXTBIN<56> temp ;
		reallocate(subkey_L.bin[i].bin, temp.L.bin) ;
		reallocate(subkey_R.bin[i].bin, temp.R.bin) ;
		printBin("SHIFTED KEY", temp) ;
	}
    
    BIN2D<16, 48> subkeysPC2;
    for(int i = 0; i < 16; i++) {
		cout << "\n---------------------------- ROUND " << i << " ----------------------------\n" ;
        EXTBIN<56> temp;
        reallocate(subkey_L.bin[i].bin, temp.L.bin);
        reallocate(subkey_R.bin[i].bin, temp.R.bin);
        BIN<56> tempPC2src(temp);
        permute(tempPC2src.bin, DES_TABLE::PC2, subkeysPC2.bin[i].bin);
		printBin("PC 2", subkeysPC2.bin[i]) ;
    }
    
    // 16 rounds of Feistel
    BIN<32> current_L, current_R;
    reallocate(plainIP_ext.L.bin, current_L.bin);
    reallocate(plainIP_ext.R.bin, current_R.bin);
    
    for(int i = 0; i < 16; i++) {
		cout << "\n---------------------------- ROUND " << i << " ----------------------------\n" ;
        BIN<48> expanded_R;
        permute(current_R.bin, DES_TABLE::EBOX, expanded_R.bin);
		printBin("EXPANDED R", expanded_R) ;
        
        // XOR with subkey
        BIN<48> xor_result;
        for(int j = 0; j < 48; j++) {
            xor_result.bin[j] = expanded_R.bin[j] ^ subkeysPC2.bin[i].bin[j];
        }
		printBin("XOR Subkey", xor_result) ;
        
        // S-box substitution
        BIN<32> sbox_output;
        SboxSubtitute(xor_result.bin, sbox_output.bin);
		printBin("S BOX", sbox_output) ;
        
        // P-box permutation
        BIN<32> pbox_output;
        permute(sbox_output.bin, DES_TABLE::PBOX, pbox_output.bin);
		printBin("P BOX", pbox_output) ;
        
        // Feistel function
        BIN<32> new_L, new_R;
        reallocate(current_R.bin, new_L.bin); // L[i+1] = R[i]
		printBin("L[i+1] = R[i]", new_L) ;
        
        // R[i+1] = L[i] XOR f(R[i], K[i])
        for(int j = 0; j < 32; j++) {
            new_R.bin[j] = current_L.bin[j] ^ pbox_output.bin[j];
        }
		printBin("R[i+1] = L[i] XOR f(R[i], K[i])", new_R) ;
        
        // Update for next round
        reallocate(new_L.bin, current_L.bin);
        reallocate(new_R.bin, current_R.bin);
    }
    
    // Final swap and IP inverse
    BIN<64> final_LR;
    // Swap L and R before final permutation
    for(int i = 0; i < 32; i++) {
        final_LR.bin[i] = current_R.bin[i];        // R goes to left
        final_LR.bin[i + 32] = current_L.bin[i];   // L goes to right
    }
    
    BIN<64> ciphertext;
    permute(final_LR.bin, DES_TABLE::IP_INV, ciphertext.bin);
    
    return ciphertext;
}

// Function untuk DES Decryption
BIN<64> des_decrypt(const BIN<64>& ciphertext, const BIN<64>& key) {
    // Initial Permutation (sama seperti encrypt)
    BIN<64> cipherIP;
    permute(ciphertext.bin, DES_TABLE::IP, cipherIP.bin);
    EXTBIN<64> cipherIP_ext = cipherIP;
	printBin("IP", cipherIP_ext) ;
    
    // Key Processing (sama seperti encrypt)
    BIN<56> keyPC1;
    permute(key.bin, DES_TABLE::PC1, keyPC1.bin);
    EXTBIN<56> keyPC1_ext = keyPC1;
	printBin("PC 1", keyPC1_ext) ;
    
    // Generate all subkeys (sama seperti encrypt)
    BIN2D<16, 28> subkey_L, subkey_R;
    rotate(keyPC1_ext.L.bin, subkey_L);
    rotate(keyPC1_ext.R.bin, subkey_R);
	for(int i = 0; i < 16; i++) {
		cout << "\n---------------------------- ROUND " << i << " ----------------------------\n" ;
		EXTBIN<56> temp ;
		reallocate(subkey_L.bin[i].bin, temp.L.bin) ;
		reallocate(subkey_R.bin[i].bin, temp.R.bin) ;
		printBin("SHIFTED KEY", temp) ;
	}
    
    BIN2D<16, 48> subkeysPC2;
    for(int i = 0; i < 16; i++) {
		cout << "\n---------------------------- ROUND " << i << " ----------------------------\n" ;
        EXTBIN<56> temp;
        reallocate(subkey_L.bin[i].bin, temp.L.bin);
        reallocate(subkey_R.bin[i].bin, temp.R.bin);
        BIN<56> tempPC2src(temp);
        permute(tempPC2src.bin, DES_TABLE::PC2, subkeysPC2.bin[i].bin);
		printBin("PC 2", subkeysPC2.bin[i]) ;
    }
    
    // 16 rounds of Feistel - KUNCI: Gunakan subkey dalam urutan TERBALIK
    BIN<32> current_L, current_R;
    reallocate(cipherIP_ext.L.bin, current_L.bin);
    reallocate(cipherIP_ext.R.bin, current_R.bin);
    
    for(int i = 15; i >= 0; i--) { // PENTING: Loop mundur dari round 15 ke 0
		cout << "\n---------------------------- ROUND " << i << " ----------------------------\n" ;
        BIN<48> expanded_R;
        permute(current_R.bin, DES_TABLE::EBOX, expanded_R.bin);
		printBin("EXPANDED R", expanded_R) ;
        
        // XOR with subkey (urutan terbalik)
        BIN<48> xor_result;
        for(int j = 0; j < 48; j++) {
            xor_result.bin[j] = expanded_R.bin[j] ^ subkeysPC2.bin[i].bin[j];
        }
		printBin("XOR Subkey", xor_result) ;
        
        // S-box substitution (sama)
        BIN<32> sbox_output;
        SboxSubtitute(xor_result.bin, sbox_output.bin);
		printBin("S BOX", sbox_output) ;
        
        // P-box permutation (sama)
        BIN<32> pbox_output;
        permute(sbox_output.bin, DES_TABLE::PBOX, pbox_output.bin);
		printBin("P BOX", pbox_output) ;
        
        // Feistel function (sama)
        BIN<32> new_L, new_R;
        reallocate(current_R.bin, new_L.bin); // L[i+1] = R[i]
		printBin("L[i+1] = R[i]", new_L) ;
        
        // R[i+1] = L[i] XOR f(R[i], K[i])
        for(int j = 0; j < 32; j++) {
            new_R.bin[j] = current_L.bin[j] ^ pbox_output.bin[j];
        }
		printBin("R[i+1] = L[i] XOR f(R[i], K[i])", new_R) ;
        
        // Update for next round
        reallocate(new_L.bin, current_L.bin);
        reallocate(new_R.bin, current_R.bin);
    }
    
    // Final swap and IP inverse (sama seperti encrypt)
    BIN<64> final_LR;
    // Swap L and R before final permutation
    for(int i = 0; i < 32; i++) {
        final_LR.bin[i] = current_R.bin[i];        // R goes to left
        final_LR.bin[i + 32] = current_L.bin[i];   // L goes to right
    }
    
    BIN<64> plaintext;
    permute(final_LR.bin, DES_TABLE::IP_INV, plaintext.bin);
    
    return plaintext;
}

// Main function untuk test encrypt dan decrypt
int main() {
    // Input data
    BIN<64> plain;
    Binerize("RAFIINDR", plain.bin);
    printBin("ORIGINAL PLAIN", plain);
    
    BIN<64> key;
    Binerize("24102004", key.bin);
    printBin("KEY", key);
    
    // Encrypt
    cout << "\n========== ENCRYPTION ==========\n";
    BIN<64> ciphertext = des_encrypt(plain, key);
    printBin("CIPHERTEXT", ciphertext);
    
    // Convert to string and show hex
    string cipher_str;
    unbinerize(ciphertext.bin, cipher_str);
	cout << "Eecrypted text: " << cipher_str << endl;
    cout << "Ciphertext in HEX: ";
    showHex(cipher_str);
    
    // // Decrypt
    // cout << "\n========== DECRYPTION ==========\n";
    // BIN<64> decrypted = des_decrypt(ciphertext, key);
    // printBin("DECRYPTED", decrypted);
    
    // // Convert back to string
    // string decrypted_str;
    // unbinerize(decrypted.bin, decrypted_str);
    // cout << "Decrypted text: " << decrypted_str << endl;
    
    // // Verification
    // cout << "\n========== VERIFICATION ==========\n";
    // bool is_same = true;
    // for(int i = 0; i < 64; i++) {
    //     if(plain.bin[i] != decrypted.bin[i]) {
    //         is_same = false;
    //         break;
    //     }
    // }
    
    // cout << "Encryption/Decryption " << (is_same ? "SUCCESS" : "FAILED") << "!" << endl;
    
    return 0;
}