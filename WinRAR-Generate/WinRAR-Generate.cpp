// WinRAR-Generate.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "WinRAR-Generate.h"

#include "SHA1.hpp"
#include "ecCurveOverGF$2$15$17.hpp"
#include "math_functions/mb_add_x.hpp"
#include "math_functions/mb_sub_x.hpp"
#include "math_functions/mb_mul_x.hpp"
#include "math_functions/mb_div_x.hpp"
#include "math_functions/mb_cmp_x.hpp"


// 这是导出变量的一个示例
char keyInfo[512] = { 0 };

// 这是导出函数的一个示例。
namespace WinRAR {
	struct SignatureData {
		uint64_t r[4];
		uint64_t s[4];
	};

	extern const uint16_t BasePoly = 0x8003;
	extern const GaloisField$2$15<BasePoly> ExtendPoly[18] = { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
	const uint64_t Order[4] = {
		0x5e0d258442bbcd31,
		0x0547840e4bf72d8b,
		0x2314691ced9bbec3,
		0x0001026dd85081b8
	};

	ecCurveOverGF$2$15$17<BasePoly, ExtendPoly> ecCurve(0, 161);
}

template<>
GaloisField$2$15$17<WinRAR::BasePoly, WinRAR::ExtendPoly>&
GaloisField$2$15$17<WinRAR::BasePoly, WinRAR::ExtendPoly>::operator*=(const GaloisField$2$15$17<WinRAR::BasePoly, WinRAR::ExtendPoly>& other) {

	GaloisField$2$15<WinRAR::BasePoly> temp[33] = {};
	for (int i = 0; i < 17; ++i) {
		for (int j = 0; j < 17; ++j) {
			temp[i + j] += Data[i] * other.Data[j];
		}
	}

	for (int i = 33 - 1; i > 17 - 1; --i) {
		if (temp[i].Value != 0) {
			temp[i - 17] += temp[i];
			temp[i - 17 + 3] += temp[i];
		}
	}

	for (int i = 0; i < 17; ++i)
		Data[i] = temp[i];

	return *this;
}

namespace WinRAR {

	auto G = ecCurve.GetPoint({
		0x38CC, 0x052F, 0x2510, 0x45AA,
		0x1B89, 0x4468, 0x4882, 0x0D67,
		0x4FEB, 0x55CE, 0x0025, 0x4CB7,
		0x0CC2, 0x59DC, 0x289E, 0x65E3,
		0x56FD
	}, {
		0x31A7, 0x65F2, 0x18C4, 0x3412,
		0x7388, 0x54C1, 0x539B, 0x4A02,
		0x4D07, 0x12D6, 0x7911, 0x3B5E,
		0x4F0E, 0x216F, 0x2BF2, 0x1974,
		0x20DA
	});

	uint64_t PrivateKey[4] = {
		0xae9044b7fd61d65e, 0x5fb9f11f467450c1,
		0xbdb95f0105271fa8, 0x59fe6abcca90
	};

	auto Public_ecPoint = ecCurve.GetPoint({
		0x3A1A, 0x1109, 0x268A, 0x12F7,
		0x3734, 0x75F0, 0x576C, 0x2EA4,
		0x4813, 0x3F62, 0x0567, 0x784D,
		0x753D, 0x6D92, 0x366C, 0x1107,
		0x3861
	}, {
		0x6C20, 0x6027, 0x1B22, 0x7A87,
		0x43C4, 0x1908, 0x2449, 0x4675,
		0x7933, 0x2E66, 0x32F5, 0x2A58,
		0x1145, 0x74AC, 0x36D0, 0x2731,
		0x12B6
	});

	uint32_t SBox[8][0x100] = {};

