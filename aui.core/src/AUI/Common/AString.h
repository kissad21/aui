/**
 * =====================================================================================================================
 * Copyright (c) 2021 Alex2772
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 * Original code located at https://github.com/aui-framework/aui
 * =====================================================================================================================
 */

#pragma once

#include <string>
#include <iostream>
#include "AUI/Core.h"
#include <AUI/Common/ASet.h>

class API_AUI_CORE AStringVector;
class API_AUI_CORE AByteBuffer;

class API_AUI_CORE AString: std::wstring
{
private:
	friend struct std::hash<AString>;
public:
	
	using iterator = std::wstring::iterator;
	using const_iterator = std::wstring::const_iterator;
	using reverse_iterator = std::wstring::reverse_iterator;
	using const_reverse_iterator = std::wstring::const_reverse_iterator;
	auto constexpr static NPOS = std::wstring::npos;


	AString(AString&& other) noexcept
		: std::wstring(static_cast<basic_string&&>(other))
	{
	}
	
	AString(const basic_string& other) noexcept
		: basic_string<wchar_t>(other)
	{
	}
	AString(const std::string& str) noexcept;

	AString(const AString& other) noexcept
		: std::wstring(other.c_str())
	{
	}

	AString(const basic_string& _Right, const std::allocator<wchar_t>& _Al) noexcept
		: basic_string<wchar_t>(_Right, _Al)
	{
	}

	template <class Iterator>
	AString(Iterator first, Iterator last) noexcept : std::wstring(first, last) {}
	
	AString() noexcept
	{
	}

	AString(wchar_t c) noexcept : std::wstring(&c, &c + 1)
	{
		
	}

	AString(const char* str) noexcept;

	explicit AString(const std::allocator<wchar_t>& _Al) noexcept
		: basic_string<wchar_t>(_Al)
	{
	}

	AString(const basic_string& _Right, size_type _Roff, const std::allocator<wchar_t>& _Al) noexcept
		: basic_string<wchar_t>(_Right, _Roff, _Al)
	{
	}

	AString(const basic_string& _Right, size_type _Roff, size_type _Count, const std::allocator<wchar_t>& _Al) noexcept
		: basic_string<wchar_t>(_Right, _Roff, _Count, _Al)
	{
	}

	AString(const wchar_t* _Ptr, size_type _Count) noexcept
		: basic_string<wchar_t>(_Ptr, _Count)
	{
	}

	AString(const wchar_t* _Ptr, size_type _Count, const std::allocator<wchar_t>& _Al) noexcept
		: basic_string<wchar_t>(_Ptr, _Count, _Al)
	{
	}

	AString(const wchar_t* _Ptr) noexcept
		: basic_string<wchar_t>(_Ptr)
	{
	}

	AString(const wchar_t* _Ptr, const std::allocator<wchar_t>& _Al) noexcept
		: basic_string<wchar_t>(_Ptr, _Al)
	{
	}

	AString(size_type _Count, wchar_t _Ch) noexcept
		: basic_string<wchar_t>(_Count, _Ch)
	{
	}

	AString(size_type _Count, wchar_t _Ch, const std::allocator<wchar_t>& _Al) noexcept
		: basic_string<wchar_t>(_Count, _Ch, _Al)
	{
	}

	AString(basic_string&& _Right) noexcept
		: basic_string<wchar_t>(_Right)
	{
	}

	AString(basic_string&& _Right, const std::allocator<wchar_t>& _Al) noexcept
		: basic_string<wchar_t>(_Right, _Al)
	{
	}

	AString(std::initializer_list<wchar_t> _Ilist) noexcept
		: basic_string<wchar_t>(_Ilist)
	{
	}

	virtual ~AString() = default;


	void push_back(wchar_t c) noexcept
	{
		std::wstring::push_back(c);
	}
	void pop_back() noexcept
	{
		std::wstring::pop_back();
	}

    AString uppercase() const;
    AString lowercase() const;

	bool startsWith(const AString& other) const noexcept
	{
		return rfind(other, 0) == 0;
	}
	bool startsWith(wchar_t c) const noexcept
	{
		return rfind(c, 0) == 0;
	}
	bool endsWith(const AString& other) const noexcept
	{
		if (length() < other.length())
		{
			return false;
		}
		size_t offset = length() - other.length();
		return std::wstring::find(other, offset) == offset;
	}
	bool endsWith(wchar_t c) const noexcept
	{
		size_t offset = length() - 1;
		return std::wstring::find(c, offset) == offset;
	}

	AStringVector split(wchar_t c) const noexcept;

	AString mid(size_t pos, size_t count = npos) const noexcept
	{
		return substr(pos, count);
	}

