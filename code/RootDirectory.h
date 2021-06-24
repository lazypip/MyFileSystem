// ��Ŀ¼
// ��������ļ�����Ϊ 32 ����32��Ŀ¼��
// ÿ��Ŀ¼��ռ�� 0.5 �� block
// 6 byte name + 1byte ռ�ÿ���� + 1byte ��ʼblock��� ���͵�ַ��
// ��û�д�����Ϊ0
// �ļ�������Ϊ�գ��Ϊ5���ַ������洢\0
#pragma once
#ifndef ROOTDIRECTORY
#define ROOTDIRECTORY
#define NAME_MAX 6
#define DIRECTORY_ENTRY_NUM 32
#define ENTRY_SIZE 8
#include<string>

using entry = unsigned __int64;  // ���� 8 byte
using byte = unsigned __int8;

class RootDirectory {
private:
	entry* m_start_addr;  // 8 byte
	entry* m_end_addr;
	
	size_t index;  // ���� 0 - 31
public:
	RootDirectory(void* in_m_start_addr = nullptr);
	~RootDirectory();

	// function offered
	void filename_show();  // ls
	bool file_open(std::string filename, byte* para);  // para[0] ��ʼ��� para[1] ����  ���ļ�
	void file_delete(std::string filename);  // ɾ���ļ�Ŀ¼��Ϣ

};

#endif // !ROOTDIRECTORY
