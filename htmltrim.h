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
*  htmltrim.h
*  声明HtmlTrim类
*
*  声明HtmlTrim类，用于程序主窗口
*
****************************************************/

#ifndef HTMLTRIM_H
#define HTMLTRIM_H

#include "stdafx.h"
#include "ui_htmltrim.h"
#include "html_parser.h"

//! HtmlTrim 主窗口类
class HtmlTrim : public QMainWindow
{
	Q_OBJECT

public:
	HtmlTrim(QWidget *parent = 0);
	~HtmlTrim();

private:
	Ui::HtmlTrimClass ui;
	QMutex mOutPutInfo;

private slots:
	//!输入浏览按钮按下
	void on_pushButtonInput_clicked();
	
	//!输出浏览按钮按下
	void on_pushButtonOutput_clicked();
	
	//!执行按钮按下
	void on_pushButtonProcess_clicked();

	//!更新输出信息
	void output_message()
	{
		mOutPutInfo.lock();
		this->ui.textEditInfo->setText(textInfo);
		mOutPutInfo.unlock();
	}

    void on_pushButtonSettings_clicked();

private:
	//!处理HTML
	void Process();
	
	//!清除消息
	void ClearMessage();
	
	//!输出消息
	void OutputMessage(QString str);
	
	//!用于储存当前消息
	QString textInfo = QString();

	//!表示当前转换设置
	html_parser::parser_flags flags;

	friend class MyHtmlTrimThreadCls;
};

#endif // HTMLTRIM_H
