#include<iostream>
#include"Code_Huffman.h"
#include"Decode_Huffman.h"

int main() {
	char* values = new char[ARRAYSIZE]();
	char filename_code[30] = { '\0' };
	char filename_decode[30] = { '\0' };
	clock_t start_t = 0, end_t = 0;
	double total = 0;

	int button = 0;
	cout << "Do you want to extract the file?   -------------  press 1" << endl << "or compress the file?              -------------  press 2" << endl;
	while (true)
	{
		cout << endl << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
		cin >> button;
		switch (button)
		{
		case 1:
			cout << "请输入被压缩文件名" << endl;
			cin >> filename_code;
			cout << "请输入压缩后新文件名" << endl;
			cin >> filename_decode;
			start_t = clock();
			cout << endl << "processing..." << endl;
			use_code(values, filename_code, filename_decode);
			end_t = clock();
			total = (end_t - start_t) / CLOCKS_PER_SEC;
			cout << "the time is:" << total << endl;
			break;
		case 2:
			cout << "请输入解压文件名" << endl;
			cin >> filename_code;
			cout << "请输入解压后新文件名" << endl;
			cin >> filename_decode;
			start_t = clock();
			cout << endl << "processing..." << endl;
			use_decode(values, filename_code, filename_decode);
			end_t = clock();
			total = (end_t - start_t) / CLOCKS_PER_SEC;
			cout << "the time is:" << total << endl;
			break;
		default:
			exit(-1);
		}
	}
	delete[] values;
}