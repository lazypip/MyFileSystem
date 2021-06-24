// 根目录
// 设置最大文件数量为 32 ，则共32个目录项
// 每个目录项占用 0.5 个 block
// 6 byte name + 1byte 占用块个数 + 1byte 初始block块号 （低地址）
// 若没有此项，块号为0
// 文件名不能为空，最长为5个字符，最后存储\0
#pragma once
#ifndef ROOTDIRECTORY
#define ROOTDIRECTORY
#define NAME_MAX 6
#define DIRECTORY_ENTRY_NUM 32
#define ENTRY_SIZE 8
#include<string>

using entry = unsigned __int64;  // 代表 8 byte
using byte = unsigned __int8;

class RootDirectory {
private:
	entry* m_start_addr;  // 8 byte
	entry* m_end_addr;
	
	size_t index;  // 索引 0 - 31
public:
	RootDirectory(void* in_m_start_addr = nullptr);
	~RootDirectory();

	// function offered
	void filename_show();  // ls
	bool file_open(std::string filename, byte* para);  // para[0] 起始块号 para[1] 块数  打开文件
	void file_delete(std::string filename);  // 删除文件目录信息

};

#endif // !ROOTDIRECTORY
