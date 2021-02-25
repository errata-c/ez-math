#pragma once
#include <cassert>
#include <cinttypes>
#include <limits>
#include <string>
#include <string_view>
#include <type_traits>
#include <xxh3.h>

namespace ez::prng {

	inline std::uint8_t rotl8(std::uint8_t val, std::uint32_t amount) {
		assert(amount < 8);
		return static_cast<std::uint8_t>(val << amount) | (val >> (((~amount)+1) & 7u));
	}
	inline std::uint16_t rotl16(std::uint16_t val, std::uint32_t amount) {
		assert(amount < 16);
		return static_cast<std::uint16_t>(val << amount) | (val >> (((~amount) + 1) & 15u));
	}
	inline std::uint32_t rotl32(std::uint32_t val, std::uint32_t amount) {
		assert(amount < 32);
		return (val << amount) | (val >> (((~amount) + 1) & 31u));
	}
	inline std::uint64_t rotl64(std::uint64_t val, std::uint32_t amount) {
		assert(amount < 64);
		return (val << amount) | (val >> (((~amount) + 1) & 63u));
	}

	inline std::uint8_t rotr8(std::uint8_t val, std::uint32_t amount) {
		assert(amount < 8);
		return static_cast<std::uint8_t>(val >> amount) | (val << (((~amount) + 1) & 7u));
	}
	inline std::uint16_t rotr16(std::uint16_t val, std::uint32_t amount) {
		assert(amount < 16);
		return static_cast<std::uint16_t>(val >> amount) | (val << (((~amount) + 1) & 15u));
	}
	inline std::uint32_t rotr32(std::uint32_t val, std::uint32_t amount) {
		assert(amount < 32);
		return (val >> amount) | (val << (((~amount) + 1) & 31u));
	}
	inline std::uint64_t rotr64(std::uint64_t val, std::uint32_t amount) {
		assert(amount < 64);
		return (val >> amount) | (val << (((~amount) + 1) & 63u));
	}

	static uint32_t xxh32(const void* data, std::size_t length) noexcept {
		return XXH32(data, length, 0);
	}
	template<typename T>
	static uint32_t xxh32(std::basic_string_view<T> view) noexcept {
		return xxh32(view.data(), view.length());
	}

	static uint64_t xxh64(const void* data, std::size_t length) noexcept {
		return XXH64(data, length, 0);
	}
	template<typename T>
	static uint64_t xxh64(std::basic_string_view<T> view) noexcept {
		return xxh64(view.data(), view.length());
	}

	static uint64_t xxh3_64(const void* data, std::size_t length) noexcept {
		return XXH3_64bits(data, length);
	}
	template<typename T>
	static uint64_t xxh3_64(std::basic_string_view<T> view) noexcept {
		return xxh3_64(view.data(), view.length());
	}

	namespace intern {
		/*
		djb2
			This algorithm (k=33) was first reported by dan bernstein many years ago in comp.lang.c.
		Another version of this algorithm (now favored by bernstein) uses xor: hash(i) = hash(i - 1) * 33 ^ str[i];
		The magic of number 33 (why it works better than many other constants, prime or not) has never been adequately explained.
		*/
		template<typename T>
		static constexpr std::uint32_t djb2HashValue(T val, std::uint32_t seed = 5381u) {
			using utype = std::make_unsigned_t<T>;
			for (int i = 0; i < sizeof(utype); ++i) {
				// Bitshifting avoids any endianess problems, and optimizes well.
				seed = (seed * 33u) ^ static_cast<std::uint32_t>(val & (static_cast<utype>(0xFF) << (i << 3)));
			}

			return seed;
		}
	};

	
	template<typename T>
	static std::uint32_t djb2_32(const T* str, std::size_t len, std::uint32_t seed = 5381u) {
		static_assert(std::is_integral_v<T>, "The value type passed to ez::djb2Hash32 must be integral!");

		using utype = std::make_unsigned_t<T>;
		const utype* it = reinterpret_cast<const utype*>(str);
		const utype* end = it + len;
		while (it != end) {
			seed = intern::djb2HashValue<utype>(*it, seed);
			++it;
		}

		return seed;
	};
	template<typename T>
	static std::uint32_t djb2_32(const std::basic_string<T>& str, std::uint32_t seed = 5381u) {
		static_assert(std::is_integral_v<T>, "The value type passed to ez::djb2Hash32 must be integral!");

		return djb2_32(str.data(), str.size(), seed);
	};
	template<typename T>
	static std::uint32_t djb2_32_cstr(const T* cstr, std::uint32_t seed = 5381u) {
		static_assert(std::is_integral_v<T>, "The value type passed to ez::djb2Hash32 must be integral!");

		using utype = std::make_unsigned_t<T>;
		const utype* it = reinterpret_cast<const utype*>(cstr);
		while (*it != 0) {
			seed = intern::djb2HashValue<utype>(*it, seed);
			++it;
		}

		return seed;
	}
	

