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
*  html_preprocesser.cpp
*  实现html_preprocesser类
*
*  声明html_preprocesser类，表示html预处理
*
****************************************************/

#include "stdafx.h"
#include "html_preprocess.h"

using namespace std;

//匿名命名空间，用于静态生成
namespace
{
	//表示一个if或endif标记
	class if_mark
	{
	public:
		//开始位置
		size_t begin;

		//长度
		size_t length;

		//指示标记为if还是endif
		enum if_mark_type { BEGINIF, ENDIF } type;

		//标记内容
		string content;

		//初始化一个空
		inline if_mark() {}

		//按照参数初始化
		inline if_mark(size_t b, size_t l, if_mark_type t, const string& c)
			: begin(b), length(l), type(t), content(c) {}

		//大小比较，用于排序
		inline bool operator<(const if_mark& right)
		{
			//采用初始位置进行排序
			return begin < right.begin;
		}
	};

	//内部函数，判断某一个vector<bool>中的元素是否都是true。
	bool is_vector_all_true(const vector<bool> s)
	{
		for (size_t i = 0; i < s.size(); ++i)
			if (!s[i])
				return false;
		return true;
	}

	//从一个文件中计算if和endif标记的位置。
	vector<if_mark> getIfMakes(const string& contents)
	{
		//正则表达式，用于匹配if
		regex reg_if(R"regex(<!(?:(?:--)|(?:))\[if(.*?)\]>)regex");

		//正则表达式，用于匹配endif
		regex reg_endif(R"regex(<!\[endif\](?:(?:--)|(?:))>)regex");

		//结果列表
		vector<if_mark> if_marks;

		smatch result;						//正则表达式匹配结果
		auto sbegin = contents.cbegin();	//字符串的开始迭代子
		auto send = contents.cend();		//字符串的结束迭代子
		size_t position_cur = 0;			//标记当前位置

		while (regex_search(sbegin, send, result, reg_if)) //从文本中搜索if的正则表达式
		{
			position_cur += result.position();	//计算开始下标
			if_marks.push_back(					//将查找结果保存
				if_mark(
					position_cur,
					result.length(),
					if_mark::BEGINIF,
					result[1]));
			position_cur += result.length();	//更新下标
			sbegin = result.suffix().first;		//更新搜索位置
		}

		sbegin = contents.begin();
		position_cur = 0;

		while (regex_search(sbegin, send, result, reg_endif))	//从文本中搜索if的正则表达式
		{
			position_cur += result.position();		//计算开始下标
			if_marks.push_back(						//将查找结果保存
				if_mark(
					position_cur,
					result.length(),
					if_mark::ENDIF,
					string()));
			position_cur += result.length();		//更新下标
			sbegin = result.suffix().first;			//更新搜索位置
		}

		sort(if_marks.begin(), if_marks.end());		//由于对于if和endif分别搜索，搜索结束后进行排序，确定位置

		return move(if_marks);		//返回结果
	}
}

//检查给定内容是否支持
//认为当前浏览器不支持任何功能
//@param str 待检查内容的字段
bool html_preprocesser::default_browser_envirnoment::check_if(const string& str) const
{
	return str.find('!') != string::npos;
}

//对文件进行预处理
//@param contents 文件内容
//@param e 浏览器环境
//@return 处理后的内容
string html_preprocesser::preprocess_if_mark(const string& contents, const browser_envirnoment& e)
{
	auto if_marks = getIfMakes(contents);	//获取if和endif标记信息
	vector<bool> if_contents;				//表示当前位置是否进行输出
	ostringstream result;					//用于存放最终的字符串结果
	size_t cur_pos = 0;						//当前处理的位置

	for (auto&item : if_marks)				//对每一个if和endif分别处理
	{
		if (is_vector_all_true(if_contents))	//如果当前所有if都为真，则将内容输出
			result.write(contents.data() + cur_pos, item.begin - cur_pos);
		cur_pos = item.begin + item.length;		//移动当前处理的位置

		if (item.type == if_mark::BEGINIF)		//如果下一个标记是if，则通过environment判断是否为真，并压栈
			if_contents.push_back(e.check_if(item.content));	
		else									//如果下一个标记是endif，则弹出最后一个if
			if(if_contents.size())				//用于容错，如果弹出失败，则忽略
				if_contents.pop_back();
	}
	if (is_vector_all_true(if_contents))		//如果当前所有if都为真，则将内容输出
		result.write(contents.data() + cur_pos, contents.size() - cur_pos);

	return result.str();		//返回结果
}
