/*
  SDL_RasPiGraphLibrary.cpp - Class Library for RasPiConnect Graph Format.
  Created by SwitchDoc Labs August 4, 2014.
  Released into the public domain.
*/

#include "Arduino.h"


#include "SDL_RasPiGraphLibrary.h"

SDL_RasPiGraphLibrary::SDL_RasPiGraphLibrary(int maxItemCount, int Mode)
{
 
  _Mode = SDL_MODE_LABELS;
  
   _currentItemCount = 0;
   _maxItemCount = maxItemCount;
   // set up the value array
  // calculate the size of the buffer to be able to store the number of elements
  // allocate enough memory for "newelements" number of elements
  
   _myFloatArray =  (float *)malloc ( _maxItemCount  * sizeof(float) );

  
}

int SDL_RasPiGraphLibrary::add_value(float value)
{
  
  if (_currentItemCount == _maxItemCount)
  {
      // we need to remove the first and add the last
      int i;
      for (i=1; i < _currentItemCount; i++)
      {
        _myFloatArray[i-1] = _myFloatArray[i];
      }
      _myFloatArray[_currentItemCount-1] = value;
  }
  else
  {
    // just add it on
    _myFloatArray[_currentItemCount] = value;
    _currentItemCount ++;
  }
}

void SDL_RasPiGraphLibrary::getRasPiString(char returnRasPiString[], char *buffer)
{
      int i;
      strcpy(returnRasPiString, "");

      for (i=0; i < _currentItemCount; i++)
      {
//        Serial.print("_currentItemCount=");
//        Serial.println(_currentItemCount);

        char tempString[30];
        char floatString[20];
        dtostrf( (double)_myFloatArray[i],6,2,floatString);
        sprintf(tempString,"%s^^",floatString);
        //strcat(returnRasPiString, tempString);
        strcat(returnRasPiString, tempString);

        //Serial.print("returnRasPiString =");
        //Serial.println(returnRasPiString);
      }
      if (strlen(returnRasPiString) > 2)
      {
          returnRasPiString[strlen(returnRasPiString)-2] = '\0';	
      }
      // now add the labels
      
      strcat(returnRasPiString, "||");
      
      for(i=0; i < _currentItemCount; i++)
      {
        char tempString[20];
        sprintf(tempString, "%i^^", i);
        strcat(returnRasPiString, tempString);
        
      }
      
      if (strlen(returnRasPiString) > 2)
      {

       returnRasPiString[strlen(returnRasPiString)-2] = '\0';
      }
//      Serial.print("returnRasPiString=");
//      Serial.println(returnRasPiString);

      strcpy(returnRasPiString, buffer);
      
}
    

void SDL_RasPiGraphLibrary::supply_labels(char *labels)
{
  
}

float SDL_RasPiGraphLibrary::returnMaxValue()
{
#define MINFLOAT ((float)-3.40282346638528860e+38)
      float Maximum;
      Maximum = MINFLOAT;
      int i;
      for (i=0; i < _currentItemCount; i++)
      {
        if (_myFloatArray[i] > Maximum)
        {
          Maximum = _myFloatArray[i];
        }
      }
      return Maximum;
  
}
      
float SDL_RasPiGraphLibrary::returnMinValue()
{

      
      float Minimum;
      Minimum = MAXFLOAT;
      int i;
      for (i=0; i < _currentItemCount; i++)
      {
        if (_myFloatArray[i] < Minimum)
        {
          Minimum = _myFloatArray[i];
        }
      }
      return Minimum;
}


