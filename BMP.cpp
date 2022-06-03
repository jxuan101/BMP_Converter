#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "BMP.h"

BMP::BMP(const std::string& file_path) {
  Read(file_path);
  // Splits the file path into the file name and
  // the working directory path to be used as
  // the name for our output target file in
  // createPhotoNegative().
  file_name_ = file_path.substr(file_path.find_last_of('/') + 1);
  outgoing_path_ = file_path.substr(0, file_path.find_last_of('/') + 1);
}

BMP::BMP(const std::string& file_path, const std::string& folder_path) {
  Read(file_path);
  file_name_ = file_path.substr(file_path.find_last_of('/') + 1);
  outgoing_path_ = folder_path;
}

// Validates and parses the input file into
// our headers and pixel data array.
void BMP::Read(const std::string& file_path) {
  // Open the input file in binary mode
  std::ifstream input_stream{ file_path, std::ifstream::binary };
  if (input_stream.good()) {
    input_stream.read((char*)&file_header_, sizeof(file_header_));
    // Validate file type to be BMP
    if (file_header_.file_type != kBmpHex)
      throw std::logic_error("Error: Unrecognized file format, please input a .BMP file!");

    input_stream.read((char*)&info_header_, sizeof(info_header_));
    // Validate bits per pixel == 24, the only
    // bpp that we're handling in this program.
    if (info_header_.bits_per_pixel != kBitsPerPixel)
      throw std::logic_error("Error: Your .BMP file is not 24 bits per pixel, please input a 24 bits per pixel file!");
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
      throw std::logic_error("Error: Your .BMP file has been compressed before, please input an uncompressed file!");

    // Calculate row size and padding of pixel array
    // for cases where the width of our image is not
    // divisible by 4.
    pixel_data_row_size_ = info_header_.width * (info_header_.bits_per_pixel / 8);
    CalculatePadding();

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
      PadStream(input_stream);
    }
  }
  // The input stream initialization failed 
  // meaning no such file was found.
  else {
    throw std::runtime_error("Error: Unable to locate BMP file at " + file_path + "!");
  }
}

// This non-destructive method (creates
// a new file) edits our pixel data array 
// to create a photo negative version.
void BMP::CreatePhotoNegative() {
  // Inverse every RGB value to create photo negative.
  // The algorithm is 255 - value = inverse
  for (size_t i = 0; i < pixel_data_.size(); i++) {
    pixel_data_[i] = uint8_t(kRGBMax - int(pixel_data_[i]));
  }

  // Construct full path to output file and
  // adds a negative identifier to the file name.
  if (outgoing_path_.length() > 0) {
    if (outgoing_path_[outgoing_path_.length() - 1] != '/') {
      outgoing_path_ += '/';
    }
  }
  std::string output_file = outgoing_path_ + "negative_" + file_name_;

  // Open an output stream to write binary 
  // data to a new file if it doesn't already
  // exist or overwrites an existing file 
  // of the same name.
  std::ofstream output_stream{ output_file, std::ofstream::binary };
  if (output_stream.good()) {
    output_stream.write((const char*)&file_header_, sizeof(file_header_));
    output_stream.write((const char*)&info_header_, sizeof(info_header_));
    // Account for padding if width is not divisible by 4
    if (info_header_.width % 4 == 0) {
      output_stream.write((const char*)pixel_data_.data(), pixel_data_.size());
    }
    else {
      PadStream(output_stream);
    }
  }
  // The output stream initialization failed 
  // meaning the output file could not be created.
  else {
    throw std::runtime_error("Error: Unable to create output file.");
  }
}

// Calculates how much padding is required
// to fill each row of a pixel array. This
// is only used if the width is not a 
// multiple of 4.
void BMP::CalculatePadding() {
  uint32_t new_row_size = pixel_data_row_size_;
  while (new_row_size % 4 != 0) {
    new_row_size++;
  }
  padding_.resize(new_row_size - pixel_data_row_size_);
}

// An overloaded function that is called 
// when reading files. This function only
// affects the pixel array data.
void BMP::PadStream(std::ifstream& input_stream) {
  // Loop through each row while reading in data then padding
  for (size_t i = 0; i < info_header_.height; i++) {
    input_stream.read((char*)(pixel_data_.data() + pixel_data_row_size_ * i), pixel_data_row_size_);
    input_stream.read((char*)padding_.data(), padding_.size());
  }
}

// An overloaded function that is called 
// when writing to files.
void BMP::PadStream(std::ofstream& output_stream) {
  // Loop through each row while writing data then padding
  for (size_t i = 0; i < info_header_.height; i++) {
    output_stream.write((char*)(pixel_data_.data() + pixel_data_row_size_ * i), pixel_data_row_size_);
    output_stream.write((char*)padding_.data(), padding_.size());
  }
}