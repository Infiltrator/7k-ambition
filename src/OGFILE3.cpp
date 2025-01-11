/*
 * Seven Kingdoms: Ancient Adversaries
 *
 * Copyright 1997,1998 Enlight Software Ltd.
 * Copyright 2010 Unavowed <unavowed@vexillium.org>
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

//Filename    : OGFILE3.CPP
//Description : Object Game file, save game and restore game, part 3

#include <OUNIT.h>

#include <OBULLET.h>
#include <OB_PROJ.h>
#include <OFIRM.h>
#include <OGFILE.h>
#include <ONATION.h>
#include <ONEWS.h>
#include <OREBEL.h>
#include <OREGION.h>
#include <OREGIONS.h>
#include <OSITE.h>
#include <OSNOWG.h>
#include <OSPY.h>
#include <OTORNADO.h>
#include <OTOWN.h>
#include <OU_MARI.h>
#include <OSaveGameArray.h>
#include <dbglog.h>
#include <file_io_visitor.h>
#include <file_reader.h>
#include <ConfigAdv.h>

#include <OGF_V1.h>
#include <OGF_REC.h>

using namespace FileIOVisitor;

DBGLOG_DEFAULT_CHANNEL(GameFile);

//------- declare static functions -------//

static char* create_monster_func();
static char* create_rebel_func();

static void write_ai_info(File* filePtr, short* aiInfoArray, short aiInfoCount, short aiInfoSize);
static void read_ai_info(File* filePtr, short** aiInfoArrayPtr, short& aiInfoCount, short& aiInfoSize);


//-------- Start of function UnitArray::write_file -------------//
//
int UnitArray::write_file(File* filePtr)
{
   int  i;
   Unit *unitPtr;

	filePtr->file_put_short(restart_recno);  // variable in SpriteArray

	filePtr->file_put_short( size()  );  // no. of units in unit_array

	filePtr->file_put_short( selected_recno );
	filePtr->file_put_short( selected_count );
	filePtr->file_put_long ( cur_group_id   );
	filePtr->file_put_long ( cur_team_id    );
	filePtr->file_put_short(idle_blocked_unit_reset_count);
	filePtr->file_put_long (unit_search_tries);
	filePtr->file_put_short(unit_search_tries_flag);

	filePtr->file_put_short(visible_unit_count);
	// unused short*4
	filePtr->file_put_short(0);
	filePtr->file_put_short(0);
	filePtr->file_put_short(0);
	filePtr->file_put_short(0);

	for( i=1; i<=size() ; i++ )
   {
      unitPtr = (Unit*) get_ptr(i);

      //----- write unitId or 0 if the unit is deleted -----//

      if( !unitPtr )    // the unit is deleted
      {
         filePtr->file_put_short(0);
      }
      else
      {
         //--------- write unit_id -------------//

         filePtr->file_put_short(unitPtr->unit_id);

         //------ write data in the base class ------//

         if( !unitPtr->write_file(filePtr) )
            return 0;

         //------ write data in the derived class ------//

         if( !unitPtr->write_derived_file(filePtr) )
				return 0;
      }
   }

   //------- write empty room array --------//

   write_empty_room(filePtr);

   return 1;
}
//--------- End of function UnitArray::write_file ---------------//


//-------- Start of function UnitArray::read_file -------------//
//
int UnitArray::read_file(File* filePtr)
{
	Unit*   unitPtr;
	int     i, unitId, emptyRoomCount=0;

	restart_recno    = filePtr->file_get_short();

	int unitCount    = filePtr->file_get_short();  // get no. of units from file

	selected_recno   = filePtr->file_get_short();
	selected_count   = filePtr->file_get_short();
	cur_group_id     = filePtr->file_get_long();
	cur_team_id      = filePtr->file_get_long();
	idle_blocked_unit_reset_count = filePtr->file_get_short();
	unit_search_tries	= filePtr->file_get_long ();
	unit_search_tries_flag = (char) filePtr->file_get_short();

   visible_unit_count					= filePtr->file_get_short();
	// unused short*4
	filePtr->file_get_short();
	filePtr->file_get_short();
	filePtr->file_get_short();
	filePtr->file_get_short();

   for( i=1 ; i<=unitCount ; i++ )
   {
      unitId = filePtr->file_get_short();

      if( unitId==0 )  // the unit has been deleted
      {
         add_blank(1);     // it's a DynArrayB function
         emptyRoomCount++;
      }
      else
      {
         //----- create unit object -----------//

			unitPtr = create_unit( unitId );
         unitPtr->unit_id = unitId;

         //---- read data in base class -----//

         if( !unitPtr->read_file( filePtr ) )
            return 0;

         //----- read data in derived class -----//

         if( !unitPtr->read_derived_file( filePtr ) )
            return 0;

			unitPtr->fix_attack_info();
      }
   }

	//-------- linkout() those record added by add_blank() ----------//
   //-- So they will be marked deleted in DynArrayB and can be -----//
	//-- undeleted and used when a new record is going to be added --//

   for( i=size() ; i>0 ; i-- )
	{
		DynArrayB::go(i);             // since UnitArray has its own go() which will call GroupArray::go()

      if( get_ptr() == NULL )       // add_blank() record
         linkout();
   }

   //------- read empty room array --------//

   read_empty_room(filePtr);

   //------- verify the empty_room_array loading -----//

#ifdef DEBUG
   err_when( empty_room_count != emptyRoomCount );

   for( i=0 ; i<empty_room_count ; i++ )
   {
      if( !is_deleted( empty_room_array[i].recno ) )
         err_here();
   }
#endif

   return 1;
}
//--------- End of function UnitArray::read_file ---------------//


//--------- Begin of function Unit::write_file ---------//
//
// Write data in derived class.
//
// If the derived Unit don't have any special data,
// just use Unit::write_file(), otherwise make its own derived copy of write_file()
//
int Unit::write_file(File* filePtr)
{
	write_record(&gf_rec.unit);
	if( !filePtr->file_write(&gf_rec, sizeof(UnitGF)) )
		return 0;

	//--------------- write memory data ----------------//

	if( result_node_array )
	{
		if( !filePtr->file_write( result_node_array, sizeof(ResultNode) * result_node_count ) )
			return 0;
	}

	//### begin alex 15/10 ###//
	if(way_point_array)
	{
		err_when(way_point_array_size==0 || way_point_array_size<way_point_count);
		if(!filePtr->file_write(way_point_array, sizeof(ResultNode)*way_point_array_size))
			return 0;
	}
	//#### end alex 15/10 ####//

	if( team_info )
	{
		if( !filePtr->file_write( team_info, sizeof(TeamInfo) ) )
			return 0;
   }

   return 1;
}
//----------- End of function Unit::write_file ---------//

template <typename Visitor>
static void visit_sprite(Visitor *v, Sprite *s)
{
	v->skip(4); /* virtual table pointer */

	visit<int16_t>(v, &s->sprite_id);
	visit<int16_t>(v, &s->sprite_recno);
	visit<int8_t>(v, &s->mobile_type);
	visit<uint8_t>(v, &s->cur_action);
	visit<uint8_t>(v, &s->cur_dir);
	visit<uint8_t>(v, &s->cur_frame);
	visit<uint8_t>(v, &s->cur_attack);
	visit<uint8_t>(v, &s->final_dir);
	visit<int8_t>(v, &s->turn_delay);
	visit<int8_t>(v, &s->guard_count);
	visit<uint8_t>(v, &s->remain_attack_delay);
	visit<uint8_t>(v, &s->remain_frames_per_step);
	visit<int16_t>(v, &s->cur_x);
	visit<int16_t>(v, &s->cur_y);
	visit<int16_t>(v, &s->go_x);
	visit<int16_t>(v, &s->go_y);
	visit<int16_t>(v, &s->next_x);
	visit<int16_t>(v, &s->next_y);
	visit_pointer(v, &s->sprite_info);
}

