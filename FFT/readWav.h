//#ifndef WAVREADER_H
//#define WAVEREADR_H
#include <stdint.h>
using namespace std;

class WavReader
{
public:
        WavReader();
        ~WavReader();
	int init();
        int GetWavInfo(ifstream& iFile);
//        int ReadWav(std::ifstream& iFile, int* buffer, int buffLength);
	int ReadWav(ifstream& iFile);
	int Finish();
	int GetPacketLength();
	int GetSongLength();
        char* m_buff;
	int m_sampleRate;
private:
	int m_packetLength;
	int m_songLength;
	//char* m_buff;
};
//#endif
