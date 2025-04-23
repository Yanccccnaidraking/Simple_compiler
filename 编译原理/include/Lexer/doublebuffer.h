#pragma once
#include <string>
#include <fstream>
#include <stdexcept>

namespace Lexer
{
    /// <summary>
    /// 实现词法分析时所使用到的双缓冲区，和双指针
    /// </summary>
    template <int size = 4096>
    class DoubleBuffer
    {
    private:
        enum BufferID {
            FIRST = 0, SECOND
        };
        std::string filepath;
        std::ifstream stream;
        BufferID curID = BufferID::FIRST;
        char buffer1[size];
        char buffer2[size];
        char* lexemeBegin = nullptr;
        char* forward = nullptr;
        bool isRollBack = false;
        void read()
        {
            if (!stream)
            {
                return;
            }
            if (isRollBack)
            {
                if (curID == BufferID::FIRST)//当前在第一个缓冲区
                {
                    forward = buffer2;//指针指向下一个缓冲区的起点
                    curID = BufferID::SECOND;//将ID切换
                }
                else//当前在第二个缓冲区
                {
                    forward = buffer1;
                    curID = BufferID::FIRST;
                }
            }
            else
            {
                if (curID == BufferID::FIRST)//当前在第一个缓冲区
                {
                    stream.read(buffer2, size - 1);
                    if (stream.gcount() < size - 1)//标志着文件读取结束
                    {
                        buffer2[stream.gcount()] = EOF;
                    }
                    forward = buffer2;//指针指向下一个缓冲区的起点
                    curID = BufferID::SECOND;//将ID切换
                }
                else//当前在第二个缓冲区
                {
                    stream.read(buffer1, size - 1);
                    if (stream.gcount() < size - 1)
                    {
                        buffer1[stream.gcount()] = EOF;
                    }
                    forward = buffer1;
                    curID = BufferID::FIRST;
                }
            }
        }

    public:
        bool end = false;

        explicit DoubleBuffer(std::string filepath)
        {
            stream.open(filepath);
            if (!stream.is_open())
            {
                throw std::runtime_error("无法打开文件：" + filepath);
            }
            //初始时将两个缓冲区的结尾固定设置成EOF，表示缓冲区的末尾
            buffer1[size - 1] = EOF;
            buffer2[size - 1] = EOF;
            buffer2[0] = '\0';//将二号缓冲区起始位置设置成未使用的状态
            stream.read(buffer1, size - 1);//初始缓冲区填充，文件中字符数不够就全部读入。
            std::streamsize count = stream.gcount();
            if (count < size - 1)//文件中字符数量不够
            {
                buffer1[count] = EOF;//在字符流的最后添加上EOF，表示文件的结束
            }
            lexemeBegin = forward = buffer1;//初始化指针的位置
        }

        ~DoubleBuffer() noexcept {
            if (stream) stream.close();
        }
        /// <summary>
        /// 提取目前分析出的词法单元
        /// </summary>
        /// <returns>词法单元</returns>
        std::string getToken()
        {
            std::string token = "";
            char* curPos = lexemeBegin;
            while (curPos != forward)
            {
                if (*curPos == EOF)
                {
                    curPos = (curID == BufferID::FIRST) ? buffer1 : buffer2;
                }
                token += *curPos++;
            }
            lexemeBegin = forward;
            return token;
        }

        /// <summary>
        /// 获取当前字符，并将指针指向下一个字符
        /// </summary>
        /// <returns>当前的字符</returns>
        char next()
        {
            //获取当前位置的字符
            char c = *forward;
            //读取到EOF，存在两种情况，
            // 一种是达到缓冲区的结尾，需要加载下一个缓冲区；
            // 一种是达到文件的结尾，需要结束读取
            if (c == EOF)
            {
                char* curBuffer = buffer1;
                if (curID == BufferID::SECOND)
                {
                    curBuffer = buffer2;
                }
                if (forward - curBuffer == size - 1)
                {
                    read();//到达文件末尾，加载下一个缓冲区,同步将forward指针切换到下一个缓冲区中
                }
                else
                {
                    isRollBack = false;
                    this->end = true;
                    return EOF;
                }
            }
            isRollBack = false;
            return *forward++;
        }
        /// <summary>
        /// 获取当前向前指针所在位置的字符
        /// </summary>
        /// <returns>当前指向的字符</returns>
        char cur()
        {
            return *forward;
        }
        /// <summary>
	    /// 指针指向的前一个字符，用于自动机的回退操作
	    /// </summary>
	    /// <returns>当前字符</returns>
	    char pre()
	    {
		    char c = *forward;
		    if (forward == buffer1)//判断当前是否在第一个缓冲区的起始位置
		    {
			    if (buffer2[0] != '\0')//仅有当前已使用过二号缓冲区才回退
			    {
				    forward = buffer2+size - 1;//回退到上一个缓冲区末尾
                    isRollBack = true;
			    }
		    }
		    else if (forward == buffer2)
		    {
			    forward = buffer1+size - 1;
                isRollBack = true;
		    }
		    else
		    {
			    forward--;//正常情况下的回退
		    }
		    return c;
	    }
    };
}
