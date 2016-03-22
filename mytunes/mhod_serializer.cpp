#include "stdafx.h"
#include "mhod_serializer.h"

using namespace mytunes;

MhodSerializer::MhodSerializer(const wstring& value, unsigned int type)
	: mValue{ value }, mType{ type }, sz{ 0 }
{
	s_len = mValue.size() * 2,	// utf-16	
	sz = header_length + sizeof(position) + sizeof(int) + sizeof(unknown3) + sizeof(unknown4) + s_len;
}

MhodSerializer::~MhodSerializer()
{
}

unsigned char * mytunes::MhodSerializer::serialize()
{
	unsigned char * bytes = (unsigned char *)malloc(sz * sizeof(unsigned char));
	memcpy(bytes, header_identifier, sizeof(int));
	memcpy(bytes + sizeof(int), &header_length, sizeof(int));
	memcpy(bytes + sizeof(int) * 2, &sz, sizeof(int));
	memcpy(bytes + sizeof(int) * 3, &mType, sizeof(int));
	memcpy(bytes + sizeof(int) * 4, &unknown1, sizeof(int));
	memcpy(bytes + sizeof(int) * 5, &unknown2, sizeof(int));
	memcpy(bytes + sizeof(int) * 6, &position, sizeof(int));
	memcpy(bytes + sizeof(int) * 7, &s_len, sizeof(int));
	memcpy(bytes + sizeof(int) * 8, &unknown3, sizeof(int));
	memcpy(bytes + sizeof(int) * 9, &unknown4, sizeof(int));

	const wchar_t * sData = mValue.data();
	memcpy(bytes + sizeof(int) * 10, sData, s_len);

	return bytes;
}
