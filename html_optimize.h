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

#ifndef HTML_OPTIMIZE
#define HTML_OPTIMIZE

#include "html_element.h"

//!表示对html进行优化的类
class html_optimizer
{
public:
	//!优化html文档
	void optimize(html_document& html);
};

#endif