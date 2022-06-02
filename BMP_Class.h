#ifndef BMP_CLASS_H_
#define BMP_CLASS_H_

#include <vector>

struct BitmapFileHeader {
  uint16_t file_type{0x4D42};             // File type, must be 0x4D42 which is 'BM'
  uint32_t file_size{0};                  // Size of the BMP file (bytes)
  uint16_t app_reserved{0};               // Reserved, will always be 0
  uint16_t app_reserved2{0};              // Reserved, will always be 0
  uint32_t bitmap_offset{0};              // Starting position of the pixel array
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
    BitmapFileHeader file_header;
    BitmapInfoHeader info_header;
    std::vector<uint8_t> bitmap_data;
      
    BMP(const char *file_name) {
      read(file_name);
    }

    void read(const char *file_name) {

    }

    void write(const char* file_name) {

    }
};

#endif // BMP_CLASS_H_