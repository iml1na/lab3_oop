#include <iostream>
#include <ctime>
using namespace std;

class Base //класс-предок
{
public:
	Base()
	{
		printf("Base()\n");
	}
	virtual void showName()
	{
		printf("Base\n");
	}
	virtual ~Base()
	{
		printf("~Base()\n");
	}
};

class Class1 : public Base //класс-потомок (1)
{
private:
	int x;
	int y;
public:
	Class1() //конструктор по умолчанию
	{
		printf("Class1()\n");
		x = 0;
		y = 0;
	}

	Class1(int x, int y) //конструктор с параметрами
	{
		printf("Class1(int x, int y)\n");
		this->x = x;
		this->y = y;
	}

	Class1(const Class1& obj) //копирующий конструктор
	{
		printf("Class1(const Class1& obj)\n");
		x = obj.x;
		y = obj.y;
	}

	~Class1()
	{
		printf("~Class1()\n");
	}

	void showName()
	{
		printf("Class1\n");
	}
};

class Class2 : public Base { //класс-потомок (2)
private:
	int x;
	int y;
	int color;
public:
	Class2() //конструктор по умолчанию
	{
		printf("Class2()\n");
		x = 0;
		y = 0;
		color = 0;
	}

	Class2(int x, int y, int color) //конструктор с параметрами
	{
		printf("Class2(int x, int y, int color)\n");
		this->x = x;
		this->y = y;
		this->color = color;
	}

	Class2(const Class2& obj) //копирующий коструктор
	{
		printf("Class2(const Class2& obj)\n");
		x = obj.x;
		y = obj.y;
		color = obj.color;
	}
	~Class2()
	{
		printf("~Class2()\n");
	}

	void showName()
	{
		printf("Class2\n");
	}
};

class MyStorage //класс хранилища
{
private:
	int size;
	Base** storage;
public:
	MyStorage(int size) //конструктор с параметром
	{
		printf("MyStorage(int size)\n");
		this->size = size;
		storage = new Base * [size];
		for (int i = 0; i < size; i++)
			storage[i] = nullptr;
	}
	MyStorage(MyStorage& storage_copy) //копирующий конструктор
	{
		printf("MyStorage(MyStorage& storage_copy)\n");
		size = storage_copy.size;
		storage = new Base * [size];
		for (int i = 0; i < size; i++)
			*storage[i] = storage_copy.GetObject(i);
	}
	~MyStorage()
	{
		printf("~MyStorage()\n");
		for (int i = 0; i < size; i++)
			delete storage[i];
		delete storage;
	}

	int StorageSize()
	{
		return size;
	}

	void GetCount() //вычисление размера хранилища и количества элементов
	{
		int count = 0;
		for (int i = 0; i < size; i++)
			if (storage[i] != nullptr)
				count = count + 1;
		printf("\nКоличество элементов: %d\nРазмер хранилища: %d\n\n", count, size);
	}

	void SetObject(int i, Base* object) //помещение объекта в хранилище
	{
		printf("SetObject(int i, Base* object), i = %d\n", i);
		storage[i] = object;
	}

	void AddObject(Base* object) //добавление элемента
	{
		size = size + 1;
		printf("AddObject(Base* object), size = %d\n", size);
		Base** storage_copy = new Base * [size]; 
		for (int i = 0; i < size - 1; i++)
			storage_copy[i] = storage[i];
		storage_copy[size - 1] = object;
		for (int j = 0; j < size - 1; j++)
			storage[j] = nullptr;
		delete storage;
		storage = storage_copy;
	}

	Base& GetObject(int i) //вызов объекта по индексу
	{
		return *storage[i];
	}

	void DeleteObject(int i) //удаление элемента
	{
		printf("DeleteObject(int i), i = %d\n", i);
		storage[i] = nullptr;
	}

	void ShowMethod(int i) //вызов метода showName
	{
		printf("ShowMethod(int i), i = %d\n", i);
		if (storage[i] != nullptr)
			storage[i]->showName();
	}

	void ReplaceObject(int i, Base* object) //замена элемента
	{
		printf("ReplaceObject(int i), i = %d\n", i);
		if (storage[i] == nullptr)
			storage[i] = object;
		else
		{
			delete storage[i];
			storage[i] = object;
		}
	}
	void ShowStorage() //вывод хранилища
	{
		for (int i = 0; i < size; i++)
			if (storage[i] != nullptr)
				storage[i]->showName();
			else printf("nullptr\n");
	}
};

int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "RUS");
	MyStorage storage(10);

	for (int i = 0; i < storage.StorageSize(); i++)
	{
		int k = rand() % 2;
		if (k)
			storage.SetObject(i, new Class1);
		else storage.SetObject(i, new Class2);
	}
	cout << endl << endl;
	for (int i = 0; i < storage.StorageSize(); i++)
		storage.GetObject(i).showName();
	unsigned int StartTime = clock();
	printf("\n\n");
	for (int i = 0; i < 100; i++)
	{
		printf("%d:\n", i);
		int action = rand() % 4;
		int randindx = rand() % storage.StorageSize();
		int c = rand() % 2;
		switch (action)
		{
		case 0:
			if (c)
			{
				storage.AddObject(new Class1);
				printf("AddObject: new Class1\n");
			}
			else
			{
				storage.AddObject(new Class2);
				printf("AddObject: new Class2\n");
			}
			break;
		case 1:
			storage.DeleteObject(randindx);
			break;
		case 2:
			storage.ShowMethod(randindx);
			break;
		case 3:
			if (c)
			{
				storage.ReplaceObject(randindx, new Class1);
				printf("ReplaceObject: new Class1\n");
			}
			else
			{
				storage.ReplaceObject(randindx, new Class2);
				printf("ReplaceObject: new Class2\n");
			}
			break;
		default:
			storage.GetCount();
			break;
		}
		cout << endl;
	}
	unsigned int EndTime = clock();
	printf("\nВремя работы = %d\n", EndTime - StartTime);
	storage.ShowStorage();
	storage.GetCount();
}
