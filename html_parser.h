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
*  html_parser.h
*  声明html_parser类
*
*  声明html_parser类，表示html进行读取
*
****************************************************/

#ifndef HTML_PARSER_HEADER
#define HTML_PARSER_HEADER

#include "html_element.h"

//!html进行读取.

//!完成html文本文件与html_document类型之间的互化.
class html_parser
{
public:
	//!将文本转换为html_document
	html_document parse(const std::string& html);

	//!将html_document转换为文本
	std::string parse(const html_document& html);

	//!在给定的html_document上构建树结构
	void constructTree(html_document& html);

	//!html转换选项
	struct parser_flags
	{
		//!指示将html_document转换为文本时是否进行缩进
		bool using_indent = false;

		//!指示将html_document转换为文本时是否进行换行
		bool using_newline = false;

		//!指示在'>'之前是否使用换行
		bool using_newline_before_right = false;

	};

	//!html转换选项
	parser_flags parserFlags;
};

#endif