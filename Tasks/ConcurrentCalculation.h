#pragma once

#include <future>
#include <iostream>
#include <random>
#include <vector>
#include <numeric>

using std::cout;
using std::vector;
using std::future;
using std::async;
using std::inner_product;

class ConcurrentCalculation {
private:
	long long getDotProduct(vector<int>& v, vector<int>& w) {
		auto vSize = v.size();

		vector<int>::iterator vbeg = v.begin();
		vector<int>::iterator vend = v.begin() + vSize / 4;

		vector<int>::iterator wbeg = w.begin();

		future<long long> future1 = async([&] {
			return inner_product(vbeg, vend, wbeg, 0LL);
		});

		vbeg = v.begin() + vSize / 4;
		vend = v.begin() + vSize / 2;
		
		wbeg = w.begin() + vSize / 4;

		future<long long> future2 = async([&] {
			return inner_product(vbeg, vend, wbeg, 0LL);
		});

		vbeg = v.begin() + vSize / 2;
		vend = v.begin() + vSize * 3 / 4;

		wbeg = w.begin() + vSize / 2;

		future<long long> future3 = async([&] {
			return inner_product(vbeg, vend, wbeg, 0LL);
		});

		vbeg = v.begin() + vSize * 3 / 4;
		vend = v.end();

		wbeg = w.begin() + vSize * 3 / 4;

		future<long long> future4 = async([&] {
			return inner_product(vbeg, vend, wbeg, 0LL);
		});

		return future1.get() + future2.get() + future3.get() + future4.get();
	}

public:
	static void main() {
		int num{ 10000000 };

		std::random_device seed;

		// generator
		std::mt19937 engine(seed());

		// distribution
		std::uniform_int_distribution<int> dist(0, 100);

		vector<int> v, w;
		
		v.reserve(num);
		w.reserve(num);

		for (int i{ 0 }; i < num; i++) {
			v.push_back(dist(engine));
			w.push_back(dist(engine));
		}

		cout << "ConcurrentCalculation: DotProduct(v, w): " << ConcurrentCalculation{}.getDotProduct(v, w) << std::endl;
		cout << "Main: DotProduct(v, w): " << inner_product(v.begin(), v.end(), w.begin(), 0LL) << std::endl;
	}
};