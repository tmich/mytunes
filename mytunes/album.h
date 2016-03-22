#pragma once
#include <string>

using namespace std;

namespace mytunes
{
	class Album
	{
	public:
		Album();
		wstring title;
		wstring artist;
		wstring artist_for_sorting;
		unsigned short id;
	};

	
}