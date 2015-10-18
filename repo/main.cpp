#include <algorithm>
#include <iostream>
#include <fstream>
#include <list>
#include <iomanip>  
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/core/ref.hpp>
#include <boost/timer/timer.hpp>
#include <boost/chrono.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "Windows.h"
using namespace std;

struct THR {
	boost::random::mt19937 gen;
	int id;
	LARGE_INTEGER StartingTime, EndingTime, ElapsedNanoseconds;
	boost::chrono::nanoseconds  elapse;
	boost::timer::cpu_timer cpu;
};

class EvenOddFunctor {
	int even_;
	int odd_;
public:
	EvenOddFunctor() : even_(0), odd_(0) {

	}
	void operator()(int x) {
		if (x % 2 == 0) even_ += x;
		else odd_ += x;
	}
	int even_sum() const { return even_; }
	int odd_sum() const { return odd_; }
};

int roll_die(boost::random::mt19937 &gen) {
	boost::random::uniform_int_distribution<> dist(1, 999);
	return dist(gen);
}

void thread_func(THR &str_thr, LARGE_INTEGER &Frequency) {
	int var = 0, i = 0;
	static int varvar = 0;
	str_thr.ElapsedNanoseconds.QuadPart = 0;
	vector<vector<LONGLONG>> str_vctr;
	//str_thr.elapse.zero();

	//str_thr.cpu.is_stopped() ? str_thr.cpu.resume() : str_thr.cpu.start();
	QueryPerformanceCounter(&str_thr.StartingTime);
	str_thr.gen.seed(static_cast<uint32_t>(str_thr.StartingTime.QuadPart));
	while (var != 444 && varvar != 444) {
		QueryPerformanceCounter(&str_thr.StartingTime);
		//str_thr.cpu.is_stopped() ? str_thr.cpu.resume() : str_thr.cpu.start();
		//str_thr.gen.seed(str_thr.StartingTime.QuadPart);
		var = roll_die(str_thr.gen);
		QueryPerformanceCounter(&str_thr.EndingTime);
		str_vctr.resize(i + 1);
		str_vctr[i].push_back(var);
		str_vctr[i].push_back(str_thr.StartingTime.QuadPart);
		str_vctr[i].push_back(str_thr.EndingTime.QuadPart);
		++i;
		//str_thr.cpu.stop();
		//str_thr.ElapsedNanoseconds.QuadPart += str_thr.EndingTime.QuadPart - str_thr.StartingTime.QuadPart;
		//str_thr.elapse += str_thr.cpu.elapsed().system + str_thr.cpu.elapsed().user;
	}
	if (varvar != 444) { varvar = 444; }
	fstream file;
	file.open("C:\\thr\\thread_" + to_string(str_thr.id) + ".txt", fstream::out | fstream::trunc);
	for (size_t i = 0; i < str_vctr.size(); ++i) {
		file << setw(4) << to_string(str_vctr[i][0]) << "    St.time " << to_string(str_vctr[i][1]) << "    En.time " << to_string(str_vctr[i][2]) << "    El.time " << to_string((str_vctr[i][2] - str_vctr[i][1])) << endl;
	}
	//str_thr.cpu.stop();
	//QueryPerformanceCounter(&str_thr.EndingTime);
	//str_thr.elapse = boost::chrono::nanoseconds(str_thr.cpu.elapsed().wall);
	//str_thr.ElapsedNanoseconds.QuadPart += str_thr.EndingTime.QuadPart - str_thr.StartingTime.QuadPart;
	//str_thr.ElapsedNanoseconds.QuadPart *= 1000000000 / Frequency.QuadPart;
	//file << "Elapsed time: " << str_thr.ElapsedNanoseconds.QuadPart << endl;
	//file << "Elapsed time: " << str_thr.elapse.count() << endl;
	file.close();
}

class A  {
public:
	static int i;
	A() {
		++i;
	};
	~A() {
		--i;
	};
	void f(int a) { cout << "AAAAAA" << a << endl; }
};

int A::i = 0;

class B : public A {
public:
	B() {};
	~B() {};
	void f(int b) { cout << "BBBBBB" << b << endl; }
};

class printer
{
public:
	printer(boost::asio::io_service& io)
		: strand_(io),
		timer1_(io, boost::posix_time::seconds(1)),
		timer2_(io, boost::posix_time::seconds(1)),
		count_(0)
	{
		timer1_.async_wait(strand_.wrap(boost::bind(&printer::print1, this)));
		timer2_.async_wait(strand_.wrap(boost::bind(&printer::print2, this)));
	}

	~printer()
	{
		std::cout << "Final count is " << count_ << std::endl;
	}

	void print1()
	{
		if (count_ < 10)
		{
			std::cout << "Timer 1: " << count_ << std::endl;
			++count_;

			timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(1));
			timer1_.async_wait(strand_.wrap(boost::bind(&printer::print1, this)));
		}
	}

	void print2()
	{
		if (count_ < 10)
		{
			std::cout << "Timer 2: " << count_ << std::endl;
			++count_;

			timer2_.expires_at(timer2_.expires_at() + boost::posix_time::seconds(1));
			timer2_.async_wait(strand_.wrap(boost::bind(&printer::print2, this)));
		}
	}

private:
	boost::asio::io_service::strand strand_;
	boost::asio::deadline_timer timer1_;
	boost::asio::deadline_timer timer2_;
	int count_;
};

int main() {
	EvenOddFunctor evenodd;
	LARGE_INTEGER Frequency;
	LARGE_INTEGER StartingTime, EndingTime, ElapsedNanoseconds;
	int my_list[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	void thread_func(THR &, LARGE_INTEGER &);
	int roll_die(boost::random::mt19937 &gen);

	int a = 1, b = 2;
	A *A1 = new A();
	A *A2;
	A A3;
	A1->~A();
	A3.~A();
	A *B1 = new B();
	A1->f(a);
	B1->f(b);

	boost::asio::io_service io;
	printer p(io);
	//boost::thread t(boost::bind(&boost::asio::io_service::run, &io));
	//io.run();
	//t.join();

	//return 0;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);
	boost::thread ** thrd_arr = new boost::thread * [4];
	boost::thread_group * rand_threads = new boost::thread_group;
	THR * str_thr_arr = new THR[4];
	for (int i = 0; i < 4; ++i) {
		str_thr_arr[i].id = i + 1;
		thrd_arr[i]=new boost::thread(boost::bind(&thread_func, boost::ref(str_thr_arr[i]), boost::ref(Frequency)));
		rand_threads->add_thread(thrd_arr[i]);
	}
	rand_threads->join_all();
	rand_threads->~thread_group();
	delete[] thrd_arr;
	QueryPerformanceCounter(&EndingTime);
	ElapsedNanoseconds.QuadPart = (EndingTime.QuadPart - StartingTime.QuadPart) * 1000000000 / Frequency.QuadPart;
	EvenOddFunctor() = for_each(my_list,
		my_list + sizeof(my_list) / sizeof(my_list[0]),
		EvenOddFunctor());

	std::cout << "Чумма ч™тных: " << evenodd.even_sum() << "\n";
	std::cout << "Чумма неч™тных: " << evenodd.odd_sum() << std::endl;

	// вывод:
	// Чумма ч™тных: 30
	// Чумма неч™тных: 25
}