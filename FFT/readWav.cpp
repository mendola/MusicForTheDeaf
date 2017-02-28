#include <iostream>
#include <fstream>
#include "readWav.h"
#include "tools/kiss_fftr.h"
#include <vector>
#include <complex>
using namespace std;

int sampleRate = 44100;
int bitDepth = 16;
float windowTime = 0.10;

WavReader::WavReader()
{
int m_packetLength;
char* m_buff;
int m_songLength = 0;
int m_sampleRate = 0;

//this->m_packetLength = windowTime * bitDepth/8 * sampleRate;
//this->wm_buff[m_packetLength];
}

WavReader::~WavReader()
{
 delete[] m_buff;
// delete m_buff;
}

int WavReader::init()
{
cout<<"windowTime = "<<windowTime<<"\nbitDepth = "<<bitDepth<<"\nsampleRate = "<<sampleRate<<endl;
this->m_packetLength = windowTime * bitDepth/8 * sampleRate;
cout<<"m_packetLength = "<<this->m_packetLength<<endl;
//this->m_buff[this->m_packetLength];
//this->m_packetLength = 10;
m_buff = new char[this->m_packetLength];
this->m_sampleRate = sampleRate;
}



int WavReader::GetWavInfo(ifstream& iFile)
{
//////METHOD TO GET SAMPLE RATE///////////
//int bitDepth = 8;
//	std::cout << "Bit Depth = " << std::cout <<bitDepth;

if(iFile)
{
  iFile.seekg(0, iFile.end);
  this->m_songLength = iFile.tellg();
  iFile.seekg(0, iFile.beg);
}
else return 1;
return 0;
}



int WavReader::ReadWav(ifstream& iFile)
{
//cout<<"ReadWav run"<<endl;

	//Open an input and output stream in binary mode
	if(iFile.is_open())
	{
//	  cout<<"File is open"<<endl;
//	  cout<<"m_packetLength="<<this->m_packetLength<<endl;
//	  cout<<"m_songLength=" << this->m_songLength<<endl;
			int loc = iFile.tellg();
                       // cout <<"location: "<<loc<<endl;
	  for(int n=0; n<m_packetLength; n++)
	  {
			iFile.read((char*)&m_buff[n],1);//this->m_packetLength);
//                        iFile.read(newbuff,pack);//this->m_packe$
	  }
//	  cout<<"Data Successfully read"<<endl;
	}
	else
	{
	  cout<<"Not opened"<<endl;
	}

//	for(n=0; n<m_maxdur; n++)
//	{
//		cout<<buffer[n]<<" ";
//	}
return 0;

}

int WavReader::Finish()
{
delete [] m_buff;
cout<<"Deleted Buffer from memory"<<endl;
return 0;
}

int WavReader::GetPacketLength()
{
int packetlength = this->m_packetLength*8/bitDepth;
cout<<"Samples per epoch = "<<packetlength<<endl;
return packetlength;
}

int WavReader::GetSongLength()
{
return this->m_songLength;
}
