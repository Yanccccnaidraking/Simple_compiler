#pragma once
#include<string>
#include<fstream>
namespace Lexer
{
	/// <summary>
	/// ʵ�ִʷ�����ʱ��ʹ�õ���˫����������˫ָ��
	/// </summary>
	template <int size = 4096>//����Ĭ�ϵĻ�������СΪ1024
	class DoubleBuffer
	{
	private:
		/// <summary>
		/// �������ı��
		/// </summary>
		enum BufferID 
		{
			FIRST=0,SECOND
		};
		std::string filepath; // �ļ�·��
		std::ifstream stream; // �ļ���
		BufferID curID = BufferID::FIRST;//��ǰ�Ļ�����ID
		char buffer1[size];//һ�Ż�����
		char buffer2[size];//���Ż�����
		buffer2[0] = '\0';//�����Ż�������ʼλ�����ó�δʹ�õ�״̬
		char* lexemeBegin = nullptr;//��ʼָ�룬ָ��ǰ�ʷ���Ԫ����ʼλ��
		char* forward = nullptr;//��ǰָ�룬ָ��ǰ���ڷ������ַ���λ��
		void read();//���ļ��ж�ȡ���ݵ�������
	public:
		bool end = false;
		explicit DoubleBuffer(std::string filepath);
		~DoubleBuffer() noexcept;
		std::string getToken();//��ȡ��ǰ����������ȷ��Token
		char next();//forward��ǰ�ƶ���ָ����һ���ַ������ص�ǰ�ַ�
		char cur();//��ȡ��ǰ�ַ�
		char pre();//��ȡ��ǰ�ַ���ָ������ƶ�
	};
	
}