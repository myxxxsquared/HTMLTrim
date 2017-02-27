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
*  html_optimizer.cpp
*  定义html_optimizer类，用于执行html的优化
*
****************************************************/

#include "stdafx.h"

#include "html_optimize.h"
#include "html_parser.h"
#include "html_css.h"

namespace {
	//! 内部函数，用于读取标签属性中的CSS。
	html_css_attrs __load_css_in_label(const std::pair<std::string, std::string> attrs)
	{
		html_css_attrs table;
		if (attrs.second[0] == '\''
			|| attrs.second[0] == '\"')
			table.Load(attrs.second.substr(1, attrs.second.size() - 2));
		else
			table.Load(attrs.second);

		return table;
	}
}

//! 执行HTML优化
void html_optimizer::optimize(html_document& html)
{
	//第一轮循环，清理无用的属性值和标签
	for (size_t i = html.size() - 1; i != (size_t)-1; --i)	//对每一个元素，倒序循环
	{
		auto& item = html[i];

		switch (item.type)
		{
		case html_basic_element::LABEL:
		case html_basic_element::LABEL_WITHEND:
		case html_basic_element::ENDLABEL:	//如果为标签
			if (item.content == "style" && i < html.size() - 2
				&& html[i+1].type == html_basic_element::NORMAL
				&& html[i+2].type == html_basic_element::ENDLABEL
				&& html[i+2].content == "style")	//style标签，即CSS样式表
			{
				html_class_table table;
				table.Load(html[i + 1].content);	//加载样式表

				for (size_t k = table.size() - 1; k!=(size_t)-1; --k)	//对样式循环
				{
					auto& cls = table[k];
	
					//清理无用样式
					if (cls.first.find("\\:") != std::string::npos
						|| cls.first.find("@font-face") != std::string::npos
						|| cls.first.find("@list") != std::string::npos)
					{
						table.erase(table.begin() + k);
						continue;
					}

					//对样式内的属性循环，清理无用属性
					auto& attrs = cls.second;
					for (size_t j = attrs.size() - 1; j != (size_t)-1; --j)
						if (attrs[j].first.find("mso-") != std::string::npos)
							attrs.erase(attrs.begin() + j);
				}
				html[i + 1].content = table.Save(false);
			}
			
			//如果遇到meta以及link，判断是否可以清理
			if (item.content == "meta" || item.content == "link")
			{
				bool check = false;
				for (auto jtem : item.attributes)
				{
					//清理无用的内容。
					if (jtem.first == "name" || jtem.first == "rel")
					{
						if (jtem.second == "ProgId"
							|| jtem.second == "Generator"
							|| jtem.second == "Originator"
							|| jtem.second == "File-List"
							|| jtem.second == "Edit-Time-Data"
							|| jtem.second == "OLE-Object-Data"
							|| jtem.second == "dataStoreItem"
							|| jtem.second == "themeData"
							|| jtem.second == "colorSchemeMapping")
						{
							check = true;
							break;
						}
					}
				}
				if (check)
				{
					html.erase(html.begin() + i);
				}
			}

			//标签中每一个属性，判断是否可以清理
			for (size_t j = item.attributes.size() - 1; j!=(size_t)-1; --j)
			{
				auto & attrs = item.attributes[j];

				if (attrs.first.find(":") != std::string::npos
					|| attrs.first.find("xmlns") != std::string::npos
					|| attrs.first.find("lang") != std::string::npos)
				{
					item.attributes.erase(item.attributes.begin() + j);
					continue;
				}

				//清理标签中内嵌的样式表
				if (attrs.first == "style")
				{
					html_css_attrs table = std::move(__load_css_in_label(attrs));
					
					char ch = 0;
					if (attrs.second[0] == '\'')
						ch = '\'';
					if (attrs.second[0] == '\"')
						ch = '\"';

					//清理无用内容
					for (size_t j = table.size() - 1; j != (size_t)-1; --j)
						if (table[j].first.find("mso-") != std::string::npos)
							table.erase(table.begin() + j);
					attrs.second = ch ? ch + table.Save() + ch : table.Save();
					continue;
				}
			}

			//判断标签是否可以清理
			if (item.content.find(":") != std::string::npos)
			{
				html.erase(html.begin() + i);
				continue;
			}
			break;
        default:
            break;
		}
	}

	html_parser parser;
	parser.constructTree(html);
	
	//第二轮循环，无格式span擦除
	for(auto i = html.begin(); i != html.end(); ++i)
	{
		auto &item = *i;
		if (item.type == html_basic_element::element_type::LABEL
			&& item.content == "span")
		{
			bool todel = true;
	
			//接下来的循环判断item中是否有格式信息
			for (auto jtem : item.attributes)
			{
				if (jtem.first != "style")	//有非style属性
				{
					todel = false;
					break;
				}
				if (jtem.first == "style")	//有style属性且非空
				{
					decltype(item.attributes.begin()) j;
					html_css_attrs table = std::move(__load_css_in_label(jtem));
					if (table.size() != 0)
					{
						todel = false;
						break;
					}
				}
			}

			//如果被标记清理，则删除该项目
			if (todel)
			{
				if (item.is_match)
				{
					html.erase(html.begin() + item.match_index);
				}

				i = html.erase(i);
				--i;
				parser.constructTree(html);
			}
		}
	}

	//第三轮循环，嵌套span合并
	for (auto i = html.begin(); i != html.end() - 1; ++i)
	{
		auto& item = *i;
		auto& item2 = *(i + 1);
		
		//判断是否为嵌套的span
		if (item.type == html_basic_element::element_type::LABEL
			&& item.content == "span"
			&& item.is_match
			&& item2.type == html_basic_element::element_type::LABEL
			&& item2.content == "span"
			&& item2.is_match
			&& item2.match_index == item.match_index - 1)
		{
			//判断是否可以合并
			bool todel = true;
			for (auto jtem : item.attributes)
			{
				if (jtem.first == "class" || jtem.first == "id")
				{
					todel = false;
					break;
				}
			}
			if (!todel)
				continue;
			for (auto jtem : item2.attributes)
			{
				if (jtem.first == "class" || jtem.first == "id")
				{
					todel = false;
					break;
				}
			}
			if (!todel)
				continue;

			//将内部的属性添加到外部
			for (auto jtem : item2.attributes)
			{
				if (jtem.first != "style")	//如果不是样式表，直接合并
					item.attributes.push_back(jtem);
				else
				{
					bool have = false;
					for (auto ktem : item.attributes)	//如果是样式表，把样式表合并
					{
						if (ktem.first == "style")
						{
							ktem.second = ktem.second + ";" + jtem.second;
							have = true;
						}
						if(!have)
							item.attributes.push_back(jtem);
					}
				}
			}

			//擦除内部样式表
			html.erase(html.begin() + item2.match_index);
			i = html.erase(i + 1);
			--i;
			--i;

			parser.constructTree(html);
		}
	}

	//第四轮循环，空span擦除
	for (auto i = html.begin(); i != html.end() - 1; ++i)
	{
		auto& item = *i;
		//判断span是否为空
		if (item.type == html_basic_element::element_type::LABEL
			&& item.content == "span"
			&& item.is_match
            && (item.match_index == (size_t)(i - html.begin()) + 1
                || item.match_index == (size_t)(i - html.begin()) + 2
				&& ((i + 1)->type == html_basic_element::element_type::LABEL
				|| (i + 1)->type == html_basic_element::element_type::ENDLABEL)
				&& (i + 1)->content == "br"))
		{
			//如果是，则擦除
			html.erase(html.begin() + item.match_index);
			i = html.erase(i);
			--i;
			parser.constructTree(html);
		}
	}

	//第五轮循环，head中空白字符擦除
	for (auto i = html.begin(); i != html.end() - 1; ++i)
	{
		auto& item = *i;
		//找到head标签，之后执行擦除
		if (item.type == html_basic_element::element_type::LABEL
			&& item.content == "head"
			&& item.is_match)
		{
            for (size_t j = item.match_index; j > (size_t)(i - html.begin()); --j)
			{
				if (html[j].type == html_basic_element::element_type::NORMAL
					&& html[j].content == " ")
				{
					html.erase(html.begin() + j);
				}
			}
			break;
		}
	}

	//第六轮循环，p之间空白字符擦除
	for (auto i = html.begin(); i != html.end() - 1; ++i)
	{
		auto& item = *i;
		if (item.type == html_basic_element::element_type::NORMAL
			&& item.content == " ")
		{
			if (i + 1 == html.end())
				break;
			auto& item2 = *(i + 1);
			//判断是否可以擦除
			if ((item2.type == html_basic_element::element_type::LABEL
				||item2.type == html_basic_element::element_type::LABEL_WITHEND)
				&& (item2.content == "p" || item2.content == "body" 
				|| item2.content == "head" || item2.content == "br"))
			{
				i = html.erase(i);
				--i;
			}
		}
	}

	parser.constructTree(html);
}
