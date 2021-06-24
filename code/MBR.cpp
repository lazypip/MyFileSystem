#include"MBR.h"
#include"FileSystem.h"
#include<cassert>
#include<iostream>

MBR::MBR(void* in_m_addr)
{
	m_addr = in_m_addr;
	m_len = 16;
	
	assert(m_addr);  // **注意用法 类包含执行顺序

	// message record
	//
	//
}

MBR::~MBR()
{

}

