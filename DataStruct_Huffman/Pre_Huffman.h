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
huffman�ṹ���ʼ��
ʵ�ֳ�ʼ��һ��Huffman�ṹ�壬��Ȩ����Ϊ0�����ҽ�㡢��Ӧ��huffmankeyֵ��Ϊ��ֵ-1
*/
bool huffman_creat(huffman Huf[MAXSIZE * 2 - 1]) {
	for (int i = 0; i < MAXSIZE; i++)//��ʼ����֪��maxsize��Ҷ��㡣
	{
		Huf[i] = { 0 , (char)i, -1, -1, -1 };
		for (int j = 0; j < 20; j++)
		{
			Huf->key[j] = '\0';
		}
	}
	for (int i = MAXSIZE; i < MAXSIZE * 2 - 1; i++)//��ʼ���Ժ�Ҫ�洢�ڽ���maxsize-1�����
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
Huffman������
ʵ��Huffman���Ĵ����������Ѿ��õ����ַ�Ƶ�ʵ�Huffman������д���
ʵ�ַ�������������weightֵ����ģ�����Ѱ��������СȨ�صĽ�㣬������Ӻ���Ϊ�½�㣬���޸�������С��ĸ������½���Ȩ��ֵ�������ӽڵ㡢�Լ�Ȩ��
*/
int HuffmanTree_Creat(huffman Huf[MAXSIZE * 2 - 1]) {
	int max = 0;
	int j = 0;
	char code[20] = { '\0' };//���ÿ���ַ���������ʺ���������
	int signal_current = 0;//��ǰ�ַ���λ��
	int signal_parent = 0;//���ڵ���ַ�λ��
	for (int i = 0; i < MAXSIZE; i++)
		//��Ҫ�Ľ�������
	{
		/*����MAXSIZE�飬ÿ����ѡȨ����С������*/
		/*��x1��x2����������Ԫ���������е�λ�ã����ñ���weight1��weight2���б������ǵ�Ȩ��*/
		int weight_min1, weight_min2;
		int x1, x2;
		weight_min1 = weight_min2 = WEIGHTMAXSIZE;//������Ҫ��αȽϺ�ȡСֵ�����Դ�����ĳ�ֵ�����㹻�Ĵ�
		x1 = x2 = -1;

		for (int j = 0; j < MAXSIZE + i; j++)
		{
			if (Huf[j].weight > 0 && Huf[j].weight < weight_min1 && Huf[j].pa == -1)
				//������1��.��֤�ý�������ַ����ֹ�����Ȩ�ش���0  
				//������2��.���ַ�С��weight_min1��ŵ��ַ���Ȩ��ʱ������weight_min1��ֵ
				//������3).ֻ��û�и��ڵ�Ľ�㣨��pa����Ϊ��ֵ-1�����б����ı�Ҫ
			{
				weight_min2 = weight_min1;
				weight_min1 = Huf[j].weight;
				x2 = x1;
				x1 = j;
			}
			else if (Huf[j].weight > 0 && Huf[j].weight < weight_min2 && Huf[j].pa == -1)
				//������2������weight_min1,С��weight_min2��ֻ�ƶ�weight_min2
			{
				weight_min2 = Huf[j].weight;
				x2 = j;
			}
		}
		/*�ڵ�n+i��λ���ϸ�ֵȨ�أ����������ӽڵ�λ�á�*/
		/*������������СȨ�ؽ��ĸ��ڵ㡣*/
		/*ֻ�е�x1��x2��ֵ�����ı�Ž��и�ֵ*/
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
		//���ÿһ����Ȩ�ص��ַ�keyֵ����
		//����maxsize��
	{
		if (Huf[i].weight != 0)//Ȩֵ��Ϊ����м���ı�Ҫ
		{
			int m = 0;
			signal_current = (int)i;//��ǰλ���ź���
			signal_parent = Huf[signal_current].pa;//��ǰλ�õĸ������ź���
			while (signal_parent != -1)//�����Ϊ-1ʱ��Ϊ��ĩ��
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
				/*��code���������keyֵ*/
			{
				if (code[k] != '\0')
				{
					Huf[i].key[m] = code[k];
					//cout << code[k];
					m++;
				}
			}
			for (int k = 0; k < 20; k++)//��������code[]������Ϊ��
				code[k] = '\0';
			j = 0;
		}
	}

	return max;
}
/*
��ӡHuffman��
*/
void printfHuf(huffman Huf[MAXSIZE * 2 - 1]) {
	for (int i = 0; i < MAXSIZE * 2 - 1; i++)
	{
		cout.width(6);
		cout << Huf[i].value;
		cout.width(4);
		cout << i << "����";
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