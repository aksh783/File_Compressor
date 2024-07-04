
#include <bits/stdc++.h> 
using namespace std; 
#include <iostream>
#include <fstream>
#include <bitset>

struct MinHeapNode { 
    char data; 
    unsigned freq; 
    MinHeapNode *left, *right; 
    MinHeapNode(char data, unsigned freq) { 
        left = right = NULL; 
        this->data = data; 
        this->freq = freq; 
    } 
}; 

struct compare { 
    bool operator()(MinHeapNode* a, MinHeapNode* b) { 
        return (a->freq > b->freq); 
    } 
}; 

void encodeHuffman(struct MinHeapNode* root, string code, unordered_map<char, string> &huffmanCode) { 
    if (!root) 
        return; 
    if (root->data != '$') 
        huffmanCode[root->data] = code;
    encodeHuffman(root->left, code + "0", huffmanCode); 
    encodeHuffman(root->right, code + "1", huffmanCode); 
} 

MinHeapNode* buildHuffmanTree(unordered_map<char, int>&frequencyMap) {  
    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap; 
    for (auto pair: frequencyMap) 
        minHeap.push(new MinHeapNode(pair.first, pair.second)); 
    while (minHeap.size() != 1) { 
        MinHeapNode* left = minHeap.top(); 
        minHeap.pop(); 
        MinHeapNode* right = minHeap.top(); 
        minHeap.pop(); 
        MinHeapNode* internal_node = new MinHeapNode('$', left->freq + right->freq); 
        internal_node->left = left; 
        internal_node->right = right; 
        minHeap.push(internal_node); 
    } 
    return minHeap.top();
} 

unordered_map<char, int> buildFrequencyMap(const string& text) {
    unordered_map<char, int> frequencyMap;
    for (char c : text) {
        frequencyMap[c]++;
    }
    return frequencyMap;
}

void compressFile(const string &inputFileName, const string &outputFileName) {
    ifstream inFile(inputFileName, ios::binary);
    if (!inFile.is_open()) {
        cerr << "Unable to open input file: " << inputFileName << endl;
        cerr << "Please check if the file exists and the path is correct." << endl;
        return;
    }

    string text((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    unordered_map<char, int> frequencyMap = buildFrequencyMap(text);
    MinHeapNode* root = buildHuffmanTree(frequencyMap);

    unordered_map<char, string> huffmanCode;
    encodeHuffman(root, "", huffmanCode);

    ofstream outFile(outputFileName, ios::binary);
    if (!outFile.is_open()) {
        cerr << "Unable to open output file: " << outputFileName << endl;
        cerr << "Please check if you have write permissions in the directory." << endl;
        return;
    }

    for (auto& pair : huffmanCode) {
        outFile << pair.first << " " << pair.second << endl;
    }
    outFile << "#####" << endl;

    string encodedBits = "";
    for (char c : text) {
        encodedBits += huffmanCode[c];
    }

    int numBytes = (encodedBits.length() + 7) / 8;
    outFile << numBytes << endl;

    for (int i = 0; i < encodedBits.length(); i += 8) {
        string byte = encodedBits.substr(i, 8);
        bitset<8> bits(byte);
        char c = char(bits.to_ulong());
        outFile.write(&c, sizeof(char));
    }

    outFile.close();
    cout << "File compressed successfully. Compressed file: " << outputFileName << endl;
}

MinHeapNode* buildHuffmanTreeFromCodes(unordered_map<string, char>& huffmanCodes) {
    MinHeapNode* root = new MinHeapNode('$', 0);
    for (auto& pair : huffmanCodes) {
        MinHeapNode* current = root;
        string code = pair.first;
        for (char bit : code) {
            if (bit == '0') {
                if (!current->left)
                    current->left = new MinHeapNode('$', 0);
                current = current->left;
            } else {
                if (!current->right)
                    current->right = new MinHeapNode('$', 0);
                current = current->right;
            }
        }
        current->data = pair.second;
    }
    return root;
}

void decodeHuffman(MinHeapNode* root, string encodedStr, ofstream &outFile) {
   
    MinHeapNode* current = root;

if (!current) {
    std::cerr << "Error: root is null." << std::endl;
    return;
}

for (char bit : encodedStr) {

    if (!current->left && !current->right) { 
       
        if (current->data == ' ' || (current->data >= 32 && current->data <= 126)) {
            outFile << current->data;
        } else {
            std::cerr << "Error: invalid character in tree node: " << current->data << std::endl;
            return;
        }
        current = root;
        if (!current) {
            std::cerr << "Error: root is null" << std::endl;
            return;
        }
    }

    if (bit == '0') {

        if (current->left) {
            current = current->left;
        } 
            else{
                continue;
            std::cerr << "Error: encountered a '0' but current node has no left child." << std::endl;
            return;
           }
        }
     
    else if (bit == '1') {

        if (current->right) {
            current = current->right;
        }
            else{
                continue;
            std::cerr << "Error: encountered a '1' but current node has no right child." << std::endl;
            return;
            }
        }
    else {
        std::cerr << "Error: encodedStr contains invalid character: " << bit << std::endl;
        return;
    }
}

}

void decompressFile(const string &inputFileName, const string &outputFileName) {
    ifstream inFile(inputFileName, ios::binary);
    if (!inFile.is_open()) {
        cerr << "Unable to open input file: " << inputFileName << endl;
        cerr << "Please check if the file exists and the path is correct." << endl;
        return;
    }

    unordered_map<string, char> huffmanCodes;
    string line;
    while (getline(inFile, line)) {
        if (line == "#####")
            break;
        stringstream ss(line);
        char ch;
        string code;
        
        ss >> ch >> code;
        
        huffmanCodes[code] = ch;
       
    }
   
    MinHeapNode* root = buildHuffmanTreeFromCodes(huffmanCodes);

    int numBytes;
    inFile >> numBytes;
    inFile.ignore(); 
    string encodedBits = "";
    char c;
    while (inFile.get(c)) {
        bitset<8> bits(c);
        encodedBits += bits.to_string();
    }

    encodedBits = encodedBits.substr(0, numBytes * 8); 

    ofstream outFile(outputFileName, ios::binary);
    if (!outFile.is_open()) {
        cerr << "Unable to open output file: " << outputFileName << endl;
        cerr << "Please check if you have write permissions in the directory." << endl;
        return;
    }

    decodeHuffman(root, encodedBits, outFile);

    outFile.close();

    cout << "File decompressed successfully. Decompressed file: " << outputFileName << endl;
}

int main() { 
    string inputFileName = "input_file.txt";
    string compressedFileName = "compressed_file.txt"; 
    string decompressedFileName = "decompressed_file.txt";

    
    ifstream checkFile(inputFileName);
    if (!checkFile.good()) {
        cerr << "Input file does not exist: " << inputFileName << endl;
        return 1;
    } else {
        cout << "Input file found: " << inputFileName << endl;
    }
    checkFile.close();

    compressFile(inputFileName, compressedFileName);
    decompressFile(compressedFileName, decompressedFileName);

    return 0; 
}