#pragma once
#include "Image.hpp"
#include <string>


/**
 * @brief Encode and write image to a file using ffmpeg.
 * @param outFile Path of the output file. The file extension determines the image format.
 * @return Exit status of ffmpeg. 0 for ok, non-zero for error.
 */
int ffEncode(const Image& img, const std::string& outFile, bool silent = true);
