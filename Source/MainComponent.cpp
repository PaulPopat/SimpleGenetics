/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
using namespace std;

MainContentComponent::MainContentComponent() {
    setSize (800, 600);
    setAudioChannels (0, 2);
    LoadGraphs();
    population->SetName("Population Size");
    mAmount->SetName("Mutation Amount");
    mChance->SetName("Mutation Number");
    cInt->SetName("Capture Interval");
    target->SetName("Target Audio");
    startTimerHz(30);
}

MainContentComponent::~MainContentComponent() {
    shutdownAudio();
    controller->stopThread(10000);
    delete controller;
    delete settings;
    delete settingsPath;
    delete workingDirectory;
    
    DeleteDisplay(fftSize);
    DeleteDisplay(waveSize);
    DeleteDisplay(waveCount);
    DeleteDisplay(sampleRate);
    DeleteDisplay(channels);
    DeleteDisplay(factor);
    DeleteDisplay(closeness);
    DeleteDisplay(fileLength);
    DeleteDisplay(complete);
    DeleteDisplay(calculationsDone);
    
    delete loadSettings;
    delete saveSettings;
    delete runCalculation;
    delete stopCalculation;
    delete listen;
    delete saveAudio;
    
    delete population;
    delete mAmount;
    delete mChance;
    delete cInt;
    delete target;
}

void MainContentComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate) {
}

void MainContentComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) {
    bufferToFill.clearActiveBufferRegion();
}

void MainContentComponent::releaseResources() {
}

void MainContentComponent::paint (Graphics& g) {
    g.fillAll (Colour(25, 25, 25));
    DrawDisplay(g, settings->FFTSize, fftSize);
    DrawDisplay(g, settings->WaveSize, waveSize);
    DrawDisplay(g, settings->WaveCount, waveCount);
    DrawDisplay(g, settings->SampleRate, sampleRate);
    DrawDisplay(g, settings->Channels, channels);
    DrawDisplay(g, settings->Factor, factor);
    
    DrawDisplay(g, controller->GetCloseness(), closeness);
    DrawDisplay(g, controller->GetAudioLength(), fileLength);
    DrawDisplay(g, controller->GetPosition() * 100, complete);
    DrawDisplay(g, controller->GetLoc(), calculationsDone);
    
    loadSettings->Draw(g);
    saveSettings->Draw(g);
    runCalculation->Draw(g);
    stopCalculation->Draw(g);
    listen->Draw(g);
    saveAudio->Draw(g);
    
    population->Draw(g, controller->GetPosition());
    mAmount->Draw(g, controller->GetPosition());
    mChance->Draw(g, controller->GetPosition());
    cInt->Draw(g, controller->GetPosition());
    target->Draw(g, controller->GetPosition());
}

void MainContentComponent::mouseDown (const MouseEvent& e) {
    pair<int, int> mousePoint;
    mousePoint.first = e.getMouseDownX();
    mousePoint.second = e.getMouseDownY();
    if (loadSettings->PointIsInBox(mousePoint))
        LoadSettings();
    else if (saveSettings->PointIsInBox(mousePoint))
        SaveSettings();
    else if (runCalculation->PointIsInBox(mousePoint)) {
        controller->stopThread(10000);
        controller->startThread();
    }
    else if (stopCalculation->PointIsInBox(mousePoint))
        controller->stopThread(10000);
    else if (saveAudio->PointIsInBox(mousePoint))
        WriteAudio(controller->GetAudio(), settings->SampleRate);
    repaint();
}

void MainContentComponent::resized() {
    setSize (800, 600);
}

void MainContentComponent::timerCallback() {
    repaint();
}

bool MainContentComponent::LoadSettings() {
    FileChooser chooser("Load A File", File::nonexistent, "*.txt");
    if (chooser.browseForFileToOpen()) {
        delete settingsPath;
        settingsPath = new File(chooser.getResult());
        delete workingDirectory;
        workingDirectory = new File(settingsPath->getParentDirectory());
        delete settings;
        settings = new Settings(settingsPath);
        controller->LoadSettings(settings);
        LoadGraphs();
        return true;
    }
    return false;
}

