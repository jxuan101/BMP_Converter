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
      
    BMP(const std::string &file_path) {
      read(file_path);
      // Splits the file path into the file name and
      // the working directory path to be used as
      // the name for our output target file in
      // createPhotoNegative().
      file_name_ = file_path.substr(file_path.find_last_of('\\') + 1);
      path_name_ = file_path.substr(0, file_path.find_last_of('\\') + 1);
    }

    // Validates and parses the input file into
    // our headers and pixel data array.
    void read(const std::string &file_path) {
        // Open the input file in binary mode
        std::ifstream input_stream{file_path, std::ifstream::binary};
        if (input_stream.good()) {
          input_stream.read((char*)&file_header_, sizeof(file_header_));
          // Validate file type to be BMP
          if (file_header_.file_type != kBmpHex)
            throw std::runtime_error("Error: Unrecognized file format, please input a .BMP file!");

          input_stream.read((char*)&info_header_, sizeof(info_header_));
          // Validate bits per pixel == 24, the only
          // bpp that we're handling in this program.
          if (info_header_.bits_per_pixel != kBitsPerPixel)
            throw std::runtime_error("Error: Your .BMP file is not 24 bits per pixel, please input a 24 bits per pixel file!");
          // Info header must be 40 bytes because we're
          // using BITMAPINFOHEADER, the common Windows
          // format for now.
          if (info_header_.size != sizeof(BitmapInfoHeader)) {
            info_header_.size = sizeof(BitmapInfoHeader);
            file_header_.data_offset = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
          }
          // Validate compression == 0, 0 means there
          // was no compression and we only want
          // uncompressed files.
          if (info_header_.compression != kCompression)
            throw std::runtime_error("Error: Your .BMP file has been compressed before, please input an uncompressed file!");

          // Calculate row size and padding of pixel array
          // for cases where the width of our image is not
          // divisible by 4.
          pixel_data_row_size_ = info_header_.width * (info_header_.bits_per_pixel / 8);
          calculatePadding();

          // Moves input stream pointer to where our pixel 
          // array data begins.
          input_stream.seekg(file_header_.data_offset, input_stream.beg);
          // Allocate the size of the bitmap array
          pixel_data_.resize(info_header_.width * info_header_.height * (info_header_.bits_per_pixel / 8));
          // When the pixel array is loaded into memory, 
          // each row must begin at a memory address 
          // that is a multiple of 4.
          if (info_header_.width % 4 == 0) {
            input_stream.read((char*)pixel_data_.data(), pixel_data_.size());
          }
          // Else we have to pad the rows with 0's
          else {
            padStream(input_stream);
          }
        }
        // The input stream initialization failed 
        // meaning no such file was found.
        else {
          throw std::runtime_error("Error: Unable to locate " + file_path + "!");
        }
    }

    // This non-destructive method (creates
    // a new file) edits our pixel data array 
    // to create a photo negative version.
    void createPhotoNegative() {
      // Inverse every RGB value to create photo negative.
      // The algorithm is 255 - value = inverse
      for (size_t i = 0; i < pixel_data_.size(); i++) {
        pixel_data_[i] = uint8_t(kRGBMax - int(pixel_data_[i]));
      }

      // Construct full path to output file and
      // adds a negative identifier to the file name.
      std::string output_file = path_name_ + "negative_" + file_name_;

      // Open an output stream to write binary 
      // data to a new file if it doesn't already
      // exist or overwrites an existing file 
      // of the same name.
      std::ofstream output_stream{output_file, std::ofstream::binary};
      if (output_stream.good()) {
        output_stream.write((const char*)&file_header_, sizeof(file_header_));
        output_stream.write((const char*)&info_header_, sizeof(info_header_));
        // Account for padding if width is not divisible by 4
        if (info_header_.width % 4 == 0) {
          output_stream.write((const char*)pixel_data_.data(), pixel_data_.size());
        }
        else {
          padStream(output_stream);
        }
        std::cout << "Photo negative of " + file_name_ + " created at " + path_name_ << std::endl;
      }
      // The output stream initialization failed 
      // meaning the output file could not be created.
      else {
        throw std::runtime_error("Error: Unable to create output file.");
      }
    }

  private:
    BitmapFileHeader file_header_;
    BitmapInfoHeader info_header_;
    std::vector<uint8_t> pixel_data_;
    std::vector<uint8_t> padding_;
    std::string file_name_;
    std::string path_name_;
    uint32_t pixel_data_row_size_;

    // Calculates how much padding is required
    // to fill each row of a pixel array. This
    // is only used if the width is not a 
    // multiple of 4.
    void calculatePadding() {
      uint32_t new_row_size = pixel_data_row_size_;
      while (new_row_size % 4 != 0) {
        new_row_size++;
      }
      padding_.resize(new_row_size - pixel_data_row_size_);
    }

    // An overloaded function that is called 
    // when reading files. This function only
    // affects the pixel array data.
    void padStream(std::ifstream &input_stream) {
      // Loop through each row while reading in data then padding
      for (int i = 0; i < info_header_.height; i++) {
        input_stream.read((char*)(pixel_data_.data() + pixel_data_row_size_ * i), pixel_data_row_size_);
        input_stream.read((char*)padding_.data(), padding_.size());
      }
    }
    
    // An overloaded function that is called 
    // when writing to files.
    void padStream(std::ofstream &output_stream) {
      // Loop through each row while writing data then padding
      for (int i = 0; i < info_header_.height; i++) {
        output_stream.write((char*)(pixel_data_.data() + pixel_data_row_size_ * i), pixel_data_row_size_);
        output_stream.write((char*)padding_.data(), padding_.size());
      }
    }
};

#endif // BMP_CLASS_H_