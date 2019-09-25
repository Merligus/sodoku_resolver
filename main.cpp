#include <fstream>
#include <iostream>
#include <vector>

class quadrado
{
public:
	int linhaIni, linhaFin;
	int colunaIni, colunaFin;
};

class duas_ocorrencias
{
public:
	int linha1, linha2;
	int coluna1, coluna2;
	int numero;
};

class celula
{
public:
	int quad, linha, coluna, nPossibilidades;
	std::vector<bool> possiveis;
	int certeza;

	celula()
	{
		int i;
		for (i = 0; i < 9; i++)
			possiveis.push_back(false);
	}

	void conta_possibilidades()
	{
		int i;
		nPossibilidades = 0;
		for (i = 0; i < 9; i++)
			if (possiveis[i] == true)
				nPossibilidades++;
	}
	
};

bool verificar_se_tem(int numero, bool linha, int linha_ou_coluna, celula tabela[][9])
{
	int i;
	if (linha)
	{
		for (i = 0; i < 9; i++)
			if (tabela[linha_ou_coluna][i].certeza == numero)
				return true;
		return false;
	}
	else
	{
		for (i = 0; i < 9; i++)
			if (tabela[i][linha_ou_coluna].certeza == numero)
				return true;
		return false;
	}
}

bool verificar_se_tem_no_quadrado(int numero, int quad, celula tabela[][9], quadrado lista_quadrados[9])
{
	int i, j;
	for (i = lista_quadrados[quad].linhaIni; i < lista_quadrados[quad].linhaFin; i++)
		for (j = lista_quadrados[quad].colunaIni; j < lista_quadrados[quad].colunaFin; j++)
			if (tabela[i][j].certeza == numero)
				return true;
	return false;
}

void atualizar_tabela(int numero, int linha, int coluna, celula tabela[][9], int quadrados[][9], quadrado lista_quadrados[9])
{
	int quad;
	int i, j, k;

	for (k = 0; k < 9; k++)
		tabela[linha][coluna].possiveis[k] = false;
	tabela[linha][coluna].possiveis[numero - 1] = true;
	tabela[linha][coluna].nPossibilidades = 1;

	for (j = 0; j < 9; j++)
	{
		tabela[linha][j].possiveis[numero - 1] = false;
		tabela[linha][j].conta_possibilidades();
	}

	for (i = 0; i < 9; i++)
	{
		tabela[i][coluna].possiveis[numero - 1] = false;
		tabela[i][coluna].conta_possibilidades();
	}

	quad = quadrados[linha][coluna];
	for (i = lista_quadrados[quad].linhaIni; i < lista_quadrados[quad].linhaFin; i++)
		for (j = lista_quadrados[quad].colunaIni; j < lista_quadrados[quad].colunaFin; j++)
			if(i != linha || j != coluna)
			{
				tabela[i][j].possiveis[numero - 1] = false;
				tabela[i][j].conta_possibilidades();
			}
}

