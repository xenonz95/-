#include "extractor.h"


int main()
{
	cout << "�����ֽ��ļ�����˴������س�ȷ��" << endl;
	string fileAddress;
	cin >> fileAddress;
	extractor one(fileAddress);
	one.findStart();

	cout << "��������ļ�������˴������س�ȷ��" << endl;
	cin >> fileAddress;
	one.splitFile(fileAddress);
	cout << "over" << endl;
	return 0;
}
