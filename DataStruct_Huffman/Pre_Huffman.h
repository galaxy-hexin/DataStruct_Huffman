#pragma once
#include<iostream>
using namespace std;
constexpr auto ARRAYSIZE = 1024 * 1024 * 10;
constexpr auto WEIGHTMAXSIZE = 1024 * 1024 * 1024;
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
bool huffman_creat(huffman Huf[MAXSIZE * 2 - 1]) {
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
	return true;
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
			if (Huf[j].weight > 0 && Huf[j].weight < weight_min1 && Huf[j].pa == -1)
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
		if (x1 != -1 && x2 != -1)
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
		if (Huf[i].weight != 0)//权值不为零才有计算的必要
		{
			int m = 0;
			signal_current = (int)i;//当前位置信号量
			signal_parent = Huf[signal_current].pa;//当前位置的父结点的信号量
			while (signal_parent != -1)//父结点为-1时则为最末端
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