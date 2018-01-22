
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include <fstream>

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
                            sOutputFilePath;

    long long               iInFileLength       = 0;        //!< length of input file

    clock_t                 time                = 0;

    float                   **ppfAudioData      = 0;

    CAudioFileIf            *phAudioFile        = 0;
    
    CAudioFileIf::FileSpec_t fileSpec;
    
    long long               block_size          = 1024;
    
    long long               current             = 0;
    
    long long               previous            = 0;

    std::string             data;
    showClInfo ();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    if (argv[1] == NULL || argv[2] == NULL) {
        cout << "Wrong number of input arguments" << endl;
        return 0;
    } else {
        sInputFilePath = argv[1];
        sOutputFilePath = argv[2];
    }
//    sInputFilePath = "/Users/Henry/Desktop/Audio_Software_Engineering/bld/sweep.wav";

    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    time = clock();
    ppfAudioData = new float*[2];
    ppfAudioData[0] = new float[1024];
    ppfAudioData[1] = new float[1024];

    // get audio data and write it to the output file
    std::ofstream output(sOutputFilePath + "/Output.txt");
//    output.open("Output.txt");
    while(!phAudioFile->isEof()) {
        phAudioFile->readData(ppfAudioData, block_size);
        phAudioFile->getPosition(current);
        for (int i = 0; i < current-previous; i++) {
            data.append(std::to_string(ppfAudioData[0][i]) + " " + std::to_string(ppfAudioData[1][i]) + "\n");
        }
        previous = current;
    }
    output << data << endl;
    cout << "reading/writing done in: \t"    << (clock()-time)*1.F/CLOCKS_PER_SEC << " seconds." << endl;
    //////////////////////////////////////////////////////////////////////////////
    // get audio info and print it to stdout
    phAudioFile->getLength(iInFileLength);
    phAudioFile->getFileSpec(fileSpec);
    cout << "Length: " << iInFileLength << endl;
    cout << "Format: " << fileSpec.eFormat << endl;
    cout << "Bit Stream Type " << fileSpec.eBitStreamType << endl;
    cout << "Number of Channels " << fileSpec.iNumChannels << endl;
    cout << "Sample Rate " << fileSpec.fSampleRateInHz << endl;
    
    //////////////////////////////////////////////////////////////////////////////
    // do processing
    cout << "Hello there!" << endl << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    output.close();
    phAudioFile->destroy(phAudioFile);
    return 0;
    
}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2018 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

