#include"FileSystem.h"
#include"RootDirectory.h"
#include<iostream>
#include<cassert>
#include<string>

RootDirectory::RootDirectory(void* in_m_start_addr)
{
	m_start_addr = (entry*)in_m_start_addr + int((MBRSIZE + FATSIZE) / 8);
	m_end_addr = m_start_addr + DIRECTORY_ENTRY_NUM;

	assert(m_start_addr);

	index = 0;
}

RootDirectory::~RootDirectory()
{

}

void RootDirectory::filename_show()
{
	using std::cout;
	using std::endl;

	bool is_empty = 1;  // 是否无文件
	entry cur_entry;  // 64位二进制记录
	byte block_num;  // 块号
	char name_part;

	for (index = 0; index < DIRECTORY_ENTRY_NUM; index++) {  // 访问目录项
		cur_entry = m_start_addr[index];
		block_num = cur_entry % (1 << 8);
		
		if (block_num) {  // 非指向块0，即非空 存在文件
			is_empty = 0;
			cur_entry = cur_entry >> 16;  // 仅保留文件名

			// 文件名输出
			for (size_t i = 0; i < NAME_MAX; i++) {  // 低位先输出，作为左端
				name_part = cur_entry % (1 << 8);
				if (name_part == 0)  // 输出完成  （可删除）
					break;

				cout << name_part;
				cur_entry = cur_entry >> 8;
			}
			cout << "  ";
		}
	}

	if (is_empty)  // 没有文件
		cout << "No File Exits";
	cout << endl;
}

bool RootDirectory::file_open(std::string filename, byte* para)
{
	using std::cout;
	using std::endl;
	using std::string;

	char name_compare[6];  // 匹配文件名
	strcpy_s(name_compare, filename.c_str());

	bool is_exist = 1;  // 文件filename 是否存在
	entry cur_entry;  // 64位二进制记录当前表项
	byte block_num;  // 块号
	byte file_block_size;  // 文件占用块数
	char name_part;

	// 寻找文件 文件存在，添加信息
	for (index = 0; index < DIRECTORY_ENTRY_NUM; index++) {  // 访问目录项
		cur_entry = m_start_addr[index];
		block_num = cur_entry % (1 << 8);
		file_block_size = (cur_entry >> 8) % (1 << 8);

		if (block_num) {  // 非指向块0，即非空 存在文件
			cur_entry = cur_entry >> 16;  // 当前表项的文件名

			// 文件名比较
			for (size_t i = 0; i < NAME_MAX; i++) {  // 低位先输出，作为左端
				name_part = cur_entry % (1 << 8);
				// 信息比较
				if (name_part != name_compare[i]) {// 信息不相符
					is_exist = 0;
					break;
				}
				cur_entry = cur_entry >> 8;
			}

			// 当前目录项匹配成功，写入参数信息
			if (is_exist) {
				para[0] = block_num;
				para[1] = file_block_size;
				return is_exist;
			}
		}
		is_exist = 1;  // reset value
	}


	// 文件不存在 创建新文件  
	is_exist = false;
	for (index = 0; index < DIRECTORY_ENTRY_NUM; index++) {  // 访问目录项
		cur_entry = m_start_addr[index];
		block_num = cur_entry % (1 << 8);

		if (block_num == 0) {  // 目录项为空 找到空闲位置
			// 修改目录项
			void* cur_addr = m_start_addr + index;  // 当前目录项起始地址
			byte* tmp = (byte*)cur_addr;
			*tmp = 1;  // 修改初始块号为1, 代表文件存在
			tmp += 1;
			*tmp = 0;  // 修改块数为0

			char* name_update = (char*)cur_addr + 2;  // 文件名地址
			for (size_t i = 0; i < 6; i++) {
				name_update[i] = name_compare[i];
			}

			// 传递进程信息
			para[0] = 1;
			para[1] = 0;
			break;
		}	
	}
	return is_exist;
}


void RootDirectory::file_delete(std::string filename)
{
	using std::cout;
	using std::endl;
	using std::string;

	char name_compare[6];  // 匹配文件名
	strcpy_s(name_compare, filename.c_str());

	entry cur_entry;  // 64位二进制记录当前表项
	bool found = 1;  // 是否匹配到目标目录项
	byte block_num;  // 初始块号
	char name_part;

	// 寻找文件 并重置目录项
	for (index = 0; index < DIRECTORY_ENTRY_NUM; index++) {  // 访问目录项
		cur_entry = m_start_addr[index];
		block_num = cur_entry % (1 << 8);

		if (block_num) {  // 非指向块0，即非空 存在文件
			cur_entry = cur_entry >> 16;  // 当前表项的文件名

			// 文件名比较
			for (size_t i = 0; i < NAME_MAX; i++) {  // 低位先输出，作为左端
				name_part = cur_entry % (1 << 8);
				// 信息比较
				if (name_part != name_compare[i]) {  // 信息不相符
					found = false;
					break;
				}
				cur_entry = cur_entry >> 8;
			}

			// 当前目录项匹配成功，重置目录项信息为0
			if (found) {
				m_start_addr[index] = 0;
				return;
			}

		}
		found = true;  // value reset
	}
}
