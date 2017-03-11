#include <iostream>
#include <fstream>
#include "readWav.h"
#include "tools/kiss_fftr.h"
#include <vector>
#include <complex>
#include <cmath>
using namespace std;

ofstream oFile;

int main(int argc, char *argv[])
{
WavReader wvRdr;

char library[50] = MUSICLIBRARY;
char filepath[75];
if(0 > sprintf(filepath, "%s/%s",library,argv[1]))
{
	cout<<"Error: Failed to create path to WAV file. Did you forget to enter the song name?"<<endl;
}
ifstream input(filepath,ios::in | ios::binary);
cout << "Filepath: "<<filepath<<endl;

if(0 != wvRdr.GetWavInfo(input))
{
cout<<"Error: Problem with WAV File. Exiting..."<<endl;
return 1;
}
int nfft = wvRdr.GetPacketLength();
cout <<"nfft = "<<nfft<<endl;
int SongLength = wvRdr.GetSongLength();
int epochCount = (SongLength + nfft -1)/nfft;
cout <<"epochCount = "<<epochCount<<endl;
//oFile.open("/home/alex/FFT/log.txt",ios::out | ios::trunc);

kiss_fftr_cfg cfg = kiss_fftr_alloc(nfft, 0,NULL,NULL); // FFT config object

kiss_fft_cpx spectrum[nfft/2+1];

kiss_fft_scalar *samples = (kiss_fft_scalar*)calloc(nfft, sizeof(kiss_fft_scalar)); //Allocate space for real time-domain data

//oFile << "Begin*********************"<<endl;

	int maxVal;
	int maxIndex;
	float peakFreq;
	float freq;
	float freqConst = wvRdr.m_sampleRate/nfft;
	float magnitude;

cout << freqConst<<endl;
	int BassBegin = BASSBANDBEGIN / freqConst;
	int BassEnd = BASSBANDEND / freqConst;
	int TrebBegin = TREBLEBANDBEGIN / freqConst;
	int TrebEnd = TREBLEBANDEND / freqConst;
	int MidBegin = MIDBANDBEGIN / freqConst;
	int MidEnd = MIDBANDEND / freqConst;

for(int i = 0; i<epochCount -1; i++)
{
	cout << "epoch: " << i << " of "<<epochCount -2 <<"|";
	maxVal = -1;
	maxIndex = -1;
	peakFreq = -1;
	wvRdr.ReadWav(input);

	memcpy(samples,wvRdr.m_buff,nfft*2);
   
   	kiss_fftr(cfg, (kiss_fft_scalar*)samples , spectrum);	

 for(int j = BassBegin; j<= BassEnd;j++)
 {
 	magnitude = sqrt(spectrum[j].r * spectrum[j].r + spectrum[j].i * spectrum[j].i);
	//oFile << magnitude<<" ";
	if(magnitude >= maxVal)
		{
			peakFreq = j*freqConst;
			maxVal = magnitude;
			maxIndex = j;
		}
		freq = j*freqConst;
 }
	cout<<" Bass Peak: "<<peakFreq;

 	maxVal = -1;
	maxIndex = -1;
	peakFreq = -1;
 for(int j = MidBegin; j<=MidEnd;j++)
 {

 	magnitude = sqrt(spectrum[j].r * spectrum[j].r + spectrum[j].i * spectrum[j].i);
	//oFile << magnitude<<" ";
	if(magnitude >= maxVal)
		{
			peakFreq = j*freqConst;
			maxVal = magnitude;
			maxIndex = j;
		}
		freq = j*freqConst;
 }
	cout <<" Mid Peak: "<<peakFreq;

 	maxVal = -1;
	maxIndex = -1;
	peakFreq = -1;
 for(int j = TrebBegin; j<=TrebEnd;j++)
 {

 	magnitude = sqrt(spectrum[j].r * spectrum[j].r + spectrum[j].i * spectrum[j].i);
	//oFile << magnitude<<" ";
	if(magnitude >= maxVal)
		{
			peakFreq = j*freqConst;
			maxVal = magnitude;
			maxIndex = j;
		}
		freq = j*freqConst;
 }
	cout <<" Treble Peak: "<<peakFreq<<endl;

//oFile << "\n";
}
free(samples);
free(cfg);
input.close();
oFile.close();
return 0;
}
