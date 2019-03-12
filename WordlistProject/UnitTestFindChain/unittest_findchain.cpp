#include "stdafx.h"
#include "CppUnitTest.h"
#include "../WordlistProject/FindChain.h"
#include "../WordlistProject/Initialization.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestFindChain
{		
	TEST_CLASS(UnitTestBuildMap)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			char* argv[10] = { "program","-w","../WordlistProject/atest.txt" };
			Init init;
			init.parse(3, argv);
			Assert::IsTrue(init.w);
			Assert::AreEqual(init.file, "../WordlistProject/atest.txt");
			init.InitWord();
			Assert::AreEqual(init.n, 5);
			Assert::AreEqual(init.words[0], "ab");
			Assert::AreEqual(init.words[1], "bc");
			Assert::AreEqual(init.words[2], "cd");
			Assert::AreEqual(init.words[3], "de");
			Assert::AreEqual(init.words[4], "ef");
			FindChain findchain;
			findchain.BuildMap(init.words, init.n, 1, init.h, init.t);
			Assert::
		}

	};
	TEST_CLASS(UnitTesthasRing)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			// TODO: 在此输入测试代码
		}

	};
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			// TODO: 在此输入测试代码
		}

	};
}