
template<typename T>
class B
{
public:
	void func2();

};

class A
{
public:
	A();
	~A();
	B<int> *b;
	void func1();
};

A::A()
{
	b = new B<int>;
}

A::~A()
{
	delete b;
}

void A::func1()
{
	b->func2();
}


int main()
{
	A* a = new A;
	a->func1();
	delete a;

	return 0;
}

template<typename T>
void B<T>::func2()
{
}
