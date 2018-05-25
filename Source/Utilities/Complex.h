/*
  ==============================================================================

    Complex.h
    Created: 25 May 2018 7:43:12pm
    Author:  paul

  ==============================================================================
*/

#pragma once

class Complex
{
public:
  Complex(int real, int imaginery)
  {
    this->Real = real;
    this->Imaginery = imaginery;
  }

  int Real;
  int Imaginery;
};
