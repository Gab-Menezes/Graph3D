#pragma once
#include <pch.h>

class IndexBuffer
{
public:
	IndexBuffer(const void* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const ;

	inline unsigned int GetCount() const
	{
		return m_Count;
	}

private:
	unsigned int m_EBO;
	unsigned int m_Count;
};