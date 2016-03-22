#pragma once
#include <string>

using namespace std;

namespace mytunes
{
	class Playlist
	{
	public:
		Playlist();

		unsigned long long id;
		wstring name;
		size_t item_count;
		bool is_master;
		bool is_podcast_list;
	};


}