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
 * @file        archive.hxx
 * @author      Sangwan Kwon (sangwan.kwon@samsung.com)
 * @brief       Define archive interface for serializer and parameter-pack.
 * @details     1. Serializer: Serialize/deserialize below types.
 *                 (fundamental types, archival object, unique_ptr, shared_ptr)
 *              2. Parameter-pack: Pack/unpack zero or more template arguments.
 */

#pragma once

#include <string>
#include <type_traits>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iterator>
#include <memory>
#include <tuple>

#include "index-sequence.hxx"

namespace rmi {
namespace stream {

class Archival;

template<typename T>
using IsFundamental = typename std::enable_if<std::is_fundamental<T>::value, int>::type;

template<typename T>
using IsArchival = typename std::enable_if<std::is_base_of<Archival, T>::value, int>::type;

class Archive {
public:
	virtual ~Archive() = default;

	template<typename Front, typename... Rest>
	void pack(const Front& front, const Rest&... rest);
	inline void pack(void);

	template<typename Front, typename... Rest>
	void unpack(Front& front, Rest&... rest);
	inline void unpack(void);

	template<typename... Ts>
	void transform(std::tuple<Ts...>& tuple);

	template<typename T, IsFundamental<T> = 0>
	Archive& operator<<(const T& value);
	template<typename T, IsArchival<T> = 0>
	Archive& operator<<(const T& object);
	template<typename T>
	Archive& operator<<(const std::unique_ptr<T>& pointer);
	template<typename T>
	Archive& operator<<(const std::shared_ptr<T>& pointer);
	inline Archive& operator<<(const std::string& value);

	template<typename T, IsFundamental<T> = 0>
	Archive& operator>>(T& value);
	template<typename T, IsArchival<T> = 0>
	Archive& operator>>(T& object);
	template<typename T>
	Archive& operator>>(std::unique_ptr<T>& pointer);
	template<typename T>
	Archive& operator>>(std::shared_ptr<T>& pointer);
	inline Archive& operator>>(std::string& value);

	std::vector<unsigned char> buffer;

protected:
	virtual inline void save(const void* bytes, std::size_t size);
	virtual inline void load(void* bytes, std::size_t size);

private:
	template<typename T>
	void transformImpl(T& tuple, EmptySequence);
	template<typename T, std::size_t... I>
	void transformImpl(T& tuple, IndexSequence<I...>);

	std::size_t current = 0;
};

class Archival {
public:
	virtual ~Archival() = default;

	virtual void pack(Archive& archive) const = 0;
	virtual void unpack(Archive& archive) = 0;
};

template<typename Front, typename... Rest>
void Archive::pack(const Front& front, const Rest&... rest)
{
	*this << front;
	this->pack(rest...);
}

void Archive::pack(void)
{
}


template<typename Front, typename... Rest>
void Archive::unpack(Front& front, Rest&... rest)
{
	*this >> front;
	this->unpack(rest...);
}

void Archive::unpack(void)
{
}

template<typename... Ts>
void Archive::transform(std::tuple<Ts...>& tuple)
{
	constexpr auto size = std::tuple_size<std::tuple<Ts...>>::value;
	this->transformImpl(tuple, make_index_sequence<size>());
}

template<typename T>
void Archive::transformImpl(T& tuple, EmptySequence)
{
}

template<typename T, std::size_t... I>
void Archive::transformImpl(T& tuple, IndexSequence<I...>)
{
	this->unpack(std::get<I>(tuple)...);
}

template<typename T, IsFundamental<T>>
Archive& Archive::operator<<(const T& value)
{
	this->save(reinterpret_cast<const void*>(&value), sizeof(value));

	return *this;
}

template<typename T>
Archive& Archive::operator<<(const std::unique_ptr<T>& pointer)
{
	return *this << *pointer;
}

template<typename T>
Archive& Archive::operator<<(const std::shared_ptr<T>& pointer)
{
	return *this << *pointer;
}

template<typename T, IsArchival<T>>
Archive& Archive::operator<<(const T& object)
{
	object.pack(*this);

	return *this;
}

inline Archive& Archive::operator<<(const std::string& value)
{
	std::size_t size = value.size();
	this->save(reinterpret_cast<const void*>(&size), sizeof(size));

	this->save(reinterpret_cast<const void*>(value.c_str()), value.size());

	return *this;
}

template<typename T, IsFundamental<T>>
Archive& Archive::operator>>(T& value)
{
	this->load(reinterpret_cast<void*>(&value), sizeof(value));

	return *this;
}

template<typename T>
Archive& Archive::operator>>(std::unique_ptr<T>& pointer)
{
	if (pointer == nullptr)
		pointer.reset(new T);

	return *this >> *pointer;
}

template<typename T>
Archive& Archive::operator>>(std::shared_ptr<T>& pointer)
{
	if (pointer == nullptr)
		pointer.reset(new T);

	return *this >> *pointer;
}

template<typename T, IsArchival<T>>
Archive& Archive::operator>>(T& object)
{
	object.unpack(*this);

	return *this;
}

inline Archive& Archive::operator>>(std::string& value)
{
	std::size_t size;
	this->load(reinterpret_cast<void*>(&size), sizeof(size));

	value.resize(size);
	this->load(reinterpret_cast<void*>(&value.front()), size);

	return *this;
}

void Archive::save(const void* bytes, std::size_t size)
{
	auto binary = reinterpret_cast<unsigned char*>(const_cast<void*>(bytes));
	std::vector<unsigned char> next(binary, binary + size) ;

	std::copy(next.begin(), next.end(), std::back_inserter(this->buffer));
}

void Archive::load(void* bytes, std::size_t size)
{
	::memcpy(bytes, reinterpret_cast<void*>(this->buffer.data() + current), size);
	current += size;
}

} // namespace stream
} // namespace rmi
