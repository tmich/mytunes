#include "stdafx.h"
#include "parser.h"
#include <algorithm>

using namespace mytunes;

Parser::Parser(string filename)
	: mFilename { filename }
{}

int Parser::read_mhbd(iTunesDB& itunesdb, FILE *fd)
{
	char header_identifier[4], lang[2];
	unsigned int header_length, total_length, unknown1, version_no, children_no;
	unsigned long long id, library_id;
		
	fread(&header_identifier, sizeof(int), 1, fd);	// 0-3:   header identifier ("mhbd")
	fread(&header_length, sizeof(int), 1, fd);		// 4-7:   header length
	fread(&total_length, sizeof(int), 1, fd);		// 8-11:  total length
	fread(&unknown1, sizeof(int), 1, fd);			// 12-15: unknown
	fread(&version_no, sizeof(int), 1, fd);		// 16-19: db version number
	fread(&children_no, sizeof(int), 1, fd);		// 20-23: no of children (mhds)
	fread(&id, sizeof(long long), 1, fd);			// 24-31: db identifier
	fseek(fd, sizeof(char) * 38, SEEK_CUR);		// 32-69: unknown
	fread(&lang, 2, 1, fd);			// 70-71: language
	fread(&library_id, sizeof(long long), 1, fd);	// 72-79: db identifier

	itunesdb.id = id;
	itunesdb.filesize = total_length;
	itunesdb.language = string(lang, 2);
	itunesdb.version = version_no;
	
	fseek(fd, header_length, SEEK_SET);

	for (size_t i = 0; i < children_no; i++)
	{
		read_mhsd(itunesdb, fd);
	}

	return children_no;
}

int Parser::read_mhsd(iTunesDB& itunesdb, FILE *fd)
{
	char header_identifier[4];
	unsigned int header_length, total_length, type;
	
	fread(&header_identifier, sizeof(unsigned int), 1, fd);	// 0-3:   header identifier ("mhbd")
	fread(&header_length, sizeof(unsigned int), 1, fd);		// 4-7:   header length
	fread(&total_length, sizeof(unsigned int), 1, fd);		// 8-11:  total length
	fread(&type, sizeof(unsigned int), 1, fd);				// 12-15: type
															//		  1: Track List 	(mhlt)
															//		  2: Playlist List	(mhlp)
															//		  3: Podcast List	(mhlp)
															//		  4: Album List	 	(mhla)
															//		  5: Smart Playlist List (mhlp)

	fseek(fd, header_length - (sizeof(unsigned int) * 4), SEEK_CUR);
	
	switch(type)
	{
		case 1:
			read_mhlt(itunesdb, fd);
			break;
		//case 2:
		case 3:
			read_mhlp(itunesdb, fd);
			break;
		case 4:
			read_mhla(itunesdb, fd);
			break;
		default:
			break;
	}
	
	//fseek(fd, total_length, SEEK_SET);
	return 1;
}

int Parser::read_mhla(iTunesDB& itunesdb, FILE *fd)
{
	long start = ftell(fd);
	char header_identifier[4];
	unsigned int header_length, children_no;
	
	fread(&header_identifier, sizeof(unsigned int), 1, fd);	// 0-3:   header identifier ("mhla")
	fread(&header_length, sizeof(unsigned int), 1, fd);		// 4-7:   header length
	fread(&children_no, sizeof(unsigned int), 1, fd);		// 8-11:  no of children
	
	fseek(fd, header_length + start, SEEK_SET);
	
	for(size_t i=0; i<children_no; i++)
	{
		read_mhia(itunesdb, fd);
	}
	
	return children_no;
}

