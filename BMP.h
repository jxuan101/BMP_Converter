#ifndef BMP_CLASS_H_
#define BMP_CLASS_H_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Constants Definitions
// kBmpHex is used to identify a BMP file.
// kBitsPerPixel is the bits per pixel that
//    that we want for inputs of this program.
// kCompression, 0 indicates no compression
//    which is what we want for this program.
// kRGBMax is the max value of a RGB value which
//    is 255.
const uint16_t kBmpHex = 0x4D42;
const uint16_t kBitsPerByte = 8;
const uint16_t kBitsPerPixel = 24;
const uint32_t kCompression = 0;
const uint16_t kRGBMax = 255;

// In C++, memory alignment is dependent on the 
// compiler, we need to use pragma pack to 
// omit the padding that the compiler inserts to 
// allow our BMP headers to store the correct
// bytes of data. This also saves memory but
// slower access.
#pragma pack(1)                           

struct BitmapFileHeader {
  uint16_t file_type{0x0000};             // File type
  uint32_t file_size{0};                  // Size of the BMP file (bytes)
  uint16_t app_reserved{0};               // Reserved, will always be 0
  uint16_t app_reserved2{0};              // Reserved, will always be 0
  uint32_t data_offset{0};                // Position of the pixel array
};

struct BitmapInfoHeader {
  uint32_t size{0};                       // Size of the DIB header (bytes)
  int32_t width{0};                       // Width of bitmap in pixels
  int32_t height{0};                      // Height of bitmap in pixels
  uint16_t number_of_planes{0};           // Number of color planes, must be 1
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
      
    BMP(const std::string &file_path);

    // This is called during constructor which
    // initializes all our private variables.
    void Read(const std::string &file_path);

    // This outputs a new BMP file that has
    // the original image but with photo negative
    // filter.
    void CreatePhotoNegative();

  private:
    BitmapFileHeader file_header_;
    BitmapInfoHeader info_header_;
    std::vector<uint8_t> pixel_data_;
    std::vector<uint8_t> padding_;
    std::string file_name_;
    std::string path_name_;
    uint32_t pixel_data_row_size_;

    // Calculates the amount of padding we
    // need to ignore when reading or supply
    // when outputting.
    void CalculatePadding();

    // This function applies only to input.
    // Ignores memory padding.
    void PadStream(std::ifstream &input_stream);
    
    // This function applies only to output.
    // Supplies memory padding according to 
    // the size of padding_.
    void PadStream(std::ofstream &output_stream);
};

#endif // BMP_CLASS_H_