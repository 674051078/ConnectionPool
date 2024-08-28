#pragma once
//是一个编译指令，作用是防止头文件被多次包含（即头文件的重复包含问题）。
//当编译器遇到 #pragma once 时，它会确保该文件只被包含一次，避免重复定义或编译错误。

#define LOG(str) \
				cout <<__FILE__<<":" <<__LINE__ <<" " << \
				__TIMESTAMP__ << " : " << str << endl;
/*
__FILE__：这是一个预定义的宏，表示当前文件的名称（包含该宏的文件名）。

__LINE__：这是另一个预定义的宏，表示当前行号（宏所在行的行号）。

__TIMESTAMP__：这是一个预定义的宏，表示文件被编译的时间戳，
格式为 Www Mmm dd hh:mm:ss yyyy（如 "Tue Aug 19 09:41:13 2024"）。

str：这是宏的参数，用于传入你想要输出的日志信息。

文件名：帮助你知道日志在哪个文件中输出的。
行号：帮助你知道日志对应的具体代码行。
时间戳：显示编译时的日期和时间，有助于追踪日志的生成时间。
自定义消息：你可以传递任意字符串作为日志消息，方便描述具体的情况。


实例：
int main() {
    LOG("This is a test log message");
    //其他代码
    return 0;
}
main.cpp:3 Tue Aug 19 09:41:13 2024 : This is a test log message
*/