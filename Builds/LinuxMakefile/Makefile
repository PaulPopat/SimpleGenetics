# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef STRIP
  STRIP=strip
endif

ifndef AR
  AR=ar
endif

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Debug
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 $(shell pkg-config --cflags alsa freetype2 libcurl x11 xext xinerama) -pthread -I../../JuceLibraryCode -I../../JuceLibraryCode/modules
  JUCE_CFLAGS += $(CFLAGS) $(JUCE_CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  JUCE_CXXFLAGS += $(CXXFLAGS) $(JUCE_CFLAGS) -std=c++14
  JUCE_LDFLAGS += $(LDFLAGS) $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) $(shell pkg-config --libs alsa freetype2 libcurl x11 xext xinerama) -lGL -ldl -lpthread -lrt 

  TARGET := SimpleGenetics
  BLDCMD = $(CXX) -o $(JUCE_OUTDIR)/$(TARGET) $(OBJECTS) $(JUCE_LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),Release)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Release
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 $(shell pkg-config --cflags alsa freetype2 libcurl x11 xext xinerama) -pthread -I../../JuceLibraryCode -I../../JuceLibraryCode/modules
  JUCE_CFLAGS += $(CFLAGS) $(JUCE_CPPFLAGS) $(TARGET_ARCH) -O3
  JUCE_CXXFLAGS += $(CXXFLAGS) $(JUCE_CFLAGS) -std=c++14
  JUCE_LDFLAGS += $(LDFLAGS) $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -fvisibility=hidden $(shell pkg-config --libs alsa freetype2 libcurl x11 xext xinerama) -lGL -ldl -lpthread -lrt 

  TARGET := SimpleGenetics
  BLDCMD = $(CXX) -o $(JUCE_OUTDIR)/$(TARGET) $(OBJECTS) $(JUCE_LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS := \
  $(JUCE_OBJDIR)/AudioBin_237afb5a.o \
  $(JUCE_OBJDIR)/EQCurveEditor_e43f4271.o \
  $(JUCE_OBJDIR)/GraphEditor_2db3113c.o \
  $(JUCE_OBJDIR)/TargetEditor_f7ab1fe7.o \
  $(JUCE_OBJDIR)/ValueEditor_193f519f.o \
  $(JUCE_OBJDIR)/FrequencyGraph_874957db.o \
  $(JUCE_OBJDIR)/PanningDisplay_dd90abba.o \
  $(JUCE_OBJDIR)/ProgressLine_f65d6fca.o \
  $(JUCE_OBJDIR)/ProgressWatcher_304737b0.o \
  $(JUCE_OBJDIR)/FFTWHelper_eaec19ba.o \
  $(JUCE_OBJDIR)/LiveAudioDecoder_bb9d1823.o \
  $(JUCE_OBJDIR)/BandCalculator_33ae7720.o \
  $(JUCE_OBJDIR)/ChannelCalculator_15640b46.o \
  $(JUCE_OBJDIR)/AudioLoader_ce8bad6a.o \
  $(JUCE_OBJDIR)/AudioWriter_97edf1ca.o \
  $(JUCE_OBJDIR)/AudioOutputSettings_4cb9631b.o \
  $(JUCE_OBJDIR)/CustomLookAndFeel_ab3d772e.o \
  $(JUCE_OBJDIR)/Settings_a85d2d2c.o \
  $(JUCE_OBJDIR)/Gene_fd470d1e.o \
  $(JUCE_OBJDIR)/ComplexFrame_5d065ca6.o \
  $(JUCE_OBJDIR)/GeneController_4323df9a.o \
  $(JUCE_OBJDIR)/UserInterface_e1a58e0f.o \
  $(JUCE_OBJDIR)/MainComponent_a6ffb4a5.o \
  $(JUCE_OBJDIR)/Main_90ebc5c2.o \
  $(JUCE_OBJDIR)/BinaryData_ce4232d4.o \
  $(JUCE_OBJDIR)/juce_audio_basics_6b797ca1.o \
  $(JUCE_OBJDIR)/juce_audio_devices_a742c38b.o \
  $(JUCE_OBJDIR)/juce_audio_formats_5a29c68a.o \
  $(JUCE_OBJDIR)/juce_audio_processors_dea3173d.o \
  $(JUCE_OBJDIR)/juce_audio_utils_c7eb679f.o \
  $(JUCE_OBJDIR)/juce_core_75b14332.o \
  $(JUCE_OBJDIR)/juce_cryptography_6de2ebff.o \
  $(JUCE_OBJDIR)/juce_data_structures_72d3da2c.o \
  $(JUCE_OBJDIR)/juce_events_d2be882c.o \
  $(JUCE_OBJDIR)/juce_graphics_9c18891e.o \
  $(JUCE_OBJDIR)/juce_gui_basics_8a6da59c.o \
  $(JUCE_OBJDIR)/juce_gui_extra_4a026f23.o \
  $(JUCE_OBJDIR)/juce_opengl_cd70b4c2.o \
  $(JUCE_OBJDIR)/juce_video_f128c512.o \

.PHONY: clean

$(JUCE_OUTDIR)/$(TARGET): check-pkg-config $(OBJECTS) $(RESOURCES)
	@echo Linking SimpleGenetics
	-@mkdir -p $(JUCE_BINDIR)
	-@mkdir -p $(JUCE_LIBDIR)
	-@mkdir -p $(JUCE_OUTDIR)
	@$(BLDCMD)

check-pkg-config:
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors alsa freetype2 libcurl x11 xext xinerama

clean:
	@echo Cleaning SimpleGenetics
	@$(CLEANCMD)

strip:
	@echo Stripping SimpleGenetics
	-@$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

$(JUCE_OBJDIR)/AudioBin_237afb5a.o: ../../Source/AudioBin.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioBin.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/EQCurveEditor_e43f4271.o: ../../Source/EQCurveEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling EQCurveEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GraphEditor_2db3113c.o: ../../Source/GraphEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GraphEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TargetEditor_f7ab1fe7.o: ../../Source/TargetEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TargetEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ValueEditor_193f519f.o: ../../Source/ValueEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ValueEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FrequencyGraph_874957db.o: ../../Source/FrequencyGraph.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FrequencyGraph.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PanningDisplay_dd90abba.o: ../../Source/PanningDisplay.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PanningDisplay.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ProgressLine_f65d6fca.o: ../../Source/ProgressLine.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ProgressLine.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ProgressWatcher_304737b0.o: ../../Source/ProgressWatcher.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ProgressWatcher.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FFTWHelper_eaec19ba.o: ../../Source/FFTWHelper.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FFTWHelper.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LiveAudioDecoder_bb9d1823.o: ../../Source/LiveAudioDecoder.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LiveAudioDecoder.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BandCalculator_33ae7720.o: ../../Source/BandCalculator.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BandCalculator.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ChannelCalculator_15640b46.o: ../../Source/ChannelCalculator.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ChannelCalculator.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioLoader_ce8bad6a.o: ../../Source/AudioLoader.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioLoader.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioWriter_97edf1ca.o: ../../Source/AudioWriter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioWriter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioOutputSettings_4cb9631b.o: ../../Source/AudioOutputSettings.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioOutputSettings.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CustomLookAndFeel_ab3d772e.o: ../../Source/CustomLookAndFeel.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomLookAndFeel.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Settings_a85d2d2c.o: ../../Source/Settings.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Settings.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Gene_fd470d1e.o: ../../Source/Gene.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Gene.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ComplexFrame_5d065ca6.o: ../../Source/ComplexFrame.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ComplexFrame.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GeneController_4323df9a.o: ../../Source/GeneController.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GeneController.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/UserInterface_e1a58e0f.o: ../../Source/UserInterface.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling UserInterface.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainComponent_a6ffb4a5.o: ../../Source/MainComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Main.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BinaryData.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_basics_6b797ca1.o: ../../JuceLibraryCode/juce_audio_basics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_devices_a742c38b.o: ../../JuceLibraryCode/juce_audio_devices.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_formats_5a29c68a.o: ../../JuceLibraryCode/juce_audio_formats.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_processors_dea3173d.o: ../../JuceLibraryCode/juce_audio_processors.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_utils_c7eb679f.o: ../../JuceLibraryCode/juce_audio_utils.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_core_75b14332.o: ../../JuceLibraryCode/juce_core.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_cryptography_6de2ebff.o: ../../JuceLibraryCode/juce_cryptography.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_cryptography.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_data_structures_72d3da2c.o: ../../JuceLibraryCode/juce_data_structures.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_events_d2be882c.o: ../../JuceLibraryCode/juce_events.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_graphics_9c18891e.o: ../../JuceLibraryCode/juce_graphics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_gui_basics_8a6da59c.o: ../../JuceLibraryCode/juce_gui_basics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_gui_extra_4a026f23.o: ../../JuceLibraryCode/juce_gui_extra.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_opengl_cd70b4c2.o: ../../JuceLibraryCode/juce_opengl.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_opengl.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_video_f128c512.o: ../../JuceLibraryCode/juce_video.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_video.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
