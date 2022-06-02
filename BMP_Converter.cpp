#include <iostream>
#include <string>

#include "BMP_Class.h"


int main() {
  std::string file_name = "";
  std::cout << "Input file path (File must be in BMP format, 24 bits per pixel, uncompressed):\n" << std::endl;
  std::cin >> file_name;
  try {
    BMP bmp(file_name);
  }
  catch (const std::runtime_error &err) {
    std::cerr << err.what() << std::endl;
    // std::cin >> file_name;
  }
}
