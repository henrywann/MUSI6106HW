
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "RingBuffer.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();
void    testCase1(CRingBuffer<float>*&, float*&);
void    testCase2(CRingBuffer<float>*&, float*&);
void    testCase3(CRingBuffer<float>*&, float*&);
void    testCase4(CRingBuffer<float>*&, float*&);
void    testCase5(CRingBuffer<float>*&, float*&);
/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    // You don't need to use an audio file this time, though you can try if you want.
    // std::string             sInputFilePath,                 //!< file paths
    //                         sOutputFilePath;
    // static const int        kBlockSize          = 1024;
    // float                   **ppfAudioData      = 0;
    // CAudioFileIf            *phAudioFile        = 0;
    // CAudioFileIf::FileSpec_t stFileSpec;
    // long long               iInFileLength       = 0;        //!< length of input file
    
    clock_t                 time                = 0;
    
    CRingBuffer<float>      *pCRingBuffer       = 0;
    float                   *pfTestSignal       = 0;
    float                   fGain               = 1.f;
    int                     iSignalLength       = 0;
    
    showClInfo ();
    
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    time                    = clock();
    pCRingBuffer = new CRingBuffer<float>(5);
    iSignalLength = 10;
    pfTestSignal = new float[iSignalLength];
    // fill the test signal (e.g., a unit impulse)
    for (int i = 0; i < iSignalLength; ++i) {
        pfTestSignal[i] = i;
    }
    
    //////////////////////////////////////////////////////////////////////////////
    // do processing and tests
    //    testCase1(pCRingBuffer, pfTestSignal);
    testCase5(pCRingBuffer, pfTestSignal);
    
    
    cout << "processing done in: \t"    << (clock()-time)*1.F/CLOCKS_PER_SEC << " seconds." << endl;
    
    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    
    return 0;
}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2018 by Alexander Lerch" << endl;
    cout  << endl;
    
    return;
}

void testCase1(CRingBuffer<float>*& buffer, float*& signal) {
    // check boundary and these get and set functions
    cout << "wirte pointer index: " << buffer->getWriteIdx() << endl;
    cout << "read pointer index: " << buffer->getReadIdx() << endl;
    cout << "after set index: " << endl;
    buffer->setReadIdx(0);
    buffer->setWriteIdx(0);
    cout << "wirte pointer index: " << buffer->getWriteIdx() << endl;
    cout << "read pointer index: " << buffer->getReadIdx() << endl;
    cout << "after set index: " << endl;
    buffer->setWriteIdx(buffer->getLength() - 1);
    buffer->setReadIdx(buffer->getLength() - 1);
    cout << "wirte pointer index: " << buffer->getWriteIdx() << endl;
    cout << "read pointer index: " << buffer->getReadIdx() << endl;
    cout << "after set index: " << endl;
    buffer->setWriteIdx(buffer->getLength() + 1);
    buffer->setReadIdx(buffer->getLength() + 1);
    cout << "wirte pointer index: " << buffer->getWriteIdx() << endl;
    cout << "read pointer index: " << buffer->getReadIdx() << endl;
}

void testCase2(CRingBuffer<float>*& buffer, float*& signal) {
    // check whether write pointer and read pointer move together if they overlap when putting data at status 1
    cout << "write pointer index: " << buffer->getWriteIdx() << endl;
    cout << "read pointer index: " << buffer->getReadIdx() << endl;
    cout << "status: " << buffer->getStatus() << endl;
    
    for (int i = 0; i < buffer->getLength(); ++i) {
        buffer->putPostInc(signal[i]);
    }
    cout << "After putting: " << buffer->getLength() << "data" << endl;
    cout << "write pointer index: " << buffer->getWriteIdx() << endl;
    cout << "read pointer index: " << buffer->getReadIdx() << endl;
    cout << "status: " << buffer->getStatus() << endl;
    
    buffer->putPostInc(signal[buffer->getLength()]);
    cout << "Put another one" << endl;
    cout << "After putting: " << buffer->getLength() << "data" << endl;
    cout << "write pointer index: " << buffer->getWriteIdx() << endl;
    cout << "read pointer index: " << buffer->getReadIdx() << endl;
    cout << "status: " << buffer->getStatus() << endl;
    if (buffer->getReadIdx() == buffer->getWriteIdx() == 1) {
        cout << "*****************************************\ntest2 passes!" << endl;
    } else {
        cout << "*****************************************\ntest2 fails!" << endl;
    }
    
}

