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

#include <memory>
#include <limits>
#include <string>
#include <cassert>

#include <gtest/gtest.h>

using namespace rmi::stream;

TEST(STREAM, SERIALIZER_FUNDAMENTAL_INT)
{
	int input1 = std::numeric_limits<int>::lowest();
	int input2 = std::numeric_limits<int>::max();

	Archive archive;
	archive << input1 << input2;

	int output1, output2;
	archive >> output1 >> output2;

	EXPECT_EQ(input1, output1);
	EXPECT_EQ(input2, output2);
}

TEST(STREAM, SERIALIZER_FUNDAMENTAL_FLOAT)
{
	float input1 = std::numeric_limits<float>::lowest();
	float input2 = std::numeric_limits<float>::max();

	Archive archive;
	archive << input1 << input2;

	float output1, output2;
	archive >> output1 >> output2;

	EXPECT_EQ(input1, output1);
	EXPECT_EQ(input2, output2);
}

TEST(STREAM, SERIALIZER_FUNDAMENTAL_DOUBLE)
{
	double input1 = std::numeric_limits<double>::lowest();
	double input2 = std::numeric_limits<double>::max();

	Archive archive;
	archive << input1 << input2;

	double output1, output2;
	archive >> output1 >> output2;

	EXPECT_EQ(input1, output1);
	EXPECT_EQ(input2, output2);
}

TEST(STREAM, SERIALIZER_FUNDAMENTAL_LONG_LONG_INT)
{
	long long int input1 = std::numeric_limits<long long int>::lowest();
	long long int input2 = std::numeric_limits<long long int>::max();

	Archive archive;
	archive << input1 << input2;

	long long int output1, output2;
	archive >> output1 >> output2;

	EXPECT_EQ(input1, output1);
	EXPECT_EQ(input2, output2);
}

TEST(STREAM, SERIALIZER_FUNDAMENTAL_BOOL)
{
	bool input1 = true;
	bool input2 = false;

	Archive archive;
	archive << input1 << input2;

	bool output1, output2;
	archive >> output1 >> output2;

	EXPECT_EQ(input1, output1);
	EXPECT_EQ(input2, output2);
}

TEST(STREAM, SERIALIZER_FUNDAMENTAL_CHAR)
{
	char input1 = 'a';
	char input2 = 'Z';

	Archive archive;
	archive << input1 << input2;

	char output1, output2;
	archive >> output1 >> output2;

	EXPECT_EQ(input1, output1);
	EXPECT_EQ(input2, output2);
}

TEST(STREAM, SERIALIZER_FUNDAMENTAL_NULLPTR)
{
	std::nullptr_t input;

	Archive archive;
	archive << input;

	std::nullptr_t output;
	archive >> output;

	assert(input == output);
}

TEST(STREAM, SERIALIZER_FUNDAMENTAL_REFERENCE)
{
	int input = std::numeric_limits<int>::max();
	int& refInput = input;

	Archive archive;
	archive << refInput;

	int output;
	int& refOutput = output;
	archive >> refOutput;

	EXPECT_EQ(input, output);
	EXPECT_EQ(refInput, refOutput);
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

TEST(STREAM, ARCHIVAL)
{
	Object input;
	input.a = 100;
	input.b = false;
	input.c = "archival string";

	Archive archive;
	archive << input;

	Object output;
	archive >> output;

	EXPECT_EQ(input.a, output.a);
	EXPECT_EQ(input.b, output.b);
	EXPECT_EQ(input.c, output.c);
}

TEST(STREAM, STRING)
{
	std::string input = "Archive string test";

	Archive archive;
	archive << input;

	std::string output;
	archive >> output;

	EXPECT_EQ(input, output);
}

TEST(STREAM, UNIQUE_PTR)
{
	std::unique_ptr<bool> input(new bool(true));

	Archive archive;
	archive << input;

	std::unique_ptr<bool> output;
	archive >> output;

	EXPECT_EQ(*input, *output);
}

TEST(STREAM, SHARED_PTR)
{
	std::shared_ptr<std::string> input = std::make_shared<std::string>();
	*input = "Archive string test";

	Archive archive;
	archive << input;

	std::shared_ptr<std::string> output;
	archive >> output;

	EXPECT_EQ(*input, *output);
}

TEST(STREAM, Archive)
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

	EXPECT_EQ(output1, input3);
	EXPECT_EQ(output2, input1);
	EXPECT_EQ(output3, input2);
}

TEST(STREAM, PARAMETER_PACK)
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

	EXPECT_EQ(input1, output1);
	EXPECT_EQ(input2, output2);
	EXPECT_EQ(input3, output3);
	EXPECT_EQ(input4, output4);
	EXPECT_EQ(*input5, *output5);
}

TEST(STREAM, PARAMETER_PACK_EMPTY)
{
	Archive archive;
	archive.pack();

	archive.unpack();
}

TEST(STREAM, PARAMETER_PACK_TRANSFORM)
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

	EXPECT_EQ(input1, std::get<0>(tuple));
	EXPECT_EQ(input2, std::get<1>(tuple));
	EXPECT_EQ(input3, std::get<2>(tuple));
	EXPECT_EQ(input4, std::get<3>(tuple));
	EXPECT_EQ(*input5, *(std::get<4>(tuple)));
}

TEST(STREAM, PARAMETER_PACK_TRANSFORM_EMPTY)
{
	auto tuple = std::make_tuple();
	Archive archive;
	archive.transform(tuple);
}
