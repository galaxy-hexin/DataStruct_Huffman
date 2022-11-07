#pragma once
#include<iostream>
#include"Pre_Huffman.h"

/*
编码时的文件打开操作
实现打开文件，并将文件中的数据读入一个足够大的数组，以便之后使用
*/
int openfile_code(char* values, char* Filename)
{
	FILE* fp;
	int file_size = 0;
	errno_t err = fopen_s(&fp, Filename, "rb");   //打开文件
	if (fp == NULL)
	{
		cout << "faile to open" << endl;
		return -1;
	}
	else
	{
		fseek(fp, 0, SEEK_END);      //将文件指针指向该文件的最后
		file_size = ftell(fp);       //根据指针位置，此时可以算出文件的字符数
		fseek(fp, 0, SEEK_SET);
		memset(values, '\0', file_size * sizeof(char));  //初始化此控件内容，否则可能会有乱码
		fread(values, sizeof(char), file_size, fp); //开始读取整个文件
		//cout << "文件内容为: " <<endl<< values << endl;
		fclose(fp);
	}
	return (int)file_size;
}
/*
字符权重计算工作
实现将打开的文件的所有数据遍历统计各字符的权重的操作，将权重写入到huffman结构体数组中，以便之后的建树操作
*/
bool getword_weight(huffman Huf[MAXSIZE * 2 - 1], char* values) {
	int i = 0;//循环计数器
	int values_ascii = 0;//记录字符的ASCII码，直接对应Huffman数组的下标
	while (values[i] != '\0')
	{
		values_ascii = (int)values[i];
		Huf[values_ascii].weight++;
		i++;
	}
	return true;
}
/*
编码操作
实现将读取的文件翻译成Huffmancode，存入一个string数组里面
实现方法：已知一颗Huffman树，根据字符对应的ASCII码访问Huffman树，并不断访问父节点。用一个变量保存当前位置，与父节点比较得到左结点还是右结点，如此类推得到一个逆序code编码
*/
int code(char* values, int length, huffman Huf[MAXSIZE * 2 - 1], string& coderesult) {
	int signal_current = 0;//当前字符的位置
	int signal_parent = 0;//父节点的字符位置
	int j = 0;//code遍历的计数器
	char code[20] = { '\0' };//存放每个字符经过逆访问后的逆序编码

	for (int i = 0; i < length; i++)
	{
		signal_current = (int)values[i];//获得当前字符的ASCII码,也就是在Huffman树中的对应的位置
		if (signal_current > 128 || signal_current < 0)//防止出现ASCII码以外的字符。
		{
			cout << "请选择正确的文本，当前文本中存在不合法字符" << endl;
			exit(1);
		}
		while (Huf[signal_current].key[j] != '\0')
		{
			coderesult += Huf[signal_current].key[j];
			j++;
		}
		j = 0;
	}
	return (int)coderesult.length();
}
/*
Huffman编码转换成二进制
实现把所有的code编码八个字节一段，作为一个char字符存储
实现方法：string流取字串，根据是0或1对一个字符串进行位操作
*/
bool Encoding_binary(string& code, char* encoding_binary) {
	//因为不知道最后是否刚好为八位，所有要单独处理不足八位的情况
	int length = (int)code.length() / 8;//取八的整数倍，用来遍历之前的编码
	int final_length = (int)code.length() / 8 + (code.length() % 8 != 0);//若余数不足八位，则实际编码后的字符长度应该加一
	string middle;//中间变量，每次从string中取长度为8的字串
	while (final_length >= length)
	{
		for (int i = 0; i < length; i++)
		{
			middle = code.substr(static_cast<std::basic_string<char, std::char_traits<char>, std::allocator<char>>::size_type>(i) * 8, 8);
			for (int j = 0; j < 8; j++)
			{
				if (middle[j] == '1') encoding_binary[i] |= (1 << (7 - j));
				else if (middle[j] == '0')encoding_binary[i] &= ~(1 << (7 - j));
			}
		}
		//处理结尾字符串,不足则补0
		if (final_length > length)
		{
			//继承末尾子串
			string end = code.substr(static_cast<std::basic_string<char, std::char_traits<char>, std::allocator<char>>::size_type>(length) * 8, 8);
			int j = 0;
			while (end[j] != '\0' && j < 8)
			{
				if (end[j] == '1') encoding_binary[length] |= (1 << (7 - j));
				else if (end[j] == '0')encoding_binary[length] &= ~(1 << (7 - j));
				j++;
			}
			for (j; j < 8; j++)encoding_binary[length] &= ~(1 << (7 - j));//补零
		}
		break;
	}
	return true;
}
/*
文件写入
实现将转换好的二进制字符串写入文件
*/
bool writefile_code(char* FILENAME, huffman Huf[MAXSIZE * 2 - 1], char* encoding_binary, int length) {
	FILE* fp;
	errno_t erro = fopen_s(&fp, FILENAME, "wb+");
	int weight = 0;
	char word = '\0';
	if (fp == NULL) {
		cout << "empty" << endl;
		return false;
	}
	else {
		for (int i = 0; i < MAXSIZE; i++)//遍历、仅写入有权重的字符的权重和字符值，以减小内存
		{
			if (Huf[i].weight != 0) {
				weight = Huf[i].weight;
				word = Huf[i].value;
				fwrite(&weight, sizeof(int), 1, fp);
				fwrite(&word, sizeof(char), 1, fp);
			}
		}
		int weight = 0;
		char word = '\0';//相当于在设置字典与正文的边界线
		fwrite(&weight, sizeof(int), 1, fp);
		fwrite(&word, sizeof(char), 1, fp);
		fwrite(encoding_binary, sizeof(char), length, fp);
		fclose(fp);
		return true;
	}
}

/*
完成压缩程序的综合实现与调用
*/
bool use_code(char* values, char* filename, char* filename_decode) {
	int word_length = 0;//文件字符长度
	int code_length = 0;//编码长度
	huffman Huf[MAXSIZE * 2 - 1] = { 0 };//设置Huffman树的存储数组
	string coderesult;//存放编码结果

	//1.初始化Huffman树
	if(huffman_creat(Huf))
		cout << "初始化创建Huffman数组成功..." << endl;
	//2.读取文件，并返回文件读取的字符数，以便之后的函数遍历
	word_length = openfile_code(values, filename);
	if(word_length == -1) 
	{
		cout << "文件不存在，请输入有效解码文件";
		return false;
	}
	else
		cout << "文件打开成功！..." << endl;
	//3.遍历获取权值
	if (getword_weight(Huf, values))
		cout << "完成全文遍历！..." << endl;
	//4.建立树
	if(HuffmanTree_Creat(Huf)!=0)
		cout << "已建立Huffman树！..." << endl;
	//5.编码得到0、1字符串，并返回字符串长度
	code_length = code(values, word_length, Huf, coderesult);
	//6.计算原编码转换成二进制后的字符数，并创建动态二维数组
	code_length = code_length / 8 + (code_length % 8 != 0);
	char* encoding_binary = new char[(double)code_length + 1]();
	//7.将0、1字符串转换为字符数组
	if(Encoding_binary(coderesult, encoding_binary))
		cout << "编码成功！正在写入目标..." << endl;
	//8.写入文件
	if(writefile_code(filename_decode, Huf, encoding_binary, code_length))
		cout << "压缩成功！" << endl;
	delete[] encoding_binary;//释放内存
	return true;
}