bool MainContentComponent::SaveSettings() {
    delete settingsPath;
    delete workingDirectory;
    FileChooser chooser("Save A File", File::nonexistent, "*.txt");
    if (chooser.browseForFileToSave(true)) {
        settingsPath = new File(chooser.getResult());
        settings->Save(settingsPath);
        workingDirectory = new File(settingsPath->getParentDirectory());
        return true;
    }
    return false;
}

File MainContentComponent::SaveAudio() {
    FileChooser chooser("Save A File", File::nonexistent, "*.aif");
    while (true) {
        if (chooser.browseForFileToSave(true))
            return File(chooser.getResult());
    }
}

void MainContentComponent::Execute() {controller->startThread();}

void MainContentComponent::WriteAudio(vector<vector<double> > audio, int sampleRate) {
    for (int i = 0 ; i < audio.size() ; i++) {
        audio[i] = NormalizeAudio(audio[i]);
    }
    File path = SaveAudio();
    AiffAudioFormat wav;
    AudioFormatWriter *writer = wav.createWriterFor(path.createOutputStream(), sampleRate, audio.size(), 24, 1, 0);
    AudioSampleBuffer *buf = new AudioSampleBuffer(audio.size(), audio[0].size());
    for (int c = 0 ; c < audio.size() ; c++)
        for (int s = 0 ; s < audio[c].size() ; s++)
            buf->setSample(c, s, audio[c][s]);
    writer->writeFromAudioSampleBuffer(*buf, 0, buf->getNumSamples());
    delete writer;
    delete buf;
}

DrawBox *MainContentComponent::LoadBox(pair<int, int> size, pair<int, int> loc, String text) {
    DrawBox *val = new DrawBox(loc, size);
    val->SetFillColor(Colour(255, 120, 0));
    val->SetLineColor(Colour(0, 0, 0));
    val->SetFontSize(16);
    val->SetText(text);
    return val;
}

template <class t1, class t2>
pair<t1*, t2*> MainContentComponent::LoadDisplay(pair<int, int> size, pair<int, int> loc, String text) {
    pair<t1*, t2*> val = make_pair(new t1(make_pair(loc.first - (size.first / 4), loc.second), make_pair(size.first / 2, size.second)), new t2(make_pair(loc.first + (size.first / 4), loc.second), make_pair(size.first / 2, size.second)));
    val.first->SetFillColor(Colour(0, 0, 0));
    val.first->SetLineColor(Colour(255, 180, 0));
    val.first->SetFontSize(16);
    
    val.second->SetFillColor(Colour(75, 75, 75));
    val.second->SetLineColor(Colour(255, 180, 0));
    val.second->SetFontSize(16);
    val.second->SetText(text);
    return val;
}

template <class t1, class t2>
void MainContentComponent::DrawDisplay(Graphics& g, int Val, pair<t1*, t2*> data) {
    data.first->SetText(to_string(Val));
    data.first->Draw(g);
    data.second->Draw(g);
}

template <class t1, class t2>
void MainContentComponent::DeleteDisplay(pair<t1*, t2*> data) {
    delete data.first;
    delete data.second;
}

void MainContentComponent::LoadGraphs() {
    population->LoadGraph(settings->Population);
    mAmount->LoadGraph(settings->MutationAmount);
    mChance->LoadGraph(settings->MutationChance);
    cInt->LoadGraph(settings->CaptureInterval);
    vector<pair<String, double> > targets;
    for (int i = 0 ; i < settings->Target.size() ; i++) {
        targets.push_back(make_pair(settings->Target[i].first, settings->Target[i].second));
    }
    target->LoadGraph(targets);
}

Component* createMainContentComponent()     { return new MainContentComponent(); }
