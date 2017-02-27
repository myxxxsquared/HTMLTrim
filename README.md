
_THIS FILE IS ENCODED IN UTF-8.
IF YOU CAN'T OPEN THIS FILE CORRECTLY, 
PLEASE RELOAD THIS FILE WITH UTF-8._

#说明

这是2016-2017学年上学期C++大作业，现在学期结束，开放源代码。
作者：张文杰 1500011394@pku.edu.cn

#HTMLTrim

2016-2017 学年上学期C++大作业的文件

北京大学 张文杰 物理学院 1500011394

## 体会

本次大作业编写了共8个头文件，8个源文件，2个Qt的UI文件，代码共约1500行，算是个比较小程序。

程序中难度有一些，主要集中在HTML的识别和操作，识别步骤比较长，编写时可能会有一些小错误，需要调试更正。

我没有使用现有的HTML库，因为找不到太满意的，所以自己写了识别。这一次的编写让我对于文本的处理有了更深的了解，对以后比较有帮助。

## 内容介绍

HtmlTrim用于裁剪由Word生成的文档，删去冗余信息，使得Word文档变小。

主要处理步骤如下

- 预处理：对文件中的[if]与[endif]块进行处理，抛去无用内容
- 删去冗余标签：link和meta标签中word会引入冗余标签以储存信息，可以删去这些标签。此外mso专用标签其他软件无法读取，也删去
- 删去冗余的CSS：mso-开头的css均为Office支持的CSS，浏览器并不支持，均删去。
- 删去冗余的span：经过以上处理，部分span标签中不再带有样式，可以删去。
- 嵌套的span处理：如果span标签嵌套，则将span合并，可以达到相同效果。
- 删除多余空白：在head和p之间有多余的空白字符，均可删去。

##文件夹说明
- build 由Visual Studio 2015 x86 Release、Qt 5.7.0生成的程序文件，HTMLTrim.exe，以及运行所必须的一些动态链接库。
- doc Doxgen生成的说明文档
- sample 程序执行的样例文件
- src 程序源代码以及工程文件

## 程序说明

打开程序后，选取输入文件和输出文件，再按下处理按钮，即可开始优化处理。

## 运行已生成的文件

在build文件夹下，为Microsoft Visual C++ 2015 x86 Release的生成结果，其中HtmlTrim.exe为程序二进制文件，其余为Qt库所需的动态链接库和Microsoft Visual C++ 2015所需的动态链接库。

## 生成

该项目采用C++11标准编写，请使用C++11标准编译器进行编译。

该项目使用了Qt开源库，需要使用Qt Creator或Visual Stdio Qt加载项进行编译。