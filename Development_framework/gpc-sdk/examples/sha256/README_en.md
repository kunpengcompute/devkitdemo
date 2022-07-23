# **sha256 demo**

English | [简体中文](README.md)

## Introduction
SHA256 is an algorithm subdivided under SHA-2.

Secure Hash Algorithm 2 (SHA-2) is a set of cryptographic hash functions designed by the United States National Security Agency (NSA). SHA-2 includes significant changes from its predecessor, SHA-1.

The SHA-2 family consists of six hash functions: SHA-224, SHA-256, SHA-384,  SHA-512, SHA-512/224, and SHA-512/256. Their structures are virtually identical, differing only in the length of the generated digest and the number of rounds.

SHA256 is a hash function.
Hash function, also known as hash algorithm, is a way to create small numeric "fingerprints" from any kind of data. The hash function compresses a message or data into a digest, which reduces the data amount and fixes the data format. It scrambles and mixes the data and recreates a fingerprint called a hash value, which is usually represented by a short random string of letters and numbers.

For messages of any length, SHA256 generates a 256-bit hash value, which is called a message digest. The digest is equivalent to a 32-byte array, usually represented by a 64-byte hexadecimal string.

## Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/sha256/
   ```

3. Compile the demo.

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```

4. Run the demo.

   ```shell
   ./sha256 ../src/sha256.c
   ```
   Remarks: The first parameter indicates the binary file to be executed, and the second parameter indicates the data file used for the test. The file can be in any format. The 'src/sha256.c' file is used as an example.
   Note: To better compare the performance of the SHA256 algorithm implemented using C language and the SHA256 algorithm implemented using assembly language, use a file whose size is greater than 1MB for test.

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build
   ```