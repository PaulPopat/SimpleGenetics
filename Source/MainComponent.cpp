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
    LoadSettingsText();
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
    DeleteDisplay(calcsDone);
    
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
    if (!isPlaying || audio.size() < 1) {
        bufferToFill.clearActiveBufferRegion();
        audio = controller->GetCurrentAudio();
        for (int c = 0 ; c < audio.size() ; c++) {
            NormalizeAudio(audio[c]);
        }
        return;
    }
    for (int i = 0 ; i < bufferToFill.numSamples ; i++) {
        if (audioPos >= audio[0].size()) {
            audio = controller->GetCurrentAudio();
            for (int c = 0 ; c < audio.size() ; c++) {
                NormalizeAudio(audio[c]);
            }
            audioPos = 0;
        }
        for (int c = 0 ; c < bufferToFill.buffer->getNumChannels() ; c++) {
            int channel = c;
            if (channel >= audio.size())
                channel = audio.size() - 1;
            bufferToFill.buffer->setSample(c, i, audio[c][audioPos]);
        }
        audioPos++;
    }
}

void MainContentComponent::releaseResources() {
}

void MainContentComponent::paint (Graphics& g) {
    g.fillAll (Colour(25, 25, 25));
    fftSize.second->Draw(g);
    waveSize.second->Draw(g);
    waveCount.second->Draw(g);
    sampleRate.second->Draw(g);
    channels.second->Draw(g);
    factor.second->Draw(g);
    
    DrawDisplay(g, controller->GetCloseness(), closeness);
    DrawDisplay(g, controller->GetAudioLength(), fileLength);
    DrawDisplay(g, controller->GetPosition() * 100, complete);
    DrawDisplay(g, controller->GetLoc(), calcsDone);
    
    loadSettings->Draw(g);
    saveSettings->Draw(g);
    runCalculation->Draw(g);
    stopCalculation->Draw(g);
    if (isPlaying)
        listen->SetFillColor(Colour(200, 200, 200));
    else
        listen->SetFillColor(Colour(255, 120, 0));
    listen->Draw(g);
    saveAudio->Draw(g);
    
    population->Draw(g, controller->GetPosition());
    mAmount->Draw(g, controller->GetPosition());
    mChance->Draw(g, controller->GetPosition());
    cInt->Draw(g, controller->GetPosition());
    target->Draw(g, controller->GetPosition());
}

void MainContentComponent::mouseDown (const MouseEvent& e) {
    pair<int,int> mousePoint;
    mousePoint.first = e.getMouseDownX();
    mousePoint.second = e.getMouseDownY();
    if (loadSettings->PointIsInBox(mousePoint))
        LoadSettings();
    else if (saveSettings->PointIsInBox(mousePoint))
        SaveSettings();
    else if (runCalculation->PointIsInBox(mousePoint)) {
        controller->stopThread(10000);
        controller->LoadSettings(settings);
        controller->startThread();
    }
    else if (stopCalculation->PointIsInBox(mousePoint))
        controller->stopThread(10000);
    else if (saveAudio->PointIsInBox(mousePoint))
        SaveAudio(controller->GetAudio(), settings->SampleRate);
    else if (listen->PointIsInBox(mousePoint))
        isPlaying = !isPlaying;
    else {
        population->Click(mousePoint, e.mods.isAltDown(), e.mods.isCtrlDown());
        mAmount->Click(mousePoint, e.mods.isAltDown(), e.mods.isCtrlDown());
        mChance->Click(mousePoint, e.mods.isAltDown(), e.mods.isCtrlDown());
        cInt->Click(mousePoint, e.mods.isAltDown(), e.mods.isCtrlDown());
        double pos = target->Click(mousePoint, e.mods.isAltDown(), e.mods.isCtrlDown());
        if (pos >= 0) {
            for (int i = settings->Target.size() - 1 ; i >= 0 ; i--) {
                if (pos < settings->Target[i].second + 0.05 &&
                    pos > settings->Target[i].second - 0.05) {
                    settings->Target.erase(settings->Target.begin() + i);
                    InsertAudio(i, e, pos);
                    return;
                }
            }
            for (int i = settings->Target.size() - 1 ; i >= 0 ; i--) {
                if (pos < settings->Target[i].second) {
                    InsertAudio(i, e, pos);
                    return;
                }
            }
        }
    }
    repaint();
}

