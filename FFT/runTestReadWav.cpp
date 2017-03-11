#define FIXED_POINT 16
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
ifstream input(argv[1],ios::in | ios::binary);
//input.open(argv[1],ios::in | ios::binary);
wvRdr.init();

if(0 != wvRdr.GetWavInfo(input))
{
cout<<"Error at __LINE__. Error in GetWavInfo(). Exiting..."<<endl;
return 1;
}
int nfft = wvRdr.GetPacketLength();
cout <<"nfft = "<<nfft<<endl;
int SongLength = wvRdr.GetSongLength();
int epochCount = (SongLength + nfft -1)/nfft;
cout << epochCount<<endl;
cout<<"epochCount = "<<epochCount<<endl;
oFile.open("/home/alex/FFT/out.txt",ios::out | ios::trunc);
cout<<"About to initialize kiss_fftr_cfg cfg"<<endl;
kiss_fftr_cfg cfg = kiss_fftr_alloc(nfft, 0,NULL,NULL); // FFT config object
cout<<"About to initialize kiss_fft_cpx *spectrum"<<endl;
//kiss_fft_cpx *spectrum = (kiss_fft_cpx*) calloc(nfft/2 + 1, sizeof(kiss_fft_cpx)); //Allocate space for complex spectrum data
kiss_fft_cpx spectrum[nfft/2+1];
cout<<"Spectrum successfully initialized"<<endl;

kiss_fft_scalar *samples = (kiss_fft_scalar*)calloc(nfft, sizeof(kiss_fft_scalar)); //Allocate space for real time-domain data

//oFile << "Begin*********************"<<endl;
	int maxVal;
	int maxIndex;
	float peakFreq;
	float freq;
	float freqConst = wvRdr.m_sampleRate/nfft;
	float magnitude;
for(int i = 0; i<epochCount; i++)
{
	maxVal = -1;
	maxIndex = -1;
	peakFreq = -1;
	wvRdr.ReadWav(input);
	//cout<<"************************"<<i<<"th Epoch*************************"<<endl;
	//kiss_fftr_cfg cfg = kiss_fftr_alloc(nfft*2, 0,NULL,NULL); // FFT object

//kiss_fft_cpx *spectrum = (kiss_fft_cpx*) calloc(nfft, sizeof(kiss_fft_cpx));



 //cout << "About to cast wvRdr.m_buff to int16_t samples" <<endl;
   //samples = (int16_t *)wvRdr.m_buff; <--------------
	memcpy(samples,wvRdr.m_buff,nfft*2);
 //cout << "Successful cast to samples" <<endl;
   kiss_fftr(cfg, (kiss_fft_scalar*)samples , spectrum);
//cout<<"Sample Rate = "<<wvRdr.m_sampleRate<<endl;
//cout<<"Packet Size = "<<nfft<<endl;
for(int j = 0;j<nfft;j++)
{
	//oFile << samples[j]<<endl;
}

 for(int j = 1; j< nfft/2;j++)
 {
 	magnitude = sqrt(spectrum[j].r * spectrum[j].r + spectrum[j].i * spectrum[j].i);
	oFile << magnitude<<" ";
	if(magnitude >= maxVal)
		{
			peakFreq = j*freqConst;
			maxVal = spectrum[j].r;
			maxIndex = j;
		}
		freq = j*freqConst;
//cout<<"Bin "<<j<<":   Freq ~= "<<freq <<"   R("<<spectrum[j].r<<")   I("<<spectrum[j].i<<")"<<" Magnitude: "<<magnitude<<endl;
  //oFile <<wvRdr.m_buff[j];

 }
 cout << "epoch: " << i << " of "<<epochCount<<endl;
 //cout << "PeakFreq = "<<peakFreq<<"	---maxVal = " <<maxVal<< "	maxIndex = "<< maxIndex<<endl<<endl;
 	oFile << "\n";
//free(spectrum);
//free(cfg);
}
cout<<"About to free spectrum"<<endl;
//free(spectrum);
//free(samples);
cout<<"About to free cfg" <<endl;
//free(cfg);
cout<<"about to run input.close()"<<endl;//
//input.close();
//cout <<"about to run oFile.close()"<<endl;
//oFile.close();
cout <<"about to run wvRdr.Finish()"<<endl;
//wvRdr.Finish(input);
cout << "Finished."<<endl;
return 0;
}