//--------- Begin of function Unit::read_file ---------//
//
int Unit::read_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(UnitGF)) )
		return 0;
	read_record(&gf_rec.unit);

	//--------------- read in memory data ----------------//

	if( result_node_array )
	{
		result_node_array = (ResultNode*) mem_add( sizeof(ResultNode) * result_node_count );

		if( !filePtr->file_read( result_node_array, sizeof(ResultNode) * result_node_count ) )
			return 0;
	}

	//### begin alex 15/10 ###//
	if(way_point_array)
	{
		way_point_array = (ResultNode*) mem_add(sizeof(ResultNode) * way_point_array_size);

		if(!filePtr->file_read(way_point_array, sizeof(ResultNode)*way_point_array_size))
			return 0;
	}
	//#### end alex 15/10 ####//

	if( team_info )
	{
		team_info = (TeamInfo*) mem_add( sizeof(TeamInfo) );

		if( !filePtr->file_read( team_info, sizeof(TeamInfo) ) )
			return 0;
	}

	//----------- post-process the data read ----------//

	// attack_info_array = unit_res.attack_info_array+unit_res[unit_id]->first_attack-1;
	sprite_info       = sprite_res[sprite_id];

	sprite_info->load_bitmap_res();

	//--------- special process of UNIT_MARINE --------//

	// move to read_derived_file
	//if( unit_res[unit_id]->unit_class == UNIT_CLASS_SHIP )
	//{
	//	((UnitMarine*)this)->splash.sprite_info = sprite_res[sprite_id];
	//	((UnitMarine*)this)->splash.sprite_info->load_bitmap_res();
	//}

   return 1;
}
//----------- End of function Unit::read_file ---------//


//--------- Begin of function Unit::write_derived_file ---------//
//
int Unit::write_derived_file(File* filePtr)
{
   //--- write data in derived class -----//

	int writeSize = unit_array.unit_class_size(unit_id)-sizeof(Unit);

   if( writeSize > 0 )
   {
      if( !filePtr->file_write( (char*) this + sizeof(Unit), writeSize ) )
         return 0;
   }

   return 1;
}
//----------- End of function Unit::write_derived_file ---------//


//--------- Begin of function Unit::read_derived_file ---------//
//
int Unit::read_derived_file(File* filePtr)
{
	//--- read data in derived class -----//

   int readSize = unit_array.unit_class_size(unit_id) - sizeof(Unit);

   if( readSize > 0 )
   {
      if( !filePtr->file_read( (char*) this + sizeof(Unit), readSize ) )
         return 0;
	}

   return 1;
}
//----------- End of function Unit::read_derived_file ---------//


//--------- Begin of function UnitCaravan::write_derived_file ---------//
int UnitCaravan::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.unit_caravan);
	if( !filePtr->file_write(&gf_rec, sizeof(UnitCaravanGF)) )
		return 0;
	return 1;
}
//--------- End of function UnitCaravan::write_derived_file ---------//


//--------- Begin of function UnitCaravan::read_derived_file ---------//
int UnitCaravan::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(UnitCaravanGF)) )
		return 0;
	read_derived_record(&gf_rec.unit_caravan);
	return 1;
}
//--------- End of function UnitCaravan::read_derived_file ---------//


//--------- Begin of function UnitExpCart::write_derived_file ---------//
int UnitExpCart::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.unit_exp_cart);
	if( !filePtr->file_write(&gf_rec, sizeof(UnitExpCartGF)) )
		return 0;
	return 1;
}
//--------- End of function UnitExpCart::write_derived_file ---------//


//--------- Begin of function UnitExpCart::read_derived_file ---------//
int UnitExpCart::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(UnitExpCartGF)) )
		return 0;
	read_derived_record(&gf_rec.unit_exp_cart);
	return 1;
}
//--------- End of function UnitExpCart::read_derived_file ---------//


//--------- Begin of function UnitGod::write_derived_file ---------//
int UnitGod::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.unit_god);
	if( !filePtr->file_write(&gf_rec, sizeof(UnitGodGF)) )
		return 0;
	return 1;
}
//--------- End of function UnitGod::write_derived_file ---------//


//--------- Begin of function UnitGod::read_derived_file ---------//
int UnitGod::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(UnitGodGF)) )
		return 0;
	read_derived_record(&gf_rec.unit_god);
	return 1;
}
//--------- End of function UnitGod::read_derived_file ---------//


//--------- Begin of function UnitMarine::write_derived_file ---------//
int UnitMarine::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.unit_marine);
	if( !filePtr->file_write(&gf_rec, sizeof(UnitMarineGF)) )
		return 0;
	return 1;
}
//--------- End of function UnitMarine::write_derived_file ---------//


//--------- Begin of function UnitMarine::read_derived_file ---------//
int UnitMarine::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(UnitMarineGF)) )
		return 0;
	read_derived_record(&gf_rec.unit_marine);

	// ------- post-process the data read --------//
	splash.sprite_info = sprite_res[splash.sprite_id];
	splash.sprite_info->load_bitmap_res();

	return 1;
}
//--------- End of function UnitMarine::read_derived_file ---------//


//--------- Begin of function UnitMonster::write_derived_file ---------//
int UnitMonster::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.unit_monster);
	if( !filePtr->file_write(&gf_rec, sizeof(UnitMonsterGF)) )
		return 0;
	return 1;
}
//--------- End of function UnitMonster::write_derived_file ---------//


//--------- Begin of function UnitMonster::read_derived_file ---------//
int UnitMonster::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(UnitMonsterGF)) )
		return 0;
	read_derived_record(&gf_rec.unit_monster);
	return 1;
}
//--------- End of function UnitMonster::read_derived_file ---------//


//--------- Begin of function UnitVehicle::write_derived_file ---------//
int UnitVehicle::write_derived_file(File *filePtr)
{
	write_derived_record(&gf_rec.unit_vehicle);
	if( !filePtr->file_write(&gf_rec, sizeof(UnitVehicleGF)) )
		return 0;
	return 1;
}
//--------- End of function UnitVehicle::write_derived_file ---------//


//--------- Begin of function UnitVehicle::read_derived_file ---------//
int UnitVehicle::read_derived_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(UnitVehicleGF)) )
		return 0;
	read_derived_record(&gf_rec.unit_vehicle);
	return 1;
}
//--------- End of function UnitVehicle::read_derived_file ---------//


//*****//


