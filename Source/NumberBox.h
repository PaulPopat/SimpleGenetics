/*
  ==============================================================================

    NumberBox.h
    Created: 9 Aug 2015 10:01:21am
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef NUMBERBOX_H_INCLUDED
#define NUMBERBOX_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include <utility>
#include "DrawBox.h"

class NumberBox    : public DrawBox
{
public:
    NumberBox(std::pair<double, double> location, std::pair<double, double> size);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NumberBox)
};


#endif  // NUMBERBOX_H_INCLUDED
