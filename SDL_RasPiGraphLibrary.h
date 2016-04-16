/*
  SDL_RasPiGraphLibrary.h - Class Library for RasPiConnect Graph Format.
  Created by SwitchDoc Labs August 5, 2014.
  Released into the public domain.

*/
#ifndef  SDL_RasPiGraphLibrary_h
#define SDL_RasPiGraphLibrary_h



#define SDL_MODE_NO_LABELS 0
#define SDL_MODE_LABELS 1


#include "Arduino.h"

class SDL_RasPiGraphLibrary
{
  public:
    SDL_RasPiGraphLibrary(int itemCount, int Mode);
  
    int add_value(float value);
    void getRasPiString(char returnRasPiString[], char *buffer);

    void supply_labels(char *labels);
    
    float returnMaxValue();
    float returnMinValue();
  
  
  private:
  
    float *_myFloatArray;
    int _currentItemCount;
    int _Mode;
    int _maxItemCount;


};

#endif

