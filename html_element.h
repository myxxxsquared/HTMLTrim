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
*  html_basic_element.h
*  声明html_basic_element类
*
*  声明html_basic_element类，表示html元素，
*  即一个标签或一段内容。
*
****************************************************/

#ifndef HTML_ELEMENT_HEADER
#define HTML_ELEMENT_HEADER

//! html元素，即一个标签或一个内容.

//! html_basic_element类可以用来表示一个标签（包括属性）或一段内容文字.
class html_basic_element
{
public:

	//!元素的内容类型.
	enum element_type 
	{
		//!一般文字内容.
		NORMAL, 
		
		//!一般标签内容.

		//!例如 &lt;table cellspacing=&quot;0&quot; cellpadding=&quot;0&quot;&gt;.
		LABEL, 

		//!结束标签内容.

		//!例如 &lt;/table&gt;.
		ENDLABEL, 
		
		//!带有结束标志的标签.

		//!例如 &lt;img src=&quot;myimg.png&quot;/&gt;.
		LABEL_WITHEND 
	};

	//!此元素的内容类型.
	element_type type;

	//!此元素的内容.

	//!类型为NORMAL时表示文字内容.
	//!类型为LABEL、ENDLABEL、LABEL_WITHEND时表示标签标题.
	std::string content;

	//!此元素包含的属性.

	//!只有在类型为LABEL、LABEL_WITHEND时使用，表示一个属性列表.
	std::vector<std::pair<std::string, std::string>> attributes;

	//!表明次元素在之后的元素当中是否有结束元素与之匹配.

	//!表示本标签时候有与之匹配的标签，只有在类型为LABEL和ENDLABEL时使用，其余时总为false.
	bool is_match = false;

	//!前一个与之匹配的元素下标.

	//!本标签的匹配位置。只有在类型为LABEL和ENDLABEL时使用，其余时总为(size_t)(-1).
	size_t match_index = (size_t)(-1);

	//!元素的层次深度.

	//!用于缩进计算，从0开始.
	size_t depth = 0;

	//!通过给定参数初始化元素
	inline html_basic_element(element_type t, std::string c, std::vector<std::pair<std::string, std::string>> a = std::vector<std::pair<std::string, std::string>>())
		: type(t), content(c), attributes(a) {}

	//!初始化一个空的元素
	inline html_basic_element() {};
};

//!表示一个html文档
class html_document
	: public std::vector<html_basic_element>
{
};

#endif