	// Compile time constexpr version
	template<std::size_t N, typename T>
	static constexpr std::uint32_t djb2_32(const T(&str)[N], std::uint32_t seed = 5381u) noexcept {
		static_assert(std::is_integral_v<T>, "The value type passed to ez::djb2Hash32 must be integral!");

		using utype = std::make_unsigned_t<T>;

		for (std::size_t u = 0; (u + 1) < N; ++u) {
			seed = intern::djb2HashValue(static_cast<utype>(str[u]), seed);
		}

		return seed;
	};
	template<typename T>
	static constexpr std::uint32_t djb2_32(const std::basic_string_view<T>& str, std::uint32_t seed = 5381u) noexcept {
		static_assert(std::is_integral_v<T>, "The value type passed to ez::djb2Hash32 must be integral!");

		using utype = std::make_unsigned_t<T>;

		for (std::size_t u = 0; u < str.size(); ++u) {
			seed = intern::djb2HashValue(static_cast<utype>(str[u]), seed);
		}

		return seed;
	};

	template<typename T>
	static std::uint64_t djb2_64(const T* str, std::size_t len, std::uint32_t seed = 5381u) {
		static_assert(std::is_integral_v<T>, "The value type passed to ez::djb2Hash64 must be integral!");

		using utype = std::make_unsigned_t<T>;
		std::uint32_t carry = intern::djb2HashValue(seed);

		const utype* it = reinterpret_cast<const utype*>(str);
		const utype* end = it + len;
		while (it != end) {
			utype val = *it;
			seed = intern::djb2HashValue(val, seed);
			carry = intern::djb2HashValue(seed, carry);
			++it;
		}

		return static_cast<std::uint64_t>(seed) | (static_cast<std::uint64_t>(carry) << 32);
	};
	template<typename T>
	static std::uint64_t djb2_64(const std::basic_string<T>& str, std::uint32_t seed = 5381u) {
		return djb2_64(str.data(), str.size(), seed);
	};
	template<typename T>
	static std::uint64_t djb2_64_cstr(const T* cstr, std::uint32_t seed = 5381u) {
		static_assert(std::is_integral_v<T>, "The value type passed to ez::djb2Hash64 must be integral!");

		using utype = std::make_unsigned_t<T>;
		std::uint32_t carry = intern::djb2HashValue(seed);

		const utype* it = reinterpret_cast<const utype*>(cstr);
		while (*it != 0) {
			utype val = *it;
			seed = intern::djb2HashValue(val, seed);
			carry = intern::djb2HashValue(seed, carry);
			++it;
		}

		return static_cast<std::uint64_t>(seed) | (static_cast<std::uint64_t>(carry) << 32);
	}

	// Compile time constexpr
	template<std::size_t N, typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
	static constexpr std::uint64_t djb2_64(const T(&str)[N], std::uint32_t seed = 5381u) {
		static_assert(std::is_integral_v<T>, "The value type passed to ez::djb2Hash64 must be integral!");

		using utype = std::make_unsigned_t<T>;
		std::uint32_t carry = intern::djb2HashValue(seed);
		for (std::size_t i = 0; (i+1) < N; ++i) {
			seed = intern::djb2HashValue(str[i], seed);
			carry = intern::djb2HashValue(seed, carry);
		}

		return static_cast<std::uint64_t>(seed) | (static_cast<std::uint64_t>(carry) << 32);
	};
	template<typename T>
	static constexpr std::uint64_t djb2_64(const std::basic_string_view<T>& str, std::uint32_t seed = 5381u) noexcept {
		static_assert(std::is_integral_v<T>, "The value type passed to ez::djb2Hash64 must be integral!");

		using utype = std::make_unsigned_t<T>;
		std::uint32_t carry = intern::djb2HashValue(seed);
		for (std::size_t i = 0; i < str.size(); ++i) {
			seed = intern::djb2HashValue(str[i], seed);
			carry = intern::djb2HashValue(seed, carry);
		}

		return static_cast<std::uint64_t>(seed) | (static_cast<std::uint64_t>(carry) << 32);
	};

