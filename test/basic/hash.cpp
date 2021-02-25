#include <limits>
#include <fmt/printf.h>
#include <iostream>
#include <ctime>

#include <ez/math/complex.hpp>
#include <ez/math/poly.hpp>
#include <ez/math/constants.hpp>
#include <ez/math/trig.hpp>
#include <ez/math/prng.hpp>

#include <ez/math/Trig.hpp>

int hash_test() {
	static constexpr char name0[] = "Ben";
	static constexpr char name1[] = "Lenz";
	static constexpr char name2[] = "Gachowski";
	static constexpr char name3[] = "Jacob";

	static constexpr wchar_t name4[] = L"Laura";
	static constexpr wchar_t name5[] = L"Bryan";
	static constexpr wchar_t name6[] = L"Helen";
	
	{ // 32bit Hashing
	// Make sure the constexpr version of hash works:
		static constexpr std::uint32_t constHash0 = ez::prng::djb2_32(name0);
		static constexpr std::uint32_t constHash1 = ez::prng::djb2_32(name1);
		static constexpr std::uint32_t constHash2 = ez::prng::djb2_32(name2);
		static constexpr std::uint32_t constHash3 = ez::prng::djb2_32(name3);

		std::uint32_t chash0 = constHash0;
		std::uint32_t chash1 = constHash1;
		std::uint32_t chash2 = constHash2;
		std::uint32_t chash3 = constHash3;

		std::uint32_t hash0 = ez::prng::djb2_32(name0, sizeof(name0)-1);
		std::uint32_t hash1 = ez::prng::djb2_32(name1, sizeof(name1)-1);
		std::uint32_t hash2 = ez::prng::djb2_32(name2, sizeof(name2)-1);
		std::uint32_t hash3 = ez::prng::djb2_32(name3, sizeof(name3)-1);

		if (chash0 != hash0) {
			fmt::print("Incorrect hash value\n");
			return -1;
		}
		if (chash1 != hash1) {
			fmt::print("Incorrect hash value\n");
			return -1;
		}
		if (chash2 != hash2) {
			fmt::print("Incorrect hash value\n");
			return -1;
		}
		if (chash3 != hash3) {
			fmt::print("Incorrect hash value\n");
			return -1;
		}

		std::uint32_t hash4 = ez::prng::djb2_32(name4, sizeof(name4) / sizeof(wchar_t) - 1);
		std::uint32_t hash5 = ez::prng::djb2_32(name5, sizeof(name5) / sizeof(wchar_t) - 1);
		std::uint32_t hash6 = ez::prng::djb2_32(name6, sizeof(name6) / sizeof(wchar_t) - 1);

		static constexpr std::uint32_t hash7 = ez::prng::djb2_32(name4);

		std::wstring name7 = name4;

		std::uint32_t hash8 = ez::prng::djb2_32(name7);

		if (hash4 != hash7) {
			fmt::print("Incorrect hash value\n");
			return -1;
		}
		if (hash4 != hash8) {
			fmt::print("Incorrect hash value\n");
			return -1;
		}

		if (hash4 != ez::prng::djb2_32(std::wstring_view(name4, sizeof(name4) / sizeof(wchar_t) - 1))) {
			fmt::print("Incorrect hash value\n");
			return -1;
		}

		// Constexpr string_view test
		static constexpr std::wstring_view cname4(name4, sizeof(name4) / sizeof(wchar_t) - 1);
		static constexpr std::string_view cname5(name0, sizeof(name0) - 1);

		static constexpr std::uint32_t constHash4 = ez::prng::djb2_32(cname4);
		static constexpr std::uint32_t constHash5 = ez::prng::djb2_32(cname5);

		std::uint32_t chash4 = constHash4;
		std::uint32_t chash5 = constHash5;

		if (hash4 != chash4) {
			fmt::print("Incorrect hash value\n");
			return -1;
		}
		if (hash0 != chash5) {
			fmt::print("Incorrect hash value\n");
			return -1;
		}
	}
	
	{ // 64bit Hashing
	// Make sure the constexpr version of hash works:
		static constexpr std::uint64_t constHash0 = ez::prng::djb2_64(name0);
		static constexpr std::uint64_t constHash1 = ez::prng::djb2_64(name1);
		static constexpr std::uint64_t constHash2 = ez::prng::djb2_64(name2);
		static constexpr std::uint64_t constHash3 = ez::prng::djb2_64(name3);

		std::uint64_t hash0 = ez::prng::djb2_64(name0, sizeof(name0)-1);
		std::uint64_t hash1 = ez::prng::djb2_64(name1, sizeof(name1)-1);
		std::uint64_t hash2 = ez::prng::djb2_64(name2, sizeof(name2)-1);
		std::uint64_t hash3 = ez::prng::djb2_64(name3, sizeof(name3)-1);
		
		if (constHash0 != hash0) {
			fmt::print("Incorrect hash value\n");
			return -1;
		}
		if (constHash1 != hash1) {
			fmt::print("Incorrect hash value\n");
			return -1;
		}
		if (constHash2 != hash2) {
			fmt::print("Incorrect hash value\n");
			return -1;
		}
		if (constHash3 != hash3) {
			fmt::print("Incorrect hash value\n");
			return -1;
		}

		static constexpr wchar_t name4[] = L"Laura";
		static constexpr wchar_t name5[] = L"Bryan";
		static constexpr wchar_t name6[] = L"Helen";

		std::uint64_t hash4 = ez::prng::djb2_64(name4, sizeof(name4) / sizeof(wchar_t) - 1);
		std::uint64_t hash5 = ez::prng::djb2_64(name5, sizeof(name5) / sizeof(wchar_t) - 1);
		std::uint64_t hash6 = ez::prng::djb2_64(name6, sizeof(name6) / sizeof(wchar_t) - 1);

		static constexpr std::uint64_t hash7 = ez::prng::djb2_64(name4);

		std::wstring name7 = name4;

		std::uint64_t hash8 = ez::prng::djb2_64(name7);

		if (hash4 != hash7) {
			fmt::print("Incorrect hash value\n");
			return -1;
		}
		if (hash4 != hash8) {
			fmt::print("Incorrect hash value\n");
			return -1;
		}

		if (hash4 != ez::prng::djb2_64(std::wstring_view(name4, sizeof(name4) / sizeof(wchar_t) - 1))) {
			fmt::print("Incorrect hash value\n");
			return -1;
		}

		// Constexpr string_view test
		static constexpr std::wstring_view cname4(name4, sizeof(name4) / sizeof(wchar_t) - 1);
		static constexpr std::uint64_t constHash4 = ez::prng::djb2_64(cname4);

		std::uint64_t chash4 = constHash4;

		if (hash4 != chash4) {
			fmt::print("Incorrect hash value\n");
			return -1;
		}
	}

	return 0;
}