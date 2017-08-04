#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;


class extractor
{
public:
	extractor(const string &address);
	~extractor();
private:
	string address;
	ifstream fin;
	ofstream fout;
	vector<string> buffer;
	string tempstr;
	
public:
	int setAddress(const string &address);
	void findStart();
	int splitFile(const string &dirAddress);
private:
	void saveBuffer(int fileNo, const string &dirAddress);
};