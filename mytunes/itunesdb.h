#pragma once
#include <string>
#include <vector>
#include "track.h"
#include "album.h"
#include "playlist.h"

using namespace std;

namespace mytunes
{
class iTunesDB
{
public:
	vector<Track> tracks;
	vector<Album> albums;
	vector<Playlist> playlists;
	string filename;
	string language;
	int filesize;
	//Itdb_Device *device;
	int version;
	//int tracks_no;
	//int albums_no;
	unsigned long long id;
};
}
