// �ļ������FAT ���ڹ�160�飬�������160��
// ͬʱ���п��пռ����Ĺ��ܣ��Կ�Ϊ��λ��
// 160�ÿһ��ռ�� 1 byte����¼��һ�����ַ
// ע�⣺��char ת��Ϊ unsigned char ���и�ֵ
// �ļ����ݴ� ��28���鿪ʼ������indexֵΪ27����0��ʼ����
// �鲻������Ч����ʱ��ʹ�� 0 ��ʾ��1�������ļ�ʹ�õ����һ��

#pragma once
#ifndef FAT_
#define FAT_
#define INDEX_BASE 27  // ������ʼֵ 0->��������   1->�������һ��
using byte = unsigned char;


class FAT {
private:
	byte* m_start_addr;  // FAT��ʼ��ַ
	byte* m_end_addr;  // ������ַ
	int index; // ��¼����
public:
	FAT(void* in_m_start_addr = nullptr);
	~FAT();

	void test();

	// function
	void file_read(byte start_block, byte block_size, byte* para);  // ���ؿ����para
	bool empty_block(int block_count, byte* block_number);  // �ҵ�block_count�����п飨0�����޸�ֵ
	void file_delete(byte start_block, byte block_size, byte* para);  // �������
};

#endif // !FAT_
