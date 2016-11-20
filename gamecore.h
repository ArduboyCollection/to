#pragma once

#include "Arduboy.h"
#include "constants.h"

#define BTN_U (B00000001)
#define BTN_D (B00000010)
#define BTN_L (B00000100)
#define BTN_R (B00001000)
#define BTN_B (B00010000)
#define BTN_A (B00100000)

struct GameCore {
  bool soundOn = true;
  
  void setup() {
    arduboy.beginNoLogo();
    nowInput  = 0x00;
    prevInput = 0xff;
  }

  void updateInput() {
    prevInput = nowInput;
    nowInput  = 0;
    if(arduboy.pressed(UP_BUTTON   )) { nowInput |= BTN_U; }
    if(arduboy.pressed(DOWN_BUTTON )) { nowInput |= BTN_D; }
    if(arduboy.pressed(LEFT_BUTTON )) { nowInput |= BTN_L; }
    if(arduboy.pressed(RIGHT_BUTTON)) { nowInput |= BTN_R; }
    if(arduboy.pressed(B_BUTTON    )) { nowInput |= BTN_A; }
    if(arduboy.pressed(A_BUTTON    )) { nowInput |= BTN_B; }
  }
  inline bool pressed(byte button) const {
    return (nowInput & button) != 0;
  }
  inline bool pushed(byte button) const {
    return (nowInput & button) != 0 && (prevInput & button) == 0;
  }
  inline bool hold(byte button) const {
    return !((nowInput & button) == 0 && (prevInput & button) == 0);
  }
#ifndef LOW_FLASH_MEMORY
  void updateQuake() {
    if(quakeForce > 0.5f) {
      quakeForce *= 0.85f;
      float dir = random(256) * PI * 2 / 256;
      qx = round(quakeForce * cos(dir));
      qy = round(quakeForce * sin(dir));
    }
    else {
      qx = qy = 0;
    }
  }
  inline void setQuake(float force = 10) { quakeForce = force; }
#endif

  // === arduboy delegation ===
  // if you build for another hardware, please implement these functions
  inline bool nextFrame() { return arduboy.nextFrame(); }
  inline void clear() { arduboy.clear(); }
  inline void display() { arduboy.display(); }
  inline int frameCount() { return arduboy.frameCount; }
  inline void drawPixel(int x, int y, byte c) { arduboy.drawPixel(x + qx, y + qy, c); }
  inline void drawBitmap(int x, int y, const byte* bitmap, byte c) {
    arduboy.drawBitmap(x + qx, y + qy, bitmap+2, pgm_read_byte(bitmap), pgm_read_byte(bitmap+1), c);
  }
  inline void fillRect(int x, int y, byte w, byte h, byte c) {
    arduboy.fillRect(x + qx, y + qy, w, h, c);
  }
  inline void setCursor(int x, int y) { arduboy.setCursor(x, y); }
  inline void print(const char* text) { arduboy.print(text); }
  inline bool playing() { return arduboy.tunes.playing(); }
  void playScore(const byte* score) {
    if(soundOn) {
      arduboy.tunes.playScore(score);
    }
  }
  inline void stopScore() { arduboy.tunes.stopScore(); }
  void tone(const unsigned int f, const unsigned long d) {
    if(soundOn) {
      arduboy.tunes.tone(f, d);
    }
  }
  // ===
  
#ifdef DEBUG
  inline Arduboy& getArduboy() { return arduboy; }
#endif
  
  public:
  Arduboy arduboy;
  private:
  byte    nowInput;
  byte    prevInput;
  float   quakeForce;
  char    qx, qy;
};

