#ifndef BMP_H_
#define BMP_H_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>

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
const uint32_t kDataOffsetV1 = 54;
const uint32_t kDataOffsetV4 = 122;
const uint32_t kDataOffsetV5 = 138;

// In C++, memory alignment is dependent on the 
// compiler, we need to use pragma pack to 
// omit the padding that the compiler inserts to 
// allow our BMP headers to store the correct
// bytes of data. This also saves memory but
// slower access.
#pragma pack(1)                           


// BMP Header Declarations

// Always gonna be 14 bytes total.
struct BitmapFileHeader {
  uint16_t file_type{0x0000};             // File type
  uint32_t file_size{0};                  // Size of the BMP file (bytes)
  uint16_t app_reserved{0};               // Reserved, will always be 0
  uint16_t app_reserved2{0};              // Reserved, will always be 0
  uint32_t data_offset{0};                // Position of the pixel array
};

// BMP Info Header Declarations

// 40 Bytes, used when data_offset == 54
struct BitmapInfoHeader {
  uint32_t size{0};                       // Size of the DIB header (bytes)
  int32_t width{0};                       // Width of bitmap in pixels
  int32_t height{0};                      // Height of bitmap in pixels
  uint16_t number_of_planes{0};           // Number of color planes, must 
                                          // be 1
  uint16_t bits_per_pixel{0};             // No. of bits per pixel
  uint32_t compression{0};                // 0 - uncompressed. 
  uint32_t image_size{0};                 // 0 - for uncompressed images.
  int32_t horizontal_resolution{0};       // Pixel per meter horizontally
  int32_t vertical_resolution{0};         // Pixel per meter vertically
  uint32_t colors{0};                     // Number of colors in the color 
                                          // palette. 0 = 2^n colors
  uint32_t colors_important{0};           // Number of important colors 
                                          // used, 0 when every color is
                                          // important
};

// The xyz coordinates of a color.
struct ColorCoords {
  int32_t ciexyzX{0};
  int32_t ciexyzY{0};
  int32_t ciexyzZ{0};
};

// The xyz coordinates of RGB endpoints.
// Used for BitmapV4InfoHeader and
// BitMapV5InfoHeader.
struct ColorEndpoints {
  ColorCoords ciexyzRed{0, 0, 0};
  ColorCoords ciexyzGreen{0, 0, 0};
  ColorCoords ciexyzBlue{0, 0, 0};
};

// 108 Bytes, used when data_offset == 122
struct BitmapV4InfoHeader : BitmapInfoHeader {
  uint32_t red_mask{0};
  uint32_t green_mask{0};
  uint32_t blue_mask{0};
  uint32_t alpha_mask{0};
  uint32_t cs_type{0};
  ColorEndpoints endpoints;
  uint32_t gamma_red{0};
  uint32_t gamma_green{0};
  uint32_t gamme_blue{0};
};

// 124 Bytes, used when data_offset == 138
struct BitmapV5InfoHeader : BitmapV4InfoHeader {
  uint32_t intent{0};
  uint32_t profile_data{0};
  uint32_t profile_size{0};
  uint32_t app_reserved3{0};
};

class BMP {
  public:
      
    BMP(const std::string& file_path);
    BMP(const std::string& file_path, const std::string& folder_path);

    // This is called during constructor which
    // initializes all our private variables.
    void Read(const std::string& file_path);

    // This outputs a new BMP file that has
    // the original image but with photo negative
    // filter.
    void CreatePhotoNegative();

  private:
    BitmapFileHeader file_header_;
    BitmapInfoHeader info_header_;
    BitmapV4InfoHeader info_header_v4_;
    BitmapV5InfoHeader info_header_v5_;
    std::vector<uint8_t> pixel_data_;
    std::vector<uint8_t> padding_;            // Contains the amount of 
                                              // padding we need for files
                                              // whose widths are not divisible
                                              // by 4.
    std::string file_name_;
    std::string outgoing_path_;
    uint32_t pixel_data_row_size_;            // The size of a row in the pixel
                                              // data including padding

    // Calculates the amount of padding we
    // need to ignore when reading or supply
    // when outputting.
    void CalculatePadding();

    // Checks if the output file's name already
    // exists. This determines if we should add
    // "copy" to the name.
    bool CheckOutputFileExists(const std::string& filename);

    // Helper function that ignores memory padding 
    // when reading in data. Only used for INPUT.
    template <typename InfoHeaderType>
    void IgnorePaddingStream(std::ifstream& input_stream, const InfoHeaderType& info_header);
    
    // Helper function that supply memory padding 
    // according to the size of padding_. Only used
    // for OUTPUT.
    template <typename InfoHeaderType>
    void AddPaddingStream(std::ofstream& output_stream, const InfoHeaderType& info_header);

    // Helper function that reads info header data 
    // into either BitmapInfoHeader, BitmapV4InfoHeader, 
    // or BitmapV5InfoHeader.
    template <typename InfoHeaderType>
    void ReadInfoHeader(std::ifstream& input_stream, InfoHeaderType& info_header);
};

#endif // BMP_H_