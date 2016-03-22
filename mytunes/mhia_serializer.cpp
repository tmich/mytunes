#include "stdafx.h"
#include "mhia_serializer.h"


using namespace mytunes;

MhiaSerializer::MhiaSerializer(Album album)
	:mAlbum(album), titleSerializer(album.title, 200), artistSerializer(album.artist, 201), artistForSortingSerializer(album.artist_for_sorting, 202)
{
	if (!album.artist.empty()) { children_no++; }
	if (!album.title.empty()) { children_no++; }
	if (!album.artist_for_sorting.empty()) { children_no++; }

	sz = header_length + titleSerializer.size() + artistSerializer.size() + artistForSortingSerializer.size();
}

unsigned char * MhiaSerializer::serialize()
{
	unsigned char * bytes = (unsigned char *)malloc(sz * sizeof(unsigned char));
	memcpy(bytes, header_identifier, sizeof(int));
	memcpy(bytes + 4, &header_length, sizeof(int));
	memcpy(bytes + 8, &sz, sizeof(int));
	memcpy(bytes + 12, &children_no, sizeof(int));
	memcpy(bytes + 16, &unknown1, sizeof(short));
	memcpy(bytes + 18, &mAlbum.id, sizeof(short));
	memcpy(bytes + 20, &timestamp, sizeof(long long));
	memcpy(bytes + 28, &unknown3, sizeof(int));
	memset(bytes + 32, 0, (header_length - 32));
	memcpy(bytes + header_length, titleSerializer.serialize(), titleSerializer.size());
	memcpy(bytes + header_length + titleSerializer.size(), artistSerializer.serialize(), artistSerializer.size());
	memcpy(bytes + header_length + titleSerializer.size() + artistSerializer.size(), artistForSortingSerializer.serialize(), artistForSortingSerializer.size());
	
	return bytes;
}
