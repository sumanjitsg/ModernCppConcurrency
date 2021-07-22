#pragma once

#include <future>
#include <iostream>
#include <chrono>

using namespace std;

class FireAndForget {
public:
	static void main() {
		async(launch::async, [] {
			this_thread::sleep_for(chrono::seconds(2));
			cout << "first thread" << endl;
		});

		async(launch::async, [] {
			this_thread::sleep_for(chrono::seconds(1));
			cout << "second thread" << endl;
		});

		cout << "main thread" << endl;
	}
};