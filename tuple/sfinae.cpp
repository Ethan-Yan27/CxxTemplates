#include <iostream>
#include <string>

template <class T, typename = void>
struct sfinae{
    static void display(T a)
    {
        std::cout << "void:" << a << std::endl;
    }
};

template <class T>
struct sfinae < T,
        typename std::enable_if_t<std::is_same_v<T, int>>>
{
    static void display(T a)
    {
        std::cout << "int:" << a << std::endl;
    }
};

template <class T>
struct sfinae < T,
        typename std::enable_if_t<std::is_same_v<T, float>>>
{
    static void display(T a)
    {
        std::cout << "float:" << a << std::endl;
    }
};

int main()
{
    sfinae<int>::display(1);
    sfinae<float>::display(1);
    sfinae<std::string>::display("1");
}