#include"p.h"

int main()
{
	RowMatrix<int> m1(2, 2);
	std::vector<int> s1 = { 1,2,3,4 };
	//FillFrom()，GetColumnCount()，GetRowCount()，GetColumnCount()函数测试
	m1.FillFrom(s1);
	printf("矩阵m1:\n维数:(%d,%d)\n", m1.GetRowCount(),m1.GetColumnCount());
	for (int i = 0; i < m1.GetRowCount(); i++)
	{
		for (int j = 0; j < m1.GetColumnCount(); j++)
		{
			printf("%d\t", m1.GetElement(i, j));
		}
		printf("\n");
	}
	printf("\n");

	//SetElement()函数测试
	m1.SetElement(0, 0, 0);
	printf("使用SetElement(0, 0, 0)将m1[0,0]设置为0\n");
	printf("矩阵m1:\n维数(%d,%d)\n", m1.GetRowCount(), m1.GetColumnCount());
	for (int i = 0; i < m1.GetRowCount(); i++)
	{
		for (int j = 0; j < m1.GetColumnCount(); j++)
		{
			printf("%d\t", m1.GetElement(i, j));
		}
		printf("\n");
	}
	printf("\n");

	//FillFrom()，GetColumnCount()，GetRowCount()，GetColumnCount()函数测试
	RowMatrix<int> m2(2, 2);
	std::vector<int> s2 = { 10,20,30,40 };
	m2.FillFrom(s2);
	printf("矩阵m2:\n维数:(%d,%d)\n", m2.GetRowCount(), m2.GetColumnCount());
	for (int i = 0; i < m2.GetRowCount(); i++)
	{
		for (int j = 0; j < m2.GetColumnCount(); j++)
		{
			printf("%d\t", m2.GetElement(i, j));
		}
		printf("\n");
	}
	printf("\n");

	//Add(),Multiply(),GEMM()函数测试
	RowMatrixOperations<int>compute;
	RowMatrix<int> *m1p = &m1;
	RowMatrix<int> *m2p = &m2;
	std::unique_ptr<RowMatrix<int>> r;
	r=compute.Add(m1p, m2p);
	printf("使用Add()函数计算矩阵m1+m2:\n");
	printf("维数:(%d,%d)\n", r->GetRowCount(), r->GetColumnCount());
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
	printf("使用Multiply()函数计算矩阵m1*m2:\n");
	printf("维数:(%d,%d)\n", r->GetRowCount(), r->GetColumnCount());
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
	printf("使用GEMM()函数计算矩阵m1*m2+m1：\n");
	printf("维数:(%d,%d)\n", r->GetRowCount(), r->GetColumnCount());
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


