#include <iostream>
#include <thread>

void test()
{
    std::cout << "Hello from test, id = " << std::this_thread::get_id() << "\n";
}

void functionA()
{
    std::cout << "Hello from functionA, id = " << std::this_thread::get_id() << "\n";
    std::thread threadC(test);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    threadC.join();
}

void functionB()
{
    std::cout << "Hello from functionB, id = " << std::this_thread::get_id() << "\n";
}

int main()
{
    // The order should be main -> threadA -> threadB -> threadC
    
    std::cout << "Hello from main, id = " << std::this_thread::get_id() << "\n";
    std::thread threadA(functionA);
    std::thread threadB(functionB);
    
    threadA.join();
    threadB.join();

    return 0;
}