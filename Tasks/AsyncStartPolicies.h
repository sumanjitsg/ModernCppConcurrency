#pragma once

#include <chrono>
#include <future>
#include <iostream>

using namespace std;

class AsyncStartPolicies {
public:
	static void main() {
		chrono::system_clock::time_point begin = chrono::system_clock::now();

		future<chrono::system_clock::time_point> asyncLazy = async(launch::deferred, [] {
			return chrono::system_clock::now();
		});

		future<chrono::system_clock::time_point> asyncEager = async(launch::async, [] {
			return chrono::system_clock::now();
		});

		this_thread::sleep_for(chrono::seconds(1));

		chrono::system_clock::duration lazyStart = asyncLazy.get() - begin;
		chrono::system_clock::duration eagerStart = asyncEager.get() - begin;

		double lazyDuration = chrono::duration<double>(lazyStart).count();
		double eagerDuration = chrono::duration<double>(eagerStart).count();

		cout << "asyncLazy evaluated after: " << lazyDuration << " seconds." << endl;
		cout << "asyncEager evaluated after: " << eagerDuration << " seconds." << endl;
	}
};