	size_type find(char c, size_type offset = 0) const noexcept
	{
		return std::wstring::find(c, offset);
	}
	size_type find(wchar_t c, size_type offset = 0) const noexcept
	{
		return std::wstring::find(c, offset);
	}
	size_type find(const AString& str, size_type offset = 0) const noexcept
	{
		return std::wstring::find(str, offset);
	}
	size_type rfind(char c, size_type offset = NPOS) const noexcept
	{
		return std::wstring::rfind(c, offset);
	}
	size_type rfind(wchar_t c, size_type offset = NPOS) const noexcept
	{
		return std::wstring::rfind(c, offset);
	}
	size_type rfind(const AString& str, size_type offset = NPOS) const noexcept
	{
		return std::wstring::rfind(str, offset);
	}
	size_type length() const noexcept
	{
		return std::wstring::length();
	}
	AString trimLeft(wchar_t symbol = ' ') const noexcept;
	AString trimRight(wchar_t symbol = ' ') const noexcept;

	AString trim(wchar_t symbol = ' ') const noexcept
	{
		return trimRight(symbol).trimLeft(symbol);
	}

	void reserve(size_t s)
	{
		std::wstring::reserve(s);
	}
	void resize(size_t s)
	{
		std::wstring::resize(s);
	}

	AString restrictLength(size_t s, const AString& stringAtEnd = "...") const;

	wchar_t* data() noexcept
	{
		return std::wstring::data();
	}
	
	const wchar_t* data() const noexcept
	{
		return std::wstring::data();
	}
	[[nodiscard]] AString replacedAll(const AString& from, const AString& to) const noexcept;
    [[nodiscard]] inline AString replacedAll(wchar_t from, wchar_t to) const noexcept {
	    AString copy;
	    copy.reserve(length() + 10);
	    for (auto c : *this) {
	        if (c == from) {
	            copy << to;
	        } else {
	            copy << c;
	        }
	    }
        return copy;
	}
    [[nodiscard]] inline AString replacedAll(const ASet<wchar_t> from, wchar_t to) const noexcept {
	    AString copy;
	    copy.reserve(length() + 10);
	    for (auto c : *this) {
	        if (from.contains(c)) {
	            copy << to;
	        } else {
	            copy << c;
	        }
	    }
        return copy;
	}
	void replaceAll(wchar_t from, wchar_t to) noexcept;

	[[nodiscard]]
	float toFloat() const noexcept;

	[[nodiscard]]
	double toDouble() const noexcept;

	[[nodiscard]]
	int toInt() const noexcept;

    [[nodiscard]]
	unsigned toUInt() const noexcept;

	[[nodiscard]]
	bool toBool() const noexcept;

	[[nodiscard]]
	bool contains(wchar_t c) const noexcept
	{
		return find(c) != npos;
	}
	[[nodiscard]]
	bool contains(const AString& other) const noexcept
	{
		return find(other) != npos;
	}

	static AString fromLatin1(const AByteBuffer& buffer);
	static AString fromUtf8(const AByteBuffer& buffer);
	static AString fromUtf8(const char* buffer, size_t length);
	static AString fromLatin1(const char* buffer);

	static AString numberHex(int i) noexcept;

    template<typename T, std::enable_if_t<std::is_integral_v<std::decay_t<T>> || std::is_floating_point_v<std::decay_t<T>>, int> = 0>
	static AString number(T i) noexcept {
	    if constexpr (std::is_same_v<bool, std::decay_t<T>>) {
            if (i)
                return "true";
            return "false";
	    } else {
            return std::to_wstring(i);
        }
	}
	int toNumberDec() const noexcept;
	int toNumberHex() const noexcept;

    /**
     * @return utf8-encoded std::string.
     */
	std::string toStdString() const noexcept;

    void resizeToNullTerminator();

	iterator erase(const_iterator begin, const_iterator end) noexcept
	{
		return std::wstring::erase(begin, end);
	}
	iterator erase(const_iterator begin) noexcept
	{
		return std::wstring::erase(begin);
	}

	AString& erase(size_type offset) noexcept
	{
		std::wstring::erase(offset);
		return *this;
	}
	AString& erase(size_type offset, size_type count) noexcept
	{
		std::wstring::erase(offset, count);
		return *this;
	}

	AByteBuffer toUtf8() const noexcept;

	void removeAt(unsigned index) noexcept
	{
		erase(begin() + index);
	}
	AString excessSpacesRemoved() const noexcept;

	iterator insert(size_type at, wchar_t c) noexcept
	{
		return std::wstring::insert(begin() + at, 1, c);
	}
	iterator insert(size_type at, const AString& c) noexcept
	{
		return std::wstring::insert(begin() + at, c.begin(), c.end());
	}
	
	template<typename Iterator>
	iterator insert(const_iterator at, Iterator begin, Iterator end) noexcept
	{
		return std::wstring::insert(at, begin, end);
	}
	
