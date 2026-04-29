🖼️ Image Steganography in C

A console-based Image Steganography application developed in C that allows users to hide and retrieve secret data within an image file. This project was built as part of my Embedded Systems learning journey and focuses on binary file handling, bit-level data manipulation, and file processing techniques in C.

🚀 Project Overview

Steganography is a technique used to hide confidential data inside another file without visibly altering it. This project implements steganography by embedding secret text data into an image file.

The program performs both encoding and decoding operations directly on the binary structure of the image file.

The system supports:

Hiding secret text data inside an image
Extracting hidden data from the image

The program ensures that the image remains visually unchanged after encoding and validates input files before processing.

✅ Key Features

Encode secret data into an image file
Decode hidden data from an image
Command line argument based execution
Image file validation
Maintains image integrity after encoding
Modular C program structure

🔍 Working Principle

The program uses Least Significant Bit (LSB) steganography:

Each bit of the secret data is stored in the least significant bits of image pixel data
This causes minimal change to the image, making the hidden data invisible
During decoding, these bits are extracted to reconstruct the original message

🛠 Technical Concepts Used

File handling (binary files)
Bitwise operations
Command line argument processing
Data encoding and decoding logic
String handling functions
Modular programming

🎯 Learning Outcome

Through this project I improved my skills in:

Bit-level data manipulation in C
Understanding steganography techniques
Working with binary file formats
Designing encoding and decoding algorithms
Writing modular and maintainable programs

🔧 Future Improvements (Planned)

Support for additional image formats
Encryption before encoding data
Improved error handling
GUI-based application
Support for larger data sizes

🤝 Feedback

Suggestions and improvements are welcome. This project is part of my continuous learning journey in C and Embedded Systems.
