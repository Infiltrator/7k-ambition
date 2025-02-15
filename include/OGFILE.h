/*
 * Seven Kingdoms: Ancient Adversaries
 *
 * Copyright 1997,1998 Enlight Software Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//Filename    : OGFILE.H
//Description : Object Game file, save and restore game

#ifndef __OGFILE_H
#define __OGFILE_H

#include <cstdint>
#include <FilePath.h>

#ifndef __ONATION_H
#include <ONATION.h>
#endif

class File;
struct SaveGameInfo;

//-------- Define class GameFile -----------//

// Struct is binary compatible to FILETIME on Windows
#pragma pack(1)
struct GameFileDate {
	uint32_t dwLowDateTime;
	uint32_t dwHighDateTime;
};
#pragma pack()

//-------- Define class GameFile -----------//

struct GameFileHeader;

class GameFile
{
public:
	uint32_t     class_size;    // for version compare
	char         file_name[FilePath::MAX_FILE_PATH+1];

	char         player_name[HUMAN_NAME_LEN+1];

	char         race_id;
	char         nation_color;

	int          game_date;     // the game date of the saved game
	GameFileDate file_date;     // saving game date
	short        terrain_set;

public:
	short        load_file_game_version;
	bool         read_file_same_version;

public:
	int   save_game(const char* filePath, const SaveGameInfo& saveGameInfo);
	int   load_game(const char* filePath, SaveGameInfo* /*out*/ saveGameInfo);

	int   read_header(const char* filePath, SaveGameInfo* /*out*/ saveGameInfo);

	const char *status_str();

private:
	int   validate_header();

	void  save_process();
	void  load_process();
	int   write_game_header(const SaveGameInfo& saveGameInfo, File* filePtr);

	int   write_file(File*);
	int   write_file_1(File*);
	int   write_file_2(File*);
	int   write_file_3(File*);

	int   read_file(File*);
	int   read_file_1(File*);
	int   read_file_2(File*);
	int   read_file_3(File*);

	void  write_book_mark(File* filePtr, short bookMark);
	int   read_book_mark(File* filePtr, short bookMark);

	void  write_record(GameFileHeader* r);
	void  read_record(GameFileHeader* r);
};

extern GameFile game_file;

//-------------------------------------//

#endif
