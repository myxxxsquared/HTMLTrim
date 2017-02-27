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
*  dialogsettings.h
*  声明DialogSettings类，用于设置对话框
*
****************************************************/

#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include "html_parser.h"

namespace Ui {
class DialogSettings;
}

//!设置对话框类

//!用于修改设置
class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(html_parser::parser_flags fl, QWidget *parent = 0);
    ~DialogSettings();

	//!保存当前设置
	html_parser::parser_flags flags;

public slots:
	//!更新当前设置
	virtual void accept();

private:
    Ui::DialogSettings *ui;
};

#endif // DIALOGSETTINGS_H
