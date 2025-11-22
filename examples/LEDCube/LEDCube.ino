#include <LEDCube.h>

LEDCube cube;

// State for animation
int voxelIndex = 0;
int colourPhase = 0;   // 0=red, 1=green, 2=blue
unsigned long lastStep = 0;
const unsigned long stepDelay = 100; // ms per voxel

void setup() {
}

void loop() {
    // Multiplex display
    cube.draw();  

    // Time to advance?
    unsigned long now = millis();
    if (now - lastStep < stepDelay) return;
    lastStep = now;

    // Clear frame
    cube.clear();

    // Pick colour based on current phase
    Colour c;
    if (colourPhase == 0)      c = Colour(65535,   0,     0); // Red
    else if (colourPhase == 1) c = Colour(   0, 65535,   0); // Green
    else                       c = Colour(   0,    0, 65535); // Blue

    // Light the current voxel
    cube.setVoxel(voxelIndex, c);

    // Advance voxel
    voxelIndex++;
    if (voxelIndex >= 64) {
        voxelIndex = 0;
        colourPhase++;
        if (colourPhase > 2) {
            colourPhase = 0;
        }
    }
}
