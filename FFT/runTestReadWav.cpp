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
//
char library[50] = MUSICLIBRARY;
char filepath[75];
if(0 > sprintf(filepath, "%s/%s",library,argv[1]))
{
	cout<<"Error: Failed to create path to WAV file. Did you forget to enter the song name?"<<endl;
}
//Read in WAV file
ifstream input(filepath,ios::in | ios::binary);
cout << "Filepath: "<<filepath<<endl;

//Reads in header of WAV file 
if(0 != wvRdr.GetWavInfo(input))
{
cout<<"Error: Problem with WAV File. Exiting..."<<endl;
return 1;
}


int nfft = wvRdr.GetPacketLength(); // # of samples per packet
cout <<"nfft = "<<nfft<<endl;
int SongLength = wvRdr.GetSongLength(); // # samples total
int epochCount = (SongLength + nfft -1)/nfft; // #
cout <<"epochCount = "<<epochCount<<endl;
//oFile.open("/home/alex/FFT/log.txt",ios::out | ios::trunc);

//Config object for FFT
kiss_fftr_cfg cfg = kiss_fftr_alloc(nfft, 0,NULL,NULL); // FFT config object

kiss_fft_cpx spectrum[nfft/2+1]; // buffer for your output frequency data

kiss_fft_scalar *samples = (kiss_fft_scalar*)calloc(nfft, sizeof(kiss_fft_scalar)); //Allocate space for real time-domain data

//oFile << "Begin*********************"<<endl;

	int maxVal;
	int maxIndex;
	float peakFreq;
	float freq;
	float freqConst = wvRdr.m_sampleRate/nfft;
	float magnitude;

cout << freqConst<<endl;
	int BassBegin = BASSBANDBEGIN / freqConst; //1st bin of bass band
	int BassEnd = BASSBANDEND / freqConst; // last bin of bass band
	int TrebBegin = TREBLEBANDBEGIN / freqConst;
	int TrebEnd = TREBLEBANDEND / freqConst;
	int MidBegin = MIDBANDBEGIN / freqConst;
	int MidEnd = MIDBANDEND / freqConst;



// Loops over the number of time windows
for(int i = 0; i<epochCount -1; i++)
{
	cout << "epoch: " << i << " of "<<epochCount -2 <<"|";
	maxVal = -1;
	maxIndex = -1;
	peakFreq = -1;
	wvRdr.ReadWav(input); // Read in a window of samples

	// Copies wvRdr into samples
	memcpy(samples,wvRdr.m_buff,nfft*2);
   
	// Performs FFT. Puts frequency data into spectrum
   	kiss_fftr(cfg, (kiss_fft_scalar*)samples , spectrum);	


	// Find peak frequencies in each band
 for(int j = BassBegin; j<= BassEnd;j++)
 {
	// Get amplitude of frequency at this frequency bin
 	magnitude = sqrt(spectrum[j].r * spectrum[j].r + spectrum[j].i * spectrum[j].i);

	//Check if this is the new peak freq
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
// Free stuff you allocated to heap
free(samples);
free(cfg);
input.close();
oFile.close();
return 0;

}
