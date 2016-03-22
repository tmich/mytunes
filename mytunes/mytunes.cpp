// mytunes.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "parser.h"
#include "itunesdb.h"
#include "mhod_serializer.h"
#include "mhia_serializer.h"
#include "mhla_serializer.h"
#include <iostream>

#ifdef WIN32
	#include <io.h>
	#include <fcntl.h>
#endif // WINDOWS

using namespace std;

int main(int argc, char *argv[])
{
	
#ifdef WIN32
	_setmode(_fileno(stdout), _O_U16TEXT);
#endif

	string filename = "iTunesDB";
	if(argc > 1)
	{
		filename = argv[1];
	}
	mytunes::Parser parser(filename);
	mytunes::iTunesDB itdb = parser.parse();
	//
	////cout << "Filename: " << itdb.filename << endl;
	//wcout << "Database Version: " << itdb.version << ", size: " << itdb.filesize;	//<< ", language: " << itdb.language << endl;
	//wcout << "Albums: " << itdb.albums.size() << endl;
	//wcout << "Tracks: " << itdb.tracks.size() << endl;	
	//
	//for (const auto& album : itdb.albums)
	//{
	//	wcout << "Album: " << album.title << ", performer: " << album.artist << ", id: " << album.id << endl;
	//}

	//for(const auto& track : itdb.tracks)
	//{
	//	wcout << "Title: " << track.title << ", artist: " << track.artist << ", album: " << track.album << endl;
	//	wcout << "\tlocation: " << track.location;
	//	wcout << " id: " << track.unique_id;
	//	wcout << " size: " << track.size;
	//	wcout << " duration: " << track.length;
	//	wcout << " type: "  << track.type; 
	//	wcout << " year: " << track.year;
	//	wcout << " genre: " << track.genre;
	//	wcout << " bitrate: " << track.bitrate;
	//	wcout << " sample rate: " << track.sample_rate;
	//	wcout << " album id: " << track.album_id;
	//	wcout << endl;
	//}

	//wcout << "PLAYLISTS" << endl;
	//for (const auto& playlist : itdb.playlists)
	//{
	//	wcout << playlist.name << (playlist.is_master ? " (master)" : "") << " (" << playlist.item_count << " elements)" << endl;
	//}

	/*auto mhod = mytunes::MhodSerializer(L"Aria", 200);
	auto mhod2 = mytunes::MhodSerializer(L"Alan Sorrenti", 201);*/

	/*mytunes::Album album;
	album.id = 4598;
	album.artist = L"Alan Sorrenti";
	album.title = L"Aria";
	album.artist_for_sorting = L"Alan Sorrenti";*/

	FILE * fout = fopen("serialized.out", "wb");
	/*for (const auto& album : itdb.albums)
	{
		auto mhia = mytunes::MhiaSerializer(album);
		fwrite(mhia.serialize(), 1, mhia.size(), fout);
	}*/

	auto mhla = mytunes::MhlaSerializer(itdb.albums);
	fwrite(mhla.serialize(), 1, mhla.size(), fout);
	
	fclose(fout);
	
	return 0;
}
