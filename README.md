# File-Zipper

This project is implemented in C++ for the compression and decompression of text files using the Huffman coding technique. It uses ASCII characters (0-127) for compression. The main steps involved are:  

## Compression:  
* Reading from the text file and storing the frequency of each character.  
* Creating a min heap based on the character frequencies.  
* Constructing a Huffman tree from the min heap and generating codes (consisting of 0s and 1s) for each character in the file.  
* Encrypting or compressing the given text with the Huffman codes (ensuring the Huffman text is divisible by 8 by appending 0s).  
* Writing the compressed text into a file as bytes and saving it with a .huf extension.  

**Note:** The compressed file includes information about the count of padding bits(0's added to make sure divisible by 8) used in the last byte.  

## Decompression:  
* Reading the encrypted text file (.huf file) as bytes and converting into the binary form.  
* Finding the characters for Huffman codes by traversing the generated Huffman tree.  
* Writing the decompressed text into a new text file.  

**Note:** Need to remove the padding bits for the last byte.  
