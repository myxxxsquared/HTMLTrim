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
*  html_css.h
*  声明CSS中的操作，用于读入和装换CSS。
*
****************************************************/

#ifndef HTML_CSS_HEADER
#define HTML_CSS_HEADER

//! 表示一个CSS属性项
class html_css_attrs
	: public std::vector<std::pair<std::string, std::string>>
{
public:
	//!读入CSS属性项
	void Load(const std::string& strcss);
	
	//!将属性项目转换为CSS文本
	std::string Save();
};

//! 表示一个CSS样式表
class html_class_table
	: public std::vector<std::pair<std::string, html_css_attrs>>
{
public:
	//! 读入CSS样式表
	void Load(const std::string& strcss);
	
	//! 将CSS样式表装换为CSS文本
	//!@param usingNewLine 在输出时使用换行
	std::string Save(bool usingNewLine);
};



#endif