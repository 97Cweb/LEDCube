#pragma once

#include <Arduino.h>
#include "Tlc5948.h"

constexpr int NUM_TLCS      = 3;
constexpr int NUM_DRAINS  = 16 * NUM_TLCS; // GS channels per TLC5948
constexpr int FRAMESIZE = 64;              // 4x4x4 cube = 64 voxels

// A single RGB colour (12-bit is typical for TLC5948, but we can store 16-bit)
struct Colour {
    uint16_t r;
    uint16_t g;
    uint16_t b;

    // handy constructors
    Colour() : r(0), g(0), b(0) {}
    Colour(uint16_t r_, uint16_t g_, uint16_t b_) : r(r_), g(g_), b(b_) {}
};

// 3D coordinate in the cube
struct Coord {
    uint8_t x;
    uint8_t y;
    uint8_t z;

    Coord() : x(0), y(0), z(0) {}
    Coord(uint8_t x_, uint8_t y_, uint8_t z_) : x(x_), y(y_), z(z_) {}
};  


class LEDCube{
  public:
    LEDCube(uint8_t layer0Pin = 19,
            uint8_t layer1Pin = 18,
            uint8_t layer2Pin = 15,
            uint8_t layer3Pin = 14);
    void setVoxel(int x, int y, int z, const Colour &rgb);
    void setVoxel(int index, const Colour &rgb); // 0..63
    void setVoxel(const Coord &xyz, const Colour &rgb);
    void setFrame(const Colour* frameData, size_t count = FRAMESIZE);
    void clear();
    
    void draw();
    
  private:
  uint8_t _layerPins[4];
    Tlc5948 tlc;
    uint16_t gs[NUM_DRAINS];
    Colour frame[FRAMESIZE];
    uint8_t currentLayer = 0; 
    
};
