#pragma once

#include <future>
#include <iostream>
#include <deque>

using namespace std;

class SumUp {
public:
	// should be public so that std::packaged_task can invoke it
	long long operator()(int begin, int end) {
		cout << "SumUp(" << begin << ", " << end << ") ..." << endl;

		long long sum{ 0 };

		for (int i{ begin }; i < end; i++) {
			sum += i;
		}

		return sum;
	}
};

class PackagedTask {
public:
	static void main() {
		SumUp s1, s2, s3, s4;

		// wrap the tasks
		packaged_task<long long(int, int)> task1(s1);
		packaged_task<long long(int, int)> task2(s2);
		packaged_task<long long(int, int)> task3(s3);
		packaged_task<long long(int, int)> task4(s4);

		// create the futures
		future<long long> res1 = task1.get_future();
		future<long long> res2 = task2.get_future();
		future<long long> res3 = task3.get_future();
		future<long long> res4 = task4.get_future();

		// push tasks to deque (packaged tasks are not copyable)
		deque<packaged_task<long long(int, int)>> tasks;
		tasks.push_back(move(task1));
		tasks.push_back(move(task2));
		tasks.push_back(move(task3));
		tasks.push_back(move(task4));

		int begin{ 1 };
		int inc{ 2500 };
		int end{ begin + inc };

		while (!tasks.empty()) {
			packaged_task<long long(int, int)> cur_task = move(tasks.front());

			tasks.pop_front();

			thread sum_thread(move(cur_task), begin, end);

			begin = end;
			end += inc;

			// join blocks until thread completes
			sum_thread.join();
		}

		long long sum = res1.get() + res2.get() + res3.get() + res4.get();

		cout << "Sum of 0 .. 10000 = " << sum << endl;
	}
};