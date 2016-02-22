/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== GenericSave.xml ==================
static const unsigned char temp_binary_data_0[] =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"\n"
"<Genetics>\n"
"  <FFTSize value=\"2048\"/>\n"
"  <FramesPerGene value=\"40\"/>\n"
"  <BreedingLoops value=\"1000\"/>\n"
"  \n"
"  <BreedingFactor value=\"3\"/>\n"
"  <CalculationLoops value=\"1\"/>\n"
"  <FrequencyBands value=\"6\"/>\n"
"  \n"
"  <CaptureInterval>\n"
"      <Ele1 value=\"300\" loc=\"0\"/>\n"
"  </CaptureInterval>\n"
"  \n"
"  <Target>\n"
"      <Ele1 value=\"None\" loc=\"0\"/>\n"
"  </Target>\n"
"          \n"
"  <MutationAmount>\n"
"      <Ele1 value=\"1\" loc=\"0\"/>\n"
"  </MutationAmount>\n"
"                  \n"
"  <MutationNumber>\n"
"      <Ele1 value=\"20\" loc=\"0\"/>\n"
"  </MutationNumber>\n"
"                  \n"
"  <Population> \n"
"    <Ele1 value=\"100\" loc=\"0\"/>\n"
"  </Population>\n"
"              \n"
"  <FrequencyWeighting> \n"
"    <Ele1 value=\"1\" loc=\"0\"/>\n"
"  </FrequencyWeighting>\n"
"                 \n"
"  <PanningMutation> \n"
"    <Ele1 value=\"0.5\" loc=\"0\"/>\n"
"  </PanningMutation>\n"
"</Genetics>";

const char* GenericSave_xml = (const char*) temp_binary_data_0;

//================== UILayout.xml ==================
static const unsigned char temp_binary_data_1[] =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
"\r\n"
"<Layout>\r\n"
"\t<FrequencyGraph left=\"0\" top=\"0\" width=\"0.3\" height=\"0.2\"/>\r\n"
"\t<PanningDisplay left=\"0.3\" top=\"0\" width=\"0.2\" height=\"0.2\"/>\r\n"
"    <ProgressWatcher left=\"0\" top=\"0.2\" width=\"0.1\" height=\"0.2\" type=\"Frame\"/>\r\n"
"    <ProgressWatcher left=\"0.1\" top=\"0.2\" width=\"0.1\" height=\"0.2\" type=\"Timbre\"/>\r\n"
"    <ProgressWatcher left=\"0.2\" top=\"0.2\" width=\"0.1\" height=\"0.2\" type=\"Panning\"/>\r\n"
"    <ValueEditor left=\"0\" top=\"0.4\" width=\"0.3\" height=\"0.04\" title=\"FFT Frame Size\" track=\"FFTSize\"/>\r\n"
"    <ValueEditor left=\"0\" top=\"0.44\" width=\"0.3\" height=\"0.04\" title=\"Frames Per Gene\" track=\"FramesPerGene\"/>\r\n"
"    <ValueEditor left=\"0\" top=\"0.48\" width=\"0.3\" height=\"0.04\" title=\"Breeding Phases\" track=\"BreedingLoops\"/>\r\n"
"    <ValueEditor left=\"0\" top=\"0.52\" width=\"0.3\" height=\"0.04\" title=\"Calculation Loops\" track=\"CalculationLoops\"/>\r\n"
"    <ValueEditor left=\"0\" top=\"0.56\" width=\"0.3\" height=\"0.04\" title=\"Breeding Random Factor\" track=\"BreedingFactor\"/>\r\n"
"    <ValueEditor left=\"0\" top=\"0.60\" width=\"0.3\" height=\"0.04\" title=\"Frequency Bands\" track=\"FrequencyBands\"/>\r\n"
"    \r\n"
"    <GraphEditor left=\"0.5\" top=\"0\" width=\"0.5\" height=\"0.2\" title=\"Population\" track=\"Population\"/>\r\n"
"    <GraphEditor left=\"0.5\" top=\"0.2\" width=\"0.5\" height=\"0.2\" title=\"Mutation Number\" track=\"MutationNumber\"/>\r\n"
"    <GraphEditor left=\"0.5\" top=\"0.4\" width=\"0.5\" height=\"0.2\" title=\"Timbre Mutation Amount\" track=\"MutationAmount\"/>\r\n"
"    <GraphEditor left=\"0.5\" top=\"0.6\" width=\"0.5\" height=\"0.2\" title=\"Panning Mutation Amount\" track=\"PanningMutation\"/>\r\n"
"    <GraphEditor left=\"0.5\" top=\"0.8\" width=\"0.5\" height=\"0.2\" title=\"Capture Interval\" track=\"CaptureInterval\"/>\r\n"
"    \r\n"
"    <EQCurveEditor left=\"0\" top=\"0.84\" width=\"0.5\" height=\"0.16\" title=\"Frequency Mutation Weighting\" track=\"FrequencyWeighting\"/>\r\n"
"    \r\n"
"    <AudioBin left=\"0.3\" top=\"0.2\" width=\"0.2\" height=\"0.44\" title=\"Audio Bin\" track=\"AudioBin\"/>\r\n"
"    \r\n"
"    <TargetEditor left=\"0\" top=\"0.64\" width=\"0.5\" height=\"0.2\" title=\"Targets\" track=\"Target\"/>\r\n"
"    \r\n"
"    <ProgressLine left=\"0.5\" top=\"0\" width=\"0.5\" height=\"1\"/>\r\n"
"    <ProgressLine left=\"0\" top=\"0.64\" width=\"0.5\" height=\"0.2\"/>\r\n"
"</Layout>";

const char* UILayout_xml = (const char*) temp_binary_data_1;


const char* getNamedResource (const char*, int&) throw();
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes) throw()
{
    unsigned int hash = 0;
    if (resourceNameUTF8 != 0)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0xec82192c:  numBytes = 825; return GenericSave_xml;
        case 0x8e671336:  numBytes = 2174; return UILayout_xml;
        default: break;
    }

    numBytes = 0;
    return 0;
}

const char* namedResourceList[] =
{
    "GenericSave_xml",
    "UILayout_xml"
};

}
