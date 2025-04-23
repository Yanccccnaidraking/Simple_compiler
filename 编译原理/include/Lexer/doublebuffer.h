#pragma once
#include<string>
#include<fstream>
namespace Lexer
{
	/// <summary>
	/// 实现词法分析时所使用到的双缓冲区，和双指针
	/// </summary>
	template <int size = 4096>//设置默认的缓冲区大小为1024
	class DoubleBuffer
	{
	private:
		/// <summary>
		/// 缓冲区的编号
		/// </summary>
		enum BufferID 
		{
			FIRST=0,SECOND
		};
		std::string filepath; // 文件路径
		std::ifstream stream; // 文件流
		BufferID curID = BufferID::FIRST;//当前的缓冲区ID
		char buffer1[size];//一号缓冲区
		char buffer2[size];//二号缓冲区
		buffer2[0] = '\0';//将二号缓冲区起始位置设置成未使用的状态
		char* lexemeBegin = nullptr;//起始指针，指向当前词法单元的起始位置
		char* forward = nullptr;//向前指针，指向当前正在分析的字符的位置
		void read();//从文件中读取数据到缓冲区
	public:
		bool end = false;
		explicit DoubleBuffer(std::string filepath);
		~DoubleBuffer() noexcept;
		std::string getToken();//获取当前分析出来正确的Token
		char next();//forward向前移动，指向下一个字符，返回当前字符
		char cur();//获取当前字符
		char pre();//获取当前字符，指针向后移动
	};
	
}