#include <iostream>
#include <fstream>
#include "readWav.h"
#include "tools/kiss_fftr.h"
#include <vector>
#include <complex>
#include <cstring>
using namespace std;

float windowTime = WINDOWTIME;

WavReader::WavReader()
{
  int m_packetLength;
  int m_songLength = 0;
  int m_sampleRate = 0;
}

WavReader::~WavReader()
{
  delete [] m_buff;
}

int WavReader::GetWavInfo(ifstream& iFile)
{
//////METHOD TO PARSE DATA FROM WAV FILE HEADER///////////

if(iFile)
{
  iFile.seekg(0, iFile.beg);

  char temp[4];
  temp[4] = '\0';
  iFile.read((char*)temp,4); // Check first 4 bytes. Should be "RIFF"
  if(strcmp(temp, "RIFF\0") != 0)
  {
  	cout<<"Error: File isn't the correct file type (Not a RIFF)" <<endl;
  	return 1;
  }
  iFile.read((char*)temp,4); // Read in ChunkSize (Total # of Bytes - 8)
  int ChunkSize = *(int*)temp;

  iFile.read((char*)temp,4); //Read in Format. Should be "WAV "
  if(strcmp(temp, "WAVE") != 0)
  {
    cout<<temp<<endl;
  	cout<<"Error: File isn't the correct file type (RIFF, but not a WAV)"<<endl;
  	return 1;
  }

  iFile.seekg(4,iFile.cur); // Skip over "fmt "

  iFile.read((char*)temp,4); // Read in Subchunk1Size (Bytes remaining until data chunk)
  int Subchunk1Size = *(int*)temp;
  if(Subchunk1Size != 16)
  {
  	cout<<"Error: Incompatible encoding format"<<endl;
  	return 1;
  }
  for(int i = 0; i<4; i++)
  {
  	temp[i] = 0;
  }

  iFile.read((char*)temp,2); // Read in AudioFormat (Should be 1 for linear Pulse Code Modulation)
  int AudioFormat = *(int*)temp;
  if(AudioFormat != 1)
  {
  	cout<<"Error: Incompatible encoding format"<<endl;
  	return 1;
  }
  temp[0] = 0;
  iFile.read((char*)temp,2); //Read in number of channels (mono or stereo)
  this->m_numChannels = *(int*)temp;
  cout<<"Number of channels: "<< m_numChannels <<endl;

  iFile.read((char*)temp,4); //Read in the sample rate (usually 44100, maybe 8000)
  this->m_sampleRate = *(int*)temp;

  iFile.read((char*)temp,4); // Read in ByteRate. = SampleRate *NumChannels *BitsPerSample / 8
  int ByteRate = *(int*)temp;
  for(int i = 0; i<4; i++)
  {
  	temp[i] = 0;
  }

  iFile.read((char*)temp,2); // Read in BlockAlign. = NumChannels * BitsPerSample/8
  int BlockAlign = *(int*)temp; // This is the number of bytes for one full audio sample

  iFile.read((char*)temp,2); //Read in BitsPerSample. Should be 8bit or 16bit
  m_bitDepth = *(int*)temp;
  cout<<"Bits per Sample: "<<m_bitDepth<<endl;

  iFile.seekg(4,iFile.cur); //Skip the string "data"

  iFile.read((char*)temp,4); //Read in the length of the data in bytes
  int dataLength = *(int*)temp;


  // @m_songLength -- Total number of samples for one channel
  this->m_songLength = floor(dataLength / this->m_numChannels * 8 / m_bitDepth);
  cout << "m_songLength = " << m_songLength<<endl;

  // @m_packetLength -- Number of samples in one epoch/time window
  this->m_packetLength = windowTime * m_sampleRate;
  cout<< "m_packetLength = "<<m_packetLength<<endl;

  // @m_buff -- Buffer to hold data
  m_buff = new char[this->m_packetLength * this->m_bitDepth / 8];

  // @m_bytesPerSample -- Number of bytes for one sample of one channel
  this->m_bytesPerSample = this->m_bitDepth / 8;
  
  // @m_skippedBytes -- Number of bytes to skip from other channel at each sample
  this->m_skippedBytes = this->m_bytesPerSample * (this->m_numChannels - 1);
}
else return 1;
return 0;
}



int WavReader::ReadWav(ifstream& iFile)
{
	if(iFile.is_open())
	{
      int loc = iFile.tellg();                      
			char a;
	  for(int n=0; n<m_packetLength; n++)
	  {
			iFile.read((char*)&m_buff[this->m_bytesPerSample*n],this->m_bytesPerSample);//this->m_packetLength);
      if(!iFile)
      {
        cout<<"READ ERROR"<<endl;
        return 1;
      }
			iFile.seekg(this->m_skippedBytes,iFile.cur);
	  }
	}
	else
	{
	  cout<<"Not opened"<<endl;
	}

return 0;

}

int WavReader::GetPacketLength()
{
return this->m_packetLength;
}

int WavReader::GetSongLength()
{
return this->m_songLength;
}
