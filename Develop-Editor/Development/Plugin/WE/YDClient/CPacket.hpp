#pragma once

#pragma pack(push, 1)
struct PACKET_HEADER
{
	UINT32 m_Magic;				// ������'YDWE'
	UINT32 m_Length;			// ����Ϣȫ��
	UINT32 m_Checksum;			// CRC32У����,Ϊ0����У��
	UINT32 m_ProtocalVersion;	// Э��汾��Ŀǰ��1
	UINT32 m_TypeId;			// ��Ϣ�����룬��ͬ��Ϣ�в�ͬ�Ķ���
};
struct PACKET_KEEPALIVE
{
	PACKET_HEADER header;
	UINT32 m_YDWEVersion;		// YDWE�İ汾��
};

class CPacketHeader
{
public:
	CPacketHeader()
		: m_Magic('YDWE'), m_Length(sizeof(CPacketHeader)), m_Checksum(0), m_ProtocalVersion(1), m_TypeId(0)
	{
	}

	PBYTE GetData()
	{
		return (PBYTE)this;
	}

	size_t GetSize()
	{
		return sizeof(CPacketHeader);
	}

protected:
	UINT32 m_Magic; // 'YDWE'
	UINT32 m_Length;
	UINT32 m_Checksum; // Ϊ0�򲻼��
	UINT32 m_ProtocalVersion; // Э��汾��Ŀǰ��1
	UINT32 m_TypeId;
};

class CPacketKeepAlive : public CPacketHeader
{
public:
	CPacketKeepAlive(UINT32 nVersion)
		: CPacketHeader(), m_YDWEVersion(nVersion)
	{
		m_TypeId   = 0x01;
		m_Length = GetSize();
	}

	size_t GetSize()
	{
		return sizeof(CPacketKeepAlive);
	}

protected:
	UINT32 m_YDWEVersion;
};

#pragma pack(pop)
