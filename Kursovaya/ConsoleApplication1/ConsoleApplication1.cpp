#include <iostream>
#include <conio.h>
using namespace std;

struct edge
{
	int weight; // вес от 0 до 1023
	char vertex1; // первая вершина
	char vertex2; // вторая вершина
	char set1; // множесто первой вершины 
	char set2; // множесто второй вершины 
	edge(char v1 = '\0', char v2 = '\0', int w = -1) // конструктор
	{
		weight = w;
		vertex1 = set1 = v1;
		vertex2 = set2 = v2;
	}
	edge& operator = (const edge &par) // оператор присваивания
	{
		if (this == &par)
			return *this;
		weight = par.weight;
		vertex1 = par.vertex1;
		vertex2 = par.vertex2;
		return *this;
	}
};

edge *inc_arr(const edge *old_arr, unsigned size, unsigned &max_size) // расширение динамического массива
{
	max_size *= 2;
	edge *new_arr = new edge[max_size];
	for (int i = 0; i < size; i++)
	{
		new_arr[i] = old_arr[i];
	}
	delete[]old_arr;
	return new_arr;
}

int getnumber(int bound) // ввод числа от 0 до границы
{
	char a;
	int number = 0;
	do
	{
		a = _getch();
		if (a == 8)
		{
			cout << "\b \b";
			number /= 10;
		}
		if ((a >= 48) && (a <= 57) && (number * 10 + (int)a - 48 <= bound))
		{
			a -= 48;
			number = number * 10 + (int)a;
			cout << (int)a;
			a = '\0';
		}
	} while (a != 13);
	return number;
}

bool edge_already_exists(edge *arr, unsigned number_of_edges) // существует ли уже такая грань в данном наборе
{
	if (arr[number_of_edges].vertex2 == '\0' || number_of_edges == 0)
		return false;
	char vertex1 = arr[number_of_edges].vertex1;
	char vertex2 = arr[number_of_edges].vertex2;
	for (int i = 0; i < number_of_edges; i++)
	{
		if ((arr[i].vertex1 == vertex1 && arr[i].vertex2 == vertex2) || (arr[i].vertex1 == vertex2 && arr[i].vertex2 == vertex1)) // если встретили пару такихже вершин, то они уже существуют
			return true;
	}
	return false;
}

bool vertex_first_appearance(edge *arr, unsigned number_of_edges) // встречается ли вершина впервые
{
	if (arr[number_of_edges].vertex2 == '\0' || number_of_edges == 0)
		return false;
	char vertex1 = arr[number_of_edges].vertex1;
	char vertex2 = arr[number_of_edges].vertex2;
	for (int i = 0; i < number_of_edges; i++)
	{
		if ((arr[i].vertex1 == vertex2 || arr[i].vertex2 == vertex2) || (arr[i].vertex1 == vertex1 || arr[i].vertex2 == vertex1)) // если встретили вершину с темже именем, то мы ее видим не в первый раз
		{
			return false;
		}
	}
	return true;
}

//bool vertices_first_appearance(edge *arr, unsigned number_of_edges) // встречаются ли обе вершины впервые
//{
//	if (number_of_edges == 0)
//		return true;
//	char vertex1 = arr[number_of_edges].vertex1;
//	int set1 = arr[number_of_edges].set1;
//	char vertex2 = arr[number_of_edges].vertex2;
//	int set2 = arr[number_of_edges].set2;
//	for (int i = 0; i < number_of_edges; i++)
//	{
//		for (int j = 0; j < number_of_edges; j++) 
//		{
//			if ((arr[i].vertex1 == vertex1 || arr[i].vertex2 == vertex1) && (arr[j].vertex1 == vertex2 || arr[j].vertex2 == vertex2))
//					return false;
//		}
//	}
//	return true;
//}

void insertion_sort_for_weight(edge *arr, int arr_size, int beg = 0)
{
	edge temp;
	int key;
	int j;
	for (int i = beg + 1; i < arr_size + beg; i++)
	{
		temp = arr[i];
		key = arr[i].weight;
		j = i;
		while ((j > beg) && (arr[j - 1].weight > key))
		{
			arr[j] = arr[j - 1];
			--j;
		}
		arr[j] = temp;
	}
}

