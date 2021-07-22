#pragma once

#include <future>
#include <iostream>
#include <string>

using namespace std;

class PromiseFuture {
private:
	static void product(promise<int>&& int_promise, int a, int b) {
		int_promise.set_value(a * b);
	}

	class Division {
	public:
		// should be public to be invokable
		void operator()(promise<int>&& int_promise, int a, int b) {
			try {
				if (b == 0) {
					string er{ "Illegal division by zero: " + to_string(a) + " / " + to_string(b) };
					throw runtime_error(er);
				}
				int_promise.set_value(a / b);
			}
			catch (...) {
				int_promise.set_exception(current_exception());
			}
		}
	};

public:
	static void main() {
		int a{ 20 }, b{ 0 };

		// declare promises
		promise<int> prod_promise;
		promise<int> div_promise;

		// get futures
		future<int> prod_res = prod_promise.get_future();
		future<int> div_res = div_promise.get_future();

		// calculate results in separate threads
		thread prod_thread(product, move(prod_promise), a, b);
		thread div_thread(Division{}, move(div_promise), a, b);

		// get results
		try {
			cout << a << " * " << b << " = " << prod_res.get() << endl;
			cout << a << " / " << b << " = " << div_res.get() << endl;
		}
		catch (runtime_error& e) {
			cout << e.what() << endl;
		}

		// join blocks until thread completes
		prod_thread.join();
		div_thread.join();
	}
};