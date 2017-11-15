#include <iostream>
#include <fstream>
#include "readWav.h"
#include "tools/kiss_fftr.h"
#include <vector>
#include <complex>
#include <cmath>
#include <time.h>
using namespace std;

ofstream oFile; // unused output file
clock_t t0;
clock_t t1;
float clockPeriod = 1.0/CLOCKS_PER_SEC;
float epochTime = WINDOWTIME;
int main(int argc, char *argv[])
{
WavReader wvRdr;  // This object reads samples from a WAV file into a buffer

char library[50] = MUSICLIBRARY;
char filepath[75];
if(0 > sprintf(filepath, "%s/%s",library,argv[1]))
{
	cout<<"Error: Failed to create path to WAV file. Did you forget to enter the song name?"<<endl;
}

// Input stream for opening and reading wav file
ifstream input(filepath,ios::in | ios::binary);
if(input.is_open()){
	cout << "Filepath: "<<filepath<<endl;
}else{
	cout <<"ERROR: failed to open "<<filepath<<"\n"<<endl;
}

//Reads in header of WAV
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

// Config object for FFT (see kiss_fft README and examples for more info)
kiss_fftr_cfg cfg = kiss_fftr_alloc(nfft, 0,NULL,NULL); // FFT config object

// Output buffer where the frequency-domain data is put
kiss_fft_cpx spectrum[nfft/2+1];

// Input buffer where the time-domain samples are put
kiss_fft_scalar *samples = (kiss_fft_scalar*)calloc(nfft, sizeof(kiss_fft_scalar)); //Allocate space for real time-domain data

//oFile << "Begin*********************"<<endl;

	int maxVal;
	int maxIndex;
	float peakBassFreq;
	float peakMidFreq;
	float peakTrebFreq;
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

// Loops over the number of time windows (epochs)
for(int i = 0; i<epochCount -1; i++)
{
	t0 = clock();
	cout << "epoch: " << i << " of "<<epochCount -2 <<"|";
	maxVal = -1;
	maxIndex = -1;
	peakBassFreq = -1;
	peakMidFreq = -1;
	peakTrebFreq = -1;
	wvRdr.ReadWav(input);

	// Copy the time-domain sample buffer from wvRdr into samples (this step should be removed)
	memcpy(samples,wvRdr.m_buff,nfft*2);

   // Perform FFT (put frequency-domain data into spectrum)
   	kiss_fftr(cfg, (kiss_fft_scalar*)samples , spectrum);


//******************************************************//
// Info about the 'spectrum' data: 						//
// spectrum[n].r -- real part of frequency content		//
// spectrum[n].i -- imaginary part of frequency content //
// * The frequency which spectrum[n] represents: 		//
//   freqency = j * wvRdr.m_sampleRate / nfft;			//
//   (m_sampleRate = WAV sample rate in Hz)				//
//   (nfft = number of samples in 1 window or epoch)	//
//******************************************************//

// Find the peak frequency from the bass band
 for(int j = BassBegin; j<= BassEnd;j++)
 {
 	// magnitude of complex frequecy is the freqency's intensity
 	magnitude = sqrt(spectrum[j].r * spectrum[j].r + spectrum[j].i * spectrum[j].i);
	//oFile << magnitude<<" ";
	if(magnitude >= maxVal)
		{
			peakBassFreq = j*freqConst;
			maxVal = magnitude;
			maxIndex = j;
		}
 }
 	maxVal = -1;
	maxIndex = -1;

// Same as above, but for midband
 for(int j = MidBegin; j<=MidEnd;j++)
 {

 	magnitude = sqrt(spectrum[j].r * spectrum[j].r + spectrum[j].i * spectrum[j].i);
	//oFile << magnitude<<" ";
	if(magnitude >= maxVal)
		{
			peakMidFreq = j*freqConst;
			maxVal = magnitude;
			maxIndex = j;
		}
 }
 	maxVal = -1;
	maxIndex = -1;

// Same as above for treble band
 for(int j = TrebBegin; j<=TrebEnd;j++)
 {

 	magnitude = sqrt(spectrum[j].r * spectrum[j].r + spectrum[j].i * spectrum[j].i);
	//oFile << magnitude<<" ";
	if(magnitude >= maxVal)
		{
			peakTrebFreq = j*freqConst;
			maxVal = magnitude;
			maxIndex = j;
		}
 }


//This Delays output to sync with real-time playback of the song.
//Remove this loop to perform FFT for whole song at Full Speed.
while(REALTIME)
{
	t1 = clock();
	float diff = (t1 - t0)*clockPeriod;
	if (diff >= epochTime)
	{
		break;
	}
}



cout <<"Bass Peak: "<<peakBassFreq<<"   Mid Peak: "<<peakMidFreq<<"   Treble Peak: "<<peakTrebFreq<<endl;
//SendFreqData(peakBassFreq, peakMidFreq, peakTrebFreq);


//oFile << "\n";
}
// free heap
free(samples);
free(cfg);
input.close();
oFile.close();
return 0;
}
