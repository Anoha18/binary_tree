#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class DataNode
{
public:
	DataNode();
	DataNode(string, string);
	~DataNode();
	string GetEnglishWord();
	string GetRussianWord();
	friend  istream& operator >> (istream&, DataNode&);
private:
	string russian_word;
	string english_word;
	
};

DataNode::DataNode(){}

DataNode::DataNode(string _russian_word, string _english_word)
{
	russian_word = _russian_word;
	english_word = _english_word;
}

istream& operator >> (istream& in, DataNode& obj)
{
	cout << "Введите английское слово: ";
	getline(in, obj.english_word);
	cout << "Введите перевод: ";
	getline(in, obj.russian_word);
	return in;
}

string DataNode::GetEnglishWord()
{
	return english_word;
}

string DataNode::GetRussianWord()
{
	return russian_word;
}

DataNode::~DataNode() {}

class NodeTree
{
public:
	NodeTree(DataNode*);
	~NodeTree();
	NodeTree* GetLeft() const;
	NodeTree* GetRight() const;
	void SetRight(NodeTree*);
	void SetLeft(NodeTree*);
	friend class BinaryTree;
private:
	NodeTree* left;
	NodeTree* right;
	DataNode* node_data;
};

NodeTree::NodeTree(DataNode* _nData)
{
	node_data = _nData;
	left = NULL;
	right = NULL;
}

NodeTree::~NodeTree() 
{
	node_data = NULL;
	left = NULL;
	right = NULL;
}

void NodeTree::SetLeft(NodeTree* _left)
{
	left = _left;
}

void NodeTree::SetRight(NodeTree* _right)
{
	right = _right;
}

NodeTree* NodeTree::GetLeft() const
{
	return left;
}

NodeTree* NodeTree::GetRight() const
{
	return right;
}

class BinaryTree
{
public:
	BinaryTree();
	~BinaryTree();
	bool AddNode(NodeTree*, DataNode*);
	void PrintTree(NodeTree*) const;
	void Print();
	void AddNode(DataNode*);
	void SearchNode(string);
	void SearchNode(NodeTree*, string);
	void DeleteNode(string);
	void AddNodeToFile(NodeTree*, ofstream&) const;
	void AddNodeToFile();
	void GetNodeFromFile();
	int GetCountWord();
	NodeTree* DeleteNode(NodeTree*, string);
	NodeTree* FindMinNode(NodeTree*);
private:
	NodeTree* root_tree;
	int count_word;
};

BinaryTree::~BinaryTree() {}

BinaryTree::BinaryTree()
{
	root_tree = NULL;
	count_word = 0;
}

BinaryTree binary_tree;

int BinaryTree::GetCountWord()
{
	return count_word;
}

bool BinaryTree::AddNode(NodeTree* node, DataNode* d_value)
{
	if (root_tree == NULL)
	{
		NodeTree* newNode = new NodeTree(d_value);
		root_tree = newNode;
		count_word++;
		return true;
	}
	
	if (d_value->GetRussianWord().compare(node->node_data->GetRussianWord()) <= 0)
	{
		if (node->GetRight() != NULL)
		{
			return AddNode(node->GetRight(), d_value);
		}
		else
		{
			NodeTree *newNode = new NodeTree(d_value);
			node->SetRight(newNode);
			count_word++;
			return true;
		}
	}
	else
	{
		if (node->GetLeft() != NULL)
		{
			return AddNode(node->GetLeft(), d_value);
		}

		else
		{
			NodeTree* newNode = new NodeTree(d_value);
			node->SetLeft(newNode);
			count_word++;
			return true;
		}
	}
}

void BinaryTree::AddNode(DataNode* d_value)
{
	AddNode(root_tree, d_value);
}

void BinaryTree::Print()
{
	cout << "-------------------- База слов --------------------" << endl;
	if (count_word == 0)
	{
		cout << "Слов нет." << endl;
	}
	else
	{
		cout << "Английское слово   -   Перевод на русский" << endl;
		PrintTree(root_tree);
	}
	cout << "---------------------------------------------------" << endl;
}

void BinaryTree::PrintTree(NodeTree* node) const
{
	while (node != NULL)
	{
		PrintTree(node->GetRight());
		cout << node->node_data->GetEnglishWord() << " - " << node->node_data->GetRussianWord() << endl;
		node = node->GetLeft();
	}
}

void BinaryTree::SearchNode(string search_value)
{
	if (count_word == 0)
	{
		cout << "В базе нет слов" << endl;
	}
	else
	{
		SearchNode(root_tree, search_value);
	}
}

