#include <iostream>
#include <string>

#include "BMP_Class.h"


int main() {
  std::string file_path = "";
  std::cout << "Input file path (File must be in BMP format, 24 bits per pixel, uncompressed):" << std::endl;
  std::cin >> file_path;
  try {
    BMP bmp(file_path);
    bmp.CreatePhotoNegative();
  }
  catch (const std::runtime_error &err) {
    std::cerr << err.what() << std::endl;
  }
  std::cin >> file_path;
}
