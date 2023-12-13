#pragma once
#include <vector>
#include <memory>


class Boid;
class Image;


// ----------------------------------- [ Functions ] ---------------------------------------- //


void generateFrame(const std::vector<std::unique_ptr<Boid>>& data, Image& outImg, int scale);


// ------------------------------------------------------------------------------------------ //