#pragma once
#include <string>

using namespace std;

namespace mytunes
{
class Track
{
public:
	Track();

	unsigned int unique_id;
	wstring title;
	wstring artist;
	wstring album;
	wstring location;
	wstring genre;
	wstring type;
	string filetype;
	unsigned short album_id;
	unsigned int size;
	unsigned int length;
	unsigned int year;
	unsigned int bitrate;
	unsigned int sample_rate;
};
}
