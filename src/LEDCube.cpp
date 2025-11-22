#include <LEDCube.h>
LEDCube::LEDCube(){

  Tlc5948 tlc;
  uint16_t LEDCube::gs[NUM_DRAINS] = {0};
  Colour   LEDCube::frame[FRAMESIZE];
  
  //setup layers
  pinMode(LAYER_1, OUTPUT);
  pinMode(LAYER_2, OUTPUT);
  pinMode(LAYER_3, OUTPUT);
  pinMode(LAYER_4, OUTPUT);
  
  //disable outputs, inverse logic
  digitalWrite(LAYER_1, 1);
  digitalWrite(LAYER_2, 1);
  digitalWrite(LAYER_3, 1);
  digitalWrite(LAYER_4, 1);

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

void LEDCube::setVoxel(int x, int y, int z, const Colour &rgb){

}

void LEDCube::setVoxel(int index, const Colour &rgb){
  if(index >= 0 && index < FRAMESIZE){
    frame[index] = rgb;
  }
}

void LEDCube::setVoxel(const Coord &xyz, const Colour &rgb){
  setVoxel(xyz.x, xyz,y, xyz.z, rgb);
}

void LEDCube::clear(){
  for (int i=0;i<NUM_DRAINS;i++){
    gs[i]=0;
  }
}

void LEDCube::draw(){
  
}

