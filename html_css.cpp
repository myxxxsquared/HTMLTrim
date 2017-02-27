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
*  html_css.cpp
*  定义CSS中的操作，用于读入和装换CSS。
*
****************************************************/

#include "stdafx.h"
#include "html_css.h"

using namespace std;
#include "__html_read_to.h"
using namespace __html_read_to;

namespace {
	//!对CSS进行预处理，去掉<!--和-->
	string css_preprocess(const std::string& strcss)
	{
		regex r(R"regex((/\*[\s\S]*?\*/)|(<!--)|(-->))regex");

		return regex_replace(strcss, r, "");
	}
}

void html_css_attrs::Load(const std::string & strcss)
{
	string str = css_preprocess(strcss);	//预处理
    auto begin = str.cbegin();				//设置当前位置
    auto end = str.cend();

	while (begin != end)
	{
		for (; begin != end && is_white_space(*begin); ++begin);	//略去空白字符
		string first = read_word_in_css(begin, end);				//读入一个字节
		for (; begin != end && is_white_space(*begin); ++begin);	//略去空白字符
		if (!first.size())		//如果什么都没有，继续循环
			continue;
		if (begin == end || begin!=end && *begin == ';')	//如果当前读入结束，但遇到结尾
		{
			if(begin!=end)
				++begin;						//读入当前内容，只有名称而无属性值
			if(first.size())
				push_back(pair<string, string>(first, string()));
		}
		else
		{
			++begin;
			for (; begin != end && is_white_space(*begin); ++begin);	//略去空白字符
			string second = read_word_in_css(begin, end);	//读入属性值
			if (first.size())
				push_back(pair<string, string>(first, second));	//将属性值保存
			for (; begin != end && is_white_space(*begin); ++begin);	//略去空白字符
			if (begin != end && *begin == ';')		//略去多余字符
			{
				++begin;
			}
			else
			{
				read_to_next_ch<false>(begin, end, ';');
			}
		}
	}
}

std::string html_css_attrs::Save()
{
	ostringstream result;		//用于保存结果

	for (auto& item : *this)	//每个属性值均输出一个来表示
		result << item.first << ':' << item.second << ';';
	return result.str();
}

void html_class_table::Load(const std::string & strcss)
{
	string str = css_preprocess(strcss);	//预处理

    auto begin = str.cbegin();		//标记开始和结尾
    auto end = str.cend();

	for (; begin != end && is_white_space(*begin); ++begin);	//略去空白字符
	
	while (begin != end)
	{
		//如果找到这些字符，略去
		if (*begin == '<' || *begin == '>' || *begin == '!' || *begin == '-')
		{
			++begin;
			continue;
		}

		string str_name = read_word_in_css_table(begin, end);	//读入名称
		for (; begin != end && is_white_space(*begin); ++begin);	//略去空白字符
		if (begin == end)	//如果已经结束
			break;
		if (*begin == '{')	//如果接下来是属性表，继续读入
		{
			++begin;
			for (; begin != end && is_white_space(*begin); ++begin);	//略去空白字符
			string str_content = read_to_next_ch_no_end(begin, end, '}');	//找到表结尾
			for (; begin != end && is_white_space(*begin); ++begin);	//略去空白字符
			if (begin != end && *begin == '}')
				++begin;

			html_css_attrs attrs;		//读入属性值
			attrs.Load(str_content);
			push_back(pair<string, html_css_attrs>(move(str_name), move(attrs)));
		}
	}
}

std::string html_class_table::Save(bool usingNewLine)
{
	ostringstream result;	//用于保存结果

	for (auto& item : *this)	//逐项输出
		result << item.first << '{' << item.second.Save() << (usingNewLine ? "}\n" : "}");

	return result.str();
}
