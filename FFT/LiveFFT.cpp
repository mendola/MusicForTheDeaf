#include<iostream>
#include<fstream>

using namespace std;

//int MAXDUR = 100;
//int buffer[100];

class WavReader
{

	unsigned int n = 0;

	std::cout << "Sample Rate = " << std::cout <<sampleRate;

	//Open an input and output stream in binary mode
	ifstream iFile("/home/alex/Music/Famous.wav",ios::binary);
	if(iFile.is_open())
	{
	//	while(!iFile.eof())
		while(n <MAXDUR)
		{
			buffer[n]= iFile.get();
			std::cout <<n<<":"<<buffer[n]<<" ";
			n++;
		}
	}

	std::cout<<"++++++++++++++++++++++++++++++++++++++++++"<<std::endl;

	for(n=0; n<MAXDUR; n++)
	{
		std::cout<<buffer[n]<<" ";
	}


	//Close both files
	iFile.close();
//return 0;
}
