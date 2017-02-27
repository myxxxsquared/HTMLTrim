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
*  main.cpp
*  定义main函数，即程序入口函数
*
****************************************************/

#include "stdafx.h"
#include "htmltrim.h"

//! 程序入口函数
int main(int , char *[])
{
	//启动QT程序
	int agc = 0;
	char *ch = nullptr;
	QApplication a(agc, &ch);
	HtmlTrim w;
	w.show();
	return a.exec();
}
