// 文件分配表FAT 由于共160块，因此设置160项
// 同时具有空闲空间管理的功能（以块为单位）
// 160项，每一项占用 1 byte，记录下一个块地址
// 注意：将char 转化为 unsigned char 进行赋值
// 文件数据从 第28个块开始索引，index值为27，从0开始计数
// 块不存在有效数据时，使用 0 表示，1代表是文件使用的最后一块

#pragma once
#ifndef FAT_
#define FAT_
#define INDEX_BASE 27  // 索引开始值 0->块无数据   1->块是最后一块
using byte = unsigned char;


class FAT {
private:
	byte* m_start_addr;  // FAT开始地址
	byte* m_end_addr;  // 结束地址
	int index; // 记录索引
public:
	FAT(void* in_m_start_addr = nullptr);
	~FAT();

	void test();

	// function
	void file_read(byte start_block, byte block_size, byte* para);  // 返回块号至para
	bool empty_block(int block_count, byte* block_number);  // 找到block_count个空闲块（0）并修改值
	void file_delete(byte start_block, byte block_size, byte* para);  // 块号置零
};

#endif // !FAT_
