// MBR����ռ�� 16 byte �� 1 �� block
// MBR�ӵ�ַ�ռ�� 0 ��ַ��ʼ�� �� 0 - 15 byte
#pragma once
#ifndef MBR_
#define MBR_

class MBR {
private:
	void* m_addr;  // ��¼��ʼ��ַ
	int m_len;  // ռ��m_len byte
public:
	MBR(void* in_m_addr = nullptr);
	~MBR();
};

#endif // !MBR
