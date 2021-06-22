// 模拟磁盘 2560 byte
// 块大小 16 byte 共 160 个块

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
	char Disk[DISK_MAXLEN] = { 0 };  // 模拟磁盘区域

	// 参数
	void* m_start_addr;
	void* m_end_addr;
	int m_disk_len;
	int m_block_size;
	
	// 包含类
	MBR* m_mbr;
	FAT* m_fat;
	RootDirectory* m_rootdirectory;

	// 进程操作的当前文件信息
	std::string cur_filename;  // ""代表没有打开的文件
	byte cur_start_block;  // 0代表没有打开的文件 1代表有文件但没分配空间
	byte cur_block_size;  // 文件占用的块数

	// function offered
	void ls();
	void open(std::string filename);
	void close();
	void read();  // 读取文件信息
	void write();  // 写入
	void file_delete();  // 文件删除
	
public:
	FileSystem();
	~FileSystem();

	void test();
	void user_opera();
};

#endif // !FILESYSTEM
