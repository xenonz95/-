#include "extractor.h"


extractor::extractor(const string &address)
{
	if(!setAddress(address))
	{
		fin.open(address);
	}
	buffer.reserve(200);
}

extractor::~extractor()
{
	fin.close();
}


void extractor::saveBuffer(int fileNo,const string &dirAddress)
{
	stringstream fullAddress;
	ofstream fout;
	string address;
	
	fullAddress << dirAddress << "\\" << fileNo << ".txt";
	fullAddress >> address;
	fout.open(address);
	for (auto i = buffer.begin(); i != buffer.end(); i++)
	{
		fout << *i << endl;
	}
	fout.close();
	buffer.clear();
	if(fileNo%100 == 0)
		cout << "saved file: " << address <<endl;
}

int extractor::setAddress(const string &address)
{
	if (address != "")
		extractor::address = address;
	return 0;
}

void extractor::findStart()
{
	fin.seekg(0);
	while (getline(fin,tempstr))
	{
		if (tempstr == "Time/sec, Potential/V")
			break;
	}
	getline(fin, tempstr);
}

int extractor::splitFile(const string &dirAddress)
{
	double data1,data2,id,lastdata,max = 0;
	char c;
	string address;
	stringstream tempss;
	fout.open(dirAddress + "\\max.txt");
	int i = 1;
	getline(fin, tempstr);
	buffer.push_back(tempstr);
	tempss << tempstr;
	tempss >> data1 >> c >> data2;
	tempss.clear();
	lastdata = data2;
	while (getline(fin, tempstr) && tempstr != "")
	{
		if (lastdata > max)
		{
			id = data1;
			max = data2;
		}
		tempss << tempstr;
		tempss >> data1 >> c >> data2;
		tempss.clear();

		if (data2 - lastdata > 0.1)
		{
			fout << id << "	" << max << endl;
			saveBuffer(i++, dirAddress);
			max = 0;
		}
		buffer.push_back(tempstr);
		lastdata = data2;
	}
	saveBuffer(i, dirAddress);
	fout.close();
	cout << "sum: " << i << " files" << endl;
	return 0;
}
