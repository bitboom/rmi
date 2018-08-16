/*
 *  Copyright (c) 2018 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */
/*
 * @file        test-archive.cpp
 * @author      Sangwan Kwon (sangwan.kwon@samsung.com)
 */

#include "stream/archive.hxx"

#include <klay/testbench.h>

#include <memory>
#include <limits>
#include <string>
#include <cassert>

using namespace rmi::stream;

TESTCASE(SERIALIZER_FUNDAMENTAL_INT)
{
	int input1 = std::numeric_limits<int>::lowest();
	int input2 = std::numeric_limits<int>::max();

	Archive archive;
	archive << input1 << input2;

	int output1, output2;
	archive >> output1 >> output2;

	TEST_EXPECT(input1, output1);
	TEST_EXPECT(input2, output2);
}

TESTCASE(SERIALIZER_FUNDAMENTAL_FLOAT)
{
	float input1 = std::numeric_limits<float>::lowest();
	float input2 = std::numeric_limits<float>::max();

	Archive archive;
	archive << input1 << input2;

	float output1, output2;
	archive >> output1 >> output2;

	TEST_EXPECT(input1, output1);
	TEST_EXPECT(input2, output2);
}

TESTCASE(SERIALIZER_FUNDAMENTAL_DOUBLE)
{
	double input1 = std::numeric_limits<double>::lowest();
	double input2 = std::numeric_limits<double>::max();

	Archive archive;
	archive << input1 << input2;

	double output1, output2;
	archive >> output1 >> output2;

	TEST_EXPECT(input1, output1);
	TEST_EXPECT(input2, output2);
}

TESTCASE(SERIALIZER_FUNDAMENTAL_LONG_LONG_INT)
{
	long long int input1 = std::numeric_limits<long long int>::lowest();
	long long int input2 = std::numeric_limits<long long int>::max();

	Archive archive;
	archive << input1 << input2;

	long long int output1, output2;
	archive >> output1 >> output2;

	TEST_EXPECT(input1, output1);
	TEST_EXPECT(input2, output2);
}

TESTCASE(SERIALIZER_FUNDAMENTAL_BOOL)
{
	bool input1 = true;
	bool input2 = false;

	Archive archive;
	archive << input1 << input2;

	bool output1, output2;
	archive >> output1 >> output2;

	TEST_EXPECT(input1, output1);
	TEST_EXPECT(input2, output2);
}

TESTCASE(SERIALIZER_FUNDAMENTAL_CHAR)
{
	char input1 = 'a';
	char input2 = 'Z';

	Archive archive;
	archive << input1 << input2;

	char output1, output2;
	archive >> output1 >> output2;

	TEST_EXPECT(input1, output1);
	TEST_EXPECT(input2, output2);
}

TESTCASE(SERIALIZER_FUNDAMENTAL_NULLPTR)
{
	std::nullptr_t input;

	Archive archive;
	archive << input;

	std::nullptr_t output;
	archive >> output;

	assert(input == output);
}

TESTCASE(SERIALIZER_FUNDAMENTAL_REFERENCE)
{
	int input = std::numeric_limits<int>::max();
	int& refInput = input;

	Archive archive;
	archive << refInput;

	int output;
	int& refOutput = output;
	archive >> refOutput;

	TEST_EXPECT(input, output);
	TEST_EXPECT(refInput, refOutput);
}

struct Object : public Archival {
	void pack(Archive& archive) const override
	{
		archive << a << b << c;
	}

	void unpack(Archive& archive) override
	{
		archive >> a >> b >> c;
	}

	int a = 0;
	bool b = true;
	std::string c = "str";
};

TESTCASE(ARCHIVAL)
{
	Object input;
	input.a = 100;
	input.b = false;
	input.c = "archival string";

	Archive archive;
	archive << input;

	Object output;
	archive >> output;

	TEST_EXPECT(input.a, output.a);
	TEST_EXPECT(input.b, output.b);
	TEST_EXPECT(input.c, output.c);
}

TESTCASE(STRING)
{
	std::string input = "Archive string test";

	Archive archive;
	archive << input;

	std::string output;
	archive >> output;

	TEST_EXPECT(input, output);
}

TESTCASE(UNIQUE_PTR)
{
	std::unique_ptr<bool> input(new bool(true));

	Archive archive;
	archive << input;

	std::unique_ptr<bool> output;
	archive >> output;

	TEST_EXPECT(*input, *output);
}

TESTCASE(SHARED_PTR)
{
	std::shared_ptr<std::string> input = std::make_shared<std::string>();
	*input = "Archive string test";

	Archive archive;
	archive << input;

	std::shared_ptr<std::string> output;
	archive >> output;

	TEST_EXPECT(*input, *output);
}

TESTCASE(Archive)
{
	std::string input1 = "Archive string test1";
	std::string input2 = "Archive string test2";
	std::string input3 = "Archive string test3";

	Archive archive1, archive2, archive3;
	archive1 << input1;
	archive2 << input2;
	archive3 << input3;

	archive1 << archive2;
	archive1 >> archive3;

	std::string output1;
	std::string output2;
	std::string output3;
	archive3 >> output1 >> output2 >> output3;

	TEST_EXPECT(output1, input3);
	TEST_EXPECT(output2, input1);
	TEST_EXPECT(output3, input2);
}

TESTCASE(PARAMETER_PACK)
{
	int input1 = std::numeric_limits<int>::max();
	float input2 = std::numeric_limits<float>::max();
	double input3 = std::numeric_limits<double>::max();
	std::string input4 = "Archive string test";
	std::unique_ptr<bool> input5(new bool(true));

	Archive archive;
	archive.pack(input1, input2, input3, input4, input5);

	int output1;
	float output2;
	double output3;
	std::string output4;
	std::unique_ptr<bool> output5;

	archive.unpack(output1, output2, output3, output4, output5);

	TEST_EXPECT(input1, output1);
	TEST_EXPECT(input2, output2);
	TEST_EXPECT(input3, output3);
	TEST_EXPECT(input4, output4);
	TEST_EXPECT(*input5, *output5);
}

TESTCASE(PARAMETER_PACK_EMPTY)
{
	Archive archive;
	archive.pack();

	archive.unpack();
}

TESTCASE(PARAMETER_PACK_TRANSFORM)
{
	int input1 = std::numeric_limits<int>::max();
	float input2 = std::numeric_limits<float>::max();
	double input3 = std::numeric_limits<double>::max();
	std::string input4 = "Archive string test";
	std::shared_ptr<bool> input5(new bool(true));

	Archive archive;
	archive.pack(input1, input2, input3, input4, input5);

	int output1;
	float output2;
	double output3;
	std::string raw;
	std::string& output4 = raw;
	std::shared_ptr<bool> output5;

	auto tuple = std::make_tuple(output1, output2, output3, output4, output5);
	archive.transform(tuple);

	TEST_EXPECT(input1, std::get<0>(tuple));
	TEST_EXPECT(input2, std::get<1>(tuple));
	TEST_EXPECT(input3, std::get<2>(tuple));
	TEST_EXPECT(input4, std::get<3>(tuple));
	TEST_EXPECT(*input5, *(std::get<4>(tuple)));
}

TESTCASE(PARAMETER_PACK_TRANSFORM_EMPTY)
{
	auto tuple = std::make_tuple();
	Archive archive;
	archive.transform(tuple);
}
