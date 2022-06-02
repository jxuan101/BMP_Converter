#ifndef BMP_CLASS_H_
#define BMP_CLASS_H_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const uint16_t kBmpHex = 0x4D42;
const uint16_t kBitsPerByte = 8;
const uint16_t kBitsPerPixel = 24;
const uint32_t kCompression = 0;

#pragma pack(1)

struct BitmapFileHeader {
  uint16_t file_type{0x0000};             // File type, must be 0x4D42 which is 'BM'
  uint32_t file_size{0};                  // Size of the BMP file (bytes)
  uint16_t app_reserved{0};               // Reserved, will always be 0
  uint16_t app_reserved2{0};              // Reserved, will always be 0
  uint32_t data_offset{0};              // Starting position of the pixel array
};

struct BitmapInfoHeader {
  uint32_t size{0};                       // Size of the DIB header (bytes)
  int32_t width{0};                       // Width of bitmap in pixels
  int32_t height{0};                      // Height of bitmap in pixels
  uint16_t number_of_planes{1};           // Number of color planes, must be 1
  uint16_t bits_per_pixel{0};             // No. of bits per pixel
  uint32_t compression{0};                // 0 - uncompressed. 
  uint32_t image_size{0};                 // 0 - for uncompressed images.
  int32_t horizontal_resolution{0};       // Pixel per meter horizontally
  int32_t vertical_resolution{0};         // Pixel per meter vertically
  uint32_t colors{0};                     // Number of colors in the color palette. 0 = 2^n colors
  uint32_t colors_important{0};           // Number of important colors used, 0 when every color is important
};

class BMP {
  public:
      
    BMP(const std::string &file_name) {
      read(file_name);
    }

    void read(const std::string &file_name) {
        // Open the input file
        std::ifstream input_stream{file_name, std::ifstream::binary};
        if (input_stream.good()) {

          // Read bitmap file header details into our file_header_ member
          input_stream.read((char*)&file_header_, sizeof(file_header_));
          // Validate file type to be BMP
          if (file_header_.file_type != kBmpHex)
            throw std::runtime_error("Error: Unrecognized file format, please input a .BMP file!");

          // Read bitmap info header details into our info_header_ member
          input_stream.read((char*)&info_header_, sizeof(info_header_));
          // Validate bits per pixel == 24
          if (info_header_.bits_per_pixel != kBitsPerPixel)
            throw std::runtime_error("Error: Your .BMP file is not 24 bits per pixel, please input a 24 bits per pixel file!");
          // Validate compression == 0
          if (info_header_.compression != kCompression)
            throw std::runtime_error("Error: Your .BMP file is compressed, please input an uncompressed file!");

        }
        else {
          throw std::runtime_error("Error: Unable to locate " + file_name + "!");
        }
    }

    void write(const char* file_name) {

    }

  private:
    BitmapFileHeader file_header_;
    BitmapInfoHeader info_header_;
    std::vector<uint8_t> bitmap_data_;

};

#endif // BMP_CLASS_H_