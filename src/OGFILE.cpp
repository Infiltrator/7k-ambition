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

//Filename    : OGFILE.CPP
//Description : Object Game file, save game and restore game

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef USE_WINDOWS
#include <objbase.h>
#endif

#include <OGFILE.h>
#include <OFILE.h>
#include <OSTR.h>
#include <OTALKRES.h>
#include <ONATION.h>
#include <OWORLD.h>
#include <OGAME.h>
#include <OTownNetwork.h>
#include <OINFO.h>
#include <OSYS.h>
#include <OAUDIO.h>
#include <OMUSIC.h>
#include <OSaveGameInfo.h>
#include <dbglog.h>
#include "gettext.h"
#include <OGF_REC.h>

DBGLOG_DEFAULT_CHANNEL(GameFile);

#pragma pack(1)
struct SaveGameHeader
{
	uint32_t class_size;    // for version compare
	SaveGameInfo info;
};
#pragma pack()

enum {CLASS_SIZE = 302};
static_assert(sizeof(SaveGameHeader) == CLASS_SIZE, "Savegame header size mismatch"); // (no packing)

enum { ERROR_NONE = 0,
	ERROR_CREATE,
	ERROR_WRITE_HEADER,
	ERROR_WRITE_DATA,
	ERROR_OPEN,
	ERROR_FILE_HEADER,
	ERROR_FILE_FORMAT,
	ERROR_FILE_CORRUPTED,
};

static int last_status = ERROR_NONE;


//-------- Begin of function GameFile::save_game --------//
//
// Saves the current game under the given filePath.
// On error, returns false.
//
int GameFile::save_game(const char* filePath, const SaveGameInfo& saveGameInfo)
{
	File file;
	bool fileOpened = false;

	last_status = ERROR_NONE;

	int rc = file.file_create(filePath, 0, 1); // 0=tell File don't handle error itself
												// 1=allow the writing size and the read size to be different
	if( !rc )
		last_status = ERROR_CREATE;
	else
		fileOpened = true;

	if( rc )
	{
		save_process();      // process game data before saving the game

		rc = write_game_header(saveGameInfo, &file);    // write saved game header information

		if( !rc )
			last_status = ERROR_WRITE_HEADER;

		if( rc )
		{
			rc = write_file(&file);

			if( !rc )
				last_status = ERROR_WRITE_DATA;
		}
	}

	file.file_close();

	//------- when saving error ---------//

	if( !rc )
	{
		if (fileOpened) remove( filePath );         // delete the file as it is not complete
	}

	return rc != 0;
}
//--------- End of function GameFile::save_game --------//


//-------- Begin of function GameFile::load_game --------//
//
// return : <int> 1 - loaded successfully.
//                0 - not loaded.
//               -1 - error and partially loaded
//
int GameFile::load_game(const char* filePath, SaveGameInfo* /*out*/ saveGameInfo)
{
	File file;
	int  rc=1;

	last_status = ERROR_NONE;

	if(rc && !file.file_open(filePath, 0, 1)) // 0=tell File don't handle error itself
	{
		rc = 0;
		last_status = ERROR_OPEN;
	}

	//-------- read in the GameFile class --------//

	if( rc )
	{
		if( !file.file_read(&gf_rec, sizeof(GameFileHeader)) )	// read the whole object from the saved game file
		{
			rc = 0;
			last_status = ERROR_FILE_HEADER;
		}

		read_record(&gf_rec.game_file);

		if( !validate_header() )
		{
			rc = 0;
			last_status = ERROR_FILE_FORMAT;
		}
	}

	//--------------------------------------------//
																  // 1=allow the writing size and the read size to be different
	if( rc )
	{
		config.terrain_set = terrain_set;

		game.deinit(1);		// deinit last game first, 1-it is called during loading of a game
		game.init(1);			// init game

		//-------- read in saved game ----------//

		switch( read_file(&file) )
		{
		case 1:
			rc = 1;
			break;
		case -1:
			rc = 0;		// consider cancel load game
			last_status = ERROR_FILE_FORMAT;
			break;
		case 0:
		default:
			rc = -1;
			last_status = ERROR_FILE_CORRUPTED;
		}

		if( rc > 0 )
		{
			load_process();           // process game data after loading the game
			
			//------- create the town network --------//
			town_network_array.recreate_after_load();
		}
	}

	file.file_close();

	//---------------------------------------//

	if (rc > 0)
	{
		strcpy(saveGameInfo->file_name, file_name);
		strcpy(saveGameInfo->player_name, player_name);
		saveGameInfo->race_id = race_id;
		saveGameInfo->nation_color = nation_color;
		saveGameInfo->game_date = game_date;
		saveGameInfo->file_date.dwLowDateTime = file_date.dwLowDateTime;
		saveGameInfo->file_date.dwHighDateTime = file_date.dwHighDateTime;
		saveGameInfo->terrain_set = terrain_set;
		strncpy(scenario_file_name, file_name, FilePath::MAX_FILE_PATH);
		scenario_file_name[FilePath::MAX_FILE_PATH] = 0;
	}

	return rc;
}
//--------- End of function GameFile::load_game --------//