bool resolve(celula tabela[][9], quadrado lista_quadrados[9], int quadrados[][9])
{
	celula tabela_backup[9][9];
	bool alterou, finished, possivel;
	int i, j, k, alternativo;
	std::vector<celula> lista_de_possiveis;
	duas_ocorrencias item;
	std::vector<duas_ocorrencias> lista_de_possiveis_31;

	alterou = true;
	while (alterou)
	{
		alterou = false;
		// 2. ve se tem só 1 possibilidade em cada casa/quadrado/linha/coluna
		// cada casa
		for (i = 0; i < 9; i++)
		{
			for (j = 0; j < 9; j++)
			{
				if (tabela[i][j].nPossibilidades == 1 && tabela[i][j].certeza == 0)
				{
					for (k = 0; k < 9; k++)
						if (tabela[i][j].possiveis[k] == true)
						{
							tabela[i][j].certeza = k + 1;
							atualizar_tabela(k + 1, i, j, tabela, quadrados, lista_quadrados);
							std::cout << "Certeza por casa: " << i << " " << j << " " << "num:" << k + 1 << "\n";
							alterou = true;
							break;
						}
				}
			}
		}

		// cada linha
		for (i = 0; i < 9; i++)
		{
			for (k = 0; k < 9; k++)
			{
				int contador = 0;
				int coluna;
				for (j = 0; j < 9; j++)
					if (tabela[i][j].possiveis[k] == true)
					{
						contador++;
						coluna = j;
					}
				if (contador == 1 && tabela[i][coluna].certeza == 0)
				{
					tabela[i][coluna].certeza = k + 1;
					atualizar_tabela(k + 1, i, coluna, tabela, quadrados, lista_quadrados);
					std::cout << "Certeza por linha: " << i << " " << coluna << " " << "num:" << k + 1 << "\n";
					alterou = true;
				}
			}
		}

		// cada coluna
		for (j = 0; j < 9; j++)
		{
			for (k = 0; k < 9; k++)
			{
				int contador = 0;
				int linha;
				for (i = 0; i < 9; i++)
					if (tabela[i][j].possiveis[k] == true)
					{
						contador++;
						linha = i;
					}
				if (contador == 1 && tabela[linha][j].certeza == 0)
				{
					tabela[linha][j].certeza = k + 1;
					atualizar_tabela(k + 1, linha, j, tabela, quadrados, lista_quadrados);
					std::cout << "Certeza por coluna: " << linha << " " << j << " " << "num:" << k + 1 << "\n";
					alterou = true;
				}
			}
		}

		// 3.1 verificar os números com 2 opções se estão 
		// nas mesmas casas.Se estiverem, tirar os outros dessa casa.
		// cada quadrado
		for (int quad = 0; quad < 9; quad++)
		{
			lista_de_possiveis_31.clear();
			for (k = 0; k < 9; k++)
			{
				int contador = 0;
				for (i = lista_quadrados[quad].linhaIni; i < lista_quadrados[quad].linhaFin; i++)
					for (j = lista_quadrados[quad].colunaIni; j < lista_quadrados[quad].colunaFin; j++)
						if (tabela[i][j].possiveis[k] == true && tabela[i][j].certeza == 0)
						{
							contador++;
							if (contador == 1)
							{
								item.linha1 = i;
								item.coluna1 = j;
							}
							else if (contador == 2)
							{
								item.linha2 = i;
								item.coluna2 = j;
							}
						}
				if (contador == 1)
				{
					tabela[item.linha1][item.coluna1].certeza = k + 1;
					atualizar_tabela(k + 1, item.linha1, item.coluna1, tabela, quadrados, lista_quadrados);
					std::cout << "Certeza por quadrado: " << item.linha1 << " " << item.coluna1 << " num:" << k + 1 << "\n";
					alterou = true;
				}
				else if (contador == 2)
				{
					lista_de_possiveis_31.push_back(item);
					lista_de_possiveis_31[lista_de_possiveis_31.size() - 1].numero = k + 1;
				}
			}
			for (int a = 0; a < lista_de_possiveis_31.size(); a++)
			{
				for (int b = a + 1; b < lista_de_possiveis_31.size(); b++)
				{
					if ((lista_de_possiveis_31[a].linha1 == lista_de_possiveis_31[b].linha1 && lista_de_possiveis_31[a].coluna1 == lista_de_possiveis_31[b].coluna1) && (lista_de_possiveis_31[a].linha2 == lista_de_possiveis_31[b].linha2 && lista_de_possiveis_31[a].coluna2 == lista_de_possiveis_31[b].coluna2))
					{
						std::cout << "3.1:par1: " << lista_de_possiveis_31[a].linha1 << " " << lista_de_possiveis_31[a].coluna1 << " par2: " << lista_de_possiveis_31[a].linha2 << " " << lista_de_possiveis_31[a].coluna2 << " numeros:" << lista_de_possiveis_31[a].numero << " " << lista_de_possiveis_31[b].numero << "\n";
						int aux = tabela[lista_de_possiveis_31[a].linha1][lista_de_possiveis_31[a].coluna1].nPossibilidades;
						for (i = 0; i < 9; i++)
							tabela[lista_de_possiveis_31[a].linha1][lista_de_possiveis_31[a].coluna1].possiveis[i] = false;
						tabela[lista_de_possiveis_31[a].linha1][lista_de_possiveis_31[a].coluna1].possiveis[lista_de_possiveis_31[a].numero - 1] = true;
						tabela[lista_de_possiveis_31[a].linha1][lista_de_possiveis_31[a].coluna1].possiveis[lista_de_possiveis_31[b].numero - 1] = true;
						tabela[lista_de_possiveis_31[a].linha1][lista_de_possiveis_31[a].coluna1].conta_possibilidades();
						if(aux != tabela[lista_de_possiveis_31[a].linha1][lista_de_possiveis_31[a].coluna1].nPossibilidades)
							alterou = true;

						aux = tabela[lista_de_possiveis_31[a].linha2][lista_de_possiveis_31[a].coluna2].nPossibilidades;
						for (i = 0; i < 9; i++)
							tabela[lista_de_possiveis_31[a].linha2][lista_de_possiveis_31[a].coluna2].possiveis[i] = false;
						tabela[lista_de_possiveis_31[a].linha2][lista_de_possiveis_31[a].coluna2].possiveis[lista_de_possiveis_31[a].numero - 1] = true;
						tabela[lista_de_possiveis_31[a].linha2][lista_de_possiveis_31[a].coluna2].possiveis[lista_de_possiveis_31[b].numero - 1] = true;
						tabela[lista_de_possiveis_31[a].linha2][lista_de_possiveis_31[a].coluna2].conta_possibilidades();
						if (aux != tabela[lista_de_possiveis_31[a].linha2][lista_de_possiveis_31[a].coluna2].nPossibilidades)
							alterou = true;
					}
				}
			}
		}

		// 3.2 verificar duas casas com apenas 2 opções, se 
		// forem os mesmos números, tirar as possibilidades 
		// desses 2 números em outras casas do mesmo quadrado.
		for (int quad = 0; quad < 9; quad++)
		{
			lista_de_possiveis.clear();
			for (i = lista_quadrados[quad].linhaIni; i < lista_quadrados[quad].linhaFin; i++)
				for (j = lista_quadrados[quad].colunaIni; j < lista_quadrados[quad].colunaFin; j++)
					if (tabela[i][j].nPossibilidades == 2 && tabela[i][j].certeza == 0)
						lista_de_possiveis.push_back(tabela[i][j]);
			for (int a = 0; a < lista_de_possiveis.size(); a++)
			{
				for (int b = a + 1; b < lista_de_possiveis.size(); b++)
				{
					std::vector<int> par1, par2;
					for (int c = 0; c < 9; c++)
						if (lista_de_possiveis[a].possiveis[c] == true)
							par1.push_back(c);
					int d = 0;
					for (int c = 0; c < 9; c++)
						if (lista_de_possiveis[b].possiveis[c] == true && par1[d] == c)
						{
							par2.push_back(c);
							d++;
						}
					if (d == 2)
					{
						for (i = lista_quadrados[quad].linhaIni; i < lista_quadrados[quad].linhaFin; i++)
							for (j = lista_quadrados[quad].colunaIni; j < lista_quadrados[quad].colunaFin; j++)
								if( (i != lista_de_possiveis[a].linha || j != lista_de_possiveis[a].coluna) && (i != lista_de_possiveis[b].linha || j != lista_de_possiveis[b].coluna))
								{
									tabela[i][j].possiveis[par2[0]] = false;
									tabela[i][j].possiveis[par2[1]] = false;
									int aux = tabela[i][j].nPossibilidades;
									tabela[i][j].conta_possibilidades();
									if (aux != tabela[i][j].nPossibilidades)
										alterou = true;
								}
						std::cout << "3.2: numeros" << par2[0] << " " << par2[1];
						std::cout << "\n";
					}
				}
			}	
		}
		// 4. verificar cada linha/coluna se o número
		// aparece em apenas um dos quadrados, tirar 
		// esse mesmo número das outras casas que não 
		// seja essa linha/coluna.
		// por linha
		for(i = 0; i < 9; i++)
		{
			for(k = 0; k < 9; k++)
			{
				int quads[3];
				quads[0] = quads[1] = quads[2] = 0;
				for(j = 0; j < 9; j++)
				{
					if(tabela[i][j].possiveis[k] == true && tabela[i][j].certeza == 0)
						quads[(int)(j/3)] = 1;
				}
				if(quads[0] + quads[1] + quads[2] == 1)
					for(int q = 0; q < 3; q++)
					{
						if(quads[q] == 1)
						{
							int quad = quadrados[i][3*q];
							std::cout << "4 por linha: " << i << " quadrado:" << quad << " num:" << k + 1;
							std::cout << std::endl;
							for (int linha = lista_quadrados[quad].linhaIni; linha < lista_quadrados[quad].linhaFin; linha++)
								for (j = lista_quadrados[quad].colunaIni; j < lista_quadrados[quad].colunaFin; j++)
									if(i != linha)
										if(tabela[linha][j].possiveis[k] == true)
										{
											tabela[linha][j].possiveis[k] = false;
											tabela[linha][j].conta_possibilidades();
											alterou = true;
										}
						}
					}
			}
		}
		// por coluna
		for(j = 0; j < 9; j++)
		{
			for(k = 0; k < 9; k++)
			{
				int quads[3];
				quads[0] = quads[1] = quads[2] = 0;
				for(i = 0; i < 9; i++)
				{
					if(tabela[i][j].possiveis[k] == true && tabela[i][j].certeza == 0)
						quads[(int)(i/3)] = 1;
				}
				if(quads[0] + quads[1] + quads[2] == 1)
					for(int q = 0; q < 3; q++)
					{
						if(quads[q] == 1)
						{
							int quad = quadrados[3*q][j];
							std::cout << "4 por coluna: " << j << " quadrado:" << quad << " num:" << k + 1;
							std::cout << std::endl;
							for (i = lista_quadrados[quad].linhaIni; i < lista_quadrados[quad].linhaFin; i++)
								for (int coluna = lista_quadrados[quad].colunaIni; coluna < lista_quadrados[quad].colunaFin; coluna++)
									if(j != coluna)
										if(tabela[i][coluna].possiveis[k] == true)
										{
											tabela[i][coluna].possiveis[k] = false;
											tabela[i][coluna].conta_possibilidades();
											alterou = true;
										}
						}
					}
			}
		}

		// 5. verificar se o número está em apenas
		// um sentido (linha ou coluna). Se estiver,
		// tirar esse mesmo número dos outros quadrados.
		for(int quad = 0; quad < 9; quad++)
		{
			for(k = 0; k < 9; k++)
			{
				int colunas[3], linhas[3];
				colunas[0] = colunas[1] = colunas[2] = 0;
				linhas[0] = linhas[1] = linhas[2] = 0;
				for (i = lista_quadrados[quad].linhaIni; i < lista_quadrados[quad].linhaFin; i++)
					for (j = lista_quadrados[quad].colunaIni; j < lista_quadrados[quad].colunaFin; j++)
						if(tabela[i][j].possiveis[k] == true && tabela[i][j].certeza == 0)
						{
							colunas[j - lista_quadrados[quad].colunaIni] = 1;
							linhas[i - lista_quadrados[quad].linhaIni] = 1;
						}
				if(colunas[0] + colunas[1] + colunas[2] == 1)
					for(int coluna = 0; coluna < 3; coluna++)
						if (colunas[coluna] == 1)
						{
							for (i = 0; i < 9; i++)
								if (quadrados[i][coluna + lista_quadrados[quad].colunaIni] != quad)
									if (tabela[i][coluna + lista_quadrados[quad].colunaIni].possiveis[k] == true)
									{
										tabela[i][coluna + lista_quadrados[quad].colunaIni].possiveis[k] = false;
										tabela[i][coluna + lista_quadrados[quad].colunaIni].conta_possibilidades();
										alterou = true;
									}
							std::cout << "5 por coluna" << coluna + lista_quadrados[quad].colunaIni << " num:" << k + 1;
							std::cout << std::endl;
						}
				if(linhas[0] + linhas[1] + linhas[2] == 1)
					for(int linha = 0; linha < 3; linha++)
						if (linhas[linha] == 1)
						{
							for (j = 0; j < 9; j++)
								if (quadrados[linha + lista_quadrados[quad].linhaIni][j] != quad)
									if (tabela[linha + lista_quadrados[quad].linhaIni][j].possiveis[k] == true)
									{
										tabela[linha + lista_quadrados[quad].linhaIni][j].possiveis[k] = false;
										tabela[linha + lista_quadrados[quad].linhaIni][j].conta_possibilidades();
										alterou = true;
									}
							std::cout << "5 por linha" << linha + lista_quadrados[quad].linhaIni << " num:" << k + 1;
							std::cout << std::endl;
						}
			}
		}
	} // fim while(alterou)

	// 7. se o loop repetiu sem nenhuma alteração, 
	// fazer um path com um chute em uma das casas 
	// com 2 opções. Quando não restar nenhuma opção 
	// com 100% verificar se não é um absurdo o chute
	// inicial.
	finished = true;
	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++)
			if (tabela[i][j].certeza != 0)
			{
				k = tabela[i][j].certeza;
				tabela[i][j].certeza = 0;
				possivel = true;
				possivel = possivel & !verificar_se_tem(k, true, i, tabela);
				possivel = possivel & !verificar_se_tem(k, false, j, tabela);
				possivel = possivel & !verificar_se_tem_no_quadrado(k, quadrados[i][j], tabela, lista_quadrados);
				if(!possivel)
					return false;
				tabela[i][j].certeza = k;
			}
			else
				finished = false;

	if(finished)
		return true;
	else
	{
		for (i = 0; i < 9; i++)
			for (j = 0; j < 9; j++)
				tabela_backup[i][j] = tabela[i][j];

		for (i = 0; i < 9; i++)
			for (j = 0; j < 9; j++)
				if(tabela[i][j].nPossibilidades == 2 && tabela[i][j].certeza == 0)
					for(k = 0; k < 9; k++)
						if(tabela[i][j].possiveis[k] == true)
						{
							tabela[i][j].certeza = k + 1;
							for(int l = k + 1; l < 9; l++)
								if(tabela[i][j].possiveis[l] == true)
									alternativo = l + 1;
							atualizar_tabela(k + 1, i, j, tabela, quadrados, lista_quadrados);
							if(resolve(tabela, lista_quadrados, quadrados))
								return true;
							else
							{
								for (int ii = 0; ii < 9; ii++)
									for (int jj = 0; jj < 9; jj++)
										tabela[ii][jj] = tabela_backup[ii][jj];
								tabela[i][j].certeza = alternativo;
								atualizar_tabela(alternativo, i, j, tabela, quadrados, lista_quadrados);
								return resolve(tabela, lista_quadrados, quadrados);
							}
						}
	}
}

