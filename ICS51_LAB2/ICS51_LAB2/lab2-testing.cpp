#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <iomanip>

#include "lodepng.h"

// These functions should be implemented in lab2.cpp
void imageThresholding(unsigned char* buffer, int dim);
void imageRotation(unsigned int* image, int dim);

extern char *yourName;
extern char *yourStudentID;

int main() {

	std::cout << "ICS 51, Lab 2" << "\nName: " << yourName << "\nStudentID: " << yourStudentID << "\n\n";

    #define TEST_P1 1
    #define TEST_P2 0

	std::vector<unsigned char> png;
	std::vector<unsigned char> image;
	unsigned int width, height;
	lodepng::State state;
	std::string filename, outFilename;
	
#if TEST_P1
	// Part 1 tests
	unsigned char testStr0[3][3] = { {0,   30,  60},
									 {90,  120, 150},
									 {180, 210, 240} };

	unsigned char exptectedStr0[3][3] = { {0,   0,   0},
										  {0,   0,   255},
										  {255, 255, 255} };
  
	imageThresholding(&testStr0[0][0], 3);

	int i, j;
	bool err = 0;
	for (i = 0; i < 3; ++i){
		for (j = 0; j < 3; ++j)	{
			if (testStr0[i][j] != exptectedStr0[i][j]) {
				std::cerr << "Part1: Test 1 failed at index: "<< i<< "," << j << std::endl;
				err = 1;
			}
		}
	}
	if (!err)
		std::cout << "Part1: Test 1 passed." << std::endl;
	
	unsigned char testStr1[4][4] = {{0,   30,  90,  120},
									{127, 45,  130, 60},
									{127, 128, 0,   130},
									{227, 228, 255, 255 }};
	unsigned char exptectedStr1[4][4] = {{0, 0,   0,   0},
	                                     {0, 0,   255, 0 },
										 {0, 255, 0,   255},
										 {255, 255, 255, 255 }};

	imageThresholding(&testStr1[0][0], 4);

	err = 0;
	for (i = 0; i < 4; ++i) {
		for (j = 0; j < 4; ++j) {
			if (testStr1[i][j]!= exptectedStr1[i][j]) {
				std::cerr << "Part1: Test 2 failed at index: " << i << "," << j << std::endl;
				err = 1;
			}
		}
	}
	if (!err)
		std::cout << "Part1: Test 2 passed." << std::endl;

	// Load a grayscale png (1Byte per pixel)
	// Make a binary image by thresholding

	filename = "Lena.png";
	lodepng::decode(image, width, height, filename, LCT_GREY, 8U);

	if (image.size() == 0) {
		std::cerr << "Part1: Lena.png was not found. Make sure it exists in your visual studio project directory." << std::endl;
		return -1;
	}
	assert(width == height);
	imageThresholding((unsigned char*)&image[0], width);

	outFilename = "Lena_binary.png";
	lodepng::encode(outFilename, image, width, height, LCT_GREY, 8U);
	image.clear();

	std::cout << "Part1: Check out generated binary image: \"Lena_binary.png\"" << std::endl;
#endif

	std::cout << std::endl;

#if TEST_P2
	// Part 2 tests
	unsigned int testStr2[3][3] = { {0x0,        0x10101010, 0x20202020},
	                                {0x30303030, 0x40404040, 0x50505050},
	                                {0x60606060, 0x70707070, 0x80808080} };

	unsigned int exptectedStr2[3][3] = { {0x60606060,  0x30303030, 0x0},
                                         {0x70707070,  0x40404040, 0x10101010},
                                         {0x80808080,  0x50505050, 0x20202020} };

	imageRotation(&testStr2[0][0], 3);

	err = 0;
	for (i = 0; i < 3; ++i){
		for (j = 0; j < 3; ++j)	{
			if (testStr2[i][j] != exptectedStr2[i][j]) {
				std::cerr << "Part2: Test 1 failed at index: "<< i << "," << j << " got: " << std::hex << testStr2[i][j] << " expected: " << exptectedStr2[i][j] << std::dec << std::endl;
				err = 1;
			}
		}
	}
	if (!err)
		std::cout << "Part2: Test 1 passed." << std::endl;

	unsigned int testStr3[4][4] = { {0, 1,   2,  3},
									{4, 5,   6,  7},
									{8, 9,  10, 11},
									{12, 13, 14, 15} };

	unsigned int exptectedStr3[4][4] = { {12,  8, 4, 0},
										 {13,  9, 5, 1},
										 {14, 10, 6, 2},
										 {15, 11, 7, 3} };

	imageRotation(&testStr3[0][0], 4);

	err = 0;
	for (i = 0; i < 4; ++i) {
		for (j = 0; j < 4; ++j) {
			if (testStr3[i][j]!= exptectedStr3[i][j]) {
				std::cerr << "Part2: Test 2 failed at index: " << i << "," << j << " got: " << testStr3[i][j] << " expected: " << exptectedStr3[i][j] << std::endl;
				err = 1;
			}
		}
	}
	if (!err)
		std::cout << "Part2: Test 2 passed." << std::endl;

	// Load a RGBA png (4-Bytes per pixel)
	// Rotate it 90 degress clockwise

	filename = "Baboon.png";
	lodepng::decode(image, width, height, filename, LCT_RGBA, 8U);
	if (image.size() == 0) {
		std::cerr << "Part2: Baboon.png was not found. Make sure it exists in your visual studio project directory." << std::endl;
		return -1;
	}
	assert(width == height);

	imageRotation((unsigned int*)&image[0], width);
	
	outFilename = "Baboon_rotated.png";
	lodepng::encode(outFilename, image, width, height, LCT_RGBA, 8U);
	
	std::cout << "Part2: Check out generated rotated image: \"Baboon_rotated.png\"" << std::endl;
#endif

	getchar();
	return 0;
}
