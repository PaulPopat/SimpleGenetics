/*
  ==============================================================================

    SettingsUtils.cpp
    Created: 6 Aug 2015 9:16:27am
    Author:  Paul Popat

  ==============================================================================
*/

#include "SettingsUtils.h"
using namespace std;

vector<string> GetFileData(File *Path) {
    vector<string> output;
    StringArray data;
    Path->readLines(data);
    for (int i = 0 ; i < data.size() ; i++) {
        output.push_back(data[i].toRawUTF8());
    }
    return output;
}

vector<double> NormalizeAudio(vector<double> audio) {
    // normalizes an audio stream so the max amplitude is 0db
    double finalVal = 0;
    for (int i = 0 ; i < audio.size() ; i++) {
        if (fabs(audio[i]) > finalVal) {
            finalVal = fabs(audio[i]);
        }
    }
    finalVal = 1 / finalVal;
    for (int i = 0 ; i < audio.size() ; i++) {
        audio[i] *= finalVal;
    }
    return audio;
}

vector<string> StringSplit(string Input) {
    // splits a string by the white space
    vector<string> output;
    vector<string> words;
    istringstream iss(Input);
    do {
        string sub;
        iss >> sub;
        words.push_back(sub);
    } while(iss);
    
    for (int i = 0 ; i < words.size() ; i++) {
        output.push_back(words[i].c_str());
    }
    
    return output;
}

vector<double> GetBin(vector<double> Input, int FFTSize) {
    vector<double> returnVal(FFTSize);
    FFT *fft = new FFT(log2(FFTSize), false);
    FFT::Complex *input = new FFT::Complex[FFTSize];
    FFT::Complex *output = new FFT::Complex[FFTSize];
    int calc = Input.size() / FFTSize;
    for (int i = 0 ; i < calc ; i++) {
        for (int x = 0 ; x < FFTSize ; x++) {
            input[x].r = Input[x + (i * FFTSize)];
            input[x].i = 0;
        }
        fft->perform(input, output);
        for (int x = 0 ; x < FFTSize ; x++) {
            returnVal[x] += sqrt(pow(output[x].r, 2) + pow(output[x].i, 2)) / calc;
        }
    }
    delete fft;
    delete input;
    delete output;
    return returnVal;
}

vector<double> LoadFile(File *filepath) {
    // loads an audio file
    vector<double> buf;
    AiffAudioFormat reader;
    AudioFormatReader *formatReader = reader.createReaderFor(filepath->createInputStream(), true);
    AudioSampleBuffer *data = new AudioSampleBuffer(formatReader->numChannels, formatReader->lengthInSamples);
    formatReader->read(data, 0, formatReader->lengthInSamples, 0, true, false);
    for (int i = 0 ; i < formatReader->lengthInSamples ; i++)
        buf.push_back(data->getSample(0, i));
    NormalizeAudio(buf);
    delete data;
    delete formatReader;
    return buf;
}

vector<double> NormalizeLoudness(vector<double> audio, double targetLoudness) {
    // normalizes so the average amplitude of all of the samples hits the target (does not care if the file peaks)
    double loudness = 0;
    for (int i = 0 ; i < audio.size() ; i++) {
        loudness += fabs(audio[i]) / audio.size();
    }
    double multiplier = targetLoudness / loudness;
    for (int i = 0 ; i < audio.size() ; i++) {
        audio[i] *= multiplier;
    }
    return audio;
}

vector<pair<double,double> > LoadVectorSettings(int &i, vector<string> settingsData) {
    vector<pair<double,double> > data;
    bool inSubLoop = true;
    while (inSubLoop) {
        i++;
        if (settingsData[i] != "end") {
            vector<string> subline = StringSplit(settingsData[i]);
            data.push_back(make_pair(atof(subline[0].c_str()), atof(subline[1].c_str())));
        }
        else inSubLoop = false;
    }
    return data;
}

vector<pair<string,double> > LoadTargetSettings(int &i, vector<string> settingsData) {
    vector<pair<string,double> > data;
    bool inSubLoop = true;
    while (inSubLoop) {
        i++;
        if (settingsData[i] != "end") {
            vector<string> subline = StringSplit(settingsData[i]);
            data.push_back(make_pair(subline[0], atof(subline[1].c_str())));
        }
        else inSubLoop = false;
    }
    return data;
}

double LinearInterp(double Distance, double Value1, double Value2) {
    double difference = Value2 - Value1;
    return Value1 + (difference * Distance);
}

double Interpolate(double Position, vector< pair< double, double > > Data) {
    // interpolates between settings values
    double output = Data[Data.size() - 1].first;
    int i = 0;
    bool inLoop = true;
    while(inLoop && i < Data.size()) {
        if (Position < Data[i].second) {
            double distance = (Position - Data[i - 1].second) / (Data[i].second - Data[i - 1].second);
            return LinearInterp(distance, Data[i - 1].first, Data[i].first);
        } else if (Position == Data[i].second) {
            return Data[i].first;
        }
        i++;
    }
    return output;
}

void CopyAudio(File InputPath, File OutputDir) {
    vector<double> buf;
    AiffAudioFormat reader;
    AudioFormatReader *formatReader = reader.createReaderFor(InputPath.createInputStream(), true);
    AudioSampleBuffer *data = new AudioSampleBuffer(formatReader->numChannels, formatReader->lengthInSamples);
    formatReader->read(data, 0, formatReader->lengthInSamples, 0, true, false);
    
    File OutputPath = File(OutputDir.getFullPathName() + "/" + InputPath.getFileName());
    
    AiffAudioFormat aif;
    AudioFormatWriter *writer = aif.createWriterFor(OutputPath.createOutputStream(), formatReader->numChannels, data->getNumChannels(), 24, 1, 0);
    writer->writeFromAudioSampleBuffer(*data, 0, data->getNumSamples());
    delete writer;
    delete data;
    delete formatReader;
}