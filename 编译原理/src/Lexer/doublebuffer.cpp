//已将实现转移到doublebuffer.h文件中
// 
// 
//#include "Lexer/doublebuffer.h"
//
//namespace Lexer
//{
//	template<int size>
//	DoubleBuffer<size>::DoubleBuffer(std::string filepath)
//	{
//		stream.open(filepath);
//		if (!stream.is_open())
//		{
//			throw std::runtime_error("无法打开文件：" + filepath);
//		}
//		//初始时将两个缓冲区的结尾固定设置成EOF，表示缓冲区的末尾
//		buffer1[size - 1] = EOF;
//		buffer2[size - 1] = EOF;
//		//buffer2[0] = '\0';//将二号缓冲区起始位置设置成未使用的状态
//		stream.read(buffer1, size - 1);//初始缓冲区填充，文件中字符数不够就全部读入。
//		std::streamsize count = stream.gcount();
//		if (count < size - 1)//文件中字符数量不够
//		{
//			buffer1[count] = EOF;//在字符流的最后添加上EOF，表示文件的结束
//		}
//		lexemeBegin = forward = buffer1;//初始化指针的位置
//	}
//	template<int size>
//	DoubleBuffer<size>::~DoubleBuffer() noexcept {
//		if (stream) {
//			stream.close();
//		}
//	}
//	template<int size>
//	void DoubleBuffer<size>::read()
//	{
//		if (!stream)
//		{
//			return;
//		}
//		if (curID == BufferID::FIRST)//当前在第一个缓冲区
//		{
//			stream.read(buffer2, size - 1);
//			if (stream.gcount() < size - 1)//标志着文件读取结束
//			{
//				buffer2[stream.gcount()] = EOF;
//			}
//			forward = buffer2;//指针指向下一个缓冲区的起点
//			curID = BufferID::SECOND;//将ID切换
//		}
//		else//当前在第二个缓冲区
//		{
//			stream.read(buffer1, size - 1);
//			if (stream.gcount() < size - 1)
//			{
//				buffer1[stream.gcount()] = EOF;
//			}
//			forward = buffer1;
//			curID = BufferID::FIRST;
//		}
//	}
//	/// <summary>
//	/// 提取目前分析出的词法单元
//	/// </summary>
//	/// <typeparam name="size">缓冲区大小</typeparam>
//	/// <returns>词法单元</returns>
//	template<int size>
//	std::string DoubleBuffer<size>::getToken()
//	{
//		std::string token="";
//		char* curPos = lexemeBegin;
//		while (curPos != forward)
//		{
//			if (*curPos == EOF)
//			{
//				if (curID == BufferID::FIRST)
//				{
//					curPos = buffer1;
//				}
//				else
//				{
//					curPos = buffer2;
//				}
//			}
//			token << *curPos++;
//		}
//		lexemeBegin = forward;
//		return token;
//	}
//
//	/// <summary>
//	/// 获取当前字符，并将指针指向下一个字符
//	/// </summary>
//	/// <typeparam name="size">缓冲区的大小</typeparam>
//	/// <returns>当前的字符</returns>
//	template<int size>
//	char DoubleBuffer<size>::next()
//	{
//		//获取当前位置的字符
//		char c = *forward;
//		//读取到EOF，存在两种情况，
//		// 一种是达到缓冲区的结尾，需要加载下一个缓冲区；
//		// 一种是达到文件的结尾，需要结束读取
//		if (c == EOF)
//		{
//			char* curBuffer=buffer1;
//			if (curID == BufferID::SECOND)
//			{
//				curBuffer = buffer2;
//			}
//			if (forward - curBuffer == size - 1)
//			{
//				read();//到达文件末尾，加载下一个缓冲区,同步将forward指针切换到下一个缓冲区中
//			}
//			else
//			{
//				this->end = true;
//				return EOF;
//			}
//		}
//		return *forward++;
//	}
//	/// <summary>
//	/// 获取当前向前指针所在位置的字符
//	/// </summary>
//	/// <typeparam name="size">缓冲区大小</typeparam>
//	/// <returns>当前指向的字符</returns>
//	template<int size>
//	char DoubleBuffer<size>::cur()
//	{
//		return *forward;
//	}
//	/// <summary>
//	/// 指针指向的前一个字符，用于自动机的回退操作
//	/// </summary>
//	/// <typeparam name="size">缓冲区大小</typeparam>
//	/// <returns>当前字符</returns>
//	template<int size>
//	char DoubleBuffer<size>::pre()
//	{
//		char c = *forward;
//		if (forward == buffer1)//判断当前是否在第一个缓冲区的起始位置
//		{
//			if (buffer2[0] != '\0')//仅有当前已使用过二号缓冲区才回退
//			{
//				forward = buffer2[size - 1];//回退到上一个缓冲区末尾
//			}
//		}
//		else if (forward == buffer2)
//		{
//			forward == buffer1[size - 1];
//		}
//		else
//		{
//			forward--;//正常情况下的回退
//		}
//		return c;
//	}
//}