void MainContentComponent::mouseDrag (const MouseEvent& e) {
    pair<double, double> mousePoint;
    mousePoint.first = e.x;
    mousePoint.second = e.y;
    population->Drag(mousePoint);
    mAmount->Drag(mousePoint);
    mChance->Drag(mousePoint);
    cInt->Drag(mousePoint);
    target->Drag(mousePoint);
    repaint();
}

void MainContentComponent::mouseUp (const MouseEvent&) {
    settings->Population = population->ClickUp();
    settings->MutationAmount = mAmount->ClickUp();
    settings->MutationChance = mChance->ClickUp();
    settings->CaptureInterval = cInt->ClickUp();
    settings->Target = target->ClickUp();
    settings->LoadTargets();
    repaint();
}

void MainContentComponent::textEditorReturnKeyPressed(TextEditor &editor) {
    if (editor.getName() == "FFTSize")
        CheckSettings(editor, settings->FFTSize);
    else if (editor.getName() == "WaveSize")
        CheckSettings(editor, settings->WaveSize);
    else if (editor.getName() == "WaveCount")
        CheckSettings(editor, settings->WaveCount);
    else if (editor.getName() == "SampleRate")
        CheckSettings(editor, settings->SampleRate);
    else if (editor.getName() == "Channels")
        CheckSettings(editor, settings->Channels);
    else if (editor.getName() == "Factor")
        CheckSettings(editor, settings->Factor);
    else if (editor.getName() == "popHigh")
        population->textEditorReturnKeyPressed(editor);
    else if (editor.getName() == "mAmountHigh")
        mAmount->textEditorReturnKeyPressed(editor);
    else if (editor.getName() == "mChanceHigh")
        mChance->textEditorReturnKeyPressed(editor);
    else if (editor.getName() == "cIntHigh")
        cInt->textEditorReturnKeyPressed(editor);
    editor.unfocusAllComponents();
}

void MainContentComponent::textEditorEscapeKeyPressed(TextEditor &editor) {
    if (editor.getName() == "FFTSize")
        editor.setText(to_string(settings->FFTSize));
    else if (editor.getName() == "WaveSize")
        editor.setText(to_string(settings->WaveSize));
    else if (editor.getName() == "WaveCount")
        editor.setText(to_string(settings->WaveCount));
    else if (editor.getName() == "SampleRate")
        editor.setText(to_string(settings->SampleRate));
    else if (editor.getName() == "Channels")
        editor.setText(to_string(settings->Channels));
    else if (editor.getName() == "Factor")
        editor.setText(to_string(settings->Factor));
    else if (editor.getName() == "popHigh")
        population->textEditorEscapeKeyPressed(editor);
    else if (editor.getName() == "mAmountHigh")
        mAmount->textEditorEscapeKeyPressed(editor);
    else if (editor.getName() == "mChanceHigh")
        mChance->textEditorEscapeKeyPressed(editor);
    else if (editor.getName() == "cIntHigh")
        cInt->textEditorEscapeKeyPressed(editor);
    editor.unfocusAllComponents();
}

void MainContentComponent::resized() {
    setSize (800, 600);
}

void MainContentComponent::timerCallback() {
    repaint();
}

void MainContentComponent::Execute() {controller->startThread();}

//************************************************************
// utilities
//************************************************************

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
        LoadSettingsText();
        return true;
    }
    return false;
}

bool MainContentComponent::SaveSettings() {
    FileChooser chooser("Save A File", File::nonexistent, "*.txt");
    if (chooser.browseForFileToSave(true)) {
        delete settingsPath;
        delete workingDirectory;
        settingsPath = new File(chooser.getResult());
        settings->Save(settingsPath);
        workingDirectory = new File(settingsPath->getParentDirectory());
        return true;
    }
    return false;
}

void MainContentComponent::SaveAudio(vector<vector<double> > Audio, int SampleRate) {
    FileChooser chooser("Save Audio Output", File::nonexistent, "*.aif");
    while (true) {
        if (chooser.browseForFileToSave(true))
            WriteAudio(Audio, SampleRate, File(chooser.getResult()));
    }
}

File MainContentComponent::LoadAudio() {
    FileChooser chooser("Load Target Audio", File::nonexistent, "*.aif");
    if (chooser.browseForFileToOpen())
        return File(chooser.getResult());
    return File();
}

