// ECE 2305 - Programming Project 09 - Substitution Cipher

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

// Function to read text from a file
string readText(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Error: Cannot open " << filename << endl;
        return "";
    }
    string text, line;
    while (getline(file, line)) {
        for (char& c : line) c = tolower(c); // Convert to lowercase
        text += line + '\n'; // Keep line breaks
    }
    file.close();
    return text;
}

// Function to write text to a file
void writeText(const string& filename, const string& text) {
    ofstream file(filename);
    file << text;
    file.close();
}

// Create lookup table for encoding based on offset and spacing
void createLookupTable(int code[26], int offset, int spacing) {
    for (int n = 0; n < 26; ++n) {
        code[n] = (offset + spacing * n) % 26;
    }
}

// Hardcoded decoding lookup table for codemessage1.txt
void getLookupForMessage1(char table[26]) {
    string decode = "yculsopxrtbmv dgril newfhij"; // SOLVED decoding table
    for (int i = 0; i < 26; ++i) {
        table[i] = decode[i];
    }
}

// Hardcoded decoding lookup table for codemessage2.txt
void getLookupForMessage2(char table[26]) {
    string decode = "wldncytbe?emgkrsouv?oah?"; // previously solved decoding
    for (int i = 0; i < 26; ++i) {
        table[i] = decode[i];
    }
}

// Encode text using lookup table
string encodeText(const string& text, int code[26]) {
    string result;
    for (char ch : text) {
        if (islower(ch)) {
            int letterValue = ch - 'a';
            int codedValue = code[letterValue];
            result += char(codedValue + 'a');
        }
        else {
            result += ch;
        }
    }
    return result;
}

// Decode text using lookup table
string decodeText(const string& text, char table[26]) {
    string result;
    for (char ch : text) {
        if (islower(ch)) {
            result += table[ch - 'a'];
        }
        else {
            result += ch;
        }
    }
    return result;
}

int main() {
    cout << "Substitution Cipher Program\n";
    cout << "Choose an option:\n";
    cout << "1. Encode message.txt\n";
    cout << "2. Decode codemessage1.txt\n";
    cout << "3. Decode codemessage2.txt\n";
    cout << "Enter your choice (1, 2, or 3): ";

    int choice;
    cin >> choice;

    string inputFile, outputFile;

    if (choice == 1) {
        inputFile = "message.txt";
        outputFile = "encoded_message.txt";
    }
    else if (choice == 2) {
        inputFile = "codemessage1.txt";
        outputFile = "decoded1.txt";
    }
    else if (choice == 3) {
        inputFile = "codemessage2.txt";
        outputFile = "decoded2.txt";
    }
    else {
        cout << "Invalid choice.\n";
        return 1;
    }

    string message = readText(inputFile);
    if (message.empty()) return 1;

    string result;

    if (choice == 1) {
        int length = message.length();
        int offset = length % 25 + 1;
        int spacing = (length % 6) * 2 + 1;

        cout << "Encoding with offset = " << offset << ", spacing = " << spacing << endl;

        int code[26];
        createLookupTable(code, offset, spacing);

        result = encodeText(message, code);
    }
    else if (choice == 2) {
        char decodeTable[26];
        getLookupForMessage1(decodeTable);
        result = decodeText(message, decodeTable);
    }
    else if (choice == 3) {
        char decodeTable[26];
        getLookupForMessage2(decodeTable);
        result = decodeText(message, decodeTable);
    }

    writeText(outputFile, result);
    cout << "✓ " << outputFile << " created.\n";

    system("pause");
    return 0;
}
