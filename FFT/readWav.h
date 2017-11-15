#ifndef WAVREADER_H
#define WAVEREADR_H

#include <stdint.h>
using namespace std;

#ifndef REALTIME
#define REALTIME false
#endif


#define WINDOWTIME 0.1
#define BASSBANDBEGIN 20
#define BASSBANDEND 70
#define MIDBANDBEGIN 71
#define MIDBANDEND 300
#define TREBLEBANDBEGIN 301
#define TREBLEBANDEND 2000

#define MUSICLIBRARY "/home/local/ANT/mendolaa/Music"


class WavReader
{
public:
        WavReader();
        ~WavReader();
        int GetWavInfo(ifstream& iFile);
//        int ReadWav(std::ifstream& iFile, int* buffer, int buffLength);
	int ReadWav(ifstream& iFile);
	int GetPacketLength();
	int GetSongLength();
        char* m_buff;
	int m_sampleRate;
	int m_numChannels;
	int m_bitDepth;
private:
	int m_packetLength;
	int m_songLength;
	int m_bytesPerSample;
	int m_skippedBytes;
	//char* m_buff;
};
#endif
