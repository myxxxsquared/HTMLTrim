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
*  html_preprocesser.h
*  声明html_preprocesser类
*
*  声明html_preprocesser类，表示html预处理
*
****************************************************/

#ifndef HTML_PREPROCESS
#define HTML_PREPROCESS

//!html预处理

//!对html文档中的if进行预处理识别，如&lt;!--[if xxx]&gt;&lt;[endif]--&gt;
class html_preprocesser
{
public:
	//!浏览器环境虚基类

	//!即当前浏览器支持何种功能，如何进行预处理
	class browser_envirnoment
	{
	public:
		//!检查给定内容是否支持

		//!@param str 待检查内容的字段
		virtual bool check_if(const std::string& str) const = 0;
	};

	//!默认浏览器环境

	//!认为当前浏览器不支持任何功能
	class default_browser_envirnoment : public browser_envirnoment
	{
	public:
		//!检查给定内容是否支持

		//!认为当前浏览器不支持任何功能
		//!@param str 待检查内容的字段
		virtual bool check_if(const std::string& str) const;
	};

	//!对文件进行预处理

	//!@param contents 文件内容
	//!@param e 浏览器环境
	//!@return 处理后的内容
	static std::string preprocess_if_mark(const std::string& contents, const browser_envirnoment& e);
};

#endif