	void InitSBox(uint32_t srcBuffer[8][0x100]) {
		if (srcBuffer[0][1] == 0) {
			for (int i = 0; i < 0x100; ++i) {

				uint32_t result = i;
				for (int j = 0; j < 8; ++j)
					if (result % 2) {
						result /= 2;
						result ^= 0xEDB88320;
					}
					else {
						result /= 2;
					}

					srcBuffer[0][i] = result;
			}
		}

		// Expanding
		for (int i = 0; i < 256; ++i) {
			uint32_t temp = srcBuffer[0][i];
			for (int j = 1; j < 8; ++j) {
				temp = (temp >> 8) ^ srcBuffer[0][static_cast<uint8_t>(temp)];
				srcBuffer[j][i] = temp;
			}
		}
	}

	uint32_t Hasher(uint32_t a0, const uint8_t* buffer, size_t length, uint32_t SBox[8][0x100]) {
		uint32_t ret = a0;
		size_t Count = length / 8;
		if (length >= 8) {
			for (size_t i = 0; i < Count; ++i) {
				uint8_t selector[2][4];
				*reinterpret_cast<uint32_t*>(selector[0]) = reinterpret_cast<const uint32_t*>(buffer)[i * 2] ^ ret;
				*reinterpret_cast<uint32_t*>(selector[1]) = reinterpret_cast<const uint32_t*>(buffer)[i * 2 + 1];

				ret = SBox[7][selector[0][0]] ^
					SBox[6][selector[0][1]] ^
					SBox[5][selector[0][2]] ^
					SBox[4][selector[0][3]] ^
					SBox[3][selector[1][0]] ^
					SBox[2][selector[1][1]] ^
					SBox[1][selector[1][2]] ^
					SBox[0][selector[1][3]];

			}
		}

		for (size_t i = Count * 8; i < length; ++i)
			ret = (ret >> 8) ^ SBox[0][static_cast<uint8_t>(ret) ^ buffer[i]];

		return ret;
	}

