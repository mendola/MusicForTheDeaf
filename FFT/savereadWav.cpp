#include<iostream>
#include<fstream>
#include "readWav.h"
using namespace std;

int sampleRate = 44100;
int bitDepth = 8;
float windowTime = 0.10;

WavReader::WavReader()
{
int m_packetLength;
char* m_buff;
int m_songLength = 0;
//this->m_packetLength = windowTime * bitDepth/8 * sampleRate;
//this->m_buff[m_packetLength];
}

WavReader::~WavReader()
{

}

int WavReader::init()
{
cout<<"windowTime = "<<windowTime<<"\nbitDepth = "<<bitDepth<<"\nsampleRate = "<<sampleRate<<endl;
this->m_packetLength = windowTime * bitDepth/8 * sampleRate;
cout<<"m_packetLength = "<<this->m_packetLength<<endl;
//this->m_buff[this->m_packetLength];
//char m_buff[10];
//this->m_packetLength = 10;
m_buff = new char[this->m_packetLength];
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
int n = 0;
cout<<"ReadWav run"<<endl;

	//Open an input and output stream in binary mode
	if(iFile.is_open())
	{
	  cout<<"File is open"<<endl;
	  cout<<"m_packetLength="<<this->m_packetLength<<endl;
			int loc = iFile.tellg();
			char newbuff[10];
			int pack = 10;
                        cout <<n<<":"<<loc<<endl;
			iFile.read(this->m_buff,this->m_packetLength);//this->m_packetLength);
//                        iFile.read(newbuff,pack);//this->m_packe$
			n++;
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
return this->m_packetLength;
}

int WavReader::GetSongLength()
{
return this->m_songLength;
}
