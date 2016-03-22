#pragma once
#include <string>
#include "serializer.h"

using namespace std;

namespace mytunes
{
	class MhodSerializer : Serializer
	{
	public:
		MhodSerializer(const wstring& value, unsigned int type);
		~MhodSerializer();

		unsigned char * serialize() override;
		inline size_t size() { return sz; }
	private:
		wstring mValue;
		unsigned int mType;
		const char header_identifier[4]{ 'm','h','o','d' };
		size_t	sz,
			header_length = 0x18,
			unknown1 = 0,
			unknown2 = 0,
			position = 1,
			s_len = 0,
			unknown3 = 0,
			unknown4 = 0;
	};
}