//-------- Begin of function GameFile::read_header --------//
//
// Reads the given file and fills the save game info from the header. Returns true if successful.
//
int GameFile::read_header(const char* filePath, SaveGameInfo* /*out*/ saveGameInfo)
{
	int rc = 1;
	File file;
	SaveGameHeader saveGameHeader;

	if( !file.file_open(filePath, 0, 1) )
		return 0;
	if( !file.file_read(&saveGameHeader, sizeof(SaveGameHeader)) )
	{
		rc = 0;
	}
	else if( !(saveGameHeader.class_size == CLASS_SIZE &&
		saveGameHeader.info.terrain_set > 0) )
	{
		rc = 0;
	}
	else
	{
		*saveGameInfo = saveGameHeader.info;
	}
	file.file_close();

	return rc;
}
//--------- End of function GameFile::read_header --------//

//-------- Begin of function GameFile::save_process -------//
//
// Make the game data ready for saving game
//
// Called before saving the game
//
void GameFile::save_process()
{
	//--------- set the total playing time --------//

	info.total_play_time += misc.get_time()-info.start_play_time;

	info.start_play_time  = misc.get_time();
}
//--------- End of function GameFile::save_process -------//


//-------- Begin of function GameFile::load_process -------//
//
// Make the game data ready after loading game
//
// Called after loading the game
//
void GameFile::load_process()
{
	info.start_play_time = misc.get_time();       // the time player start playing the game
	config.disable_ai_flag = 0;

	//-- if the player is in the diplomatic message screen, rebuild the talk choice list --//

	if( sys.view_mode==MODE_NATION && info.nation_report_mode==NATION_REPORT_TALK )
		talk_res.set_talk_choices();

	// reflect the effect of config.music_flag, config.wav_music_volume
	audio.set_wav_volume(config.sound_effect_volume);
	if( config.music_flag )
	{
		if( music.is_playing() )
		{
			music.change_volume(config.wav_music_volume);
		}
	}
	else
	{
		music.stop();
	}
}
//--------- End of function GameFile::load_process -------//


//------- Begin of function GameFile::write_game_header -------//
//
// Write saved game header info to the saved game file.
//
// Return : <int> 1 - file written successfully
//                0 - not successful
//
int GameFile::write_game_header(const SaveGameInfo& saveGameInfo, File* filePtr)
{
	class_size = sizeof(GameFileHeader);

	Nation* playerNation = ~nation_array;

	strncpy( player_name, playerNation->king_name(), HUMAN_NAME_LEN );
	player_name[HUMAN_NAME_LEN] = '\0';

	race_id      = playerNation->race_id;
	nation_color = playerNation->nation_color;
	terrain_set  = config.terrain_set;

	game_date    = info.game_date;

	//----- set the file date ------//

#ifdef USE_WINDOWS
	CoFileTimeNow((FILETIME*)&file_date);
#else
	file_date.dwLowDateTime = 0;
	file_date.dwHighDateTime = 0;
#endif

	//------- write GameFile to the saved game file -------//

	write_record(&gf_rec.game_file);
	return filePtr->file_write(&gf_rec, sizeof(GameFileHeader));     // write the whole object to the saved game file
}
//--------- End of function GameFile::write_game_header -------//


//--------- Begin of function GameFile::validate_header -------//
int GameFile::validate_header()
{
	return class_size == sizeof(GameFileHeader) && terrain_set > 0;
}
//--------- End of function GameFile::validate_header -------//


//--------- Begin of function GameFile::status_str -------//
const char *GameFile::status_str()
{
	switch(last_status)
	{
	case ERROR_NONE:
		return _("Success");
	case ERROR_CREATE:
		return _("Error creating saved game file");
	case ERROR_WRITE_HEADER:
		return _("Error creating saved game header");
	case ERROR_WRITE_DATA:
		return _("Error writing saved game data");
	case ERROR_OPEN:
		return _("Cannot open save game file");
	case ERROR_FILE_HEADER:
		return _("Cannot read file header");
	case ERROR_FILE_FORMAT:
		return _("Incompatible save game");
	case ERROR_FILE_CORRUPTED:
		return _("Load game error");
	}
	err_here();
	return "";
}
//--------- End of function GameFile::status_str -------//
