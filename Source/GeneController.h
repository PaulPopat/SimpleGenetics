/*
  ==============================================================================

    GeneController.h
    Created: 5 Aug 2015 12:00:06pm
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef GENECONTROLLER_H_INCLUDED
#define GENECONTROLLER_H_INCLUDED
#include <vector>
#include <utility>
#include "../JuceLibraryCode/JuceHeader.h"
#include "PopMember.h"
#include "Settings.h"

class GeneController    : public Thread
{
public:
    GeneController(Settings *Setting);
    ~GeneController();
    void run() override;
    double GetPosition();
    double GetCloseness();
    int GetLoc();
    int GetAudioLength();
    std::vector<std::vector<double> > GetAudio();
    void LoadSettings(Settings *Setting);
private:
    double RandomVal(double min, double max, double weighting);
    bool RandomBool();
    static bool Sorter(const std::pair<int, double> &left, const std::pair<int, double> &right);
    int loc = 0;
    double pos = 0;
    double cPos = 0;
    Settings *settings;
    std::vector<std::pair<int, double> > closeness;
    std::vector<PopMember*> population;
    PopMember *final = new PopMember;
};


#endif  // GENECONTROLLER_H_INCLUDED
