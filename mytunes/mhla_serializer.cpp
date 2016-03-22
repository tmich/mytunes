#include "stdafx.h"
#include "mhla_serializer.h"

using namespace mytunes;

MhlaSerializer::MhlaSerializer(const vector<Album>& albums)
	: children_no{ albums.size() }
{
	sz = header_length;

	for (const auto& album : albums)
	{
		auto mhia = MhiaSerializer(album);
		mhias.push_back(mhia);
		sz += mhia.size();
	}
}


MhlaSerializer::~MhlaSerializer()
{
}

unsigned char * mytunes::MhlaSerializer::serialize()
{
	unsigned char * bytes = (unsigned char *)malloc(sz * sizeof(unsigned char));
	memcpy(bytes, header_identifier, sizeof(int));
	memcpy(bytes + 4, &header_length, sizeof(int));
	memcpy(bytes + 8, &children_no, sizeof(int));
	memset(bytes + 12, 0, (header_length - 12));
	unsigned int written = header_length;

	for (auto& mhia : mhias)
	{
		memcpy(bytes + written, mhia.serialize(), mhia.size());
		written += mhia.size();
	}

	return bytes;
}