	struct Murmur32 {
		Murmur32()
			: state(0)
		{}
		Murmur32(std::uint32_t s)
			: state(s)
		{}

		~Murmur32() = default;
		Murmur32(const Murmur32&) = default;
		Murmur32& operator=(const Murmur32&) = default;

		std::uint32_t operator()(const std::string& str) {
			assert(str.size() <= std::numeric_limits<int>::max());
			return advance(str.data(), static_cast<int>(str.size()));
		}
		std::uint32_t operator()(const void* key, int len) {
			return advance(key, len);
		}

		std::uint32_t advance(const std::string & str) {
			return advance(str.data(), str.size());
		}
		std::uint32_t advance(const void* key, std::intptr_t len) {
			std::uint32_t result = eval(key, len);

			state = result;
			return result;
		}

		std::uint32_t eval(const std::string & str) const {
			return eval(str.data(), str.size());
		}
		std::uint32_t eval(const void* key, std::intptr_t len) const {
			/* 32-Bit MurmurHash3: https://code.google.com/p/smhasher/wiki/MurmurHash3*/
			std::uint32_t h1 = state;
			std::uint32_t k1;
			const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key);
			const std::uint8_t* keyptr = data;
			constexpr int bsize = sizeof(k1);
			const std::intptr_t nblocks = len / bsize;

			const std::uint32_t c1 = 0xcc9e2d51u;
			const std::uint32_t c2 = 0x1b873593u;
			const std::uint8_t* tail;
			std::intptr_t i;

			/* body */
			if (!key) return 0;
			for (i = 0; i < nblocks; ++i, keyptr += bsize) {
				k1 = 0;
				k1 |= static_cast<std::uint32_t>(keyptr[0]);
				k1 |= static_cast<std::uint32_t>(keyptr[1]) << 8;
				k1 |= static_cast<std::uint32_t>(keyptr[2]) << 16;
				k1 |= static_cast<std::uint32_t>(keyptr[3]) << 24;

				k1 *= c1;
				k1 = rotl32(k1, 15);
				k1 *= c2;

				h1 ^= k1;
				h1 = rotl32(h1, 13);
				h1 = h1 * 5u + 0xe6546b64u;
			}

			/* tail */
			tail = (const uint8_t*)(data + nblocks * static_cast<std::intptr_t>(4));
			k1 = 0;
			switch (len & 3) {
			case 3: k1 ^= (std::uint32_t)(tail[2] << 16); /* fallthrough */
			case 2: k1 ^= (std::uint32_t)(tail[1] << 8u); /* fallthrough */
			case 1: k1 ^= tail[0];
				k1 *= c1;
				k1 = rotl32(k1, 15);
				k1 *= c2;
				h1 ^= k1;
				break;
			default: break;
			}

			/* finalization */
			h1 ^= (std::uint32_t)len;
			/* fmix32 */
			h1 ^= h1 >> 16;
			h1 *= 0x85ebca6bu;
			h1 ^= h1 >> 13;
			h1 *= 0xc2b2ae35u;
			h1 ^= h1 >> 16;

			return h1;
		}
		
