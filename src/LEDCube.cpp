#include <LEDCube.h>
LEDCube::LEDCube( uint8_t layer0Pin,
                  uint8_t layer1Pin,
                  uint8_t layer2Pin,
                  uint8_t layer3Pin){
                  
  // Save layer pins into the instance
  _layerPins[0] = layer0Pin;
  _layerPins[1] = layer1Pin;
  _layerPins[2] = layer2Pin;
  _layerPins[3] = layer3Pin;

  for (int i = 0; i < 4; ++i) {
    pinMode(_layerPins[i], OUTPUT);
    digitalWrite(_layerPins[i], HIGH);
  }
  
  clear();                // clear logical frame
  for (int i = 0; i < NUM_DRAINS; ++i) {
    gs[i] = 0;
  }
  
  tlc.begin(/*usingSPI*/ true, /*numberTlcs*/ NUM_TLCS);

  tlc.setDcData(Channels::all, 0x7f);
  tlc.setBcData(0x7f);

  Fctrls f = tlc.getFctrlBits();
  f &= ~(Fctrls::dsprpt_mask);  f |= Fctrls::dsprpt_mode_1;
  f &= ~(Fctrls::tmgrst_mask);  f |= Fctrls::tmgrst_mode_1;
  f &= ~(Fctrls::espwm_mask);   f |= Fctrls::espwm_mode_1;
  f &= ~(Fctrls::blank_mask);   f |= Fctrls::blank_mode_0;
  tlc.setFctrlBits(f);

  tlc.writeControlBufferSPI();
  tlc.startBuiltinGsclk();

  
  
  tlc.writeGsBufferSPI16(gs, NUM_DRAINS);
}

static inline int xyzToIndex(int x, int y, int z){
  if (x < 0 || x > 3 || y < 0 || y > 3 || z < 0 || z > 3) {
    return -1;
  }
  return z*16 + y*4 + x;  
}

void LEDCube::setVoxel(int x, int y, int z, const Colour &rgb){
  int idx = xyzToIndex(x,y,z);
  if(idx < 0){
    return;
  }
  setVoxel(idx,rgb);
}

void LEDCube::setVoxel(int index, const Colour &rgb){
  if(index >= 0 && index < FRAMESIZE){
    frame[index] = rgb;
  }
}

void LEDCube::setVoxel(const Coord &xyz, const Colour &rgb){
  setVoxel(xyz.x, xyz.y, xyz.z, rgb);
}

void LEDCube::clear(){
  for (int i=0;i<FRAMESIZE;i++){
    frame[i]=Colour(0,0,0);
  }
}

void LEDCube::draw() {
    // --- A) BLANK OUTPUTS ---  
    Fctrls f = tlc.getFctrlBits();
    f &= ~(Fctrls::blank_mask);
    f |= Fctrls::blank_mode_1;      // force outputs off
    tlc.setFctrlBits(f);
    tlc.writeControlBufferSPI();
    
    


    // --- B) Turn all layers off ---
    for (int i = 0; i < 4; ++i) {
        digitalWrite(_layerPins[i], HIGH);
    }
    
    
    int baseIndex = currentLayer * 16;

    // --- C) Fill gs buffer ---
    for (int i = 0; i < 16; ++i) {
        const Colour &c = frame[baseIndex + i];
        gs[i      ] = c.r;
        gs[i + 16 ] = c.g;
        gs[i + 32 ] = c.b;
    }
    for (int ch = 48; ch < NUM_DRAINS; ch++){
      gs[ch] = 0;
    }
    
    tlc.writeGsBufferSPI16(gs, NUM_DRAINS);

    // --- D) Enable this layer ---
    digitalWrite(_layerPins[currentLayer], LOW);
    

    // --- E) UNBLANK OUTPUTS ---
    f &= ~(Fctrls::blank_mask);
    f |= Fctrls::blank_mode_0;      // normal operation
    tlc.setFctrlBits(f);
    tlc.writeControlBufferSPI();

    // next layer
    currentLayer = (currentLayer + 1) % 4;
}