int Parser::read_mhia(iTunesDB& itunesdb, FILE *fd)
{
	long start = ftell(fd);
	char header_identifier[4];
	unsigned int header_length, total_size, strings_no;
	unsigned short unknown1, id;
	
	fread(&header_identifier, sizeof(unsigned int), 1, fd);	// 0-3:   header identifier ("mhia")
	fread(&header_length, sizeof(unsigned int), 1, fd);		// 4-7:   header length
	fread(&total_size, sizeof(unsigned int), 1, fd);		// 8-11:  total size
	fread(&strings_no, sizeof(unsigned int), 1, fd);		// 12-15: no of strings (mhods)
	fread(&unknown1, sizeof(unsigned short), 1, fd);		// 16-17: unknown
	fread(&id, sizeof(unsigned short), 1, fd);				// 18-19: album id for track
	
	if (id==0)
	{
		id = unknown1;
	}
	
	fseek(fd, (long)header_length + start, SEEK_SET);

	Album album;
	album.id = id;
	for (size_t i = 0; i < strings_no; i++)
	{
		read_mhod(album, fd);
	}

	itunesdb.albums.push_back(album);

	fseek(fd, total_size + start, SEEK_SET);
	
	return 0;
}

int Parser::read_mhlp(iTunesDB& itunesdb, FILE *fd)
{
	long start = ftell(fd);
	char header_identifier[4];
	unsigned int header_length, children_no;

	fread(&header_identifier, sizeof(unsigned int), 1, fd);	// 0-3:   header identifier ("mhlp")
	fread(&header_length, sizeof(unsigned int), 1, fd);		// 4-7:   header length
	fread(&children_no, sizeof(unsigned int), 1, fd);		// 8-11:  no of children

	fseek(fd, header_length + start, SEEK_SET);

	for (size_t i = 0; i<children_no; i++)
	{
		read_mhyp(itunesdb, fd);
	}

	return children_no;
}

int Parser::read_mhyp(iTunesDB& itunesdb, FILE *fd)
{
	long start = ftell(fd);
	unsigned char header_identifier[4], master_flag, unknown1[3], podcast_flag, group_flag;
	unsigned int header_length, total_size, mhods_no, item_count, timestamp, unknown2, sort_order;
	unsigned short strings_no;
	unsigned long long playlist_id;

	fread(&header_identifier, sizeof(unsigned char) * 4, 1, fd);	// 0-3:   header identifier ("mhyp")
	fread(&header_length, sizeof(unsigned int), 1, fd);				// 4-7:   header length
	fread(&total_size, sizeof(unsigned int), 1, fd);				// 8-11:  total size
	fread(&mhods_no, sizeof(unsigned int), 1, fd);
	fread(&item_count, sizeof(unsigned int), 1, fd);
	fread(&master_flag, sizeof(unsigned char), 1, fd);
	fread(&unknown1, sizeof(unsigned char) * 3, 1, fd);
	fread(&timestamp, sizeof(unsigned int), 1, fd);
	fread(&playlist_id, sizeof(unsigned long long), 1, fd);
	fread(&unknown2, sizeof(unsigned int), 1, fd);
	fread(&strings_no, sizeof(unsigned short), 1, fd);
	fread(&podcast_flag, sizeof(unsigned char), 1, fd);
	fread(&group_flag, sizeof(unsigned char), 1, fd);
	fread(&sort_order, sizeof(unsigned int), 1, fd);
	fseek(fd, (long)header_length + start, SEEK_SET);

	Playlist plist;
	plist.item_count = item_count;
	plist.is_master = (master_flag == '\x1');

	for (size_t i = 0; i < strings_no; i++)
	{
		read_mhod(plist, fd);
	}

	itunesdb.playlists.push_back(plist);

	fseek(fd, total_size + start, SEEK_SET);

	return 0;
}

int Parser::read_mhlt(iTunesDB& itunesdb, FILE *fd)
{
	char header_identifier[4];
	unsigned int header_length, children_no;
	
	fread(&header_identifier, sizeof(unsigned int), 1, fd);	// 0-3:   header identifier ("mhlt")
	fread(&header_length, sizeof(unsigned int), 1, fd);		// 4-7:   header length
	fread(&children_no, sizeof(unsigned int), 1, fd);		// 8-11:  no of children (songs)
	
	fseek(fd, header_length - (sizeof(unsigned int) * 3), SEEK_CUR);
	
	for(size_t i=0; i<children_no; i++)
	{
		read_mhit(itunesdb, fd);
	}
	
	return children_no;
}

