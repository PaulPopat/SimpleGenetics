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
#include "InputGraph.h"
#include "TargetDisplay.h"
#include <vector>
#include <utility>

class MainContentComponent :
public AudioAppComponent, private Timer, public TextEditorListener, public ButtonListener {
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
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent&) override;
    void textEditorReturnKeyPressed(TextEditor &editor) override;
    void textEditorEscapeKeyPressed(TextEditor &editor) override;
    void buttonClicked(Button *button);
    
    
private:
    Settings *settings = new Settings();
    GeneController *controller = new GeneController(settings);
    File *settingsPath = new File();
    File *workingDirectory = new File(File::getCurrentWorkingDirectory());
    bool isPlaying = false;
    int audioPos = 0;
    double maxLevelVal = 0;
    std::vector<std::vector<double> > audio;
    
    TextEditor *popHigh = new TextEditor("popHigh");
    TextEditor *mAmountHigh = new TextEditor("mAmountHigh");
    TextEditor *mChanceHigh = new TextEditor("mChanceHigh");
    TextEditor *cIntHigh = new TextEditor("cIntHigh");
    
    InputGraph* population = new InputGraph(255, 55, 500, 100, Colour(0, 0, 0), Colour(255, 180, 0), popHigh);
    InputGraph* mAmount = new InputGraph(255, 160, 500, 100, Colour(0, 0, 0), Colour(255, 180, 0), mAmountHigh);
    InputGraph* mChance = new InputGraph(255, 265, 500, 100, Colour(0, 0, 0), Colour(255, 180, 0), mChanceHigh);
    InputGraph* cInt = new InputGraph(255, 370, 500, 100, Colour(0, 0, 0), Colour(255, 180, 0), cIntHigh);
    TargetDisplay* target = new TargetDisplay(255, 475, 500, 100, Colour(0, 0, 0), Colour(255, 180, 0));
    TextButton* loadSettings = LoadButton(150, 30, 720, 245, "Load Settings");
    TextButton* saveSettings = LoadButton(150, 30, 720, 280, "Save Settings");
    TextButton* runCalculation = LoadButton(150, 30, 720, 315, "Run Calculation");
    TextButton* stopCalculation = LoadButton(150, 30, 720, 350, "Stop Calculation");
    ToggleButton* listen = LoadToggle(150, 30, 720, 385, "Monitor Output");
    TextButton* saveAudio = LoadButton(150, 30, 720, 420, "Save Audio");
    DrawBox* maxLevel = LoadBox(100, 30, 400, 580, "Audio Level");
    
    std::pair<TextEditor*,DrawBox*> fftSize = LoadEditor(240, 30, 675, 20, "FFT Size", "FFTSize");
    std::pair<TextEditor*,DrawBox*> waveSize = LoadEditor(240, 30, 675, 55, "Bins Per Frame", "WaveSize");
    std::pair<TextEditor*,DrawBox*> waveCount = LoadEditor(240, 30, 675, 90, "Frames To Make", "WaveCount");
    std::pair<TextEditor*,DrawBox*> sampleRate = LoadEditor(240, 30, 675, 125, "Sample Rate", "SampleRate");
    std::pair<TextEditor*,DrawBox*> channels = LoadEditor(240, 30, 675, 160, "Channels", "Channels");
    std::pair<TextEditor*,DrawBox*> factor = LoadEditor(240, 30, 675, 195, "Factor", "Factor");
    
    std::pair<DrawBox*,DrawBox*> closeness = LoadDisplay(260, 30, 665, 580, "Close To Target");
    std::pair<DrawBox*,DrawBox*> fileLength = LoadDisplay(260, 30, 665, 545, "File Length (sec)");
    std::pair<DrawBox*,DrawBox*> complete = LoadDisplay(260, 30, 665, 510, "Percent Complete");
    std::pair<DrawBox*,DrawBox*> calcsDone = LoadDisplay(260, 30, 665, 475, "Frames Complete");
    
    void Execute();
    
    bool LoadSettings();
    bool SaveSettings();
    void SaveAudio(std::vector<std::vector<double> > Audio, int SampleRate);
    File LoadAudio();
    void InsertAudio(int i, const MouseEvent& e, double pos);
    
    DrawBox *LoadBox(int sx, int sy, int lx, int ly, String text);
    TextButton *LoadButton(int sx, int sy, int lx, int ly, String text);
    ToggleButton *LoadToggle(int sx, int sy, int lx, int ly, String text);
    std::pair<DrawBox*,DrawBox*> LoadDisplay(int sx, int sy, int lx, int ly, String text);
    std::pair<TextEditor*,DrawBox*> LoadEditor(int sx, int sy, int lx, int ly, String text, String name);
    void DrawDisplay(Graphics& g, int Val, std::pair<DrawBox*, DrawBox*> data);
    template <class t1, class t2>
    void DeleteDisplay(std::pair<t1*, t2*> data);
    
    void LoadGraphs();
    void LoadSettingsText();
    void CheckSettings(TextEditor &editor, int &setting);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};



#endif  // MAINCOMPONENT_H_INCLUDED