	AString& operator<<(char c) noexcept
	{
		append(1, c);
		return *this;
	}
	AString& operator<<(wchar_t c) noexcept
	{
		append(1, c);
		return *this;
	}
	
	inline ::AString& operator+=(const AString& str) noexcept
	{
		append(str);
		return *this;
	}
	inline ::AString& operator+=(const char* str) noexcept
	{
		*this += AString(str);
		return *this;
	}

	[[nodiscard]] bool empty() const noexcept {
		return std::wstring::empty();
	}
	[[nodiscard]] size_type size() const noexcept {
		return std::wstring::size();
	}
	wchar_t operator[](size_type index) const
	{
		return std::wstring::at(index);
	}
	wchar_t& operator[](size_type index)
	{
		return std::wstring::at(index);
	}
	bool operator<(const AString& other) const noexcept
	{
		return compare(other) < 0;
	}

	void clear() noexcept
	{
		std::wstring::clear();
	}

	wchar_t& front() noexcept
	{
		return std::wstring::front();
	}
	wchar_t& back() noexcept
	{
		return std::wstring::back();
	}
	const wchar_t& front() const noexcept
	{
		return std::wstring::front();
	}
	const wchar_t& back() const noexcept
	{
		return std::wstring::back();
	}
	wchar_t& first() noexcept
	{
		return std::wstring::front();
	}
	wchar_t& last() noexcept
	{
		return std::wstring::back();
	}
	const wchar_t& first() const noexcept
	{
		return std::wstring::front();
	}
	const wchar_t& last() const noexcept
	{
		return std::wstring::back();
	}

	const wchar_t* c_str() const
	{
		return std::wstring::c_str();
	}

	iterator begin() noexcept
	{
		return std::wstring::begin();
	}
	iterator end() noexcept
	{
		return std::wstring::end();
	}

	const_iterator begin() const noexcept
	{
		return std::wstring::begin();
	}
	const_iterator end() const noexcept
	{
		return std::wstring::end();
	}

	reverse_iterator rbegin() noexcept
	{
		return std::wstring::rbegin();
	}
	reverse_iterator rend() noexcept
	{
		return std::wstring::rend();
	}

	const_reverse_iterator rbegin() const noexcept
	{
		return std::wstring::rbegin();
	}
	const_reverse_iterator rend() const noexcept
	{
		return std::wstring::rend();
	}

	AString& append(const AString& s) noexcept
	{
		std::wstring::append(s);
		return *this;
	}

	AString& append(size_t count, wchar_t ch) noexcept
	{
		std::wstring::append(count, ch);
		return *this;
	}

	const AString& operator=(const AString& value) noexcept
	{
		std::wstring::operator=(value);
		return *this;
	}

	const AString& operator=(AString&& value) noexcept
	{
		std::wstring::operator=(value);
		return *this;
	}
	
	bool operator==(const AString& other) const noexcept
	{
		return wcscmp(c_str(), other.c_str()) == 0;
	}
	bool operator==(const wchar_t* other) const noexcept
	{
		return wcscmp(c_str(), other) == 0;
	}
	bool operator==(const char* other) const noexcept
	{
		return *this == AString(other);
	}

	bool operator!=(const AString& other) const noexcept
	{
		return wcscmp(c_str(), other.c_str()) != 0;
	}
	bool operator!=(const wchar_t* other) const noexcept
	{
		return wcscmp(c_str(), other) != 0;
	}
	bool operator!=(const char* other) const noexcept
	{
		return *this != AString(other);
	}

    template<typename... Args>
    inline AString format(Args&&... args);

    AString processEscapes() const;
};

inline AString operator+(const AString& l, const AString& r) noexcept
{
	auto x = l;
	x.append(r);
	return x;
}
inline AString operator+(const AString& l, wchar_t r) noexcept
{
	auto x = l;
	x.append(r);
	return x;
}
inline AString operator+(const AString& one, const char* other) noexcept
{
	return one + AString(other);
}

inline AString operator+(const char* other, const AString& one) noexcept {
	return AString(other) + one;
}

inline AString operator+(char lhs, const AString& cs) noexcept
{
	AString s(lhs);
	s += cs;
	return s;
}

inline AString operator"" _as(const char* str, size_t len)
{
    return {str};
}

inline std::ostream& operator<<(std::ostream& o, const AString& s)
{
	o << s.toStdString();
	return o;
}
namespace std
{
	template<>
	struct hash<AString>
	{
		size_t operator()(const AString& t) const
		{
			return hash<std::wstring>()(t);
		}
	};
}

// gtest printer for AString
inline void PrintTo(const AString& s, std::ostream* stream) {
    *stream << s.toStdString();
}