int Parser::read_mhit(iTunesDB& itunesdb, FILE *fd)
{
	long start = ftell(fd);
	char header_identifier[4], filetype[4];
	unsigned int header_length, total_size, strings_no, unique_id, visible, size, length, year, bitrate, sample, album_id;
	
	fread(&header_identifier, sizeof(unsigned int), 1, fd);	// 0-3:   header identifier ("mhit")
	fread(&header_length, sizeof(unsigned int), 1, fd);		// 4-7:   header length
	fread(&total_size, sizeof(unsigned int), 1, fd);		// 8-11:  total size
	fread(&strings_no, sizeof(unsigned int), 1, fd);		// 12-15: no of strings (mhods)
	fread(&unique_id, sizeof(unsigned int), 1, fd);			// 16-19: unique_id
	fread(&visible, sizeof(unsigned int), 1, fd);			// 20-23: visible on iPod
	fread(&filetype, sizeof(unsigned int), 1, fd);			// 24-27: file type
	fseek(fd, (sizeof(unsigned int) * 2), SEEK_CUR);
	fread(&size, sizeof(unsigned int), 1, fd);				// 30-33: track size in bytes
	fread(&length, sizeof(unsigned int), 1, fd);			// 34-37: track length in milliseconds
	fseek(fd, (sizeof(unsigned int) * 2), SEEK_CUR);
	fread(&year, sizeof(unsigned int), 1, fd);				// 46-49: year
	fread(&bitrate, sizeof(unsigned int), 1, fd);			// 50-53: bitrate
	fseek(fd, (sizeof(unsigned char) * 2), SEEK_CUR);
	fread(&sample, sizeof(unsigned int), 1, fd);			// 56-59: sample rate
	fseek(fd, (sizeof(unsigned char) * 222), SEEK_CUR);
	fread(&album_id, sizeof(unsigned int), 1, fd);			
	
	Track track;
	track.unique_id = unique_id; 
	track.filetype = string(filetype, 4);
	reverse(track.filetype.begin(), track.filetype.end());
	track.size = size;
	track.length = length;
	track.year = year;
	track.bitrate = bitrate;
	track.sample_rate = sample;
	track.album_id = album_id;

	fseek(fd, (long)header_length + start, SEEK_SET);

	for (size_t i = 0; i < strings_no; i++)
	{
		read_mhod(track, fd);
	}
	
	itunesdb.tracks.push_back(track);
	
	//printf("%s\n", string(header_identifier, 4).c_str());
	
	fseek(fd, (long)total_size + start, SEEK_SET);
	
	return 0;
}

int Parser::read_mhod(Track& track, FILE *fd)
{
	long start = ftell(fd);

	char header_identifier[4];
	unsigned int header_length, total_size, type, unknown1, unknown2, position, value_length, unknown3, unknown4;
	wstring value;

	fread(&header_identifier, sizeof(unsigned int), 1, fd);	// 0-3:   header identifier ("mhod")
	fread(&header_length, sizeof(unsigned int), 1, fd);		// 4-7:	  header length
	fread(&total_size, sizeof(unsigned int), 1, fd);		// 8-11:  total size
	fread(&type, sizeof(unsigned int), 1, fd);				// 12-15: total size
	fread(&unknown1, sizeof(unsigned int), 1, fd);
	fread(&unknown2, sizeof(unsigned int), 1, fd);
	fread(&position, sizeof(unsigned int), 1, fd);
	fread(&value_length, sizeof(unsigned int), 1, fd);
	fread(&unknown3, sizeof(unsigned int), 1, fd);
	fread(&unknown4, sizeof(unsigned int), 1, fd);

	// if type < 15, mhod are of type (w)string
	if (type < 15)
	{
		wchar_t *buffer = (wchar_t*)malloc(value_length);
		fread(buffer, value_length, 1, fd);
		value = wstring(buffer, value_length / 2);
		//printf("%ws\n", value.c_str());

		switch (type)
		{
		case 1:
			track.title = value; break;
		case 2:
			track.location = value; break;
		case 3:
			track.album = value; break;
		case 4:
			track.artist = value; break;
		case 5:
			track.genre = value; break;
		case 6:
			track.type = value; break;
		default:
			break;
		}
		
	}
	
	fseek(fd, (long)total_size + start, SEEK_SET);

	return 0;
}

