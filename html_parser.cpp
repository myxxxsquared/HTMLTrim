/*

THIS FILE IS ENCODED IN UTF-8.
IF YOU CAN'T OPEN THIS FILE CORRECTLY, 
PLEASE RELOAD THIS FILE WITH UTF-8.

*/

/****************************************************
*  HTMLTrim
*  这是2016-2017学年上学期C++大作业的文件
*  作者：张文杰 物理学院
*
*  html_parser.cpp
*  定义html_parser类
*
*  定义html_parser类，表示html进行读取
*
****************************************************/


#include "stdafx.h"
#include "html_parser.h"


using namespace std;
#include "__html_read_to.h"
using namespace __html_read_to;

namespace {
	//内部函数，用于读取属性列表
	vector<pair<string, string>> read_attribute_in_label(string::const_iterator& begin, string::const_iterator end)
	{
		vector<pair<string, string>> result;							//读取结果

		while (begin != end)											//对每一个字符
		{
			for (; is_white_space(*begin) && begin != end; ++begin);	//略去空白字符
			if (begin == end || *begin == '/' || *begin == '>')			//判断是否结束
				break;

			string attr = read_word_in_label(begin, end);				//读取一个单词，即属性名
            std::transform(attr.begin(), attr.end(), attr.begin(), (int(*)(int))tolower);	//转换为小写
			for (; is_white_space(*begin) && begin != end; ++begin);	//略去空白字符

			if (begin != end && *begin == '=')							//判断是否还有内容
			{
				begin++;												//跳过等号
				for (; is_white_space(*begin && begin != end); ++begin);//略去空白字符
				string value = read_word_in_label(begin, end);			//读取一个单词，即属性值
				result.push_back(pair<string, string>(attr, value));	//添加属性
			}
			else
			{
				result.push_back(pair<string, string>(attr, string()));	//添加属性
			}

			if (begin == end || *begin == '/' || *begin == '>')			//判断是否结束
				break;
		}

		return move(result);
	}
}

//将文本转换为html_document
html_document html_parser::parse(const string& html)
{
	html_document result;	//用于保存结果

	auto htmlb = html.cbegin();		//网页开始迭代子
	auto htmle = html.cend();		//网页结束迭代子

	for (; htmlb != htmle;)			//对字符开始循环
	{
		string content = read_to_next_label(htmlb, htmle);	//先读入一段一般文版
		if (content.size())								//如果一般文本长度不为0，则插入内容
			result.push_back(html_basic_element(html_basic_element::NORMAL, content));

		if (htmlb != htmle)								//如果没有到达结尾
		{
			++htmlb;									//跳过当前的<
			if (*htmlb == '/')							//判断是否为结束标签
			{
				if (htmlb != htmle)
					++htmlb;

				string label = read_name_in_label(htmlb, htmle);			//读取标签内容
				for (; is_white_space(*htmlb) && htmlb != htmle; ++htmlb);	//跳过空白
				if (htmlb != htmle && *htmlb == '>')						//跳过>
					htmlb++;
				result.push_back(html_basic_element(html_basic_element::ENDLABEL, label));//插入标签到结果
			}
			else														//不是结束标签
			{
				bool with_end = false;									//标记结尾是否为/>
				string label = read_name_in_label(htmlb, htmle);		//读取标签名称
                std::transform(label.begin(), label.end(), label.begin(), (int(*)(int))tolower);	//装换为小写
				auto at = read_attribute_in_label(htmlb, htmle);		//读取属性表
				for (; is_white_space(*htmlb) && htmlb != htmle; ++htmlb);	//跳过空白字符
				if (htmlb != htmle && *htmlb == '>')					//跳过>和/>
					htmlb++;
				else if (htmlb != htmle && *htmlb == '/' && ++htmlb != htmle && *htmlb == '>')
				{
					htmlb++;
					with_end = true;
				}
				result.push_back(html_basic_element(with_end ? html_basic_element::LABEL_WITHEND : html_basic_element::LABEL, label, at));
																		//保存读取到结果
			}
		}
	}

	constructTree(result);		//构建树结构
	return move(result);		//返回结果
}