	template<size_t length>
	void ConvertToHexString(const uint64_t(&in_data)[length], char(&out_string)[2 * length * sizeof(uint64_t) + 1], bool use_lowercase = true) {
		static const char UppercaseConvertTable[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
		static const char LowercaseConvertTable[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

		const uint8_t* cur_ptr = reinterpret_cast<const uint8_t*>(in_data + length) - 1;
		const char* ConvertTable = use_lowercase ? LowercaseConvertTable : UppercaseConvertTable;

		while (*cur_ptr == 0)
			--cur_ptr;

		out_string[2 * (cur_ptr - reinterpret_cast<const uint8_t*>(in_data) + 1)] = 0;
		for (size_t i = 0; cur_ptr >= reinterpret_cast<const uint8_t*>(in_data); i += 2) {
			out_string[i] = ConvertTable[*cur_ptr >> 4];
			out_string[i + 1] = ConvertTable[*cur_ptr & 0xF];
			--cur_ptr;
		}
	}

	template<size_t length>
	size_t GetByteLength(const uint64_t(&in_data)[length]) {
		const uint8_t* cur_ptr = reinterpret_cast<const uint8_t*>(in_data + length);
		while (*(--cur_ptr) == 0);
		++cur_ptr;

		return cur_ptr - reinterpret_cast<const uint8_t*>(in_data);
	}

	void InitializeGenerator(uint32_t(&out_Generator)[16],
		const uint8_t in_private[], size_t in_private_length,
		const uint8_t in_data[], size_t in_data_length) {

		memset(out_Generator, 0, sizeof(out_Generator));

		if (in_private == nullptr) {
			out_Generator[1] = 0xeb3eb781;
			out_Generator[2] = 0x50265329;
			out_Generator[3] = 0xdc5ef4a3;
			out_Generator[4] = 0x6847b9d5;
			out_Generator[5] = 0xcde43b4c;
		}
		else {
			accel::algorithm::hash::SHA1::Initialize(out_Generator + 1);
			accel::algorithm::hash::SHA1::MainCycle(in_private, in_private_length, out_Generator + 1);
			accel::algorithm::hash::SHA1::Final(in_private, in_private_length, out_Generator + 1);
			for (int i = 1; i < 6; ++i)
				out_Generator[i] = _byteswap_ulong(out_Generator[i]);
		}

		accel::algorithm::hash::SHA1::Initialize(out_Generator + 6);
		accel::algorithm::hash::SHA1::MainCycle(in_data, in_data_length, out_Generator + 6);
		accel::algorithm::hash::SHA1::Final(in_data, in_data_length, out_Generator + 6);
		accel::algorithm::hash::SHA1::MainCycle(nullptr, 0, out_Generator + 6);
		accel::algorithm::hash::SHA1::Final(nullptr, 0, out_Generator + 11);
		for (int i = 6; i < 16; ++i)
			out_Generator[i] = _byteswap_ulong(out_Generator[i]);
	}

	void GeneratePrivateKey(uint32_t(&in_Generator)[6], uint64_t(&out_PrivateKey)[4]) {
		memset(out_PrivateKey, 0, sizeof(out_PrivateKey));

		uint32_t hash[5];
		for (int i = 0; i < 15; ++i) {
			accel::algorithm::hash::SHA1::Initialize(hash);
			in_Generator[0] = i + 1;
			accel::algorithm::hash::SHA1::MainCycle(reinterpret_cast<const uint8_t*>(in_Generator),
				sizeof(uint32_t) * 6, hash);
			accel::algorithm::hash::SHA1::Final(reinterpret_cast<const uint8_t*>(in_Generator),
				sizeof(uint32_t) * 6, hash);
			reinterpret_cast<uint16_t*>(out_PrivateKey)[i] = static_cast<uint16_t>(_byteswap_ulong(hash[0]));
		}
	}

	void GeneratePrivateKey(const uint8_t in_private[], size_t in_private_length, uint64_t(&out_PrivateKey)[4]) {
		uint32_t Generator[6];

		if (in_private == nullptr) {
			Generator[1] = 0xeb3eb781;
			Generator[2] = 0x50265329;
			Generator[3] = 0xdc5ef4a3;
			Generator[4] = 0x6847b9d5;
			Generator[5] = 0xcde43b4c;
		}
		else {
			accel::algorithm::hash::SHA1::Initialize(Generator + 1);
			accel::algorithm::hash::SHA1::MainCycle(in_private, in_private_length, Generator + 1);
			accel::algorithm::hash::SHA1::Final(in_private, in_private_length, Generator + 1);
			for (int i = 1; i < 6; ++i)
				Generator[i] = _byteswap_ulong(Generator[i]);
		}

		GeneratePrivateKey(Generator, out_PrivateKey);
	}

	void GeneratePublicX(const uint64_t(&in_PrivateKey)[4], uint64_t(&out_PublicX)[4]) {
		auto nP = G.GetNP(in_PrivateKey, 4);
		nP.X.ToDecimal(out_PublicX);
#if defined(_M_X64)
		accel::math::mb_mul_u64(out_PublicX, 4, 2);
		accel::math::mb_add_u64(out_PublicX, 4, (nP.Y / nP.X).Value[0] & 1, 0);
#elif defined(_M_IX86)
		accel::math::mb_mul_u32(reinterpret_cast<uint32_t*>(out_PublicX), 8, 2);
		accel::math::mb_add_u32(reinterpret_cast<uint32_t*>(out_PublicX), 8, (nP.Y / nP.X).Value[0] & 1, 0);
#endif
	}

	void GeneratePublicX(const uint8_t in_private[], size_t in_private_length, uint64_t(&out_PublicX)[4]) {
		uint64_t PrivateKey[4];
		GeneratePrivateKey(in_private, in_private_length, PrivateKey);
		GeneratePublicX(PrivateKey, out_PublicX);
	}

	void GenerateK(uint32_t(&in_Generator)[16], uint64_t(&out_K)[4]) {
		memset(out_K, 0, sizeof(out_K));

		uint32_t hash[5];
		for (int i = 0; i < 15; ++i) {
			accel::algorithm::hash::SHA1::Initialize(hash);
			++in_Generator[0];
			accel::algorithm::hash::SHA1::MainCycle(reinterpret_cast<const uint8_t*>(in_Generator),
				sizeof(in_Generator), hash);
			accel::algorithm::hash::SHA1::Final(reinterpret_cast<const uint8_t*>(in_Generator),
				sizeof(in_Generator), hash);
			reinterpret_cast<uint16_t*>(out_K)[i] = static_cast<uint16_t>(_byteswap_ulong(hash[0]));
		}
	}

	void Signature(const uint8_t in_data[], size_t in_data_length,
		const uint8_t in_private[], size_t in_private_length,
		SignatureData& out_SignData) {

		uint32_t Generator[16];
		InitializeGenerator(Generator, in_private, in_private_length, in_data, in_data_length);

		uint64_t PrivateKey[4];
		uint64_t K[4];
		GeneratePrivateKey(reinterpret_cast<uint32_t(&)[6]>(Generator), PrivateKey);
		GenerateK(Generator, K);

		uint64_t Hash[4] = {};
		memcpy_s(Hash, 15 * sizeof(uint16_t), Generator + 6, 15 * sizeof(uint16_t));

		memset(out_SignData.r, 0, sizeof(out_SignData.r));
		G.GetNP(K, 4).X.ToDecimal(out_SignData.r);
#if defined(_M_X64)
		accel::math::mb_add_mb(out_SignData.r, 4, Hash, 4, 0);
		accel::math::mb_div_mb(out_SignData.r, 4, Order, 4, out_SignData.s, 4);

		memset(out_SignData.s, 0, sizeof(out_SignData.s));
		uint64_t PrivateKey_mul_r[8] = {};
		uint64_t temp_quotient[5] = {};
		accel::math::mb_mul_mb(PrivateKey, 4, out_SignData.r, 4, PrivateKey_mul_r, 8);
		accel::math::mb_div_mb(PrivateKey_mul_r, 8, Order, 4, temp_quotient, 5);

		if (accel::math::mb_sub_mb(K, 4, PrivateKey_mul_r, 4, out_SignData.s, 4, 0))
			accel::math::mb_add_mb(out_SignData.s, 4, Order, 4, 0);
#elif defined(_M_IX86)
		accel::math::mb_add_mb(reinterpret_cast<uint32_t*>(out_SignData.r), 8,
			reinterpret_cast<uint32_t*>(Hash), 8,
			0);
		accel::math::mb_div_mb(reinterpret_cast<uint32_t*>(out_SignData.r), 8,
			reinterpret_cast<const uint32_t*>(Order), 8,
			reinterpret_cast<uint32_t*>(out_SignData.s), 8);

		memset(out_SignData.s, 0, sizeof(out_SignData.s));
		uint64_t PrivateKey_mul_r[8] = {};
		uint64_t temp_quotient[5] = {};
		accel::math::mb_mul_mb(reinterpret_cast<uint32_t*>(PrivateKey), 8,
			reinterpret_cast<uint32_t*>(out_SignData.r), 8,
			reinterpret_cast<uint32_t*>(PrivateKey_mul_r), 16);

		accel::math::mb_div_mb(reinterpret_cast<uint32_t*>(PrivateKey_mul_r), 16,
			reinterpret_cast<const uint32_t*>(Order), 8,
			reinterpret_cast<uint32_t*>(temp_quotient), 10);

		if (accel::math::mb_sub_mb(reinterpret_cast<uint32_t*>(K), 8,
			reinterpret_cast<uint32_t*>(PrivateKey_mul_r), 8,
			reinterpret_cast<uint32_t*>(out_SignData.s), 8,
			0)) {
			accel::math::mb_add_mb(reinterpret_cast<uint32_t*>(out_SignData.s), 8,
				reinterpret_cast<const uint32_t*>(Order), 8,
				0);
		}
#endif
	}

}

#ifdef _DEBUG
void StartTest() {
	GaloisField$2$15$17<WinRAR::BasePoly, WinRAR::ExtendPoly> a0({
		0x4CAB, 0x7F00, 0x409B, 0x784F,
		0x6105, 0x2D19, 0x4699, 0x4D0F,
		0x5420, 0x5625, 0x7342, 0x2D0D,
		0x1DCE, 0x1052, 0x3450, 0x0595,
		0x6CCD
	});

	GaloisField$2$15$17<WinRAR::BasePoly, WinRAR::ExtendPoly> b0({
		0x5AA7, 0x7315, 0x2132, 0x12D1,
		0x1DD3, 0x1540, 0x71BF, 0x42C6,
		0x3BAE, 0x3F1C, 0x3A14, 0x2619,
		0x63E7, 0x6936, 0x2919, 0x76ED,
		0x1D9B
	});

	GaloisField$2$15$17<WinRAR::BasePoly, WinRAR::ExtendPoly> c0({
		0x65C2, 0x677A, 0x0931, 0x5067,
		0x1FDA, 0x0E0C, 0x1801, 0x779D,
		0x0918, 0x6F18, 0x3A36, 0x688F,
		0x78EF, 0x17E0, 0x27C7, 0x7F67,
		0x10DB
	});

	assert(a0 * b0 == c0);
	_tprintf_s(TEXT("Test 1 passed.\n"));

	GaloisField$2$15$17<WinRAR::BasePoly, WinRAR::ExtendPoly> a1({
		0x7CEC, 0x3646, 0x2C52, 0x1363,
		0x7A87, 0x4666, 0x083E, 0x2B1E,
		0x088B, 0x5142, 0x1D95, 0x0BAD,
		0x31A0, 0x6EB6, 0x2116, 0x5818,
		0x0147
	});

	GaloisField$2$15$17<WinRAR::BasePoly, WinRAR::ExtendPoly> b1({
		0x5AA7, 0x7315, 0x2132, 0x12D1,
		0x1DD3, 0x1540, 0x71BF, 0x42C6,
		0x3BAE, 0x3F1C, 0x3A14, 0x2619,
		0x63E7, 0x6936, 0x2919, 0x76ED,
		0x1D9B
	});

	assert(a1.Inverse() == b1);
	_tprintf_s(TEXT("Test 2 passed.\n"));

	ecCurveOverGF$2$15$17<WinRAR::BasePoly, WinRAR::ExtendPoly> ecCurve(0, 161);

	auto a2 = ecCurve.GetPoint({
		0x655E, 0x4AEC, 0x5EC4, 0x280E,
		0x2D9C, 0x018A, 0x6CE6, 0x6446,
		0x4F96, 0x64E4, 0x5A48, 0x36AB,
		0x79E3, 0x7854, 0x785A, 0x6CB9,
		0x5E76
	}, {
		0x5243, 0x4418, 0x0E66, 0x5BC2,
		0x7474, 0x6F88, 0x18F1, 0x66FC,
		0x70AC, 0x21D0, 0x32C8, 0x3048,
		0x3670, 0x5494, 0x645F, 0x2000,
		0x02E0
	});

	auto b2 = ecCurve.GetPoint({
		0x16C8, 0x6E22, 0x6C50, 0x4047,
		0x21C2, 0x0076, 0x4E69, 0x0713,
		0x5BB9, 0x1876, 0x53E3, 0x4D6A,
		0x4E1C, 0x3C95, 0x0849, 0x2C91,
		0x4CB3
	}, {
		0x4079, 0x498C, 0x48CC, 0x2623,
		0x4BAD, 0x5660, 0x6E3E, 0x5C7B,
		0x1E5C, 0x5CD6, 0x3C60, 0x42C7,
		0x0285, 0x3C96, 0x282C, 0x291A,
		0x3D9B
	});

	auto c2 = ecCurve.GetPoint({
		0x20BE, 0x22A6, 0x3969, 0x7EF6,
		0x72E3, 0x2EB7, 0x59B0, 0x254D,
		0x3E78, 0x43C1, 0x6209, 0x0D70,
		0x1BA0, 0x1BB0, 0x765E, 0x094F,
		0x2A88
	}, {
		0x19B7, 0x235B, 0x1E4D, 0x33A4,
		0x07A3, 0x73E5, 0x793A, 0x698E,
		0x4E2E, 0x4966, 0x3FF9, 0x4004,
		0x0AAF, 0x1692, 0x7900, 0x27B5,
		0x16EA
	});

	assert(a2 + b2 == c2);
	_tprintf_s(TEXT("Test 3 passed.\n"));

	auto a3 = ecCurve.GetPoint({
		0x38CC, 0x052F, 0x2510, 0x45AA,
		0x1B89, 0x4468, 0x4882, 0x0D67,
		0x4FEB, 0x55CE, 0x0025, 0x4CB7,
		0x0CC2, 0x59DC, 0x289E, 0x65E3,
		0x56FD
	}, {
		0x31A7, 0x65F2, 0x18C4, 0x3412,
		0x7388, 0x54C1, 0x539B, 0x4A02,
		0x4D07, 0x12D6, 0x7911, 0x3B5E,
		0x4F0E, 0x216F, 0x2BF2, 0x1974,
		0x20DA
	});

	uint64_t b3[4] = {
		0x5b634cf28190bdf4, 0x8519aa87488bf38f,
		0xc5c62f0238f183d2, 0x35c6ab9048e2
	};

	auto c3 = ecCurve.GetPoint({
		0x651C, 0x1207, 0x331A, 0x5769,
		0x76BD, 0x690D, 0x5159, 0x672C,
		0x10C7, 0x559C, 0x004C, 0x5F04,
		0x4476, 0x7DDB, 0x6912, 0x2E81,
		0x5FA6
	}, {
		0x1DD9, 0x38CE, 0x6F31, 0x24A9,
		0x66D6, 0x4E59, 0x0301, 0x08C6,
		0x1759, 0x39EE, 0x2B52, 0x1839,
		0x6F5A, 0x7FBC, 0x59A3, 0x30F8,
		0x66E0
	});

	assert(a3.GetNP(b3, 4) == c3);
	_tprintf_s(TEXT("Test 4 passed.\n"));
}
#endif // _DEBUG

bool GenerateRegisterData(const char* UserName, const char* LicenseType, char(&RegisterData)[4][512], char(&UID)[512]) {
	memset(RegisterData, 0, sizeof(RegisterData));

	uint64_t temp_value[4];
	char temp_string[2][sizeof(temp_value) * 2 + 1];
	WinRAR::GeneratePublicX(reinterpret_cast<const uint8_t*>(UserName), strlen(UserName), temp_value);
	WinRAR::ConvertToHexString(temp_value, temp_string[0]);
	if (_stprintf_s(RegisterData[3], TEXT("%zd%.48s"), strlen(temp_string[0]) - 4, temp_string) == -1)
		return false;

	WinRAR::GeneratePublicX(reinterpret_cast<const uint8_t*>(RegisterData[3]), strlen(RegisterData[3]), temp_value);
	WinRAR::ConvertToHexString(temp_value, temp_string[1]);
	if (_stprintf_s(RegisterData[0], TEXT("%s"), temp_string[1]) == -1)
		return false;

	if (_stprintf_s(UID, TEXT("UID=%.16s%.4s"), temp_string[0] + 48, temp_string[1]) == -1)
		return false;

	WinRAR::SignatureData LicenseType_Signature;
	WinRAR::Signature(reinterpret_cast<const uint8_t*>(LicenseType), strlen(LicenseType), nullptr, 0, LicenseType_Signature);
	WinRAR::ConvertToHexString(LicenseType_Signature.r, temp_string[0]);
	WinRAR::ConvertToHexString(LicenseType_Signature.s, temp_string[1]);
	if (_stprintf_s(RegisterData[1], TEXT("60%060s%060s"), temp_string[1], temp_string[0]) == -1)
		return false;

	WinRAR::SignatureData UserName_Signature;
	if (_stprintf_s(RegisterData[2], TEXT("%s%s"), UserName, RegisterData[0]) == -1)
		return false;
	WinRAR::Signature(reinterpret_cast<const uint8_t*>(RegisterData[2]), strlen(RegisterData[2]),
		nullptr, 0,
		UserName_Signature);
	WinRAR::ConvertToHexString(UserName_Signature.r, temp_string[0]);
	WinRAR::ConvertToHexString(UserName_Signature.s, temp_string[1]);
	if (_stprintf_s(RegisterData[2], TEXT("60%060s%060s"), temp_string[1], temp_string[0]) == -1)
		return false;

	return true;
}

uint32_t GenerateChecksum(const char* UserName, const char* LicenseType, char(&RegisterData)[4][512]) {
	uint32_t result = WinRAR::Hasher(0xffffffff,
		reinterpret_cast<const uint8_t*>(LicenseType),
		strlen(LicenseType),
		WinRAR::SBox);
	result = WinRAR::Hasher(result, reinterpret_cast<const uint8_t*>(UserName),
		strlen(UserName),
		WinRAR::SBox);

	result = WinRAR::Hasher(result, reinterpret_cast<const uint8_t*>(RegisterData[0]),
		strlen(RegisterData[0]),
		WinRAR::SBox);
	result = WinRAR::Hasher(result, reinterpret_cast<const uint8_t*>(RegisterData[1]),
		strlen(RegisterData[1]),
		WinRAR::SBox);
	result = WinRAR::Hasher(result, reinterpret_cast<const uint8_t*>(RegisterData[2]),
		strlen(RegisterData[2]),
		WinRAR::SBox);
	result = WinRAR::Hasher(result, reinterpret_cast<const uint8_t*>(RegisterData[3]),
		strlen(RegisterData[3]),
		WinRAR::SBox);
	return result;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 WinRAR-Generate.h
_LIBAPI char* KeyGenerate(const char* UserName, const char* LicenseType) {

	GaloisField$2$15<WinRAR::BasePoly>::InitTable();
	WinRAR::InitSBox(WinRAR::SBox);

	char keyHead[512] = { 0 };
	char keyData[512] = { 0 };

	char RegisterData[4][512] = { 0 };
	char UID[512] = { 0 };
	//if (GenerateRegisterData(UserName, LicenseType, RegisterData, UID) == false)
	//	throw std::overflow_error("buffer is too small.");
	GenerateRegisterData(UserName, LicenseType, RegisterData, UID);

	uint32_t checksum = GenerateChecksum(UserName, LicenseType, RegisterData);

	int stlen = _stprintf_s(keyHead, TEXT("RAR registration data\n%s\n%s\n%s\n"),
		UserName,
		LicenseType,
		UID);
	memset(&keyHead[stlen], 0, sizeof(keyHead) - stlen);

	stlen = _stprintf_s(UID, TEXT("%zd%zd%zd%zd%s%s%s%s%lu"),
		strlen(RegisterData[0]),
		strlen(RegisterData[1]),
		strlen(RegisterData[2]),
		strlen(RegisterData[3]),
		RegisterData[0],
		RegisterData[1],
		RegisterData[2],
		RegisterData[3],
		checksum);
	memset(&UID[stlen], 0, sizeof(UID) - stlen);

	int keylen = 0;
	for (int i = 0; i < 8; ++i) {
		char temp[56] = { 0 };
		stlen = _stprintf_s(temp, TEXT("%.54s\n"), UID + i * 54);
		memset(&temp[stlen], 0, sizeof(temp) - stlen);
		if (stlen != 1 && temp[0] != '\n') {
			keylen += stlen;
			_tcscat_s(keyData, temp);
		}
	}
	memset(&keyData[keylen], 0, sizeof(keyData) - keylen);

	//_tcscat_s(keyInfo, keyHead);
	//_tcscat_s(keyInfo, keyData);
	_stprintf_s(keyInfo, TEXT("%s%s"), keyHead, keyData);

	stlen = (int)strlen(keyInfo);
	memset(&keyInfo[stlen], 0, sizeof(keyInfo) - stlen);
	//MessageBoxA(NULL, keyInfo, "UID", MB_OK);

	return keyInfo;
}