//-------- Start of function BulletArray::write_file -------------//
//
int BulletArray::write_file(File* filePtr)
{
	filePtr->file_put_short(restart_recno);  // variable in SpriteArray

	int    i, emptyRoomCount=0;;
	Bullet *bulletPtr;

	filePtr->file_put_short( size() );  // no. of bullets in bullet_array

	for( i=1; i<=size() ; i++ )
	{
		bulletPtr = (Bullet*) get_ptr(i);

		//----- write bulletId or 0 if the bullet is deleted -----//

		if( !bulletPtr )    // the bullet is deleted
		{
			filePtr->file_put_short(0);
			emptyRoomCount++;
		}
		else
		{
			filePtr->file_put_short(bulletPtr->sprite_id);      // there is a bullet in this record

			//------ write data in the base class ------//

			if( !bulletPtr->write_file(filePtr) )
				return 0;

			//------ write data in the derived class -------//

			if( !bulletPtr->write_derived_file(filePtr) )
				return 0;
		}
	}

	//------- write empty room array --------//

	write_empty_room(filePtr);

	//------- verify the empty_room_array loading -----//

#ifdef DEBUG
	err_when( empty_room_count != emptyRoomCount );

   for( i=0 ; i<empty_room_count ; i++ )
   {
		if( !is_deleted( empty_room_array[i].recno ) )
         err_here();
   }
#endif

	return 1;
}
//--------- End of function BulletArray::write_file -------------//


//-------- Start of function BulletArray::read_file -------------//
//
int BulletArray::read_file(File* filePtr)
{
	restart_recno    = filePtr->file_get_short();

	int     i, bulletRecno, bulletCount, emptyRoomCount=0, spriteId;
	Bullet* bulletPtr;

	bulletCount = filePtr->file_get_short();  // get no. of bullets from file

	for( i=1 ; i<=bulletCount ; i++ )
	{
		spriteId = filePtr->file_get_short();
		if( spriteId == 0 )
		{
			add_blank(1);     // it's a DynArrayB function

			emptyRoomCount++;
		}
		else
		{
			//----- create bullet object -----------//

			bulletRecno = create_bullet(spriteId);
			bulletPtr   = bullet_array[bulletRecno];

         //----- read data in base class --------//

         if( !bulletPtr->read_file( filePtr ) )
            return 0;

			//----- read data in derived class -----//

			if( !bulletPtr->read_derived_file( filePtr ) )
				return 0;
      }
	}

   //-------- linkout() those record added by add_blank() ----------//
	//-- So they will be marked deleted in DynArrayB and can be -----//
	//-- undeleted and used when a new record is going to be added --//

	for( i=1 ; i<=size() ; i++ )
	{
		DynArrayB::go(i);             // since BulletArray has its own go() which will call GroupArray::go()

		if( get_ptr() == NULL )       // add_blank() record
			linkout();
	}

	//------- read empty room array --------//

	read_empty_room(filePtr);

	//------- verify the empty_room_array loading -----//

#ifdef DEBUG
	err_when( empty_room_count != emptyRoomCount );

	for( i=0 ; i<empty_room_count ; i++ )
	{
		if( !is_deleted( empty_room_array[i].recno ) )
			err_here();
	}
#endif

	return 1;
}
//--------- End of function BulletArray::read_file ---------------//


//--------- Begin of function Bullet::write_file ---------//
//
int Bullet::write_file(File* filePtr)
{
	write_record(&gf_rec.bullet);
	if( !filePtr->file_write(&gf_rec, sizeof(BulletGF)) )
		return 0;
	return 1;
}
//----------- End of function Bullet::write_file ---------//


//--------- Begin of function Bullet::read_file ---------//
//
int Bullet::read_file(File* filePtr)
{
	if( !filePtr->file_read(&gf_rec, sizeof(BulletGF)) )
		return 0;
	read_record(&gf_rec.bullet);

	//------------ post-process the data read ----------//

	sprite_info = sprite_res[sprite_id];

	sprite_info->load_bitmap_res();

	return 1;
}
//----------- End of function Bullet::read_file ---------//


//----------- Begin of function Bullet::write_derived_file ---------//
int Bullet::write_derived_file(File *filePtr)
{
	//--- write data in derived class -----//

	int writeSize = bullet_array.bullet_class_size(sprite_id)-sizeof(Bullet);

	if( writeSize > 0 )
	{
		if( !filePtr->file_write( (char*) this + sizeof(Bullet), writeSize ) )
			return 0;
	}

	return 1;

}
//----------- End of function Bullet::write_derived_file ---------//


//----------- Begin of function Bullet::read_derived_file ---------//
int Bullet::read_derived_file(File *filePtr)
{
	//--- read data in derived class -----//

	int readSize = bullet_array.bullet_class_size(sprite_id) - sizeof(Bullet);

	if( readSize > 0 )
	{
		if( !filePtr->file_read( (char*) this + sizeof(Bullet), readSize ) )
			return 0;
	}

	return 1;
}
//----------- End of function Bullet::read_derived_file ---------//

template <typename Visitor>
static void visit_projectile(Visitor *v, Projectile *p)
{
	visit<float>(v, &p->z_coff);
	visit_sprite(v, &p->act_bullet);
	visit_sprite(v, &p->bullet_shadow);
}

enum { PROJECTILE_RECORD_SIZE = 72 };

//----------- Begin of function Projectile::read_derived_file ---------//

int Projectile::write_derived_file(File *filePtr)
{
	return write_with_record_size(filePtr, this, &visit_projectile<FileWriterVisitor>,
											PROJECTILE_RECORD_SIZE);
}

int Projectile::read_derived_file(File *filePtr)
{
	if (!read_with_record_size(filePtr, this, &visit_projectile<FileReaderVisitor>,
										PROJECTILE_RECORD_SIZE))
		return 0;

   //----------- post-process the data read ----------//
	act_bullet.sprite_info = sprite_res[act_bullet.sprite_id];
	act_bullet.sprite_info->load_bitmap_res();
	bullet_shadow.sprite_info = sprite_res[bullet_shadow.sprite_id];
	bullet_shadow.sprite_info->load_bitmap_res();

	return 1;
}
//----------- End of function Projectile::read_derived_file ---------//

//*****//

