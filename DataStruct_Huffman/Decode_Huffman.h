#pragma once
#include<iostream>
#include"Pre_Huffman.h"

/*
读取操作
实现读取需要解码文件的数据，并返回文件正文长度
*/
int openfile_decode(huffman Huf[MAXSIZE * 2 - 1], char* values, char* Filename)
{
	FILE* fp;
	int file_size_current = 0;
	int file_size_length = 0;
	int file_size_word = 0;
	int weight = 0;
	char word = '\0';
	int i = 0;//读取字典计数器
	errno_t err = fopen_s(&fp, Filename, "rb+");   //打开文件
	if (fp == NULL)
	{
		cout << "faile to open" << endl;
		return -1;
	}
	else
	{
		fseek(fp, 0, SEEK_END);      //将文件指针指向该文件的最后
		file_size_length = ftell(fp);       //根据指针位置，此时可以算出文件的长度
		fseek(fp, 0, SEEK_SET);
		fread(&weight, sizeof(int), 1, fp);
		fread(&word, sizeof(char), 1, fp);
		while (weight != 0 && word != '\0')//判断是否到达写入的边界线，读入字典
		{
			Huf[(int)word].weight = weight;
			fread(&weight, sizeof(int), 1, fp);
			fread(&word, sizeof(char), 1, fp);
		}
		file_size_current = ftell(fp);
		file_size_word = file_size_length - file_size_current;//计算正文字符的个数
		memset(values, '\0', file_size_word * sizeof(char));  //初始化此控件内容，否则可能会有乱码
		fread(values, sizeof(char), file_size_word, fp); //开始读取正文部分
		fclose(fp);
	}
	return (int)file_size_word;
}
/*
二进制字符转换成char
*/
void decode_key(char* code, int length, string& decoding)

{
	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < 8; j++) {
			int num = code[i];
			//0x80 :1000 0000
			if ((num & 0x80) == 0x80) {
				decoding += "1";
			}
			else {
				decoding += "0";
			}
			code[i] <<= 1;
		}
	}
}
/*
解码
*/
bool decode(char* values, int length, huffman Huf[MAXSIZE * 2 - 1], int max, string& decode_word) {
	int m = max;
	string num = { '\0' };
	decode_key(values, length, num);
	//cout << num;

	for (int i = 0; i < length * 8; i++)
	{
		if (num[i] == '0')
		{
			//cout << "0";
			m = Huf[m].ln;
		}
		else if (num[i] == '1')
		{
			//cout << "1";
			m = Huf[m].rn;
		}
		if (Huf[m].ln == -1 && Huf[m].rn == -1)
		{
			//cout << " ";
			decode_word += Huf[m].value;
			//cout << Huf[m].value;
			//cout << endl;
			m = max;//将m置为堆顶位置，以便重新遍历。
		}
	}
	return true;
}
/*
解码文件写入操作
*/
bool writefile_decode(char* FILENAME, string& decode_word) {
	FILE* fp;
	errno_t erro = fopen_s(&fp, FILENAME, "wb");
	if (fp == NULL) {
		cout << "erro" << endl;
		return false;
	}
	else {
		int length = 0;
		length = (int)decode_word.length();
		char* value = new char[length]();
		for (int i = 0; i < length; i++)
		{
			value[i] = decode_word[i];
		}
		fwrite(value, sizeof(char), length, fp);
		fclose(fp);
		delete[] value;
		return true;
	}
}

/*
完成解压程序的综合实现和调用
*/
bool use_decode(char* values, char* FILENAME, char* FILENAME_target) {
	int max = 0;
	int length_decode = 0;
	huffman Huf[MAXSIZE * 2 - 1];
	string decode_word;
	//1.初始化一个huffman结构体数组
	if(huffman_creat(Huf))
		cout << "初始化成功！..." << endl;
	//2.打开目标文件，读取字典到Huffman数组，并返回解压内容及其长度
	length_decode = openfile_decode(Huf, values, FILENAME);
	if (length_decode == -1)
	{
		cout << "文件不存在，请输入有效解码文件";
		return false;
	}
	else
		cout << "文件打开成功！.." << endl;
	//3.重建Huffman树，并返回堆顶元素位置
	max = HuffmanTree_Creat(Huf);
	if(max!=0)
		cout << "Huffman树重建成功！..." << endl;
	//4.解码
	if(decode(values, length_decode, Huf, max, decode_word))
		cout << "解压成功！正在写入目标文件..." << endl;
	//5.写入解码文件
	if(writefile_decode(FILENAME_target, decode_word))
		cout << "完成解压！" << endl;
	return true;
}