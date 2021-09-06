#include"p.h"

int main()
{
	RowMatrix<int> m1(2, 2);
	std::vector<int> s1 = { 1,2,3,4 };
	//FillFrom()��GetColumnCount()��GetRowCount()��GetColumnCount()��������
	m1.FillFrom(s1);
	printf("����m1:\nά��:(%d,%d)\n", m1.GetRowCount(),m1.GetColumnCount());
	for (int i = 0; i < m1.GetRowCount(); i++)
	{
		for (int j = 0; j < m1.GetColumnCount(); j++)
		{
			printf("%d\t", m1.GetElement(i, j));
		}
		printf("\n");
	}
	printf("\n");

	//SetElement()��������
	m1.SetElement(0, 0, 0);
	printf("ʹ��SetElement(0, 0, 0)��m1[0,0]����Ϊ0\n");
	printf("����m1:\nά��(%d,%d)\n", m1.GetRowCount(), m1.GetColumnCount());
	for (int i = 0; i < m1.GetRowCount(); i++)
	{
		for (int j = 0; j < m1.GetColumnCount(); j++)
		{
			printf("%d\t", m1.GetElement(i, j));
		}
		printf("\n");
	}
	printf("\n");

	//FillFrom()��GetColumnCount()��GetRowCount()��GetColumnCount()��������
	RowMatrix<int> m2(2, 2);
	std::vector<int> s2 = { 10,20,30,40 };
	m2.FillFrom(s2);
	printf("����m2:\nά��:(%d,%d)\n", m2.GetRowCount(), m2.GetColumnCount());
	for (int i = 0; i < m2.GetRowCount(); i++)
	{
		for (int j = 0; j < m2.GetColumnCount(); j++)
		{
			printf("%d\t", m2.GetElement(i, j));
		}
		printf("\n");
	}
	printf("\n");

	//Add(),Multiply(),GEMM()��������
	RowMatrixOperations<int>compute;
	RowMatrix<int> *m1p = &m1;
	RowMatrix<int> *m2p = &m2;
	std::unique_ptr<RowMatrix<int>> r;
	r=compute.Add(m1p, m2p);
	printf("ʹ��Add()�����������m1+m2:\n");
	printf("ά��:(%d,%d)\n", r->GetRowCount(), r->GetColumnCount());
	for (int i = 0; i < r->GetRowCount(); i++)
	{
		for (int j = 0; j < r->GetColumnCount(); j++)
		{
			printf("%d\t", r->GetElement(i, j));
		}
		printf("\n");
	}
	printf("\n");


	r = compute.Multiply(m1p, m2p);
	printf("ʹ��Multiply()�����������m1*m2:\n");
	printf("ά��:(%d,%d)\n", r->GetRowCount(), r->GetColumnCount());
	for (int i = 0; i < r->GetRowCount(); i++)
	{
		for (int j = 0; j < r->GetColumnCount(); j++)
		{
			printf("%d\t", r->GetElement(i, j));
		}
		printf("\n");
	}
	printf("\n");

	r = compute.GEMM(m1p, m2p, m1p);
	printf("ʹ��GEMM()�����������m1*m2+m1��\n");
	printf("ά��:(%d,%d)\n", r->GetRowCount(), r->GetColumnCount());
	for (int i = 0; i < r->GetRowCount(); i++)
	{
		for (int j = 0; j < r->GetColumnCount(); j++)
		{
			printf("%d\t", r->GetElement(i, j));
		}
		printf("\n");
	}

	return 0;
}