void    testCase3(CRingBuffer<float>*& buffer, float*& signal) {
    // check whether write pointer and read pointer move together if they overlap when writing data at status 0
    cout << "write pointer index: " << buffer->getWriteIdx() << endl;
    cout << "read pointer index: " << buffer->getReadIdx() << endl;
    cout << "status: " << buffer->getStatus() << endl;
    for (int i = 0; i < buffer->getLength() - 1; ++i) {
        buffer->putPostInc(signal[i]);
        cout << buffer->getPostInc() << endl;
    }
    cout << "write pointer index: " << buffer->getWriteIdx() << endl;
    cout << "read pointer index: " << buffer->getReadIdx() << endl;
    cout << "status: " << buffer->getStatus() << endl;
    cout << '\n';
    cout << buffer->getPostInc() << endl;
    cout << buffer->getPostInc() << endl;
    cout << buffer->getPostInc() << endl;
    cout << '\n';
    cout << "write pointer index: " << buffer->getWriteIdx() << endl;
    cout << "read pointer index: " << buffer->getReadIdx() << endl;
    cout << "status: " << buffer->getStatus() << endl;
    
    buffer->putPostInc(signal[0]);
    cout << "write pointer index: " << buffer->getWriteIdx() << endl;
    cout << "read pointer index: " << buffer->getReadIdx() << endl;
    cout << "status: " << buffer->getStatus() << endl;
    if (buffer->getReadIdx() == 2 && buffer->getWriteIdx() == 3) {
        cout << "*****************************************\ntest3 passes!" << endl;
    } else {
        cout << "*****************************************\ntest3 fails!" << endl;
    }
    
}

void    testCase4(CRingBuffer<float>*& buffer, float*& signal) {
    // check the switch between these two statuses and get(iOffset) function
    cout << "write pointer index: " << buffer->getWriteIdx() << endl;
    cout << "read pointer index: " << buffer->getReadIdx() << endl;
    cout << "status: " << buffer->getStatus() << endl;
    for (int i = 0; i < buffer->getLength(); ++i) {
        buffer->putPostInc(signal[i]);
        cout << buffer->get(i) << '\n';
    }
    if (buffer->getStatus() != 1) {
        cout << "*****************************************\ntest4 fails!" << endl;
        return;
    }
    cout << "write pointer index: " << buffer->getWriteIdx() << endl;
    cout << "read pointer index: " << buffer->getReadIdx() << endl;
    cout << "status: " << buffer->getStatus() << endl;
    
    for (int j = 0; j < buffer->getLength(); ++j) {
        cout << buffer->getPostInc() << endl;
    }
    cout << "write pointer index: " << buffer->getWriteIdx() << endl;
    cout << "read pointer index: " << buffer->getReadIdx() << endl;
    cout << "status: " << buffer->getStatus() << endl;
    if (buffer->getStatus() != 0) {
        cout << "*****************************************\ntest4 fails!" << endl;
        return;
    } else {
        cout << "*****************************************\ntest4 passes!" << endl;
    }
}

void    testCase5(CRingBuffer<float>*& buffer, float*& signal) {
    // test get(iOffset)
    
    // default value
    for (int i = 0; i < buffer->getLength(); ++i) {
        cout << buffer->get(i) << ' ';
    }
    cout << '\n';
    for (int j = 0; j < buffer->getLength(); ++j) {
        buffer->putPostInc(signal[j]);
    }
    cout << '\n';
    cout << buffer->getStatus() << endl;
    for (int k = 0; k < buffer->getLength(); ++k) {
        cout << buffer->getPostInc() << ' ';
    }
    cout << '\n';
    
    cout << "write pointer index: " << buffer->getWriteIdx() << endl;
    cout << "read pointer index: " << buffer->getReadIdx() << endl;
    cout << "status: " << buffer->getStatus() << endl;
    // positive offset
    cout << "positive offset" << endl;
    for (int m = 0; m < buffer->getLength(); ++m) {
        cout << buffer->get(m) << ' ';
    }
    
    // negative offset
    cout << "negative offset" << endl;
    for (int l = 0; l < buffer->getLength(); ++l) {
        cout << buffer->get(-l) << ' ';
    }
    cout << '\n';
}
