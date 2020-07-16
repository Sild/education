#include <thread>
#include <iostream>

std::mutex resource_one;
std::mutex resource_two;
std::condition_variable cond;


void executor1() {
    std::cout << "executor1 started\n";

    std::lock_guard<std::mutex> lg1(resource_one);
    std::unique_lock<std::mutex> cond_lock(resource_two);
    cond.wait(cond_lock);
    std::cout << "executor1 done\n";
}

void executor2() {
    std::cout << "executor2 started\n";
    
    std::lock_guard<std::mutex> lg2(resource_two);
    cond.notify_all();
    std::lock_guard<std::mutex> lg1(resource_one);
    std::cout << "executor2 done\n";
}

#include <string>

std::string foo()
{
	return "";
}

int main() {
//    std::thread th1(executor1);
//    std::thread th2(executor2);

//    th1.join();
//    th2.join();
    std::string&& test = foo();

	if (test.empty())
		return 0;

	return 1;
}
