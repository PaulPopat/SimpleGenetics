/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

MainContentComponent::MainContentComponent()
    : laf(new CustomLookAndFeel)
{
    settings = new Settings();
    devices = new AudioDeviceSelectorComponent(deviceManager, 0, 0, 0, 256, false, false, false, false);

    setLookAndFeel(laf);

    ScopedPointer<XmlElement> e = XmlDocument::parse(BinaryData::UILayout_xml);
    interface = new UserInterface(e, settings);
    interface->AddSettingsListeners(settings);
    addAndMakeVisible(interface);

    if (commands == nullptr)
        commands = new ApplicationCommandManager();
    addKeyListener(commands->getKeyMappings());

    setApplicationCommandManagerToWatch(commands);
    commands->registerAllCommandsForTarget(this);

    setSize(1200, 800);
}

MainContentComponent::~MainContentComponent()
{
    shutdownAudio();
    CancelAlgorithm();
    PopupMenu::dismissAllActiveMenus();
    audioSettings.deleteAndZero();
}

void MainContentComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
}

void MainContentComponent::releaseResources()
{
}

void MainContentComponent::paint(Graphics &g)
{
}

void MainContentComponent::resized()
{
    interface->setBounds(getBounds());
}

ApplicationCommandTarget *MainContentComponent::getNextCommandTarget()
{
    return findFirstTargetParentComponent();
}

void MainContentComponent::getAllCommands(Array<CommandID> &commands)
{
    // this returns the set of all commands that this target can perform..
    const CommandID ids[] = {
        MainContentComponent::CommandIDs::Open,
        MainContentComponent::CommandIDs::Save,
        MainContentComponent::CommandIDs::SaveAs,
        MainContentComponent::CommandIDs::Create,
        MainContentComponent::CommandIDs::SaveOutput,
        MainContentComponent::CommandIDs::PlaybackSettings,
        MainContentComponent::CommandIDs::Run,
        MainContentComponent::CommandIDs::Cancel,
    };

    commands.addArray(ids, numElementsInArray(ids));
}

void MainContentComponent::getCommandInfo(CommandID commandID, ApplicationCommandInfo &result)
{
    const String settingsCat("Settings");
    const String audioCat("Audio");
    const String algorithmCat("Algorithm");

    switch (commandID)
    {
    case MainContentComponent::CommandIDs::Open:
        result.setInfo("Open", "Open up a new settings xml file", settingsCat, 0);
        result.addDefaultKeypress('o', ModifierKeys::commandModifier);
        break;

    case MainContentComponent::CommandIDs::Save:
        result.setInfo("Save", "Save settings over currently loaded", settingsCat, 0);
        result.addDefaultKeypress('s', ModifierKeys::commandModifier);
        break;

    case MainContentComponent::CommandIDs::SaveAs:
        result.setInfo("Save As", "Save settings in a new location", settingsCat, 0);
        result.addDefaultKeypress('w', ModifierKeys::commandModifier);
        break;

    case MainContentComponent::CommandIDs::Create:
        result.setInfo("Create", "Create new settings file in a given location", settingsCat, 0);
        result.addDefaultKeypress('n', ModifierKeys::commandModifier);
        break;

    case MainContentComponent::CommandIDs::SaveOutput:
        result.setInfo("Save Output", "Save the audio from the algorithm in the project folder", audioCat, 0);
        result.addDefaultKeypress('a', ModifierKeys::commandModifier);
        break;

    case MainContentComponent::CommandIDs::PlaybackSettings:
        result.setInfo("Playback Settings", "Hardware settings for live playback", audioCat, 0);
        result.addDefaultKeypress('p', ModifierKeys::commandModifier);
        break;

    case MainContentComponent::CommandIDs::Run:
        result.setInfo("Run", "Run the algorithm with current settings", algorithmCat, 0);
        result.addDefaultKeypress('r', ModifierKeys::commandModifier);
        break;

    case MainContentComponent::CommandIDs::Cancel:
        result.setInfo("Cancel", "Cancel the algorithm if it is running", algorithmCat, 0);
        result.addDefaultKeypress('c', ModifierKeys::commandModifier);
        break;

    default:
        break;
    }
}

bool MainContentComponent::perform(const InvocationInfo &info)
{
    switch (info.commandID)
    {
    case MainContentComponent::CommandIDs::Open:
        settings->LoadSettings();
        break;
    case MainContentComponent::CommandIDs::Save:
        settings->SaveSettings();
        break;
    case MainContentComponent::CommandIDs::SaveAs:
        settings->SaveSettingsAs();
        break;
    case MainContentComponent::CommandIDs::Create:
        settings->CreateNew();
        break;
    case MainContentComponent::CommandIDs::SaveOutput:
        SaveAudio();
        break;
    case MainContentComponent::CommandIDs::PlaybackSettings:
        AudioSettings();
        break;
    case MainContentComponent::CommandIDs::Run:
        RunAlgorithm();
        break;
    case MainContentComponent::CommandIDs::Cancel:
        CancelAlgorithm();
        break;

    default:
        return false;
    }

    return true;
}