template <typename Visitor>
static void visit_firm(Visitor *v, Firm *f)
{
	v->skip(4); /* virtual table pointer */

	visit<int8_t>(v, &f->firm_id);
	visit<int16_t>(v, &f->firm_build_id);
	visit<int16_t>(v, &f->firm_recno);
	visit<int8_t>(v, &f->firm_ai);
	visit<int8_t>(v, &f->ai_processed);
	visit<int8_t>(v, &f->ai_status);
	visit<int8_t>(v, &f->ai_link_checked);
	visit<int8_t>(v, &f->ai_sell_flag);
	visit<int8_t>(v, &f->race_id);
	visit<int16_t>(v, &f->nation_recno);
	visit<int16_t>(v, &f->closest_town_name_id);
	visit<int16_t>(v, &f->firm_name_instance_id);
	visit<int16_t>(v, &f->loc_x1);
	visit<int16_t>(v, &f->loc_y1);
	visit<int16_t>(v, &f->loc_x2);
	visit<int16_t>(v, &f->loc_y2);
	visit<int16_t>(v, &f->abs_x1);
	visit<int16_t>(v, &f->abs_y1);
	visit<int16_t>(v, &f->abs_x2);
	visit<int16_t>(v, &f->abs_y2);
	visit<int16_t>(v, &f->center_x);
	visit<int16_t>(v, &f->center_y);
	visit<uint8_t>(v, &f->region_id);
	visit<int8_t>(v, &f->cur_frame);
	visit<int8_t>(v, &f->remain_frame_delay);
	visit<float>(v, &f->hit_points);
	visit<float>(v, &f->max_hit_points);
	visit<int8_t>(v, &f->under_construction);
	visit<int8_t>(v, &f->firm_skill_id);
	visit<int16_t>(v, &f->overseer_recno);
	visit<int16_t>(v, &f->overseer_town_recno);
	visit<int16_t>(v, &f->builder_recno);
	visit<uint8_t>(v, &f->builder_region_id);
	visit<float>(v, &f->productivity);
	visit_pointer(v, &f->worker_array);
	visit<int8_t>(v, &f->worker_count);
	visit<int8_t>(v, &f->selected_worker_id);
	visit<int8_t>(v, &f->player_spy_count);
	visit<uint8_t>(v, &f->sabotage_level);
	visit<int8_t>(v, &f->linked_firm_count);
	visit<int8_t>(v, &f->linked_town_count);
	visit_array<int16_t>(v, f->linked_firm_array, MAX_LINKED_FIRM_FIRM);
	visit_array<int16_t>(v, f->linked_town_array, MAX_LINKED_FIRM_TOWN);

	visit_array<int8_t>(v, f->linked_firm_enable_array,
							  MAX_LINKED_FIRM_FIRM);

	visit_array<int8_t>(v, f->linked_town_enable_array,
							  MAX_LINKED_FIRM_TOWN);

	visit<float>(v, &f->last_year_income);
	visit<float>(v, &f->cur_year_income);
	visit<int32_t>(v, &f->setup_date);
	visit<int8_t>(v, &f->should_set_power);
	visit<int32_t>(v, &f->last_attacked_date);
	visit<int8_t>(v, &f->should_close_flag);
	visit<int8_t>(v, &f->no_neighbor_space);
	visit<int8_t>(v, &f->ai_should_build_factory_count);
}

enum { FIRM_RECORD_SIZE = 254 };

static bool read_firm(File *file, Firm *firm)
{
	return read_with_record_size(file, firm, &visit_firm<FileReaderVisitor>, FIRM_RECORD_SIZE);
}

static bool write_firm(File *file, Firm *firm)
{
	return write_with_record_size(file, firm, &visit_firm<FileWriterVisitor>, FIRM_RECORD_SIZE);
}

//-------- Start of function FirmArray::write_file -------------//
//
int FirmArray::write_file(File* filePtr)
{
   int  i;
   Firm *firmPtr;

   filePtr->file_put_short( size()  );  // no. of firms in firm_array
   filePtr->file_put_short( process_recno );
	filePtr->file_put_short( selected_recno );

	filePtr->file_put_short( Firm::firm_menu_mode );
	filePtr->file_put_short( Firm::action_spy_recno );
	filePtr->file_put_short( Firm::bribe_result );
	filePtr->file_put_short( Firm::assassinate_result );

	for( i=1; i<=size() ; i++ )
   {
      firmPtr = (Firm*) get_ptr(i);

      //----- write firmId or 0 if the firm is deleted -----//

      if( !firmPtr )    // the firm is deleted
		{
         filePtr->file_put_short(0);
      }
      else
      {
         //--------- write firm_id -------------//

         filePtr->file_put_short(firmPtr->firm_id);

         //------ write data in base class --------//

			if (!write_firm(filePtr, firmPtr))
			  return 0;

         //--------- write worker_array ---------//

         if( firmPtr->worker_array )
         {
            if( !filePtr->file_write( firmPtr->worker_array, MAX_WORKER*sizeof(Worker) ) )
               return 0;
         }

         //------ write data in derived class ------//

         if( !firmPtr->write_derived_file(filePtr) )
            return 0;
      }
   }

   //------- write empty room array --------//

	write_empty_room(filePtr);

   return 1;
}
//--------- End of function FirmArray::write_file ---------------//

//-------- Start of function FirmArray::read_file -------------//
//
int FirmArray::read_file(File* filePtr)
{
	Firm*   firmPtr;
	int     i, firmId, firmRecno;

	int firmCount      = filePtr->file_get_short();  // get no. of firms from file
	process_recno      = filePtr->file_get_short();
	selected_recno     = filePtr->file_get_short();

	Firm::firm_menu_mode  	 = (char) filePtr->file_get_short();
	Firm::action_spy_recno   = filePtr->file_get_short();
	Firm::bribe_result    	 = (char) filePtr->file_get_short();
	Firm::assassinate_result = (char) filePtr->file_get_short();

   for( i=1 ; i<=firmCount ; i++ )
   {
      firmId = filePtr->file_get_short();

      if( firmId==0 )  // the firm has been deleted
      {
         add_blank(1);     // it's a DynArrayB function
      }
      else
      {
         //----- create firm object -----------//

         firmRecno = create_firm( firmId );
         firmPtr   = firm_array[firmRecno];

			if (!read_firm(filePtr, firmPtr))
			  return 0;

         //---- read data in base class -----//

			if(!GameFile::read_file_same_version && firmPtr->firm_id > FIRM_BASE)
				firmPtr->firm_build_id += MAX_RACE - VERSION_1_MAX_RACE;

         //--------- read worker_array ---------//

         if( firm_res[firmId]->need_worker )
         {
            firmPtr->worker_array = (Worker*) mem_add( MAX_WORKER*sizeof(Worker) );

            if( !filePtr->file_read( firmPtr->worker_array, MAX_WORKER*sizeof(Worker) ) )
               return 0;

            firmPtr->sort_worker(); // if this one selected, refresh interface
         }

         //----- read data in derived class -----//

         if( !firmPtr->read_derived_file( filePtr ) )
            return 0;

         if( config_adv.game_file_patching &&
             GameFile::load_file_game_version < 200 &&
             firmPtr->firm_id == FIRM_MARKET )
         {
            FirmMarket* firmMarket = (FirmMarket*) firmPtr;
            // Below game version 200, the restock type was not initialized for
            // human players.
            if( !firmMarket->firm_ai )
               firmMarket->restock_type = 0;
         }
      }
   }

   //-------- linkout() those record added by add_blank() ----------//
   //-- So they will be marked deleted in DynArrayB and can be -----//
   //-- undeleted and used when a new record is going to be added --//

   for( i=size() ; i>0 ; i-- )
   {
      DynArrayB::go(i);             // since FirmArray has its own go() which will call GroupArray::go()

      if( get_ptr() == NULL )       // add_blank() record
         linkout();
   }

   //------- read empty room array --------//

   read_empty_room(filePtr);

   return 1;
}
//--------- End of function FirmArray::read_file ---------------//


