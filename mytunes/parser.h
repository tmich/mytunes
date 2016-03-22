#pragma once
#include <cstdio>
#include <string>
#include "itunesdb.h"

using namespace std;

namespace mytunes
{
class Parser
{
public:
	Parser(string filename);
	iTunesDB parse();
protected:
	int read_mhbd(iTunesDB& itunesdb, FILE *fd);
	int read_mhsd(iTunesDB& itunesdb, FILE *fd);
	int read_mhla(iTunesDB& itunesdb, FILE *fd);
	int read_mhia(iTunesDB& itunesdb, FILE *fd);
	int read_mhlt(iTunesDB& itunesdb, FILE *fd);
	int read_mhit(iTunesDB& itunesdb, FILE *fd);
	int read_mhlp(iTunesDB& itunesdb, FILE *fd);
	int read_mhyp(iTunesDB& itunesdb, FILE *fd);
	int read_mhod(Track& track, FILE *fd);
	int read_mhod(Album& album, FILE *fd);
	int read_mhod(Playlist& album, FILE *fd);
	string mFilename;
};

}