void BinaryTree::SearchNode(NodeTree* node, string search_value)
{
	if (node->GetLeft() == NULL && node->GetRight() == NULL && search_value.compare(node->node_data->GetRussianWord()) != 0)
	{
		cout << "Ошибка!\nСлово не найдено или введенные данные некорректны." << endl;
	}
	else if (search_value.compare(node->node_data->GetRussianWord()) == 0)
	{
		cout << "Перевод слова < " << search_value << " > на английское : " << node->node_data->GetEnglishWord() << endl;
	}
	else
	{
		if (search_value.compare(node->node_data->GetRussianWord()) <= 0)
		{
			SearchNode(node->GetRight(), search_value);
		}
		else
		{
			SearchNode(node->GetLeft(), search_value);
		}
	}
}

NodeTree* BinaryTree::FindMinNode(NodeTree* node)
{
	while (node->GetLeft() != NULL)
	{
		node = node->GetLeft();
	}

	return node;
}

void BinaryTree::DeleteNode(string delete_value)
{
	if (count_word == 0)
	{
		cout << "В базе нет слов" << endl;
	}
	else
	{
		root_tree = DeleteNode(root_tree, delete_value);
	}
}

NodeTree* BinaryTree::DeleteNode(NodeTree* node, string delete_value)
{
	if (root_tree == NULL)
	{
		return node;
	}
	else if (node->GetLeft() == NULL && node->GetRight() == NULL && delete_value.compare(node->node_data->GetRussianWord()) != 0)
	{
		cout << "Ошибка!\nСлово не найдено или введенные данные некорректны." << endl;
	}
	else if (delete_value.compare(node->node_data->GetRussianWord()) < 0)
	{
		node->SetRight(DeleteNode(node->GetRight(), delete_value));
	}
	else if (delete_value.compare(node->node_data->GetRussianWord()) > 0)
	{
		node->SetLeft(DeleteNode(node->GetLeft(), delete_value));
	}
	else
	{
		if (node->GetLeft() == NULL && node->GetRight() == NULL)
		{
			node = NULL;
			count_word--;
		}
		else if (node->GetLeft() == NULL)
		{
			node = node->GetRight();
			count_word--;
		}
		else if (node->GetRight() == NULL)
		{
			node = node->GetLeft();
			count_word--;
		}
		else
		{
			NodeTree *temp_min_node = FindMinNode(node->GetRight());
			NodeTree *temp = new NodeTree(node->node_data);
			node->node_data = temp_min_node->node_data;
			temp_min_node->node_data = temp->node_data;
			node->SetRight(DeleteNode(node->GetRight(), delete_value));
		}
	}
	return node;
}

void BinaryTree::GetNodeFromFile()
{
	ifstream data_file_in("data_base.txt");
	string str, str_english, str_russian;
	int count = 0;
	while (getline(data_file_in, str, '.'))
	{
		count++;
		if (str[0] == '\n')
		{
			str.erase(0, 1);
		}

		if (count == 1)
		{
			str_english = str;
		}

		if (count == 2)
		{
			str_russian = str;
			DataNode *temp = new DataNode(str_russian, str_english);
			binary_tree.AddNode(temp);
			count = 0;
		}
	}
	data_file_in.close();
}

void BinaryTree::AddNodeToFile(NodeTree* node, ofstream& fout) const
{
	while (node != NULL)
	{
		AddNodeToFile(node->GetRight(), fout);
		fout << node->node_data->GetEnglishWord() << "." << node->node_data->GetRussianWord() << ".\n";
		node = node->GetLeft();
	}
}

void BinaryTree::AddNodeToFile()
{
	ofstream data_file_out("data_base.txt");
	AddNodeToFile(root_tree, data_file_out);
	count_word = 0;
	data_file_out.close();
}

void Cl()
{
	system("pause");
	system("cls");
}

void Menu()
{
	cout << "-------------------- Меню --------------------" << endl
		<< "Количество слов в базе: " << binary_tree.GetCountWord() << endl
		<< "[1] - Добавить слово в словарь" << endl
		<< "[2] - Найти слово по переводу" << endl
		<< "[3] - Вывести все слова" << endl
		<< "[4] - Удалить слово по переводу" << endl
		<< "[0] - Закрыть программу" << endl
		<< "----------------------------------------------" << endl
		<< ">> ";
}

void OperationMenu()
{
	int point;

	do
	{
		Menu();
		cin >> point;
		switch (point)
		{
			case 1:
			{
				DataNode *temp = new DataNode;
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				cin >> *(temp);
				binary_tree.AddNode(temp);
				Cl();
				break;
			}
			case 2:
			{
				cout << "Введите перевод: ";
				string temp;
				cin >> temp;
				binary_tree.SearchNode(temp);
				Cl();
				break;
			}
			case 3:
			{
				binary_tree.Print();
				Cl();
				break;
			}
			case 4:
			{
				cout << "Введите перевод: ";
				string temp;
				cin >> temp;
				binary_tree.DeleteNode(temp);
				Cl();
				break;
			}
		}
	} while (point != 0);
}

int main()
{
	setlocale(LC_ALL, "rus");

	binary_tree.GetNodeFromFile();
	OperationMenu();
	binary_tree.AddNodeToFile();

	system("pause");
	return 0;
}