void insertion_sort_for_vertices(edge *arr, int arr_size, int beg = 0)
{
	edge temp;
	char key;
	int j;
	for (int i = beg + 1; i < arr_size + beg; i++)
	{
		temp = arr[i];
		key = arr[i].vertex1 > arr[i].vertex2? arr[i].vertex1:arr[i].vertex2;
		j = i;
		while ((j > beg) && (arr[j - 1].vertex1 > arr[j - 1].vertex2 ? arr[j - 1].vertex1 : arr[j - 1].vertex2 > key))
		{
			arr[j] = arr[j - 1];
			--j;
		}
		arr[j] = temp;
	}
}

void make_union(edge* arr, unsigned number_of_edges, char set1, char set2, int curr_edge) // создаем объединение
{
	char set = set1 < set2 ? set1 : set2;
	//arr[curr_edge].set1 = arr[curr_edge].set2 = set;
		char vertex1 = arr[curr_edge].vertex1;
		char vertex2 = arr[curr_edge].vertex2;
		for (int i = 0; i < number_of_edges; i++)
		{
			for (int j = 0; j < number_of_edges; j++)
			{
				if ((arr[i].vertex1 == vertex1 || arr[i].vertex2 == vertex1) && arr[i].set1 > set)	
					arr[j].set1 = set;
				if ((arr[j].vertex1 == vertex2 || arr[j].vertex2 == vertex2) && arr[i].set2 > set)
					arr[j].set2 = set;
			}
		}
			//if ((arr[j].vertex1 == vertex2 || arr[j].vertex2 == vertex2) && (arr[j].vertex1 == vertex1 || arr[j].vertex2 == vertex1))
				
}

edge* minimum_spanning_tree(edge *arr, int number_of_edges, unsigned &number_of_edges_of_a_tree, unsigned &max_number_of_edges_of_a_tree, int &sum_of_weights)
{
	edge *tree_arr = new edge[max_number_of_edges_of_a_tree];
	for (int i = 0; i < number_of_edges; i++)
	{
		if (arr[i].set1 != arr[i].set2) // нашли вершины из разных множеств
		{
			make_union(arr, number_of_edges, arr[i].set1, arr[i].set2, i);
			tree_arr[number_of_edges_of_a_tree] = arr[i];
			++number_of_edges_of_a_tree;
			sum_of_weights += arr[i].weight;
		}
	}
	return tree_arr;
}

int main()
{
	int number_of_vertices = 0;
	char a = '\0';
	unsigned number_of_edges = 0;
	unsigned max_number_of_edges = 10;
	edge *arr = new edge[max_number_of_edges];
	cout << "Enter adjacent vertices and edge value (double \"Enter\" to finish)\n\n";
	while (true)
	{
		a = _getch();
		if (a < 'A' || a > 'Z')
			break;
		if (number_of_vertices == 50) // если кол-во вершин 50, завершаем ввод
		{
			break;
		}
		if (number_of_edges == max_number_of_edges)
			inc_arr(arr, number_of_edges, max_number_of_edges);
		arr[number_of_edges].vertex1 = arr[number_of_edges].set1 = a;
		cout << arr[number_of_edges].vertex1; // далее проверки на корректность ввода
		while (arr[number_of_edges].vertex2 < 'A' || arr[number_of_edges].vertex2 > 'Z' || arr[number_of_edges].vertex1 == arr[number_of_edges].vertex2 || edge_already_exists(arr, number_of_edges) || vertex_first_appearance(arr, number_of_edges))
		{
			arr[number_of_edges].vertex2 = arr[number_of_edges].set2 = _getch();
		}
		cout << arr[number_of_edges].vertex2;
		while (arr[number_of_edges].weight < 0 || arr[number_of_edges].weight > 1023)
		{
			arr[number_of_edges].weight = getnumber(1023);
		}
		++number_of_edges;
		cout << endl;
	}
	//--number_of_edges;
	cout << endl;
	insertion_sort_for_weight(arr, number_of_edges);
	unsigned number_of_edges_of_a_tree = 0;
	int sum_of_weights = 0;
	edge *tree_arr = minimum_spanning_tree(arr, number_of_edges, number_of_edges_of_a_tree, number_of_edges, sum_of_weights);
	//insertion_sort_for_vertices(tree_arr, number_of_edges_of_a_tree);
	for (int i = 0; i < number_of_edges_of_a_tree; i++)
		cout << tree_arr[i].vertex1 << tree_arr[i].vertex2 << endl;
	cout << sum_of_weights;
	cout << endl;
	return 0;
}