//--------- Begin of function Firm::write_derived_file ---------//
//
// Write data in derived class.
//
// If the derived Firm don't have any special data,
// just use Firm::write_file(), otherwise make its own derived copy of write_file()
//
int Firm::write_derived_file(File* filePtr)
{
   //--- write data in derived class -----//

   int writeSize = firm_array.firm_class_size(firm_id)-sizeof(Firm);

   if( writeSize > 0 )
   {
      if( !filePtr->file_write( (char*) this + sizeof(Firm), writeSize ) )
         return 0;
   }

   return 1;
}
//----------- End of function Firm::write_derived_file ---------//


//--------- Begin of function Firm::read_derived_file ---------//
//
// Read data in derived class.
//
// If the derived Firm don't have any special data,
// just use Firm::read_file(), otherwise make its own derived copy of read_file()
//
int Firm::read_derived_file(File* filePtr)
{
   //--- read data in derived class -----//

   int readSize = firm_array.firm_class_size(firm_id)-sizeof(Firm);

   if( readSize > 0 )
   {
		MSG(__FILE__":%d: file_read(this, ...);\n", __LINE__);

      if( !filePtr->file_read( (char*) this + sizeof(Firm), readSize ) )
         return 0;
   }

   return 1;
}
//----------- End of function Firm::read_derived_file ---------//


//*****//


//-------- Start of function SiteArray::write_file -------------//
//
int SiteArray::write_file(File* filePtr)
{
	filePtr->file_put_short(selected_recno);
	filePtr->file_put_short(untapped_raw_count);
	filePtr->file_put_short(scroll_count);
	filePtr->file_put_short(gold_coin_count);
	filePtr->file_put_short(std_raw_site_count);

	return DynArrayB::write_file( filePtr );
}
//--------- End of function SiteArray::write_file ---------------//


//-------- Start of function SiteArray::read_file -------------//
//
int SiteArray::read_file(File* filePtr)
{
	selected_recno		 = filePtr->file_get_short();
	untapped_raw_count =	filePtr->file_get_short();
	scroll_count		 = filePtr->file_get_short();
	gold_coin_count	 =	filePtr->file_get_short();
	std_raw_site_count =	filePtr->file_get_short();

	return DynArrayB::read_file( filePtr );
}
//--------- End of function SiteArray::read_file ---------------//


//*****//


//-------- Start of function TownArray::write_file -------------//
//
int TownArray::write_file(File* filePtr)
{
   int  i;
   Town *townPtr;

	filePtr->file_put_short( size()  );  // no. of towns in town_array
	filePtr->file_put_short( selected_recno );
	filePtr->file_write( race_wander_pop_array, sizeof(race_wander_pop_array) );

	filePtr->file_put_short( Town::if_town_recno );

	//-----------------------------------------//

	for( i=1; i<=size() ; i++ )
	{
		townPtr = (Town*) get_ptr(i);

      //----- write townId or 0 if the town is deleted -----//

      if( !townPtr )    // the town is deleted
      {
         filePtr->file_put_short(0);
      }
      else
		{
			#ifdef DEBUG
				townPtr->verify_slot_object_id_array();		// for debugging only
			#endif

			filePtr->file_put_short(1);      // the town exists

         if( !filePtr->file_write( townPtr, sizeof(Town) - Town::SIZEOF_NONSAVED_ELEMENTS ) )
            return 0;
      }
   }

   //------- write empty room array --------//

   write_empty_room(filePtr);

   return 1;
}
//--------- End of function TownArray::write_file ---------------//


//-------- Start of function TownArray::read_file -------------//
//
int TownArray::read_file(File* filePtr)
{
   Town*   townPtr;
   int     i;

	int townCount = filePtr->file_get_short();  // get no. of towns from file
	selected_recno = filePtr->file_get_short();

	if(!GameFile::read_file_same_version)
	{
		memset(race_wander_pop_array, 0, sizeof(race_wander_pop_array));
		filePtr->file_read( race_wander_pop_array, sizeof(race_wander_pop_array[0])*VERSION_1_MAX_RACE );
	}
	else
		filePtr->file_read( race_wander_pop_array, sizeof(race_wander_pop_array) );

	Town::if_town_recno = filePtr->file_get_short();

	//------------------------------------------//

	for( i=1 ; i<=townCount ; i++ )
	{
		if( filePtr->file_get_short()==0 )  // the town has been deleted
		{
			add_blank(1);     // it's a DynArrayB function
		}
		else
		{
			townPtr = town_array.create_town();

			if(!GameFile::read_file_same_version)
			{
				Version_1_Town *oldTown = (Version_1_Town*) mem_add(sizeof(Version_1_Town));
				if(!filePtr->file_read(oldTown, sizeof(Version_1_Town)))
				{
					mem_del(oldTown);
					return 0;
				}

				oldTown->convert_to_version_2(townPtr);
				mem_del(oldTown);
			}
			else
			{
				if( !filePtr->file_read( townPtr, sizeof(Town) - Town::SIZEOF_NONSAVED_ELEMENTS ) )
					return 0;
			}

			#ifdef DEBUG
				townPtr->verify_slot_object_id_array();		// for debugging only
			#endif
		}
	}

	//-------- linkout() those record added by add_blank() ----------//
	//-- So they will be marked deleted in DynArrayB and can be -----//
	//-- undeleted and used when a new record is going to be added --//

	for( i=size() ; i>0 ; i-- )
	{
		DynArrayB::go(i);             // since TownArray has its own go() which will call GroupArray::go()

		if( get_ptr() == NULL )       // add_blank() record
			linkout();
	}

	//------- read empty room array --------//

	read_empty_room(filePtr);

	return 1;
}
//--------- End of function TownArray::read_file ---------------//


//*****//


//-------- Start of function NationArray::write_file -------------//
//
int NationArray::write_file(File* filePtr)
{
	//------ write info in NationArray ------//

	write_record(&gf_rec.nation_array);
	if( !filePtr->file_write(&gf_rec, sizeof(NationArrayGF)) )
		return 0;

   //---------- write Nations --------------//

   int    i;
   Nation *nationPtr;

   filePtr->file_put_short( size() );  // no. of nations in nation_array

   for( i=1; i<=size() ; i++ )
   {
      nationPtr = (Nation*) get_ptr(i);

      //----- write nationId or 0 if the nation is deleted -----//

      if( !nationPtr )    // the nation is deleted
      {
         filePtr->file_put_short(0);
      }
      else
      {
         filePtr->file_put_short(1);      // there is a nation in this record

         //------ write data in the base class ------//

         if( !nationPtr->write_file(filePtr) )
            return 0;
      }
   }

   //------- write empty room array --------//

   write_empty_room(filePtr);

   return 1;
}
//--------- End of function NationArray::write_file -------------//


