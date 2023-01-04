#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	TEST_CLASS(strmath__compliment)
	{
		public:
		TEST_METHOD(Compliment_U1)
		{
			char result[100];
			
			str_compliment_P("1011,011", result, 2);
			Assert::AreEqual(result, "0100,100");

			str_compliment_P("0.01101,0010", result, 2);
			Assert::AreEqual(result, "1.10010,1101");

			str_compliment_P("1.1101,11", result, 2);
			Assert::AreEqual(result, "0.0010,00");

			str_compliment_P("32,1", result, 10);
			Assert::AreEqual(result, "67,8");
		}

		TEST_METHOD(Compliment_U2)
		{
			char result[100];

			str_compliment_Pm1("1011,011", result, 2);
			Assert::AreEqual(result, "0100,101");

			str_compliment_Pm1("0.01101,0010", result, 2);
			Assert::AreEqual(result, "1.10010,1110");

			str_compliment_Pm1("1.1101,11", result, 2);
			Assert::AreEqual(result, "0.0010,01");

			str_compliment_Pm1("1.000", result, 2);
			Assert::AreEqual(result, "0.1000");

			str_compliment_Pm1("32,1", result, 10);
			Assert::AreEqual(result, "67,9");


		}
	};
}