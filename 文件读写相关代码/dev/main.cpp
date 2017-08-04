#include "extractor.h"


int main()
{
	cout << "将待分解文件拖入此处，并回车确认" << endl;
	string fileAddress;
	cin >> fileAddress;
	extractor one(fileAddress);
	one.findStart();

	cout << "将待存放文件夹拖入此处，并回车确认" << endl;
	cin >> fileAddress;
	one.splitFile(fileAddress);
	cout << "over" << endl;
	return 0;
}