//-------- Start of function NationArray::read_file -------------//
//
int NationArray::read_file(File* filePtr)
{
   //------ read info in NationArray ------//
	if(!GameFile::read_file_same_version)
	{
		Version_1_NationArray *oldNationArrayPtr = (Version_1_NationArray*) mem_add(sizeof(Version_1_NationArray));
		if( !filePtr->file_read(&gf_rec, sizeof(Version_1_NationArrayGF)) )
		{
			mem_del(oldNationArrayPtr);
			return 0;
		}
		oldNationArrayPtr->read_record(&gf_rec.nation_array_v1);
		oldNationArrayPtr->convert_to_version_2(this);
		mem_del(oldNationArrayPtr);
	}
	else
	{
		if( !filePtr->file_read(&gf_rec, sizeof(NationArrayGF)) )
			return 0;
		read_record(&gf_rec.nation_array);
	}

   //---------- read Nations --------------//

   int     i, nationRecno, nationCount;
   Nation* nationPtr;

   nationCount = filePtr->file_get_short();  // get no. of nations from file

   for( i=1 ; i<=nationCount ; i++ )
   {
      if( filePtr->file_get_short() == 0 )
      {
         add_blank(1);     // it's a DynArrayB function
      }
      else
      {
         //----- create nation object -----------//

         nationRecno = create_nation();
         nationPtr   = nation_array[nationRecno];

         //----- read data in base class --------//

         if( !nationPtr->read_file( filePtr ) )
            return 0;
      }
   }

   //-------- linkout() those record added by add_blank() ----------//
   //-- So they will be marked deleted in DynArrayB and can be -----//
   //-- undeleted and used when a new record is going to be added --//

   for( i=size() ; i>0 ; i-- )
   {
      DynArrayB::go(i);             // since NationArray has its own go() which will call GroupArray::go()

      if( get_ptr() == NULL )       // add_blank() record
         linkout();
   }

	//-------- set NationArray::player_ptr -----------//

   player_ptr = nation_array[player_recno];

	//------- read empty room array --------//

	read_empty_room(filePtr);

	return 1;
}
//--------- End of function NationArray::read_file ---------------//


//--------- Begin of function Nation::write_file ---------//
//
int Nation::write_file(File* filePtr)
{
	write_record(&gf_rec.nation);
	if( !filePtr->file_write(&gf_rec, sizeof(NationGF)) )
		return 0;

	//----------- write AI Action Array ------------//

	action_array.write_record(&gf_rec.dyn_array);
	if( !filePtr->file_write(&gf_rec, sizeof(DynArrayGF)) )
		return 0;
	if( action_array.last_ele )
	{
		int action_node_bytes = sizeof(ActionNodeGF)*action_array.last_ele;
		ActionNodeGF *action_node_array = (ActionNodeGF*)mem_add(action_node_bytes);
		for( int i=1; i<=action_array.last_ele; i++ )
		{
			ActionNode *actionNodePtr = (ActionNode*)action_array.get(i);
			actionNodePtr->write_record(action_node_array+i-1);
		}
		if( !filePtr->file_write(action_node_array, action_node_bytes) )
		{
			mem_del(action_node_array);
			return 0;
		}
		mem_del(action_node_array);
	}

	//------ write AI info array ---------//

	write_ai_info(filePtr, ai_town_array, ai_town_count, ai_town_size);

	write_ai_info(filePtr, ai_base_array, ai_base_count, ai_base_size);
	write_ai_info(filePtr, ai_mine_array, ai_mine_count, ai_mine_size);
	write_ai_info(filePtr, ai_factory_array, ai_factory_count, ai_factory_size);
	write_ai_info(filePtr, ai_market_array, ai_market_count, ai_market_size);
	write_ai_info(filePtr, ai_inn_array, ai_inn_count, ai_inn_size);
	write_ai_info(filePtr, ai_camp_array, ai_camp_count, ai_camp_size);
	write_ai_info(filePtr, ai_research_array, ai_research_count, ai_research_size);
	write_ai_info(filePtr, ai_war_array, ai_war_count, ai_war_size);
	write_ai_info(filePtr, ai_harbor_array, ai_harbor_count, ai_harbor_size);

	write_ai_info(filePtr, ai_caravan_array, ai_caravan_count, ai_caravan_size);
	write_ai_info(filePtr, ai_ship_array, ai_ship_count, ai_ship_size);
	write_ai_info(filePtr, ai_general_array, ai_general_count, ai_general_size);

	return 1;
}
//----------- End of function Nation::write_file ---------//


//--------- Begin of static function write_ai_info ---------//
//
static void write_ai_info(File* filePtr, short* aiInfoArray, short aiInfoCount, short aiInfoSize)
{
	filePtr->file_put_short( aiInfoCount );
	filePtr->file_put_short( aiInfoSize  );
	filePtr->file_write( aiInfoArray, sizeof(short) * aiInfoCount );
}
//----------- End of static function write_ai_info ---------//


//--------- Begin of function Nation::read_file ---------//
//
int Nation::read_file(File* filePtr)
{
	if(!GameFile::read_file_same_version)
	{
		Version_1_Nation *oldNationPtr = (Version_1_Nation*) mem_add(sizeof(Version_1_Nation));
		if( !filePtr->file_read(&gf_rec, sizeof(Version_1_NationGF)) )
		{
			mem_del(oldNationPtr);
			return 0;
		}
		oldNationPtr->read_record(&gf_rec.nation_v1);
		oldNationPtr->convert_to_version_2(this);
		mem_del(oldNationPtr);
	}
	else
	{
		if( !filePtr->file_read(&gf_rec, sizeof(NationGF)) )
			return 0;
		read_record(&gf_rec.nation);
	}

	//-------------- read AI Action Array --------------//

	if( !filePtr->file_read(&gf_rec, sizeof(DynArrayGF)) )
		return 0;
	action_array.read_record(&gf_rec.dyn_array);
	action_array.resize(action_array.ele_num); //alloc body_buf again
	if( action_array.last_ele )
	{
		int action_node_bytes = sizeof(ActionNodeGF)*action_array.last_ele;
		ActionNodeGF *action_node_array = (ActionNodeGF*)mem_add(action_node_bytes);
		if( !filePtr->file_read(action_node_array, action_node_bytes) )
		{
			mem_del(action_node_array);
			return 0;
		}
		for( int i=1; i<=action_array.last_ele; i++ )
		{
			ActionNode *actionNodePtr = (ActionNode*)action_array.get(i);
			actionNodePtr->read_record(action_node_array+i-1);
		}
		mem_del(action_node_array);
	}

	//------ write AI info array ---------//

	read_ai_info(filePtr, &ai_town_array, ai_town_count, ai_town_size);

	read_ai_info(filePtr, &ai_base_array, ai_base_count, ai_base_size);
	read_ai_info(filePtr, &ai_mine_array, ai_mine_count, ai_mine_size);
	read_ai_info(filePtr, &ai_factory_array, ai_factory_count, ai_factory_size);
	read_ai_info(filePtr, &ai_market_array, ai_market_count, ai_market_size);
	read_ai_info(filePtr, &ai_inn_array, ai_inn_count, ai_inn_size);
	read_ai_info(filePtr, &ai_camp_array, ai_camp_count, ai_camp_size);
	read_ai_info(filePtr, &ai_research_array, ai_research_count, ai_research_size);
	read_ai_info(filePtr, &ai_war_array, ai_war_count, ai_war_size);
	read_ai_info(filePtr, &ai_harbor_array, ai_harbor_count, ai_harbor_size);

	read_ai_info(filePtr, &ai_caravan_array, ai_caravan_count, ai_caravan_size);
	read_ai_info(filePtr, &ai_ship_array, ai_ship_count, ai_ship_size);
	read_ai_info(filePtr, &ai_general_array, ai_general_count, ai_general_size);

	return 1;
}
//----------- End of function Nation::read_file ---------//


