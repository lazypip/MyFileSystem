// ģ����� 2560 byte
// ���С 16 byte �� 160 ����

#pragma once
#ifndef FILESYSTEM
#define FILESYSTEM
#define DISK_MAXLEN 2560
#define BLOCK_SIZE 16

#define MBRSIZE 16
#define FATSIZE 160
#define DIRECTORY_SIZE 256

#include"MBR.h"
#include"FAT.h"
#include"RootDirectory.h"
#include<string>
using byte = unsigned __int8;

class FileSystem {
private:
	char Disk[DISK_MAXLEN] = { 0 };  // ģ���������

	// ����
	void* m_start_addr;
	void* m_end_addr;
	int m_disk_len;
	int m_block_size;
	
	// ������
	MBR* m_mbr;
	FAT* m_fat;
	RootDirectory* m_rootdirectory;

	// ���̲����ĵ�ǰ�ļ���Ϣ
	std::string cur_filename;  // ""����û�д򿪵��ļ�
	byte cur_start_block;  // 0����û�д򿪵��ļ� 1�������ļ���û����ռ�
	byte cur_block_size;  // �ļ�ռ�õĿ���

	// function offered
	void ls();
	void open(std::string filename);
	void close();
	void read();  // ��ȡ�ļ���Ϣ
	void write();  // д��
	void file_delete();  // �ļ�ɾ��
	
public:
	FileSystem();
	~FileSystem();

	void test();
	void user_opera();
};

#endif // !FILESYSTEM
