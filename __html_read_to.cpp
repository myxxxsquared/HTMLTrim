/*

THIS FILE IS ENCODED IN UTF-8.
IF YOU CAN'T OPEN THIS FILE CORRECTLY, 
PLEASE RELOAD THIS FILE WITH UTF-8.

*/

/****************************************************
*  HTMLTrim
*  这是2016-2017学年上学期C++大作业的文件
*  作者：张文杰 物理学院 1500011394
*
*  __html_read_to.cpp
*  定义一系列用于html读取的函数，在html读入时使用。
*
****************************************************/

#include "stdafx.h"

using namespace std;
#include "__html_read_to.h"

namespace __html_read_to
{
	//! 空白字符列表
	const char white_spaces[5] = { ' ', '\t', '\r', '\n', 0 };

	//! 判断是否为空白字符
	bool is_white_space(char ch)
	{
		for (const char* item = white_spaces; *item; ++item)
			if (ch == *item)
				return true;
		return false;
	}

	//!内部函数，判断一个字符是否为字母
	bool is_abc(char ch)
	{
		return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z';
	}

	//!内部函数，用于从一个字符串中读取，直到遇到标签为止
	string read_to_next_label(string::const_iterator& begin, string::const_iterator end)
	{
		const auto& lambda =
			[](string::const_iterator& begin, string::const_iterator end) -> bool
		{
			return *begin == begin_label
				&& begin + 1 != end
				&& (is_abc(*(begin + 1)) || *(begin + 1) == '/');
		};
		return read_to<false, true, false, decltype(lambda)>(begin, end, lambda);
	}

	//!内部函数，用于读取属性当中的一个单词
    string read_word_in_css(string::const_iterator& begin, string::const_iterator end)
	{
        if (*begin == '\'')								//如果是单引号或双引号开头，则使用相应函数搜索
			return read_to_next_ch<true>(begin, end, '\'');
		if (*begin == '\"')
			return read_to_next_ch<true>(begin, end, '\"');

        auto lambda = [](string::const_iterator& begin, string::const_iterator /*end*/)
		{return *begin == ';' || *begin == ':'; };
		return read_to<false, true, true, decltype(lambda)>(begin, end, lambda);
	}

	//!内部函数，用于读取属性当中的一个单词
	string read_word_in_css_table(string::const_iterator& begin, string::const_iterator end)
	{
        if (*begin == '\'')								//如果是单引号或双引号开头，则使用相应函数搜索
			return read_to_next_ch<true>(begin, end, '\'');
		if (*begin == '\"')
			return read_to_next_ch<true>(begin, end, '\"');

        auto lambda = [](string::const_iterator& begin, string::const_iterator /*end*/)
		{return *begin == '{' || *begin == '}'; };
		return read_to<false, true, true, decltype(lambda)>(begin, end, lambda);
	}

	//!内部函数，用于读取标签名称
	string read_name_in_label(string::const_iterator& begin, string::const_iterator end)
	{
        auto lambda = [](string::const_iterator& begin, string::const_iterator /*end*/)
		{ return is_white_space(*begin) || *begin == '>'; };
		return read_to<false, true, true, decltype(lambda)>(begin, end, lambda);
	}

	//!内部函数，用于读取属性当中的一个单词
	string read_word_in_label(string::const_iterator& begin, string::const_iterator end)
	{
		auto origin_begin = begin;						//保存开始的起始位置

		if (*begin == '\'')								//如果是单引号或双引号开头，则使用相应函数搜索
			return read_to_next_ch<true>(begin, end, '\'');
		if (*begin == '\"')
			return read_to_next_ch<true>(begin, end, '\"');

		for (; begin != end; ++begin)					//如果遇到end或>或=或/，结束循环
			if (is_white_space(*begin) || *begin == '>' || *begin == '=' || *begin == '/')
				break;
		return string(origin_begin, begin);				//返回读到的内容
	}
}