		std::uint32_t state;
	};


	struct XorShift32 {
		XorShift32()
			: state(0)
		{}
		XorShift32(std::uint32_t s)
			: state(s)
		{}

		~XorShift32() = default;
		XorShift32(const XorShift32&) = default;
		XorShift32& operator=(const XorShift32&) = default;

		std::uint32_t operator()() {
			return advance();
		}

		std::uint32_t advance() {
			state = eval();
			return state;
		}

		std::uint32_t eval() const {
			std::uint32_t x = state;
			x ^= x << 13;
			x ^= x >> 17;
			x ^= x << 5;
			return x;
		}

		std::uint32_t state;
	};

	struct XorShift64 {
		XorShift64()
			: state(0)
		{}
		XorShift64(std::uint64_t s)
			: state(s)
		{}

		~XorShift64() = default;
		XorShift64(const XorShift64&) = default;
		XorShift64& operator=(const XorShift64&) = default;

		std::uint64_t operator()() {
			return advance();
		}

		std::uint64_t advance() {
			state = eval();
			return state;
		}

		std::uint64_t eval() const {
			std::uint64_t x = state;
			x ^= x << 13;
			x ^= x >> 7;
			x ^= x << 17;
			return x;
		}

		std::uint64_t state;
	};

	struct XorShift128 {
		XorShift128()
			: state{0,0,0,0}
		{}
		XorShift128(std::uint32_t s0, std::uint32_t s1, std::uint32_t s2, std::uint32_t s3)
			: state{s0, s1, s2, s3}
		{}

		~XorShift128() = default;
		XorShift128(const XorShift128&) = default;
		XorShift128& operator=(const XorShift128&) = default;

		std::uint32_t operator()() {
			return advance();
		}

		std::uint32_t advance() {
			/* Algorithm "xor128" from p. 5 of Marsaglia, "Xorshift RNGs" */
			std::uint32_t t = state[3];

			std::uint32_t const s = state[0];
			state[3] = state[2];
			state[2] = state[1];
			state[1] = s;

			t ^= t << 11;
			t ^= t >> 8;
			state[0] = t ^ s ^ (s >> 19);
			return state[0];
		}

		std::uint32_t eval() const {
			/* Algorithm "xor128" from p. 5 of Marsaglia, "Xorshift RNGs" */
			std::uint32_t t = state[3];
			std::uint32_t s = state[0];

			t ^= t << 11;
			t ^= t >> 8;
			return t ^ s ^ (s >> 19);
		}

		std::uint32_t state[4];
	};


	struct XorWow {
		XorWow()
			: state{0,0,0,0}
			, counter(0)
		{}
		XorWow(std::uint32_t s0, std::uint32_t s1, std::uint32_t s2, std::uint32_t s3)
			: state{s0, s1, s2, s3}
			, counter(0)
		{}

		~XorWow() = default;
		XorWow(const XorWow&) = default;
		XorWow& operator=(const XorWow&) = default;

		std::uint32_t operator()() {
			return advance();
		}

		std::uint32_t advance() {
			/* Algorithm "xorwow" from p. 5 of Marsaglia, "Xorshift RNGs" */
			std::uint32_t t = state[3];

			std::uint32_t const s = state[0];
			state[3] = state[2];
			state[2] = state[1];
			state[1] = s;

			t ^= t >> 2;
			t ^= t << 1;
			t ^= s ^ (s << 4);
			state[0] = t;

			counter += 362437;
			return t + counter;
		}

		std::uint32_t eval() const {
			/* Algorithm "xorwow" from p. 5 of Marsaglia, "Xorshift RNGs" */
			std::uint32_t t = state[3];
			std::uint32_t const s = state[0];

			t ^= t >> 2;
			t ^= t << 1;
			t ^= s ^ (s << 4);

			return t + counter + 362437;
		}

		std::uint32_t state[4];
		std::uint32_t counter;
	};

	struct SplitMix64 {
		SplitMix64()
			: state(0)
		{}
		SplitMix64(std::uint64_t s)
			: state(s)
		{}

		~SplitMix64() = default;
		SplitMix64(const SplitMix64&) = default;
		SplitMix64& operator=(const SplitMix64&) = default;

		std::uint64_t operator()() {
			return advance();
		}

		std::uint64_t advance() {
			std::uint64_t result = state;

			state = result + 0x9E3779B97f4A7C15;
			result = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9;
			result = (result ^ (result >> 27)) * 0x94D049BB133111EB;
			return result ^ (result >> 31);
		}

		std::uint64_t eval() const {
			std::uint64_t result = state;

			//state = result + 0x9E3779B97f4A7C15;
			result = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9;
			result = (result ^ (result >> 27)) * 0x94D049BB133111EB;
			return result ^ (result >> 31);
		}

		std::uint64_t state;
	};


	static float randomToFloat(std::uint32_t value) {
		constexpr float maxval = static_cast<float>(1u << 23);
		constexpr float inv = 1.f / static_cast<float>(maxval);

		value = value >> 9; // Only the 23 most significant bits can be used.
		// Its assumed that the values of the bits are uniformly random.

		return static_cast<float>(value) * inv;
	}

	static double randomToDouble(std::uint64_t value) {
		constexpr double maxval = static_cast<double>(1ull << 52);
		constexpr double inv = 1.0 / static_cast<double>(maxval);
		value = value >> 12;
		return static_cast<double>(value) * inv;
	}
};