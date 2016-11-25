#pragma once
#include <memory.h>
#include <unordered_map>

namespace algorithm_lib
{
	const int m1e9n7 = 1000000007;

	template<typename T = int64_t, typename U = int64_t>
	inline T mod(T v, U m = m1e9n7)
	{
		if (v >= 0)
			return v % m;
		else
			return (v % m + m) % m;
	}

	template<size_t A, size_t B, size_t C>
	inline void matrix_multiply(int64_t a[A][B], int64_t b[B][C], int64_t c[A][C], int64_t m = m1e9n7)
	{
		for(size_t k0 = 0; k0 < A; ++k0)
			for (size_t k2 = 0; k2 < C; ++k2)
			{
				int64_t sum = 0;
				for (size_t k1 = 0; k1 < B; ++k1)
					sum = mod(sum + a[k0][k1] * b[k1][k2], m);
				c[k0][k2] = sum;
			}
	}

	template<size_t A, size_t B, size_t C>
	inline void matrix_multiply_inplace(int64_t a[A][B], int64_t b[B][C], int64_t c[A][C], int64_t m = m1e9n7)
	{
		int64_t c0[A][C];
		matrix_multiply<A, B, C>(a, b, c0, m);
		memcpy(c, c0, sizeof(c0));
	}

	template<size_t N, int64_t M = m1e9n7>
	inline void matrix_power_mod(int64_t m[N][N], int64_t p)
	{
		if (0 == p)
		{
			for (size_t r = 0; r < N; ++r)
				for (size_t c = 0; c < N; ++c)
					m[r][c] = r == c;
			return;
		}
		if (1 == p)
			return;
		if (p & 1)
		{
			int64_t m1[N][N];
			memcpy(m1, m, sizeof(m1));
			matrix_power_mod<N>(m, p / 2);
			matrix_multiply_inplace<N, N, N>(m, m, m);
			matrix_multiply_inplace<N, N, N>(m, m1, m);
		}
		else
		{
			matrix_power_mod<N>(m, p / 2);
			matrix_multiply_inplace<N, N, N>(m, m, m);
		}
	}

	template<typename T>
	inline T modular_multiplicative_inverse(const T& a, const T& m)
	{
		int64_t mat[2][2] = { 1, 0, 0, 1 };
		T b = m;
		T va = a;
		do {
			auto r = va % b;
			auto q = va / b;
			va = b;
			b = r;
			int64_t mat0[2][2] = { 0, 1, 1, -q };
			matrix_multiply_inplace<2, 2, 2>(mat0, mat, mat, m);
			if (1 == r)
				break;
		} while (true);
		return (T)mod(mat[1][0], m);
	}

	template<typename T>
	inline T power_mod(const T& a, const T& p, const T& m = m1e9n7)
	{
		if (0 > p)
			return modular_multiplicative_inverse(power_mod(a, -p, m), m);

		if (1 == p)
			return a % m;
		if (0 == p)
			return 1;

		T r = power_mod(a, p / 2, m);
		r = r * r % m;
		if (p & 1)
			return r * a % m;
		else
			return r;
	}

	template<typename T>
	inline T dlog(const T& a, const T& n, const T& e)
	{
		T sqrt_n = (T)sqrt(n);
		std::vector<T> s0((size_t)sqrt_n);
		std::unordered_map<T, size_t> vmap;
		s0[0] = 1;
		vmap[1] = 0;
		for (size_t k = 1; k < (size_t)sqrt_n; ++k)
		{
			s0[k] = s0[k - 1] * a % n;
			vmap[s0[k]] = k;
		}

		auto ak = power_mod(a, -sqrt_n, n);
		auto r = e;
		for (T k = sqrt_n; k < n; k += sqrt_n)
		{
			r = mod(r * ak, n);
			auto it = vmap.find(r);
			if (vmap.end() != it)
				return mod(it->second + k, n - 1);
		}

		return -1;
	}
}
