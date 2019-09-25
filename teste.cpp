#include <iostream>
#include <vector>

int teste(int mat[][3])
{
	mat[0][0] += 3;
	return 1;
}

int maian()
{
	int n;
	int matriz[3][3];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			matriz[i][j] = 2;
	matriz[0][0] = 0;
	teste(matriz);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			std::cout << matriz[i][j] << " ";
		std::cout << std::endl;
	}
	/*std::vector<int> vec;
	std::cin >> n;
	vec.push_back(n);
	std::cin >> n;
	vec.push_back(n);
	std::cout << vec[0] << " " << vec[1];*/
	system("pause");
	return 0;
}