🕵️‍♂️ Steganography Project in C
This is a terminal-based Steganography System built in C that allows secure embedding and extraction of secret messages within .bmp image files using Least Significant Bit (LSB) encoding technique. It enables basic steganographic operations such as encoding text data into an image and decoding hidden information from an image without visibly altering it. The system emphasizes modular design, file handling, bitwise operations, and strict validation to ensure correctness and robustness.

📌 Project Overview
Traditional methods of securing information such as encryption draw attention due to obvious transformations in data. Steganography offers a discreet alternative by concealing the very presence of the message. This project implements a digital solution that allows the user to embed or extract text messages into/from .bmp files via a command-line interface. It is designed with persistent file access, clear modular structure, and a flexible file-handling system.

⚙️ Tools and Technologies Used
C Programming Language

GCC Compiler

Command-Line Interface (CLI)

24-bit BMP Image Format

Modular Programming Design

📁 File Modules
less
Copy
Edit

├── main.c                  // Handles command-line arguments and dispatches encode/decode operation

├── encode.c / encode.h     // Encoding logic: embeds data into BMP image

├── decode.c / decode.h     // Decoding logic: extracts data from stego image

├── common.c / common.h     // Reusable file functions and utilities

├── types.h                 // Status codes and enum definitions

├── Makefile                // Build automation

├── stego.bmp               // Output BMP file with embedded data

├── secret.txt              // Input text file to encode

├── output.txt              // Output text file after decoding

🧠 Working Principle
The system follows the Least Significant Bit (LSB) method to hide and recover messages from BMP files. It manipulates the least significant bit of each image byte to encode or decode binary data, ensuring the visual integrity of the image remains unaffected.

🔧 Features
✅ Encode secret messages into BMP files

✅ Decode hidden messages from stego images

✅ Uses magic string to verify presence of hidden data

✅ Bitwise encoding using LSB manipulation

✅ File validation and error checking

✅ Modular architecture for scalability

✅ Supports any printable character set

✅ CLI arguments for flexibility

🗃️ Command-Line Usage
Encoding:

bash
Copy
Edit
./a.out -e <source_image.bmp> <secret.txt> <stego_image.bmp>
Decoding:

bash
Copy
Edit
./a.out -d <stego_image.bmp> <output.txt>
🧪 Input Validation
✅ Validates .bmp file format (supports only 24-bit BMP)

✅ Validates file pointers for all input/output files

✅ Checks magic string for presence of embedded data

✅ Validates image size is sufficient for data encoding

🛠️ Core Operations
🔹 Encode
Reads secret text file

Embeds magic string, file extension, secret size, and content using LSB

Outputs a new stego image that visually matches the original

🔹 Decode
Reads stego image

Extracts and validates magic string

Decodes file extension and secret content

Writes decoded content to output file

🔐 Security & Limitations
Steganography hides presence of data, not content — not a substitute for encryption.

Can only encode limited data depending on image size.

No authentication or encryption layers implemented (future scope).

📈 Future Enhancements
🔒 Integrate with encryption for dual-layer security

🖼️ Support for other image formats (PNG, JPEG)

🧾 GUI version using GTK or Qt

📊 Add image metadata manipulation

👤 Password-protected access

📜 Logging and detailed report generation

📦 Archive multiple files using ZIP + encode

🎬 Demo
📽️ Demo Video: Watch here
https://drive.google.com/file/d/1r2EjTu0njZT2sw_ED3XcBarh2Jic7eeX/view?usp=sharing

📸 Sample Terminal Output:
<img width="1920" height="1080" alt="2" src="https://github.com/user-attachments/assets/8ee55f66-9d03-44fd-8084-cc4a616d27b2" />
<img width="1918" height="515" alt="2 1" src="https://github.com/user-attachments/assets/e5dd2f94-61a6-4566-aed1-59427f9fd954" />

## DECODING OPERATION STARTED ##

Opening image file... Done
Verifying magic string... Verified ✅
Extracting file extension... Done
Reading secret data... Completed successfully!

✅ Output written to output.txt
📸 Sample Terminal Output:
<img width="1920" height="1080" alt="3" src="https://github.com/user-attachments/assets/c6dbacfa-b870-4c2e-a939-e67cd53e5081" />
<img width="1918" height="417" alt="4" src="https://github.com/user-attachments/assets/800c4d5f-f066-4294-9ac0-91c7fabbb504" />

👨‍💻 Developed By
Vamsi T
Aspiring Embedded Systems Engineer | Passionate about C Programming & Digital Security
📧 vamsithummaluri@gmail.com
