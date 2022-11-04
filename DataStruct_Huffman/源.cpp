#include<iostream>
using namespace std;
constexpr auto ARRAYSIZE = 1024*1024*10;
constexpr auto WEIGHTMAXSIZE = 1024*1024*1024;
constexpr auto MAXSIZE = 128;

struct huffman {
	int weight;
	char value;
	int ln;//the left node
	int rn;//the right node
	int pa;//the parent node
	char key[20];//the key of the values
};
/*
huffman结构体初始化
实现初始化一个Huffman结构体，将权重置为0，左右结点、对应的huffmankey值置为初值-1
*/
void huffman_creat(huffman Huf[MAXSIZE*2-1]) {
	for (int i = 0; i < MAXSIZE; i++)//初始化已知的maxsize个叶结点。
	{
		Huf[i] = { 0 , (char)i, -1, -1, -1 };
		for (int j = 0; j < 20; j++)
		{
			Huf->key[j] = '\0';
		}
	}
	for (int i = MAXSIZE; i < MAXSIZE * 2 - 1; i++)//初始化以后要存储内结点的maxsize-1个结点
	{
		Huf[i] = { 0, -1, -1, -1, -1 };
		for (int j = 0; j < 20; j++)
		{
			Huf->key[j] = '\0';
		}
	}
}
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
void getword_weight(huffman Huf[MAXSIZE*2-1], char* values) {
	int i = 0;//循环计数器
	int values_ascii = 0;//记录字符的ASCII码，直接对应Huffman数组的下标
	while (values[i]!='\0')
	{
		values_ascii = (int)values[i];
		Huf[values_ascii].weight++;
		i++;
	}
}
/*
Huffman树创建
实现Huffman树的创建，根据已经得到各字符频率的Huffman数组进行创建
实现方法：遍历所有weight值非零的，不断寻找两个最小权重的结点，将其相加后置为新结点，并修改两个最小点的父结点和新结点的权重值、左右子节点、以及权重
*/
int HuffmanTree_Creat(huffman Huf[MAXSIZE * 2 - 1]) {
	int max = 0; 
	int j = 0;
	char code[20] = { '\0' };//存放每个字符经过逆访问后的逆序编码
	int signal_current = 0;//当前字符的位置
	int signal_parent = 0;//父节点的字符位置
	for (int i = 0; i < MAXSIZE; i++)
		//主要的建树操作
	{
		/*遍历MAXSIZE遍，每次挑选权重最小的两个*/
		/*用x1和x2保存这两个元素在数组中的位置，并用变量weight1和weight2进行保存他们的权重*/
		int weight_min1, weight_min2;
		int x1, x2;
		weight_min1 = weight_min2 = WEIGHTMAXSIZE;//由于需要逐次比较后取小值，所以存放量的初值必须足够的大
		x1 = x2 = -1;

		for (int j = 0; j < MAXSIZE + i; j++)
		{
			if (Huf[j].weight>0 &&Huf[j].weight < weight_min1 && Huf[j].pa == -1)
			//条件（1）.保证该结点代表的字符出现过，即权重大于0  
			//条件（2）.当字符小于weight_min1存放的字符的权重时，更改weight_min1的值
			//条件（3).只有没有父节点的结点（即pa变量为初值-1）才有遍历的必要
			{
				weight_min2 = weight_min1;
				weight_min1 = Huf[j].weight;
				x2 = x1;
				x1 = j;
			}
			else if (Huf[j].weight > 0 && Huf[j].weight < weight_min2 && Huf[j].pa == -1)
			//条件（2）大于weight_min1,小于weight_min2的只移动weight_min2
			{
				weight_min2 = Huf[j].weight;
				x2 = j;
			}
		}
		/*在第n+i的位置上赋值权重，设置左右子节点位置。*/
		/*并设置两个最小权重结点的父节点。*/
		/*只有当x1或x2的值发生改变才进行赋值*/
		if (x1 != -1 && x2!=-1) 
		{
			max = MAXSIZE + i;
			Huf[x1].pa = MAXSIZE + i;
			Huf[x2].pa = MAXSIZE + i;
			Huf[i + MAXSIZE].weight = weight_min1 + weight_min2;
			Huf[i + MAXSIZE].ln = x1;
			Huf[i + MAXSIZE].rn = x2;
		}
	}

	for (int i = 0; i < MAXSIZE; i++)
		//完成每一项有权重的字符key值计算
		//遍历maxsize遍
	{
		if (Huf[i].weight!=0)//权值不为零才有计算的必要
		{
			int m = 0;
			signal_current = (int)i;//当前位置信号量
			signal_parent = Huf[signal_current].pa;//当前位置的父结点的信号量
			while (signal_parent!=-1)//父结点为-1时则为最末端
			{
				if (Huf[signal_parent].ln == signal_current)
					code[j] = '0';
				else if (Huf[signal_parent].rn == signal_current)
					code[j] = '1';
				signal_current = signal_parent;
				signal_parent = Huf[signal_current].pa;
				j++;
			}
			for (int k = 19; k >= 0; k--)
				/*将code逆序处理放入key值*/
			{
				if (code[k] != '\0')
				{
					 Huf[i].key[m] = code[k];
					//cout << code[k];
					 m++;
				}
			}
			for (int k = 0; k < 20; k++)//缓存数组code[]重新置为空
				code[k] = '\0';
			j = 0;
		}
	}

	return max;
}
/*
编码操作
实现将读取的文件翻译成Huffmancode，存入一个string数组里面
实现方法：已知一颗Huffman树，根据字符对应的ASCII码访问Huffman树，并不断访问父节点。用一个变量保存当前位置，与父节点比较得到左结点还是右结点，如此类推得到一个逆序code编码
*/
int code(char* values, int length, huffman Huf[MAXSIZE * 2 - 1],string& coderesult) {
	int signal_current = 0;//当前字符的位置
	int signal_parent = 0;//父节点的字符位置
	int j = 0;//code遍历的计数器
	char code[20] = { '\0' };//存放每个字符经过逆访问后的逆序编码

	for (int i = 0; i < length; i++)
	{
		signal_current = (int)values[i];//获得当前字符的ASCII码,也就是在Huffman树中的对应的位置
		if (signal_current>128||signal_current<0)//防止出现ASCII码以外的字符。
		{
			cout << "请选择正确的文本，当前文本中存在不合法字符"<<endl;
			exit(1);
		}
		while (Huf[signal_current].key[j]!='\0')
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
void Encoding_binary(string& code,char* encoding_binary) {
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
}
/*
文件写入
实现将转换好的二进制字符串写入文件
*/
void writefile_code(char* FILENAME,huffman Huf[MAXSIZE*2-1], char* encoding_binary, int length) {
	FILE* fp;
	errno_t erro =  fopen_s(&fp, FILENAME, "wb+");
	int weight = 0;
	char word = '\0';
	if (fp == NULL) {
		cout<<"erro"<<endl;
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
	}
}
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
	}
	else
	{
		fseek(fp, 0, SEEK_END);      //将文件指针指向该文件的最后
		file_size_length = ftell(fp);       //根据指针位置，此时可以算出文件的长度
		fseek(fp, 0, SEEK_SET);
		fread(&weight, sizeof(int), 1, fp);
		fread(&word, sizeof(char), 1, fp);
		while (weight!=0&&word!='\0')//判断是否到达写入的边界线，读入字典
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
void decode(char* values, int length, huffman Huf[MAXSIZE * 2 - 1],int max,string& decode_word ) {
	int m = max;
	string num = { '\0' };
	decode_key(values, length, num);//
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

}
/*
解码文件写入操作
*/
void writefile_decode(char* FILENAME,string& decode_word) {
	FILE* fp;
	errno_t erro = fopen_s(&fp, FILENAME, "wb+");
	if (fp == NULL) {
		cout << "erro" << endl;
	}
	else {
		int length = 0;
		length = (int)decode_word.length();
		char* values = new char[length]();
		fwrite(values, sizeof(char), length, fp);
		fclose(fp);
		delete[] values;
	}
}
/*
打印Huffman树
*/
void printfHuf(huffman Huf[MAXSIZE * 2 - 1]) {
	for (int i = 0; i < MAXSIZE * 2 - 1; i++)
	{
		cout.width(6);
		cout << Huf[i].value;
		cout.width(4);
		cout << i << "序数";
		cout.width(6);
		cout << Huf[i].weight;
		cout.width(4);
		cout << Huf[i].ln;
		cout.width(4);
		cout << Huf[i].rn;
		cout.width(4);
		cout << Huf[i].pa;
		cout << endl;
	}
}
/*
完成压缩程序的综合实现与调用
*/
void use_code(char* values, char* filename, char* filename_decode) {
	int word_length = 0;//文件字符长度
	int code_length = 0;//编码长度
	huffman Huf[MAXSIZE * 2 - 1] = { 0 };//设置Huffman树的存储数组
	string coderesult;//存放编码结果

	//1.初始化Huffman树
	huffman_creat(Huf);
	//2.读取文件，并返回文件读取的字符数，以便之后的函数遍历
	word_length = openfile_code(values,filename);
	cout << "文件打开成功！..." << endl;
	//3.遍历获取权值
	getword_weight(Huf, values);
	cout << "完成全文遍历！..." << endl;
	//4.建立树
	HuffmanTree_Creat(Huf);
	cout << "已建立Huffman树！..." << endl;
	//5.编码得到0、1字符串，并返回字符串长度
	code_length = code(values, word_length, Huf, coderesult);
	//6.计算原编码转换成二进制后的字符数，并创建动态二维数组
	code_length = code_length / 8 + (code_length % 8 != 0);
	char* encoding_binary = new char[code_length+1]();
	//7.将0、1字符串转换为字符数组
	Encoding_binary(coderesult,encoding_binary);
	cout << "编码成功！正在写入目标..." << endl;
	//8.写入文件
	writefile_code(filename_decode,Huf,encoding_binary,code_length);
	cout << "压缩成功！" << endl;
	delete[] encoding_binary;//释放内存
}
/*
完成解压程序的综合实现和调用
*/
void use_decode(char* values,char* FILENAME,char* FILENAME_target) {
	int max = 0;
	int length_decode = 0;
	huffman Huf[MAXSIZE * 2 - 1];
	string decode_word;
	//1.初始化一个huffman结构体数组
	huffman_creat(Huf);
	cout << "初始化成功！..." << endl;
	//2.打开目标文件，读取字典到Huffman数组，并返回解压内容及其长度
	length_decode = openfile_decode(Huf, values, FILENAME);
	cout << "文件打开成功！.." << endl;
	//3.重建Huffman树，并返回堆顶元素位置
	max = HuffmanTree_Creat(Huf);
	cout << "Huffman树重建成功！..." << endl;
	//4.解码
	decode(values, length_decode, Huf, max,decode_word);
	cout << "解压成功！正在写入目标文件..." << endl;
	//5.写入解码文件
	writefile_decode(FILENAME_target, decode_word);
	cout << "完成解压！" << endl;
}
int main() {
	char* values = new char[ARRAYSIZE]();
	char filename_code[30] = {'\0'};
	char filename_decode[30] = { '\0' };
	clock_t start_t, end_t;
	double total = 0;

	int button=0;
	cout << "Do you want to extract the file?   -------------  press 1" << endl << "or compress the file?              -------------  press 2" << endl;
	
	while(true) 
	{ 
		cout <<endl<< "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
		cin >> button;
		switch (button)
		{
			case 1:
				cout << "请输入被压缩文件名" << endl;
				cin >> filename_code;
				cout << "请输入压缩后新文件名" << endl;
				cin >> filename_decode;
				start_t = clock();
				cout <<endl<< "processing..." << endl;
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
				cout <<endl<< "processing..." << endl;
				use_decode(values, filename_code, filename_decode);
				end_t = clock();
				total = (end_t - start_t) / CLOCKS_PER_SEC;
				cout  << "the time is:" << total << endl;
				break;
			default:
				exit(-1);
		}
	}
	delete[] values;
}
