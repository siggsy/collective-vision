#pragma once
#include "Image.hpp"
#include <string>


/**
 * @brief Encode and write image to a file using ffmpeg.
 * @return Exit status of ffmpeg. 0 for ok, non-zero for error.
 */
int ffEncode(const Image& img, const std::string& outFile, bool silent = true);
