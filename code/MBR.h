// MBR区域共占用 16 byte 即 1 个 block
// MBR从地址空间的 0 地址开始， 即 0 - 15 byte
#pragma once
#ifndef MBR_
#define MBR_

class MBR {
private:
	void* m_addr;  // 记录开始地址
	int m_len;  // 占用m_len byte
public:
	MBR(void* in_m_addr = nullptr);
	~MBR();
};

#endif // !MBR
