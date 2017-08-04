#include "extractor.h"


int main()
{
	cout << "将待分解文件拖入此处，并回车确认(或输入文件地址)" << endl;
	string fileAddress;
	cin >> fileAddress;
	extractor one(fileAddress);
	one.findStart();

	cout << "将待存放文件夹拖入此处，并回车确认(或输入文件夹地址)" << endl;
	cin >> fileAddress;
	one.splitFile(fileAddress);
	cout << "over" << endl;
	// ifstream fin;
	// string line;
	// fin.open("./all.txt");
	// while(fin)
	// {
	// 	getline(fin,line);
	// 	cout << line;
	// }
	return 0;
}