//--------- Begin of static function read_ai_info ---------//
//
static void read_ai_info(File* filePtr, short** aiInfoArrayPtr, short& aiInfoCount, short& aiInfoSize)
{
	aiInfoCount = filePtr->file_get_short();
	aiInfoSize  = filePtr->file_get_short();

	*aiInfoArrayPtr = (short*) mem_add( aiInfoSize * sizeof(short) );

	filePtr->file_read( *aiInfoArrayPtr, sizeof(short) * aiInfoCount );
}
//----------- End of static function read_ai_info ---------//

//*****//

//-------- Start of function TornadoArray::write_file -------------//
//
int TornadoArray::write_file(File* filePtr)
{
	filePtr->file_put_short(restart_recno);  // variable in SpriteArray

	int    i;
   Tornado *tornadoPtr;

   filePtr->file_put_short( size() );  // no. of tornados in tornado_array

   for( i=1; i<=size() ; i++ )
   {
      tornadoPtr = (Tornado*) get_ptr(i);

      //----- write tornadoId or 0 if the tornado is deleted -----//

      if( !tornadoPtr )    // the tornado is deleted
      {
         filePtr->file_put_short(0);
      }
      else
      {
         filePtr->file_put_short(1);      // there is a tornado in this record

         //------ write data in the base class ------//

         if( !tornadoPtr->write_file(filePtr) )
            return 0;
      }
   }

   //------- write empty room array --------//

   write_empty_room(filePtr);

   return 1;
}
//--------- End of function TornadoArray::write_file -------------//


//-------- Start of function TornadoArray::read_file -------------//
//
int TornadoArray::read_file(File* filePtr)
{
	restart_recno    = filePtr->file_get_short();

   int     i, tornadoRecno, tornadoCount;
   Tornado* tornadoPtr;

   tornadoCount = filePtr->file_get_short();  // get no. of tornados from file

   for( i=1 ; i<=tornadoCount ; i++ )
   {
      if( filePtr->file_get_short() == 0 )
      {
         add_blank(1);     // it's a DynArrayB function
      }
      else
      {
         //----- create tornado object -----------//

         tornadoRecno = tornado_array.create_tornado();
         tornadoPtr   = tornado_array[tornadoRecno];

         //----- read data in base class --------//

         if( !tornadoPtr->read_file( filePtr ) )
            return 0;
      }
   }

   //-------- linkout() those record added by add_blank() ----------//
   //-- So they will be marked deleted in DynArrayB and can be -----//
   //-- undeleted and used when a new record is going to be added --//

   for( i=size() ; i>0 ; i-- )
   {
      DynArrayB::go(i);             // since TornadoArray has its own go() which will call GroupArray::go()

      if( get_ptr() == NULL )       // add_blank() record
         linkout();
   }

   //------- read empty room array --------//

   read_empty_room(filePtr);

   return 1;
}
//--------- End of function TornadoArray::read_file ---------------//

template <typename Visitor>
static void visit_tornado(Visitor *v, Tornado *t)
{
	visit_sprite(v, t);
   visit<float>(v, &t->attack_damage);
   visit<int16_t>(v, &t->life_time);
   visit<int16_t>(v, &t->dmg_offset_x);
   visit<int16_t>(v, &t->dmg_offset_y);
}

enum { TORNADO_RECORD_SIZE = 44 };

//--------- Begin of function Tornado::write_file ---------//
//
int Tornado::write_file(File* filePtr)
{
	return write_with_record_size(filePtr, this, &visit_tornado<FileWriterVisitor>,
											TORNADO_RECORD_SIZE);
}
//----------- End of function Tornado::write_file ---------//

//--------- Begin of function Tornado::read_file ---------//
//
int Tornado::read_file(File* filePtr)
{
	if (!read_with_record_size(filePtr, this, &visit_tornado<FileReaderVisitor>,
										TORNADO_RECORD_SIZE))
		return 0;

   //------------ post-process the data read ----------//

   sprite_info = sprite_res[sprite_id];

	sprite_info->load_bitmap_res();

	return 1;
}
//----------- End of function Tornado::read_file ---------//


//*****//


//-------- Start of function RebelArray::write_file -------------//
//
int RebelArray::write_file(File* filePtr)
{
	return write_ptr_array(filePtr, sizeof(Rebel));
}
//--------- End of function RebelArray::write_file ---------------//


//-------- Start of function RebelArray::read_file -------------//
//
int RebelArray::read_file(File* filePtr)
{
	return read_ptr_array(filePtr, sizeof(Rebel), create_rebel_func);
}
//--------- End of function RebelArray::read_file ---------------//


//-------- Start of static function create_rebel_func ---------//
//
static char* create_rebel_func()
{
	Rebel *rebelPtr = new Rebel;

	rebel_array.linkin(&rebelPtr);

	return (char*) rebelPtr;
}
//--------- End of static function create_rebel_func ----------//


//*****//


//-------- Start of function SpyArray::write_file -------------//
//
int SpyArray::write_file(File* filePtr)
{
	filePtr->file_put_unsigned_short(29); // sizeof(DynArray)
	// write DynArray -- 29 bytes
	filePtr->file_put_long(ele_num);
	filePtr->file_put_long(block_num);
	filePtr->file_put_long(cur_pos);
	filePtr->file_put_long(last_ele);
	filePtr->file_put_long(15); // sizeof(Spy)
	filePtr->file_put_long(sort_offset);
	filePtr->file_put_char(sort_type);
	filePtr->file_put_long(0);

        //---------- write body_buf ---------//

	if( last_ele > 0 )
	{
		Spy spyBlank;
		memset(&spyBlank, 0, sizeof(spyBlank));

		filePtr->file_put_unsigned_short(15*last_ele); // sizeof(Spy)*last_ele

		for( int i=1; i<=last_ele; i++ )
		{
			Spy* spyPtr = (Spy*) get_ptr(i);
			if( !spyPtr )
				spyPtr = &spyBlank;

			// write Spy -- 15 bytes
			filePtr->file_put_short(spyPtr->spy_recno);
			filePtr->file_put_char(spyPtr->spy_place);
			filePtr->file_put_short(spyPtr->spy_place_para);
			filePtr->file_put_char(spyPtr->spy_skill);
			filePtr->file_put_char(spyPtr->spy_loyalty);
			filePtr->file_put_char(spyPtr->true_nation_recno);
			filePtr->file_put_char(spyPtr->cloaked_nation_recno);
			filePtr->file_put_char(spyPtr->notify_cloaked_nation_flag);
			filePtr->file_put_char(spyPtr->exposed_flag);
			filePtr->file_put_char(spyPtr->race_id);
			filePtr->file_put_unsigned_short(spyPtr->name_id);
			filePtr->file_put_char(spyPtr->action_mode);
		}
	}

	//---------- write empty_room_array ---------//

	write_empty_room(filePtr);

	return 1;
}
//--------- End of function SpyArray::write_file ---------------//


