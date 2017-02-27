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
*  dialogsettings.cpp
*  定义DialogSettings类，用于设置对话框
*
****************************************************/

#include "stdafx.h"
#include "dialogsettings.h"
#include "ui_dialogsettings.h"

DialogSettings::DialogSettings(html_parser::parser_flags fl, QWidget *parent) :
    QDialog(parent),
	flags(fl),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);

	//根据当前设置更新对话框选项
	ui->checkBoxLAB->setChecked(fl.using_newline || fl.using_newline_before_right);
	ui->checkBoxINT->setChecked(fl.using_newline);
	ui->checkBoxIND->setChecked(fl.using_indent);
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::accept()
{
	//根据对话框选项更新设置
	if (ui->checkBoxLAB->isChecked())
	{
		if (ui->checkBoxINT->isChecked())
		{
			flags.using_newline = true;
			flags.using_newline_before_right = false;
			flags.using_indent = ui->checkBoxIND->isChecked();
		}
		else
		{
			flags.using_newline = false;
			flags.using_newline_before_right = true;
			flags.using_indent = ui->checkBoxIND->isChecked();
		}
	}
	else
	{
		flags.using_newline = false;
		flags.using_newline_before_right = false;
		flags.using_indent = false;
	}
	QDialog::accept();
}