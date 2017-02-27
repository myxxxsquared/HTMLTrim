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
*  htmltrim.cpp
*  定义HtmlTrim类
*
*  定义HtmlTrim类，用于程序主窗口
*
****************************************************/

#include "stdafx.h"
#include "htmltrim.h"
#include "html_parser.h"
#include "html_preprocess.h"
#include "html_optimize.h"
#include "dialogsettings.h"

HtmlTrim::HtmlTrim(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

HtmlTrim::~HtmlTrim()
{

}

void HtmlTrim::on_pushButtonInput_clicked()
{
	//“浏览”按钮按下，弹出一个窗口显示信息
	QString path 
		= QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("Open HTML File"), QString::fromLocal8Bit("."), QString::fromLocal8Bit("Html Files(*.htm , *.html)\nAll Files(*.*)"));
	if (!path.isEmpty())
	{
		this->ui.lineEditInput->setText(path);
	}
}

void HtmlTrim::on_pushButtonOutput_clicked()
{
	//“浏览”按钮按下，弹出一个窗口显示信息
	QString path
		= QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("Save HTML File"), QString::fromLocal8Bit("."), QString::fromLocal8Bit("Html Files(*.htm , *.html)\nAll Files(*.*)"));
	if (path.endsWith(QString::fromLocal8Bit(",")))
		path = path.mid(0, path.size() - 1);
	if (!path.isEmpty())
	{
		this->ui.lineEditOutput->setText(path);
	}
}

namespace {
	//从一个文件读入所有数据
	std::string readAll(std::ifstream & file)
	{
		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string contents(buffer.str());
		return std::move(contents);
	}
}

class MyHtmlTrimThreadCls : public QThread
{
public:
	MyHtmlTrimThreadCls(HtmlTrim* wnd)
		:mywnd(wnd) {}
	HtmlTrim* mywnd;
protected:
	void run()
	{
		mywnd->Process();
	}
};

void HtmlTrim::on_pushButtonProcess_clicked()
{
	//如果执行按钮按下，开一个新线程执行操作
	MyHtmlTrimThreadCls* c = new MyHtmlTrimThreadCls(this);
	c->start();
}

void HtmlTrim::Process()
{
	QMetaObject::invokeMethod(this, "setEnabled", Q_ARG(bool, false));

	{
		ClearMessage();
		OutputMessage(QString::fromLocal8Bit("开始优化处理"));
		OutputMessage(QString::fromLocal8Bit("正在打开文件"));
		std::ifstream ifs(this->ui.lineEditInput->text().toLocal8Bit());
		if (!ifs.is_open())
		{
			OutputMessage(QString::fromLocal8Bit("无法打开输入文件"));
			goto exit_process;
		}
		std::ofstream ofs(this->ui.lineEditOutput->text().toLocal8Bit());
		if (!ofs.is_open())
		{
			OutputMessage(QString::fromLocal8Bit("无法打开输出文件"));
			goto exit_process;
		}

		OutputMessage(QString::fromLocal8Bit("正在读入文件"));
		std::string filecontents = readAll(ifs);

		size_t originSize = filecontents.size();

		OutputMessage(QString::fromLocal8Bit("正在预处理"));
		filecontents = html_preprocesser::preprocess_if_mark(filecontents, html_preprocesser::default_browser_envirnoment());

		html_parser parser;
		parser.parserFlags.using_indent = false;
		parser.parserFlags.using_newline = false;
		parser.parserFlags.using_newline_before_right = false;

		OutputMessage(QString::fromLocal8Bit("正在第一轮优化：分析html结构"));
		auto list = parser.parse(filecontents);
		html_optimizer op;

		OutputMessage(QString::fromLocal8Bit("正在第一轮优化：优化html结构"));
		op.optimize(list);

		OutputMessage(QString::fromLocal8Bit("正在第一轮优化：生成新html"));
		filecontents = parser.parse(list);

		OutputMessage(QString::fromLocal8Bit("正在第二轮优化：分析html结构"));
		list = parser.parse(filecontents);

		OutputMessage(QString::fromLocal8Bit("正在第二轮优化：优化html结构"));
		op.optimize(list);
		parser.parserFlags = flags;
		OutputMessage(QString::fromLocal8Bit("正在第二轮优化：生成新html"));
		filecontents = parser.parse(list);

		OutputMessage(QString::fromLocal8Bit("正在输出文件"));
		ofs.write(filecontents.data(), filecontents.length());

		OutputMessage(QString::fromLocal8Bit("文件处理完成"));
		OutputMessage(QString::fromLocal8Bit("原始大小：") + QString::number(originSize)
			+ QString::fromLocal8Bit("，优化后大小：") + QString::number(filecontents.size())
			+ QString::fromLocal8Bit("，文件体积缩小为原来的") + QString::number((double)filecontents.size() / originSize * 100)
			+ QString::fromLocal8Bit("%。"));
	}

exit_process:
	QMetaObject::invokeMethod(this, "setEnabled", Q_ARG(bool, true));
}

void HtmlTrim::ClearMessage()
{
	mOutPutInfo.lock();		//锁定
	textInfo = QString();	//清空消息
	mOutPutInfo.unlock();
	QMetaObject::invokeMethod(this, "output_message");		//更新显示
}

void HtmlTrim::OutputMessage(QString str)
{
	mOutPutInfo.lock();		//锁定
	textInfo += str;
	textInfo += '\n';		//设置消息
	mOutPutInfo.unlock();
	QMetaObject::invokeMethod(this, "output_message");		//更新显示
}

void HtmlTrim::on_pushButtonSettings_clicked()
{
    DialogSettings dlg(flags);
    if(dlg.exec() == QDialog::Accepted)
		flags = dlg.flags;
}