//-------- Start of function SpyArray::read_file -------------//
//
int SpyArray::read_file(File* filePtr)
{
	unsigned short recSize = filePtr->file_get_unsigned_short();
	if( recSize != 29 )
		return 0;

	int32_t fileEleNum = filePtr->file_get_long(); // skip overwriting ele_num
	resize(fileEleNum);
	filePtr->file_get_long(); // skip overwriting block_num
	filePtr->file_get_long(); // skip overwriting cur_pos
	int32_t readNum = filePtr->file_get_long(); // skip overwriting last_ele
	filePtr->file_get_long(); // skip overwriting ele_size
	filePtr->file_get_long(); // skip overwriting sort_offset
	filePtr->file_get_char(); // skip overwriting sort_type
	filePtr->file_get_long();

	//---------- read body_buf ---------//

	if( readNum > 0 )
	{
		filePtr->file_get_unsigned_short(); // skip body_buf record len

		for( int i=0; i<readNum; i++ )
		{
			int spy_recno = filePtr->file_get_short();

			if( spy_recno )
			{
				spy_recno = spy_array.add_spy();

				Spy* spyPtr = spy_array[spy_recno];
				//spyPtr->spy_recno = spy_recno;
				spyPtr->spy_place = filePtr->file_get_char();
				spyPtr->spy_place_para = filePtr->file_get_short();
				spyPtr->spy_skill = filePtr->file_get_char();
				spyPtr->spy_loyalty = filePtr->file_get_char();
				spyPtr->true_nation_recno = filePtr->file_get_char();
				spyPtr->cloaked_nation_recno = filePtr->file_get_char();
				spyPtr->notify_cloaked_nation_flag = filePtr->file_get_char();
				spyPtr->exposed_flag = filePtr->file_get_char();
				spyPtr->race_id = filePtr->file_get_char();
				spyPtr->name_id = filePtr->file_get_unsigned_short();
				spyPtr->action_mode = filePtr->file_get_char();
			}
			else
			{
				add_blank(1);     // it's a DynArrayB function

				// read 13 zeroed bytes
				filePtr->file_get_char(); //skip spy_place
				filePtr->file_get_short(); //skip spy_place_para
				filePtr->file_get_char(); //skip spy_skill
				filePtr->file_get_char(); // skip spy_loyalty
				filePtr->file_get_char(); // skip true_nation_recno
				filePtr->file_get_char(); // skip cloaked_nation_recno
				filePtr->file_get_char(); // skip notify_cloaked_nation_flag
				filePtr->file_get_char(); // skip exposed_flag
				filePtr->file_get_char(); // skip race_id
				filePtr->file_get_unsigned_short(); // skip name_id
				filePtr->file_get_char(); // skip action_mode
			}
		}
	}

	//---------- read empty_room_array ---------//

	read_empty_room(filePtr);

	//------------------------------------------//

	start();    // go top

	return 1;
}
//--------- End of function SpyArray::read_file ---------------//


//*****//


//-------- Start of function SnowGroundArray::write_file -------------//
//
int SnowGroundArray::write_file(File* filePtr)
{
	MSG(__FILE__":%d: file_write(this, ...);\n", __LINE__);

   if( !filePtr->file_write( this, sizeof(SnowGroundArray)) )
      return 0;

   return 1;
}
//--------- End of function SnowGroundArray::write_file ---------------//


//-------- Start of function SnowGroundArray::read_file -------------//
//
int SnowGroundArray::read_file(File* filePtr)
{
	MSG(__FILE__":%d: file_read(this, ...);\n", __LINE__);

   if( !filePtr->file_read( this, sizeof(SnowGroundArray)) )
      return 0;

   return 1;
}
//--------- End of function SnowGroundArray::read_file ---------------//

//*****//

template <typename Visitor>
static void visit_region_array(Visitor *v, RegionArray *ra)
{
	visit<int32_t>(v, &ra->init_flag);
	visit_pointer(v, &ra->region_info_array);
	visit<int32_t>(v, &ra->region_info_count);
	visit_pointer(v, &ra->region_stat_array);
	visit<int32_t>(v, &ra->region_stat_count);
	visit_pointer(v, &ra->connect_bits);
	visit_array<uint8_t>(v, ra->region_sorted_array, MAX_REGION);
}

enum { REGION_ARRAY_RECORD_SIZE = 279 };

//-------- Start of function RegionArray::write_file -------------//
//
int RegionArray::write_file(File* filePtr)
{
	if (!write_with_record_size(filePtr, this, &visit_region_array<FileWriterVisitor>,
										 REGION_ARRAY_RECORD_SIZE))
		return 0;

	if( !filePtr->file_write( region_info_array, sizeof(RegionInfo)*region_info_count ) )
		return 0;

	//-------- write RegionStat ----------//

	filePtr->file_put_short( region_stat_count );

	if( !filePtr->file_write( region_stat_array, sizeof(RegionStat)*region_stat_count ) )
		return 0;

	//--------- write connection bits ----------//

	int connectBit = (region_info_count -1) * (region_info_count) /2;
	int connectByte = (connectBit +7) /8;

	if( connectByte > 0)
	{
		if( !filePtr->file_write(connect_bits, connectByte) )
			return 0;
	}

	return 1;
}
//--------- End of function RegionArray::write_file ---------------//


//-------- Start of function RegionArray::read_file -------------//
//
int RegionArray::read_file(File* filePtr)
{
	if (!read_with_record_size(filePtr, this, &visit_region_array<FileReaderVisitor>,
										REGION_ARRAY_RECORD_SIZE))
		return 0;

   if( region_info_count > 0 )
      region_info_array = (RegionInfo *) mem_add(sizeof(RegionInfo)*region_info_count);
   else
      region_info_array = NULL;

   if( !filePtr->file_read( region_info_array, sizeof(RegionInfo)*region_info_count))
      return 0;

	//-------- read RegionStat ----------//

	region_stat_count = filePtr->file_get_short();

	region_stat_array = (RegionStat*) mem_add( region_stat_count * sizeof(RegionStat) );

	if( !filePtr->file_read( region_stat_array, sizeof(RegionStat)*region_stat_count ) )
		return 0;

	//--------- read connection bits ----------//

	int connectBit = (region_info_count -1) * (region_info_count) /2;
	int connectByte = (connectBit +7) /8;

	if( connectByte > 0)
	{
		connect_bits = (unsigned char *)mem_add(connectByte);
		if( !filePtr->file_read(connect_bits, connectByte) )
			return 0;
	}
	else
	{
		connect_bits = NULL;
	}

	return 1;
}
//--------- End of function RegionArray::read_file ---------------//

//*****//

//-------- Start of function NewsArray::write_file -------------//
//
int NewsArray::write_file(File* filePtr)
{
   //----- save news_array parameters -----//

   filePtr->file_write( news_type_option, sizeof(news_type_option) );

   filePtr->file_put_short(news_who_option);
   filePtr->file_put_long (last_clear_recno);

   //---------- save news data -----------//

   return DynArray::write_file(filePtr);
}
//--------- End of function NewsArray::write_file ---------------//


//-------- Start of function NewsArray::read_file -------------//
//
int NewsArray::read_file(File* filePtr)
{
   //----- read news_array parameters -----//

   filePtr->file_read( news_type_option, sizeof(news_type_option) );

   news_who_option   = (char) filePtr->file_get_short();
   last_clear_recno  = filePtr->file_get_long();

   //---------- read news data -----------//

   return DynArray::read_file(filePtr);
}
//--------- End of function NewsArray::read_file ---------------//