StringArray MainContentComponent::getMenuBarNames()
{
    const char *const names[] = {"Settings", "Audio", "Algorithm", nullptr};
    return StringArray(names);
}

PopupMenu MainContentComponent::getMenuForIndex(int menuIndex, const String &name)
{
    PopupMenu menu;
    if (commands == nullptr)
        commands = new ApplicationCommandManager();
    if (menuIndex == 0)
    {
        menu.addCommandItem(commands, MainContentComponent::CommandIDs::Open);
        menu.addCommandItem(commands, MainContentComponent::CommandIDs::Save);
        menu.addCommandItem(commands, MainContentComponent::CommandIDs::SaveAs);
        menu.addCommandItem(commands, MainContentComponent::CommandIDs::Create);
    }

    if (menuIndex == 1)
    {
        menu.addCommandItem(commands, MainContentComponent::CommandIDs::SaveOutput);
        menu.addCommandItem(commands, MainContentComponent::CommandIDs::PlaybackSettings);
    }

    if (menuIndex == 2)
    {
        menu.addCommandItem(commands, MainContentComponent::CommandIDs::Run);
        menu.addCommandItem(commands, MainContentComponent::CommandIDs::Cancel);
    }

    return menu;
}

void MainContentComponent::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
}

//************************************************************
// utilities
//************************************************************

void MainContentComponent::SaveAudio() const
{
    if (!settings->IsLoaded())
        return;
    FileChooser chooser("Create Settings", File::nonexistent, "*.aif");
    if (!chooser.browseForFileToSave(true))
        return;
    File path = chooser.getResult();
    if (path.exists())
        path.deleteRecursively();

    Component::SafePointer<SimpleDocumentWindow> writeAudio = new SimpleDocumentWindow("Audio Output Settings",
                                                                                       findColour(CustomLookAndFeel::ColourIDs::Background),
                                                                                       DocumentWindow::TitleBarButtons::closeButton,
                                                                                       true);
    writeAudio->setLookAndFeel(laf);
    writeAudio->setResizable(true, true);
    writeAudio->setVisible(true);
    writeAudio->setUsingNativeTitleBar(true);
    writeAudio->centreWithSize(200, 300);

    writeAudio->setContentOwned(new AudioOutputSettings(File(settings->GetWorkingDirectory().getFullPathName() + "/Data"),
                                                        path),
                                false);
}

void MainContentComponent::AudioSettings()
{
    audioSettings = new SimpleDocumentWindow("Playback Settings",
                                             findColour(CustomLookAndFeel::ColourIDs::Background),
                                             DocumentWindow::TitleBarButtons::closeButton,
                                             true);
    audioSettings->setLookAndFeel(laf);
    audioSettings->setResizable(true, true);
    audioSettings->setVisible(true);
    audioSettings->setUsingNativeTitleBar(true);
    audioSettings->centreWithSize(400, 200);

    audioSettings->setContentNonOwned(devices, false);
}

void MainContentComponent::RunAlgorithm()
{
    if (!settings->IsLoaded())
        return;
    File data(settings->GetWorkingDirectory().getFullPathName() + "/Data/Data1.bin");
    if (data.exists())
    {
        if (!Settings::WarningAccepted("There is already data saved to these settings. Do you wish to overwrite this?"))
            return;
    }
    bands.clear();
    settings->UpdateFromUI();
    int numbands = settings->GetIntValue("FrequencyBands");
    //int fftsize = settings->GetIntValue("FFTSize");
    //int framesPerGene = settings->GetIntValue("FramesPerGene");
    //decoder = new FFTW::LiveAudioDecoder(numbands, fftsize, framesPerGene);

    for (int i = 0; i < numbands; i++)
    {
        File target(settings->GetWorkingDirectory().getFullPathName() + "/Data/Data" + String(i + 1) + ".bin");
        if (target.existsAsFile())
            target.deleteFile();
        GeneController *input = new GeneController(*settings, target, i, &gen);
        input->startThread();
        interface->AddControllerListeners(input);
        //input->AddListener(decoder);
        bands.add(input);
    }

    //decoder->startThread();

    isRunning = true;
}

void MainContentComponent::CancelAlgorithm()
{
    //if (isRunning)
    //if (decoder->isThreadRunning())
    //decoder->stopThread(10000);
    for (auto &b : bands)
        if (b->isThreadRunning())
            b->stopThread(10000);
    bands.clear();
    isRunning = false;
}

Component *createMainContentComponent() { return new MainContentComponent(); }