int main()
{
	celula tabela[9][9];
	quadrado lista_quadrados[9];
	int quadrados[9][9];
	int i, j, k;
	bool possivel;
	std::ifstream input_game;

	lista_quadrados[0].linhaIni = lista_quadrados[1].linhaIni = lista_quadrados[2].linhaIni = lista_quadrados[0].colunaIni = lista_quadrados[3].colunaIni = lista_quadrados[6].colunaIni = 0;
	lista_quadrados[0].linhaFin = lista_quadrados[1].linhaFin = lista_quadrados[2].linhaFin = lista_quadrados[0].colunaFin = lista_quadrados[3].colunaFin = lista_quadrados[6].colunaFin = lista_quadrados[3].linhaIni = lista_quadrados[4].linhaIni = lista_quadrados[5].linhaIni = lista_quadrados[1].colunaIni = lista_quadrados[4].colunaIni = lista_quadrados[7].colunaIni = 3;
	lista_quadrados[1].colunaFin = lista_quadrados[4].colunaFin = lista_quadrados[7].colunaFin = lista_quadrados[2].colunaIni = lista_quadrados[5].colunaIni = lista_quadrados[8].colunaIni = lista_quadrados[3].linhaFin = lista_quadrados[4].linhaFin = lista_quadrados[5].linhaFin = lista_quadrados[6].linhaIni = lista_quadrados[7].linhaIni = lista_quadrados[8].linhaIni = 6;
	lista_quadrados[2].colunaFin = lista_quadrados[5].colunaFin = lista_quadrados[8].colunaFin = lista_quadrados[6].linhaFin = lista_quadrados[7].linhaFin = lista_quadrados[8].linhaFin = 9;
	
	for (k = 0; k < 9; k++)
		for (i = lista_quadrados[k].linhaIni; i < lista_quadrados[k].linhaFin; i++)
			for (j = lista_quadrados[k].colunaIni; j < lista_quadrados[k].colunaFin; j++)
				quadrados[i][j] = k;

	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
			std::cout << quadrados[i][j] << " ";
		std::cout << "\n";
	}
	std::cout << "\n";

	input_game.open("game.txt", std::fstream::in);
	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++)
		{
			input_game >> tabela[i][j].certeza;
			tabela[i][j].linha = i;
			tabela[i][j].coluna = j;
			tabela[i][j].quad = quadrados[i][j];
		}
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
			std::cout << tabela[i][j].certeza << " ";
		std::cout << "\n";
	}

	// 1. coloca todas as possibilidades
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (tabela[i][j].certeza == 0)
			{
				for (k = 1; k <= 9; k++)
				{
					possivel = true;
					possivel = possivel & !verificar_se_tem(k, true, i, tabela);
					possivel = possivel & !verificar_se_tem(k, false, j, tabela);
					possivel = possivel & !verificar_se_tem_no_quadrado(k, quadrados[i][j], tabela, lista_quadrados);
					std::cout << i << " " << j << " " << "num:" << k << " " << (possivel ? 1 : 0) << "\n";
					tabela[i][j].possiveis[k - 1] = possivel;
				}
				tabela[i][j].conta_possibilidades();
			}
			else
			{
				tabela[i][j].possiveis[tabela[i][j].certeza - 1] = true;
				tabela[i][j].nPossibilidades = 1;
			}
		}
	}

	resolve(tabela, lista_quadrados, quadrados);

	// cabo
	std::cout << "\n";
	std::cout << "RESPOSTA\n";
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
			std::cout << tabela[i][j].certeza << " ";
		std::cout << "\n";
	}
	std::cout << "\n";

	input_game.close();
	system("pause");
	return 0;
}