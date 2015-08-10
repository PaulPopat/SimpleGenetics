/*
  ==============================================================================

    MainComponent.h
    Created: 9 Aug 2015 11:46:21am
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings.h"
#include "GeneController.h"
#include "DrawBox.h"
#include "NumberBox.h"
#include "InputGraph.h"
#include "TargetDisplay.h"
#include <vector>
#include <utility>

class MainContentComponent   : public AudioAppComponent, private Timer {
public:
    MainContentComponent();
    ~MainContentComponent();
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void paint (Graphics& g) override;
    void resized() override;
    void timerCallback() override;
    void mouseDown (const MouseEvent& e) override;
    
    
private:
    Settings *settings = new Settings();
    GeneController *controller = new GeneController(settings);
    File *settingsPath = new File();
    File *workingDirectory = new File();
    
    InputGraph* population = new InputGraph(std::make_pair(255, 55), std::make_pair(500, 100), Colour(0, 0, 0), Colour(255, 180, 0));
    InputGraph* mAmount = new InputGraph(std::make_pair(255, 160), std::make_pair(500, 100), Colour(0, 0, 0), Colour(255, 180, 0));
    InputGraph* mChance = new InputGraph(std::make_pair(255, 265), std::make_pair(500, 100), Colour(0, 0, 0), Colour(255, 180, 0));
    InputGraph* cInt = new InputGraph(std::make_pair(255, 370), std::make_pair(500, 100), Colour(0, 0, 0), Colour(255, 180, 0));
    TargetDisplay* target = new TargetDisplay(std::make_pair(255, 475), std::make_pair(500, 100), Colour(0, 0, 0), Colour(255, 180, 0));
    DrawBox* loadSettings = LoadBox(std::make_pair(150, 30), std::make_pair(720, 245), "Load Settings");
    DrawBox* saveSettings = LoadBox(std::make_pair(150, 30), std::make_pair(720, 280), "Save Settings");
    DrawBox* runCalculation = LoadBox(std::make_pair(150, 30), std::make_pair(720, 315), "Run Calculation");
    DrawBox* stopCalculation = LoadBox(std::make_pair(150, 30), std::make_pair(720, 350), "Stop Calculation");
    DrawBox* listen = LoadBox(std::make_pair(150, 30), std::make_pair(720, 385), "Playback Product");
    DrawBox* saveAudio = LoadBox(std::make_pair(150, 30), std::make_pair(720, 420), "Save Audio");
    
    std::pair<DrawBox*, DrawBox*> fftSize = LoadDisplay<DrawBox, DrawBox>(std::make_pair(240, 30), std::make_pair(675, 20), "FFT Size");
    std::pair<NumberBox*, DrawBox*> waveSize = LoadDisplay<NumberBox, DrawBox>(std::make_pair(240, 30), std::make_pair(675, 55), "Bins Per Frame");
    std::pair<NumberBox*, DrawBox*> waveCount = LoadDisplay<NumberBox, DrawBox>(std::make_pair(240, 30), std::make_pair(675, 90), "Frames To Make");
    std::pair<DrawBox*, DrawBox*> sampleRate = LoadDisplay<DrawBox, DrawBox>(std::make_pair(240, 30), std::make_pair(675, 125), "Sample Rate");
    std::pair<NumberBox*, DrawBox*> channels = LoadDisplay<NumberBox, DrawBox>(std::make_pair(240, 30), std::make_pair(675, 160), "Channels");
    std::pair<NumberBox*, DrawBox*> factor = LoadDisplay<NumberBox, DrawBox>(std::make_pair(240, 30), std::make_pair(675, 195), "Factor");
    
    std::pair<DrawBox*, DrawBox*> closeness = LoadDisplay<DrawBox, DrawBox>(std::make_pair(260, 30), std::make_pair(665, 580), "Close To Target");
    std::pair<DrawBox*, DrawBox*> fileLength = LoadDisplay<DrawBox, DrawBox>(std::make_pair(260, 30), std::make_pair(665, 545), "File Length (sec)");
    std::pair<DrawBox*, DrawBox*> complete = LoadDisplay<DrawBox, DrawBox>(std::make_pair(260, 30), std::make_pair(665, 510), "Percent Complete");
    std::pair<DrawBox*, DrawBox*> calculationsDone = LoadDisplay<DrawBox, DrawBox>(std::make_pair(260, 30), std::make_pair(665, 475), "Frames Complete");
    
    bool LoadSettings();
    bool SaveSettings();
    File SaveAudio();
    void Execute();
    void WriteAudio(std::vector<std::vector<double> > audio, int sampleRate);
    DrawBox *LoadBox(std::pair<int, int> size, std::pair<int, int> loc, String text);
    
    template <class t1, class t2>
    std::pair<t1*, t2*> LoadDisplay(std::pair<int, int> size, std::pair<int, int> loc, String text);
    template <class t1, class t2>
    void DrawDisplay(Graphics& g, int Val, std::pair<t1*, t2*> data);
    template <class t1, class t2>
    void DeleteDisplay(std::pair<t1*, t2*> data);
    
    void LoadGraphs();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};



#endif  // MAINCOMPONENT_H_INCLUDED
