#pragma once
#include <string>
#include <fstream>
#include <stdexcept>

namespace Lexer
{
    /// <summary>
    /// ʵ�ִʷ�����ʱ��ʹ�õ���˫����������˫ָ��
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
                if (curID == BufferID::FIRST)//��ǰ�ڵ�һ��������
                {
                    forward = buffer2;//ָ��ָ����һ�������������
                    curID = BufferID::SECOND;//��ID�л�
                }
                else//��ǰ�ڵڶ���������
                {
                    forward = buffer1;
                    curID = BufferID::FIRST;
                }
            }
            else
            {
                if (curID == BufferID::FIRST)//��ǰ�ڵ�һ��������
                {
                    stream.read(buffer2, size - 1);
                    if (stream.gcount() < size - 1)//��־���ļ���ȡ����
                    {
                        buffer2[stream.gcount()] = EOF;
                    }
                    forward = buffer2;//ָ��ָ����һ�������������
                    curID = BufferID::SECOND;//��ID�л�
                }
                else//��ǰ�ڵڶ���������
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
                throw std::runtime_error("�޷����ļ���" + filepath);
            }
            //��ʼʱ�������������Ľ�β�̶����ó�EOF����ʾ��������ĩβ
            buffer1[size - 1] = EOF;
            buffer2[size - 1] = EOF;
            buffer2[0] = '\0';//�����Ż�������ʼλ�����ó�δʹ�õ�״̬
            stream.read(buffer1, size - 1);//��ʼ��������䣬�ļ����ַ���������ȫ�����롣
            std::streamsize count = stream.gcount();
            if (count < size - 1)//�ļ����ַ���������
            {
                buffer1[count] = EOF;//���ַ�������������EOF����ʾ�ļ��Ľ���
            }
            lexemeBegin = forward = buffer1;//��ʼ��ָ���λ��
        }

        ~DoubleBuffer() noexcept {
            if (stream) stream.close();
        }
        /// <summary>
        /// ��ȡĿǰ�������Ĵʷ���Ԫ
        /// </summary>
        /// <returns>�ʷ���Ԫ</returns>
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
        /// ��ȡ��ǰ�ַ�������ָ��ָ����һ���ַ�
        /// </summary>
        /// <returns>��ǰ���ַ�</returns>
        char next()
        {
            //��ȡ��ǰλ�õ��ַ�
            char c = *forward;
            //��ȡ��EOF���������������
            // һ���Ǵﵽ�������Ľ�β����Ҫ������һ����������
            // һ���Ǵﵽ�ļ��Ľ�β����Ҫ������ȡ
            if (c == EOF)
            {
                char* curBuffer = buffer1;
                if (curID == BufferID::SECOND)
                {
                    curBuffer = buffer2;
                }
                if (forward - curBuffer == size - 1)
                {
                    read();//�����ļ�ĩβ��������һ��������,ͬ����forwardָ���л�����һ����������
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
        /// ��ȡ��ǰ��ǰָ������λ�õ��ַ�
        /// </summary>
        /// <returns>��ǰָ����ַ�</returns>
        char cur()
        {
            return *forward;
        }
        /// <summary>
	    /// ָ��ָ���ǰһ���ַ��������Զ����Ļ��˲���
	    /// </summary>
	    /// <returns>��ǰ�ַ�</returns>
	    char pre()
	    {
		    char c = *forward;
		    if (forward == buffer1)//�жϵ�ǰ�Ƿ��ڵ�һ������������ʼλ��
		    {
			    if (buffer2[0] != '\0')//���е�ǰ��ʹ�ù����Ż������Ż���
			    {
				    forward = buffer2+size - 1;//���˵���һ��������ĩβ
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
			    forward--;//��������µĻ���
		    }
		    return c;
	    }
    };
}
