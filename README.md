# File_Compressor_tool
This project implements a file compressor and decompressor using the Huffman coding algorithm in C++. Huffman coding is a popular algorithm used for lossless data compression.
## Huffman Coding Algorithm
Huffman coding is a technique used for lossless data compression, where each character is assigned a variable-length binary code. 
The frequency of each character determines its code, with more frequent characters having shorter codes. 
This ensures that the most common characters are represented with fewer bits, thereby reducing the overall size of the compressed data.
## Steps Involved in Huffman Coding:
### Build Frequency Map: 
Calculate the frequency of each character (or symbol) in the input data.
### Build Huffman Tree: 
Construct a Huffman tree using a priority queue (min-heap) based on the character frequencies.
### Generate Huffman Codes: 
Traverse the Huffman tree to generate unique binary codes (Huffman codes) for each character.
### Encode Data: 
Replace each character in the input data with its corresponding Huffman code to generate the compressed data.
### Decode Data
Use the Huffman tree to decode the compressed binary data back into the original input data.
## Usage
### Compile:
Compile the program using a C++ compiler (e.g., g++)
### Run:
Execute the compiled binary to compress and decompress files using Huffman coding:
Replace input.txt with the name of the file you want to compress, compressed.txt with the name of the compressed output file and decompressed.txt with the name of the decompressed output
### Output: 
The program will generate a compressed and decompressed file.
