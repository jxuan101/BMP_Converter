#include "BMP.h"

// Default output folder path
BMP::BMP(const std::string& file_path) {
  Read(file_path);
  // Splits the file path into the file name and
  // the working directory path to be used as
  // the name for our output target file in
  // createPhotoNegative().
  file_name_ = file_path.substr(file_path.find_last_of('/') + 1);
  outgoing_path_ = file_path.substr(0, file_path.find_last_of('/') + 1);
}

// User supplied custom output folder path
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

    // Choose an appropriate info header type (regular, v4, v5).
    // If the data_offset == 54, we use BitmapInfoHeader.
    // If the data_offset == 122, we use BitmapV4InfoHeader.
    // If the data_offset == 138, we use BitmapV5InfoHeader.
    // Anything else, we throw an error.
    switch (file_header_.data_offset) {
      case kDataOffsetV1:                 // 54
        ReadInfoHeader(input_stream, info_header_);
        break;
      case kDataOffsetV4:                 // 122
        ReadInfoHeader(input_stream, info_header_v4_);
        break;
      case kDataOffsetV5:                 // 138
        ReadInfoHeader(input_stream, info_header_v5_);
        break;
      // Anything else, we pass in
      // the regular header. An
      // exception will be thrown
      // upon validation.
      default:
        ReadInfoHeader(input_stream, info_header_);
    }
  }
  // The input stream initialization failed 
  // meaning no such file was found.
  else {
    throw std::runtime_error("Error: Unable to locate BMP file at " + file_path);
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
  // Add " - copy" to the end of the file name
  // if the output file already exists so as to
  // not overwrite any files.
  if (outgoing_path_.length() > 0) {
    if (outgoing_path_[outgoing_path_.length() - 1] != '/') {
      outgoing_path_ += '/';
    }
  }
  std::string output_file_path = outgoing_path_ + "negative_" + file_name_;
  // Loop names with additional " - copy" until one
  // is not found.
  std::string file_name_copy = file_name_;
  bool file_check_loop = true;
  while (file_check_loop) {
    if (CheckOutputFileExists(output_file_path)) {
      uint32_t extension_pos = file_name_.find_last_of('.');
      if (extension_pos != std::string::npos) {
        file_name_copy.insert(extension_pos, " - copy");
        output_file_path = outgoing_path_ + "negative_" + file_name_copy;
      }
      else {
        throw std::runtime_error("Error: Unable to create output file.");
      }
    }
    else {
      file_check_loop = false;
    }
  }

  // Open an output stream to write in binary, all
  // the BMP data to the newly created file.
  std::ofstream output_stream{ output_file_path, std::ofstream::binary };
  if (output_stream.good()) {
    output_stream.write((const char*)&file_header_, sizeof(file_header_));
    
    // Write the appropriate info header type and
    // account for padding if we have to.
    // 54 == BitmapInfoHeader
    // 122 == BitmapV4InfoHeader
    // 138 == BitMapV5InfoHeader
    switch (file_header_.data_offset) {
      case kDataOffsetV1:                 // 54
        output_stream.write((const char*)&info_header_, sizeof(info_header_));
        // Account for padding if width is not divisible by 4
        if (info_header_.width % 4 == 0) {
          output_stream.write((const char*)pixel_data_.data(), pixel_data_.size());
        }
        else {
          AddPaddingStream(output_stream, info_header_);
        }
        break;
      case kDataOffsetV4:                 // 122
        output_stream.write((const char*)&info_header_v4_, sizeof(info_header_v4_));
        if (info_header_v4_.width % 4 == 0) {
          output_stream.write((const char*)pixel_data_.data(), pixel_data_.size());
        }
        else {
          AddPaddingStream(output_stream, info_header_v4_);
        }
        break;
      case kDataOffsetV5:                 // 138
        output_stream.write((const char*)&info_header_v5_, sizeof(info_header_v5_));
        if (info_header_v5_.width % 4 == 0) {
          output_stream.write((const char*)pixel_data_.data(), pixel_data_.size());
        }
        else {
          AddPaddingStream(output_stream, info_header_v5_);
        }
        break;
      default:
        throw std::logic_error("Error: The BMP file might be corrupted. Please try another.");
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

bool BMP::CheckOutputFileExists(const std::string& filepath) {
  struct stat buf;
  if (stat(filepath.c_str(), &buf) != 0) {
    return 0;
  }
  else if (buf.st_mode & S_IFREG) {
    return 1;
  }
  else {
    return 0;
  }
}

// PadStream function that affect input, ignores
// padding while reading in pixel data.
template <typename InfoHeaderType>
void BMP::IgnorePaddingStream(std::ifstream& input_stream, const InfoHeaderType& info_header) {
  // Loop through each row while reading in data then padding
  for (size_t i = 0; i < info_header.height; i++) {
    input_stream.read((char*)(pixel_data_.data() + pixel_data_row_size_ * i), pixel_data_row_size_);
    input_stream.read((char*)padding_.data(), padding_.size());
  }
}

// PadStream functions that affect output, adds
// padding with zeros according to padding_ size.
template <typename InfoHeaderType>
void BMP::AddPaddingStream(std::ofstream& output_stream, const InfoHeaderType& info_header) {
  // Loop through each row while writing data then padding
  for (size_t i = 0; i < info_header.height; i++) {
    output_stream.write((char*)(pixel_data_.data() + pixel_data_row_size_ * i), pixel_data_row_size_);
    output_stream.write((char*)padding_.data(), padding_.size());
  }
}

// Read in BMP info header data into the appropriate
// info header type. Also validates info header fields
// to our program's assumptions (24 bpp, no compression).
template <typename InfoHeaderType>
void BMP::ReadInfoHeader(std::ifstream& input_stream, InfoHeaderType& info_header) {
  input_stream.read((char*)&info_header, sizeof(info_header));
  // Validate bits per pixel == 24, the only
  // bpp that we're handling in this program.
  if (info_header.bits_per_pixel != kBitsPerPixel)
    throw std::logic_error("Error: Your .BMP file is not 24 bits per pixel, please input a 24 bits per pixel file!");

  // Validate compression == 0, 0 means there
  // was no compression and we only want
  // uncompressed files.
  if (info_header.compression != kCompression)
    throw std::logic_error("Error: Your .BMP file has been compressed before, please input an uncompressed file!");

  // Calculate row size and padding of pixel array
  // for cases where the width of our image is not
  // divisible by 4.
  pixel_data_row_size_ = info_header.width * (info_header.bits_per_pixel / 8);
  CalculatePadding();

  // Moves input stream pointer to where our pixel 
  // array data begins.
  input_stream.seekg(file_header_.data_offset, input_stream.beg);
  // Allocate the size of the bitmap array
  pixel_data_.resize(info_header.width * info_header.height * (info_header.bits_per_pixel / 8));
  // When the pixel array is loaded into memory, 
  // each row must begin at a memory address 
  // that is a multiple of 4.
  if (info_header.width % 4 == 0) {
    input_stream.read((char*)pixel_data_.data(), pixel_data_.size());
  }
  // Else we have to pad the rows with 0's
  else {
    IgnorePaddingStream(input_stream, info_header);
  }
}