//将html_document转换为文本
string html_parser::parse(const html_document& html)
{
	ostringstream stream;						//储存结果
	for (auto it = html.begin(); it != html.end(); ++it)			//对html中每个元素
	{
		auto& item = *it;

		if(this->parserFlags.using_indent && parserFlags.using_newline)		//输出制表符缩进
			for (size_t i = 0; i < item.depth; ++i)	
				stream << '\t';

		switch (item.type)						//选取当前元素类型
		{
		case html_basic_element::NORMAL:				//一般类型，直接输出结果
			stream << item.content;
			break;
		case html_basic_element::LABEL:				//标签，输出 <名称 属性名=属性值...>
			stream << '<' << item.content;
			if (parserFlags.using_newline_before_right	//如果标记了换行，则输出换行
				&& it != html.begin()
				&& (it - 1)->type == html_basic_element::element_type::NORMAL)
			{
				stream << '\n';
				if (parserFlags.using_indent)
					for (size_t i = 0; i < item.depth; ++i)
						stream << '\t';
			}
			for (auto& jtem : item.attributes)
				if (jtem.second.size())
					stream << white_space << jtem.first << '=' << jtem.second;
				else
					stream << white_space << jtem.first;
			if (this->parserFlags.using_newline_before_right
				&& it != html.end() - 1)						//如果标记了换行，则输出换行
			{
				stream << '\n';
				for (size_t i = 0; i < (it + 1)->depth; ++i)
					stream << '\t';
			}
			stream << '>';
			break;
		case html_basic_element::LABEL_WITHEND:		//有结尾标签，输出 <名称 属性名=属性值.../>
			stream << '<' << item.content;
			if (parserFlags.using_newline_before_right			//如果标记了换行，则输出换行
				&& it != html.begin()
				&& (it - 1)->type == html_basic_element::element_type::NORMAL)
			{
				stream << '\n';
				if (parserFlags.using_indent)
					for (size_t i = 0; i < item.depth; ++i)
						stream << '\t';
			}
			for (auto& jtem : item.attributes)
				if (jtem.second.size())
					stream << white_space << jtem.first << '=' << jtem.second;
				else
					stream << white_space << jtem.first;
			if (this->parserFlags.using_newline_before_right
				&& it != html.end() - 1)
			{
				stream << '\n';
				for (size_t i = 0; i < (it + 1)->depth; ++i)		//如果标记了换行，则输出换行
					stream << '\t';
			}
			stream << "/>";
			break;
		case html_basic_element::ENDLABEL:			//结束标签，输出 <名称/>
			stream << "</" << item.content;
			if (parserFlags.using_newline_before_right
				&& it != html.begin()
				&& (it - 1)->type == html_basic_element::element_type::NORMAL)
			{
				stream << '\n';
				if (parserFlags.using_indent)
					for (size_t i = 0; i < item.depth; ++i)			//如果标记了换行，则输出换行
						stream << '\t';
			}
			if (this->parserFlags.using_newline_before_right
				&& it != html.end() - 1)
			{
				stream << '\n';
				for (size_t i = 0; i < (it + 1)->depth; ++i)			//如果标记了换行，则输出换行
					stream << '\t';
			}
			stream << ">";
			break;
		}
		if(this->parserFlags.using_newline)		//输出换行
			stream << '\n';
	}
	return stream.str();
}


//在给定的html_document上构建树结构
void html_parser::constructTree(html_document & result)
{
	for (auto& item : result)		//恢复到初始状态
	{
		item.depth = 0;
		item.is_match = false;
		item.match_index = (size_t)(-1);
	}

	vector<size_t> stack_to_input;				//栈，用于构建树结构
	for (size_t i = 0; i < result.size(); ++i)	//对于每一个元素
	{
		auto& item = result[i];

		switch (item.type)
		{
		case html_basic_element::LABEL:				//如果是标签元素，则压栈
			stack_to_input.push_back(i);
			break;
		case html_basic_element::ENDLABEL:			//如果是标签结束元素，则寻找与之匹配的元素
			for (size_t j = stack_to_input.size() - 1; j != 0; --j)
			{
				if (item.content == result[stack_to_input[j]].content)
				{								//找到匹配元素，设置匹配信息
					result[stack_to_input[j]].is_match = true;
					result[stack_to_input[j]].match_index = i;
					item.match_index = stack_to_input[j];
					item.is_match = true;
					for (; j < stack_to_input.size(); stack_to_input.pop_back());
					break;
				}
			}
			break;
        default:
            break;
		}
	}


	stack_to_input.clear();
	size_t current_depth = 0;					//当前深度
	for (size_t i = 0; i < result.size(); ++i)	//再一次循环，构建深度信息
	{
		auto& item = result[i];
		item.depth = current_depth;				//设置为当前深度

		switch (item.type)
		{
		case html_basic_element::LABEL:				//如果为标签且有对应的结束标签，则加深
			if (item.is_match)
				++current_depth;
			break;
		case html_basic_element::ENDLABEL:			//如果为结束标签且有对应的开始标签，则深度回到之前的开始标签相同
			if (item.match_index != (size_t)-1)
				current_depth = item.depth = result[item.match_index].depth;
			break;
        default:
            break;
		}
	}
}
