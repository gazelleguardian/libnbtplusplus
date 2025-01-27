/*
 * libnbt++ - A library for the Minecraft Named Binary Tag format.
 * Copyright (C) 2013, 2015  ljfa-ag
 *
 * This file is part of libnbt++.
 *
 * libnbt++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libnbt++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libnbt++.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "tag_array.h"
#include "io/stream_reader.h"
#include "io/stream_writer.h"
#include <istream>

namespace nbt
{

//Slightly different between byte_array and int_array
//Reading
template<>
void tag_array<int8_t>::read_payload(io::stream_reader& reader)
{
    int32_t length;
    reader.read_num(length);
    if(length < 0)
        reader.get_istr().setstate(std::ios::failbit);
    if(!reader.get_istr())
        throw io::input_error("Error reading length of tag_byte_array");

    data.resize(length);
    reader.get_istr().read(reinterpret_cast<char*>(data.data()), length);
    if(!reader.get_istr())
        throw io::input_error("Error reading contents of tag_byte_array");
}

template<typename T>
void tag_array<T>::read_payload(io::stream_reader& reader)
{
    int32_t length;
    reader.read_num(length);
    if(length < 0)
        reader.get_istr().setstate(std::ios::failbit);
    if(!reader.get_istr())
        throw io::input_error("Error reading length of generic array tag");

    data.clear();
    data.reserve(length);
    for(T i = 0; i < length; ++i)
    {
        T val;
        reader.read_num(val);
        data.push_back(val);
    }
    if(!reader.get_istr())
        throw io::input_error("Error reading contents of generic array tag");
}

template<>
void tag_array<int64_t>::read_payload(io::stream_reader& reader)
{
    int32_t length;
    reader.read_num(length);
    if(length < 0)
        reader.get_istr().setstate(std::ios::failbit);
    if(!reader.get_istr())
        throw io::input_error("Error reading length of tag_long_array");

    data.clear();
    data.reserve(length);
    for(int32_t i = 0; i < length; ++i)
    {
        int64_t val;
        reader.read_num(val);
        data.push_back(val);
    }
    if(!reader.get_istr())
        throw io::input_error("Error reading contents of tag_long_array");
}

//Writing
template<>
void tag_array<int8_t>::write_payload(io::stream_writer& writer) const
{
    if(size() > io::stream_writer::max_array_len)
    {
        writer.get_ostr().setstate(std::ios::failbit);
        throw std::length_error("Byte array is too large for NBT");
    }
    writer.write_num(static_cast<int32_t>(size()));
    writer.get_ostr().write(reinterpret_cast<const char*>(data.data()), data.size());
}

template<typename T>
void tag_array<T>::write_payload(io::stream_writer& writer) const
{
    if(size() > io::stream_writer::max_array_len)
    {
        writer.get_ostr().setstate(std::ios::failbit);
        throw std::length_error("Generic array is too large for NBT");
    }
    writer.write_num(static_cast<int32_t>(size()));
    for(T i: data)
        writer.write_num(i);
}

template<>
void tag_array<int64_t>::write_payload(io::stream_writer& writer) const
{
    if(size() > io::stream_writer::max_array_len)
    {
        writer.get_ostr().setstate(std::ios::failbit);
        throw std::length_error("Long array is too large for NBT");
    }
    writer.write_num(static_cast<int32_t>(size()));
    for(int64_t i: data)
        writer.write_num(i);
}

//Explicit instantiations
template class NBT_EXPORT tag_array<int8_t>;
template class NBT_EXPORT tag_array<int32_t>;
template class NBT_EXPORT tag_array<int64_t>;

}
