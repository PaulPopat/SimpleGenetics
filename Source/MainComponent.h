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
#include "Settings/AudioOutputSettings.h"
#include "Settings/CustomLookAndFeel.h"
#include "Algorithm/GeneController.h"
#include "Utilities/RandomGen.h"
#include "Settings/Settings.h"
#include "UserInterface.h"

class SimpleDocumentWindow : public DocumentWindow
{
    using DocumentWindow::DocumentWindow;
    void closeButtonPressed() override
    {
        delete this;
    }
};

class MainContentComponent : public Component,
                             public MenuBarModel,
                             public ApplicationCommandTarget
{
  public:
    MainContentComponent();
    ~MainContentComponent();

    void paint(Graphics &g) override;
    void resized() override;

    ApplicationCommandTarget *getNextCommandTarget() override;
    void getAllCommands(Array<CommandID> &commands) override;
    void getCommandInfo(CommandID commandID, ApplicationCommandInfo &result) override;
    bool perform(const InvocationInfo &info) override;

    StringArray getMenuBarNames() override;
    PopupMenu getMenuForIndex(int menuIndex, const String &) override;
    void menuItemSelected(int menuItemID, int) override;

    enum CommandIDs
    {
        Open = 0x1001,
        Save = 0x1002,
        SaveAs = 0x1003,
        Create = 0x1004,
        SaveOutput = 0x2001,
        Run = 0x3001,
        Cancel = 0x3002,
    };

  private:
    ScopedPointer<Settings> settings;
    Array<ScopedPointer<GeneController>> bands;
    Utilities::Random gen;
    ScopedPointer<UserInterface> interface;
    ScopedPointer<ApplicationCommandManager> commands;
    std::unique_ptr<MenuBarComponent> menuBar;
    bool isRunning = false; // declaring value here so the audio can't be called before it is set

    ScopedPointer<CustomLookAndFeel> laf;

    void SaveAudio() const;
    void RunAlgorithm();
    void CancelAlgorithm();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};

#endif // MAINCOMPONENT_H_INCLUDED