int Parser::read_mhod(Album& album, FILE *fd)
{
	long start = ftell(fd);

	char header_identifier[4];
	unsigned int header_length, total_size, type, unknown1, unknown2, position, value_length, unknown3, unknown4;
	wstring value;

	fread(&header_identifier, sizeof(unsigned int), 1, fd);	// 0-3:   header identifier ("mhod")
	fread(&header_length, sizeof(unsigned int), 1, fd);		// 4-7:	  header length
	fread(&total_size, sizeof(unsigned int), 1, fd);		// 8-11:  total size
	fread(&type, sizeof(unsigned int), 1, fd);				// 12-15: total size
	fread(&unknown1, sizeof(unsigned int), 1, fd);
	fread(&unknown2, sizeof(unsigned int), 1, fd);
	fread(&position, sizeof(unsigned int), 1, fd);
	fread(&value_length, sizeof(unsigned int), 1, fd);
	fread(&unknown3, sizeof(unsigned int), 1, fd);
	fread(&unknown4, sizeof(unsigned int), 1, fd);

	// if type < 15, mhod are of type (w)string
	
	wchar_t *buffer = (wchar_t*)malloc(value_length);
	fread(buffer, value_length, 1, fd);
	value = wstring(buffer, value_length / 2);
	//printf("%ws\n", value.c_str());

	if (type == 200 || type == 201)
	{
		switch (type)
		{
		case 200:
			album.title = value; break;
		case 201:
			album.artist = value; break;
		case 202:
			album.artist_for_sorting = value; break;
		default:
			break;
		}
	}
	
	fseek(fd, (long)total_size + start, SEEK_SET);

	return 0;
}

int Parser::read_mhod(Playlist & playlist, FILE * fd)
{
	long start = ftell(fd);

	char header_identifier[4];
	unsigned int header_length, total_size, type, unknown1, unknown2, position, value_length, unknown3, unknown4;
	wstring value;

	fread(&header_identifier, sizeof(unsigned int), 1, fd);	// 0-3:   header identifier ("mhod")
	fread(&header_length, sizeof(unsigned int), 1, fd);		// 4-7:	  header length
	fread(&total_size, sizeof(unsigned int), 1, fd);		// 8-11:  total size
	fread(&type, sizeof(unsigned int), 1, fd);				// 12-15: total size
	fread(&unknown1, sizeof(unsigned int), 1, fd);
	fread(&unknown2, sizeof(unsigned int), 1, fd);
	fread(&position, sizeof(unsigned int), 1, fd);
	fread(&value_length, sizeof(unsigned int), 1, fd);
	fread(&unknown3, sizeof(unsigned int), 1, fd);
	fread(&unknown4, sizeof(unsigned int), 1, fd);

	// if type < 15, mhod are of type (w)string
	if (type < 15)
	{
		wchar_t *buffer = (wchar_t*)malloc(value_length);
		fread(buffer, value_length, 1, fd);
		value = wstring(buffer, value_length / 2);
		//printf("%ws\n", value.c_str());

		switch (type)
		{
		case 1:
			playlist.name = value; break;
		/*case 2:
			track.location = value; break;
		case 3:
			track.album = value; break;
		case 4:
			track.artist = value; break;
		case 5:
			track.genre = value; break;
		case 6:
			track.type = value; break;*/
		default:
			break;
		}

		free(buffer);
	}

	fseek(fd, (long)total_size + start, SEEK_SET);
	return 0;
}

iTunesDB Parser::parse()
{
	iTunesDB itunesdb;
	
	FILE * fd;
	fd = fopen(mFilename.c_str(), "rb");

	if (fd != nullptr)
	{
		itunesdb.filename = mFilename;
		
		int no_mhsd = read_mhbd(itunesdb, fd);
		
		
		
		/**
		printf("Current position: %ld\n", ftell(fd));
		printf("header id: %s\n", string(header_identifier, 4).c_str());
		printf("header length: %d\n", header_length);
		printf("total length: %d\n", total_length);
		printf("id: %llu\n", id);		
		printf("language: %s\n", string(lang, 2).c_str());
		printf("library id: %llu\n", library_id);
		**/
	
		fclose(fd);
	}
	
	return itunesdb;
}
