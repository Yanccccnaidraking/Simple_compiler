//�ѽ�ʵ��ת�Ƶ�doublebuffer.h�ļ���
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
//			throw std::runtime_error("�޷����ļ���" + filepath);
//		}
//		//��ʼʱ�������������Ľ�β�̶����ó�EOF����ʾ��������ĩβ
//		buffer1[size - 1] = EOF;
//		buffer2[size - 1] = EOF;
//		//buffer2[0] = '\0';//�����Ż�������ʼλ�����ó�δʹ�õ�״̬
//		stream.read(buffer1, size - 1);//��ʼ��������䣬�ļ����ַ���������ȫ�����롣
//		std::streamsize count = stream.gcount();
//		if (count < size - 1)//�ļ����ַ���������
//		{
//			buffer1[count] = EOF;//���ַ�������������EOF����ʾ�ļ��Ľ���
//		}
//		lexemeBegin = forward = buffer1;//��ʼ��ָ���λ��
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
//		if (curID == BufferID::FIRST)//��ǰ�ڵ�һ��������
//		{
//			stream.read(buffer2, size - 1);
//			if (stream.gcount() < size - 1)//��־���ļ���ȡ����
//			{
//				buffer2[stream.gcount()] = EOF;
//			}
//			forward = buffer2;//ָ��ָ����һ�������������
//			curID = BufferID::SECOND;//��ID�л�
//		}
//		else//��ǰ�ڵڶ���������
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
//	/// ��ȡĿǰ�������Ĵʷ���Ԫ
//	/// </summary>
//	/// <typeparam name="size">��������С</typeparam>
//	/// <returns>�ʷ���Ԫ</returns>
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
//	/// ��ȡ��ǰ�ַ�������ָ��ָ����һ���ַ�
//	/// </summary>
//	/// <typeparam name="size">�������Ĵ�С</typeparam>
//	/// <returns>��ǰ���ַ�</returns>
//	template<int size>
//	char DoubleBuffer<size>::next()
//	{
//		//��ȡ��ǰλ�õ��ַ�
//		char c = *forward;
//		//��ȡ��EOF���������������
//		// һ���Ǵﵽ�������Ľ�β����Ҫ������һ����������
//		// һ���Ǵﵽ�ļ��Ľ�β����Ҫ������ȡ
//		if (c == EOF)
//		{
//			char* curBuffer=buffer1;
//			if (curID == BufferID::SECOND)
//			{
//				curBuffer = buffer2;
//			}
//			if (forward - curBuffer == size - 1)
//			{
//				read();//�����ļ�ĩβ��������һ��������,ͬ����forwardָ���л�����һ����������
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
//	/// ��ȡ��ǰ��ǰָ������λ�õ��ַ�
//	/// </summary>
//	/// <typeparam name="size">��������С</typeparam>
//	/// <returns>��ǰָ����ַ�</returns>
//	template<int size>
//	char DoubleBuffer<size>::cur()
//	{
//		return *forward;
//	}
//	/// <summary>
//	/// ָ��ָ���ǰһ���ַ��������Զ����Ļ��˲���
//	/// </summary>
//	/// <typeparam name="size">��������С</typeparam>
//	/// <returns>��ǰ�ַ�</returns>
//	template<int size>
//	char DoubleBuffer<size>::pre()
//	{
//		char c = *forward;
//		if (forward == buffer1)//�жϵ�ǰ�Ƿ��ڵ�һ������������ʼλ��
//		{
//			if (buffer2[0] != '\0')//���е�ǰ��ʹ�ù����Ż������Ż���
//			{
//				forward = buffer2[size - 1];//���˵���һ��������ĩβ
//			}
//		}
//		else if (forward == buffer2)
//		{
//			forward == buffer1[size - 1];
//		}
//		else
//		{
//			forward--;//��������µĻ���
//		}
//		return c;
//	}
//}