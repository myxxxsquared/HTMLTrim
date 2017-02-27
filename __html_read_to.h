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
*  __html_read_to.h
*  声明一系列用于html读取的函数，在html读入时使用。
*
****************************************************/

namespace __html_read_to
{
	//!表示空格字符
	constexpr char white_space = ' ';

	//!表示一个标签的开始字符
	constexpr char begin_label = '<';

	//!表示所有空白字符
	extern const char white_spaces[5];

	//!内部函数，判断一个字符是否为空白字符
	extern bool is_white_space(char ch);

	//!内部函数，判断一个字符是否为字母
	extern bool is_abc(char ch);

	//!内部函数，用于从一个字符串中读取，直到遇到标签为止
	extern string read_to_next_label(string::const_iterator& begin, string::const_iterator end);

	//!内部函数，用于读取属性当中的一个单词
	extern string read_word_in_label(string::const_iterator& begin, string::const_iterator end);

	//!内部函数，用于读取标签名称
	extern string read_name_in_label(string::const_iterator& begin, string::const_iterator end);

	//!内部函数，用于读取属性当中的一个单词
	extern string read_word_in_css_table(string::const_iterator& begin, string::const_iterator end);

	//!内部函数，用于读取属性当中的一个单词
	extern string read_word_in_css(string::const_iterator& begin, string::const_iterator end);

    //!内部函数，读取到指定位置
	//!@param containEnd 输出结果是否包含结尾字符
	//!@param firstLoopCheck 第一个循环是否检查
	//!@param ignore_whitespace_beginend 是否跳过开头结尾的空白字符
	template<bool containEnd, bool firstLoopCheck, bool ignore_whitespace_beginend, typename IsEndFunc>
	inline string read_to(
		string::const_iterator& begin,
		string::const_iterator end,
		IsEndFunc isEnd)
	{
		ostringstream s;				//用于保存结果
		bool have_white = false;		//用于指定当前是否输出了空白字符
		bool current_begin = true;		//用于指定当前是否为开始

		if (!ignore_whitespace_beginend)
			current_begin = false;

        if (firstLoopCheck && begin != end)
			if (containEnd)
			{
				if (isEnd(begin, end))
				{
					s << *begin;
					++begin;
					goto end_of_func;
				}
			}
			else
			{
				if (isEnd(begin, end))
					goto end_of_func;
			}

		if (begin == end)
			goto end_of_func;

		while (true)	//对于每一个字符开始循环
		{
			if (is_white_space(*begin))	//如果是空白字符且没有写入空格，则写入空格
			{
				if (!have_white && !current_begin)
				{
					s << white_space;
					have_white = true;
				}
			}
			else						//如果不是空白字符则判断是否为标签开始
			{
				current_begin = false;	//清除开始位置选项
				have_white = false;		//清除已写入空白字符选项
				s << *begin;
			}

			++begin;
			if (begin == end)
				break;

			if (containEnd)
			{
				if (isEnd(begin, end))
				{
					s << *begin;
					++begin;
					break;
				}
			}
			else
			{
				if (isEnd(begin, end))
					break;
			}
		}

	end_of_func:
		
		if (ignore_whitespace_beginend)
		{
			string result = s.str();
			if (result.size() != 0 && result.back() == white_space)
				return result.substr(0, result.length() - 1);
			return s.str();					//返回结果
		}
		else
		{
			return s.str();					//返回结果
		}
	}

	inline string read_to_next_ch_no_end(string::const_iterator& begin, string::const_iterator end, char ch)
	{
		auto lambda = [ch](string::const_iterator& begin, string::const_iterator /*end*/)
		{ return *begin == ch; };
		return read_to<false, true, true, decltype(lambda)>(begin, end, lambda);
	}

    //!内部函数，读取到下一个ch指定的字符为止
    template<bool skip>
    inline string read_to_next_ch(string::const_iterator& begin, string::const_iterator end, char ch)
    {
        auto lambda = [ch](string::const_iterator& begin, string::const_iterator /*end*/)
        { return *begin == ch; };
        return read_to<true, !skip, true, decltype(lambda)>(begin, end, lambda);
    }
}
