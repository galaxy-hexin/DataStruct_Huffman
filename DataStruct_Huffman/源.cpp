#include<iostream>
using namespace std;
constexpr auto ARRAYSIZE = 1024*1024;
constexpr auto WEIGHTMAXSIZE = 10000;
constexpr auto MAXSIZE = 128;

struct huffman {
	int weight;
	char value;
	int ln;//the left node
	int rn;//the right node
	int pa;//the parent node
	char key[10];//the key of the values
};

/*
huffman结构体初始化
实现初始化一个Huffman结构体，将权重置为0，左右结点、对应的huffmankey值置为初值-1
*/
void huffman_creat(huffman Huf[MAXSIZE*2-1]) {
	for (int i = 0; i < MAXSIZE; i++)//初始化已知的maxsize个叶结点。
	{
		Huf[i] = { 0 , (char)i, -1, -1, -1 };
		for (int j = 0; j < 10; j++)
		{
			Huf->key[j] = '\0';
		}
	}
	for (int i = MAXSIZE; i < MAXSIZE * 2 - 1; i++)//初始化以后要存储内结点的maxsize-1个结点
	{
		Huf[i] = { 0, -1, -1, -1, -1 };
		for (int j = 0; j < 10; j++)
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
		cout << "文件内容为: " <<endl<< values << endl;
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
	for (int i = 0; i < MAXSIZE - 1; i++)
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
	char code[15] = { '\0' };//存放每个字符经过逆访问后的逆序编码

	for (int i = 0; i < length; i++)
	{
		signal_current = (int)values[i];//获得当前字符的ASCII码,也就是在Huffman树中的对应的位置
		if (signal_current>255||signal_current<0)//防止出现ASCII码以外的字符。
		{
			cout << "请选择正确的文本，当前文本中存在不合法字符"<<endl;
			exit(1);
		}
		signal_parent = Huf[signal_current].pa;//获得父结点的位置

		while (signal_parent != -1) 
		//不断向父节点遍历，并且验证是左节点还是右节点，左0右1，存入code
		{
			if (Huf[signal_parent].ln == signal_current)
				code[j] = '0';
			else if (Huf[signal_parent].rn == signal_current)
				code[j] = '1';
			signal_current = signal_parent;
			signal_parent = Huf[signal_current].pa;
			j++;
		}
		
		for (int k = 14; k >= 0; k--)
		/*将code逆序处理放入缓存string流*/
		{
			if (code[k] != '\0')
			{
				coderesult = coderesult + code[k];
			}
		}
		for (int k = 0; k < 15; k++)//缓存数组code[]重新置为空
			code[k] = '\0';
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
void writefile_code(char* FILENAME, char* encoding_binary, int length) {
	FILE* fp;
	errno_t erro =  fopen_s(&fp, FILENAME, "wb+");
	if (fp == NULL) {
		cout<<"erro"<<endl;
	}
	else {
		fwrite(encoding_binary, sizeof(char), length, fp);
		fclose(fp);
	}
}

/*
读取操作
实现读取需要解码文件的数据，并返回文件长度
*/
int openfile_decode(char* values, char* Filename)
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
		//cout << "文件内容为: " << endl << values << endl;
		fclose(fp);
	}
	return (int)file_size;
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
void decode(char* values, int length, huffman Huf[MAXSIZE * 2 - 1],int max) {
	int m = max;
	string num = { '\0' };
	decode_key(values, length, num);
	//cout << num;

	for (int i = 0; i < length * 8+1; i++)
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
			cout << Huf[m].value;
			//cout << endl;
			m = max;//将m置为堆顶位置，以便重新遍历。
		}
	}

}

void use_code() {
	int word_length = 0;//文件字符长度
	int code_length = 0;//编码长度
	int max = 0;//堆顶元素获取
	char filename[] = { "1.txt" };
	char filename_decode[] = { "2.txt" };
	char* values= new char[ARRAYSIZE]{ '\0' };
	huffman Huf[MAXSIZE * 2 - 1] = { 0 };//设置Huffman树的存储数组
	string coderesult;//存放编码结果

	huffman_creat(Huf);//初始化Huffman树
	word_length = openfile_code(values,filename);//获取文件读取的字符数，以便之后的函数遍历
	getword_weight(Huf, values);//遍历获取权值
	max = HuffmanTree_Creat(Huf);//建立树并得到树顶元素
	code_length = code(values, word_length, Huf, coderesult);//编码得到0、1字符串，并返回字符串长度
	//cout << coderesult << endl;
	code_length = code_length / 8 + (code_length % 8 != 0);//计算原编码转换成二进制后的字符数，以便创建二维数组
	char* encoding_binary = new char[code_length+1]();//创建一个数组
	Encoding_binary(coderesult,encoding_binary);//将0、1字符串转换为字符数组
	writefile_code(filename_decode,encoding_binary,code_length);//写入文件
	delete[] encoding_binary;//释放内存

	/*for (int i = 0; i < MAXSIZE * 2 - 1; i++)
{
	cout.width(6);
	cout << Huf[i].value;
	cout.width(4);
	cout << i<<"序数";
	cout.width(6);
	cout << Huf[i].weight;
	cout.width(4);
	cout << Huf[i].ln;
	cout.width(4);
	cout << Huf[i].rn;
	cout.width(4);
	cout << Huf[i].pa;
	cout << endl;
}*/


	cout << "译码内容为：" << endl;
	int length_decode = 0;
	length_decode = openfile_decode(values,filename_decode);
	decode(values,length_decode,Huf,max);
	delete[]  values;
}

int main() {
	use_code();;
}

