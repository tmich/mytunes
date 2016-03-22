#pragma once
#include "serializer.h"
#include "album.h"
#include "mhod_serializer.h"

namespace mytunes
{
	class MhiaSerializer : public Serializer
	{
	public:
		MhiaSerializer(Album album);
		~MhiaSerializer() {};

		unsigned char * serialize() override;
		inline size_t size() { return sz; }
	private:
		Album mAlbum;
		MhodSerializer titleSerializer;
		MhodSerializer artistSerializer;
		MhodSerializer artistForSortingSerializer;
		const char header_identifier[4]{ 'm','h','i','a' };
		size_t	sz,
			header_length = 0x58,
			children_no = 0,
			unknown3 = 0;
		unsigned short unknown1 = 0;
		unsigned long long timestamp = 0;
	};
}