void MainContentComponent::InsertAudio(int i, const MouseEvent& e, double pos) {
    if (workingDirectory->getFullPathName() == File::getCurrentWorkingDirectory().getFullPathName()) {
        if(!SaveSettings())
            return;
    }
    if (e.mods.isCtrlDown()) {
        settings->Target.insert(settings->Target.begin() + i, make_pair("Silence", pos));
        LoadGraphs();
        return;
    }
    File toCopy = LoadAudio();
    if (!toCopy.exists()) {
        return;
    }
    String name = toCopy.getFileName();
    settings->Target.insert(settings->Target.begin() + i, make_pair(name.toRawUTF8(), pos));
    CopyAudio(toCopy, *workingDirectory);
    LoadGraphs();
}

//****************************************************************
// box positioning
//****************************************************************

DrawBox *MainContentComponent::LoadBox(int sx, int sy, int lx, int ly, String text) {
    DrawBox *val = new DrawBox(make_pair(lx, ly), make_pair(sx, sy));
    val->SetFillColor(Colour(255, 120, 0));
    val->SetLineColor(Colour(0, 0, 0));
    val->SetFontSize(16);
    val->SetText(text);
    return val;
}

pair<DrawBox*,DrawBox*>MainContentComponent::LoadDisplay(int sx, int sy, int lx, int ly, String text) {
    DrawBox *part1 = new DrawBox(make_pair(lx - (sx / 4), ly), make_pair(sx / 2, sy));
    part1->SetFillColor(Colour(0, 0, 0));
    part1->SetLineColor(Colour(255, 180, 0));
    part1->SetFontSize(16);
    
    DrawBox *part2 = new DrawBox(make_pair(lx + (sx / 4), ly), make_pair(sx / 2, sy));
    part2->SetFillColor(Colour(75, 75, 75));
    part2->SetLineColor(Colour(255, 180, 0));
    part2->SetFontSize(16);
    part2->SetText(text);
    
    pair<DrawBox*, DrawBox*> val = make_pair(part1, part2);
    return val;
}

pair<TextEditor*,DrawBox*>MainContentComponent::LoadEditor(int sx, int sy, int lx, int ly, String text, String name) {
    TextEditor *part1 = new TextEditor(name);
    part1->setMultiLine(false);
    part1->setCentrePosition(lx - (sx / 2), ly - (sy / 2));
    part1->setSize(sx / 2, sy);
    addAndMakeVisible(part1);
    part1->addListener(this);
    DrawBox *part2 = new DrawBox(make_pair(lx + (sx / 4), ly), make_pair(sx / 2, sy));
    part2->SetFillColor(Colour(75, 75, 75));
    part2->SetLineColor(Colour(255, 180, 0));
    part2->SetFontSize(16);
    part2->SetText(text);
    
    pair<TextEditor*, DrawBox*> val = make_pair(part1, part2);
    return val;
}

void MainContentComponent::DrawDisplay(Graphics& g, int Val, pair<DrawBox*, DrawBox*> data) {
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
    addAndMakeVisible(popHigh);
    popHigh->addListener(this);
    
    mAmount->LoadGraph(settings->MutationAmount);
    addAndMakeVisible(mAmountHigh);
    mAmountHigh->addListener(this);
    
    mChance->LoadGraph(settings->MutationChance);
    addAndMakeVisible(mChanceHigh);
    mChanceHigh->addListener(this);
    
    cInt->LoadGraph(settings->CaptureInterval);
    addAndMakeVisible(cIntHigh);
    cIntHigh->addListener(this);
    
    vector<pair<String, double> > targets;
    for (int i = 0 ; i < settings->Target.size() ; i++) {
        targets.push_back(make_pair(settings->Target[i].first, settings->Target[i].second));
    }
    target->LoadGraph(targets);
}

void MainContentComponent::LoadSettingsText() {
    fftSize.first->setText(to_string(settings->FFTSize));
    waveSize.first->setText(to_string(settings->WaveSize));
    waveCount.first->setText(to_string(settings->WaveCount));
    sampleRate.first->setText(to_string(settings->SampleRate));
    channels.first->setText(to_string(settings->Channels));
    factor.first->setText(to_string(settings->Factor));
}

void MainContentComponent::CheckSettings(TextEditor &editor, int &setting) {
    String text = editor.getText();
    if (atoi(text.getCharPointer())) {
        setting = atoi(text.getCharPointer());
    }
    editor.setText(to_string(setting));
    
}

Component* createMainContentComponent()     { return new MainContentComponent(); }
