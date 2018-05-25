/*
  ==============================================================================

    ListenerComponents.h
    Created: 21 Feb 2016 9:55:13am
    Author:  Paul Popat

  ==============================================================================
*/

#ifndef LISTENERCOMPONENTS_H_INCLUDED
#define LISTENERCOMPONENTS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Algorithm/GeneController.h"
#include "../Settings/Settings.h"

class GeneControllerListenerComponent : public GeneController::Listener,
                                        public Component,
                                        protected AsyncUpdater
{
  public:
    void handleAsyncUpdate() override { repaint(); }
};

class SettingsListenerComponent : public Settings::Listener,
                                  public Component
{
  public:
    SettingsListenerComponent(String Name)
        : Settings::Listener(Name)
    {
    }
};

#endif // LISTENERCOMPONENTS_H_INCLUDED
