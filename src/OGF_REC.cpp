/*
 * Seven Kingdoms: Ancient Adversaries
 *
 * Cpyright 2024 Jesse Allen
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

//Filename    : OGF_REC.CPP
//Description : Read and write records to file and their objects
//Owner       : Jesse Allen

#include <OGF_REC.h>

#define ReadInt8(a) a=r->a
#define ReadInt16(a) a=SDL_SwapLE16(r->a)
#define ReadInt32(a) a=SDL_SwapLE32(r->a)
#define ReadFloat(a) a=SDL_SwapFloatLE(r->a)
#define ReadCall(a) a.read_record(&r->a)

#define ReadInt8Array(a,n) memcpy(&a,&r->a,n)
#define ReadInt16Array(a,n) for(int __i=0;__i<(n);__i++) ReadInt16(a[__i])
#define ReadInt32Array(a,n) for(int __i=0;__i<(n);__i++) ReadInt32(a[__i])
#define ReadFloatArray(a,n) for(int __i=0;__i<(n);__i++) ReadFloat(a[__i])
#define ReadCallArray(a,n) for(int __i=0;__i<(n);__i++) ReadCall(a[__i])

#define WriteInt8(a) r->a=a
#define WriteInt16(a) r->a=SDL_SwapLE16(a)
#define WriteInt32(a) r->a=SDL_SwapLE32(a)
#define WriteFloat(a) r->a=SDL_SwapFloatLE(a)
#define WriteCall(a) a.write_record(&r->a)
#define WriteZero(a) r->a=0

#define WriteInt8Array(a,n) memcpy(&r->a,&a,n)
#define WriteInt16Array(a,n) for(int __i=0;__i<(n);__i++) WriteInt16(a[__i])
#define WriteInt32Array(a,n) for(int __i=0; __i<(n); __i++) WriteInt32(a[__i])
#define WriteFloatArray(a,n) for(int __i=0;__i<(n);__i++) WriteFloat(a[__i])
#define WriteCallArray(a,n) for(int __i=0;__i<(n);__i++) WriteCall(a[__i])
#define WriteZeroBytes(a,n) memset(&r->a,0,n)

union GFRec gf_rec;

void DynArray::write_record(DynArrayGF *r)
{
	WriteInt32(ele_num);
	WriteInt32(block_num);
	WriteInt32(cur_pos);
	WriteInt32(last_ele);
	WriteInt32(ele_size);
	WriteInt32(sort_offset);
	WriteInt8(sort_type);
	WriteZero(body_buf); // the unknown array type must be saved by derived users
}

void DynArray::read_record(DynArrayGF *r)
{
	ReadInt32(ele_num);
	ReadInt32(block_num);
	ReadInt32(cur_pos);
	ReadInt32(last_ele);
	ReadInt32(ele_size);
	ReadInt32(sort_offset);
	ReadInt8(sort_type);
	// skip body_buf // the unknown array type must be restored by derived users
}

void Skill::write_record(SkillGF *r)
{
	WriteInt8(combat_level);
	WriteInt8(skill_id);
	WriteInt8(skill_level);
	WriteInt8(combat_level_minor);
	WriteInt8(skill_level_minor);
	WriteInt8(skill_potential);
}

void Skill::read_record(SkillGF *r)
{
	ReadInt8(combat_level);
	ReadInt8(skill_id);
	ReadInt8(skill_level);
	ReadInt8(combat_level_minor);
	ReadInt8(skill_level_minor);
	ReadInt8(skill_potential);
}

void Unit::write_record(UnitGF *r)
{
	WriteZero(vtp);

	// Sprite
	WriteInt16(sprite_id);
	WriteInt16(sprite_recno);
	WriteInt8(mobile_type);
	WriteInt8(cur_action);
	WriteInt8(cur_dir);
	WriteInt8(cur_frame);
	WriteInt8(cur_attack);
	WriteInt8(final_dir);
	WriteInt8(turn_delay);
	WriteInt8(guard_count);
	WriteInt8(remain_attack_delay);
	WriteInt8(remain_frames_per_step);
	WriteInt16(cur_x);
	WriteInt16(cur_y);
	WriteInt16(go_x);
	WriteInt16(go_y);
	WriteInt16(next_x);
	WriteInt16(next_y);
	WriteZero(sprite_info);

	// Unit
	WriteInt8(unit_id);
	WriteInt8(rank_id);
	WriteInt8(race_id);
	WriteInt8(nation_recno);
	WriteInt8(ai_unit);
	WriteInt16(name_id);
	WriteInt32(unit_group_id);
	WriteInt32(team_id);
	WriteInt8(selected_flag);
	WriteInt8(group_select_id);
	WriteInt8(waiting_term);
	WriteInt8(blocked_by_member);
	WriteInt8(swapping);
	WriteInt16(leader_unit_recno);
	WriteInt8(action_misc);
	WriteInt16(action_misc_para);
	WriteInt8(action_mode);
	WriteInt16(action_para);
	WriteInt16(action_x_loc);
	WriteInt16(action_y_loc);
	WriteInt8(action_mode2);
	WriteInt16(action_para2);
	WriteInt16(action_x_loc2);
	WriteInt16(action_y_loc2);
	WriteInt8Array(blocked_edge, 4);
	WriteInt8(attack_dir);
	WriteInt16(range_attack_x_loc);
	WriteInt16(range_attack_y_loc);
	WriteInt16(move_to_x_loc);
	WriteInt16(move_to_y_loc);
	WriteInt8(loyalty);
	WriteInt8(target_loyalty);
	WriteFloat(hit_points);
	WriteInt16(max_hit_points);
	WriteCall(skill);
	WriteInt8(unit_mode);
	WriteInt16(unit_mode_para);
	WriteInt16(spy_recno);
	WriteInt16(nation_contribution);
	WriteInt16(total_reward);
	WriteZero(attack_info_array);
	WriteInt8(attack_count);
	WriteInt8(attack_range);
	WriteInt16(cur_power);
	WriteInt16(max_power);
	r->has_result_node_array = result_node_array ? 0xdeadbeef : 0;
	WriteInt32(result_node_count);
	WriteInt16(result_node_recno);
	WriteInt16(result_path_dist);
	r->has_way_point_array = way_point_array ? 0xdeadbeef : 0;
	WriteInt16(way_point_array_size);
	WriteInt16(way_point_count);
	WriteInt16(ai_action_id);
	WriteInt8(original_action_mode);
	WriteInt16(original_action_para);
	WriteInt16(original_action_x_loc);
	WriteInt16(original_action_y_loc);
	WriteInt16(original_target_x_loc);
	WriteInt16(original_target_y_loc);
	WriteInt16(ai_original_target_x_loc);
	WriteInt16(ai_original_target_y_loc);
	WriteInt8(ai_no_suitable_action);
	WriteInt8(can_guard_flag);
	WriteInt8(can_attack_flag);
	WriteInt8(force_move_flag);
	WriteInt16(home_camp_firm_recno);
	WriteInt8(aggressive_mode);
	WriteInt8(seek_path_fail_count);
	WriteInt8(ignore_power_nation);
	r->has_team_info = team_info ? 0xdeadbeef : 0;
}

void Unit::read_record(UnitGF *r)
{
	// skip vtp

	// Sprite
	ReadInt16(sprite_id);
	ReadInt16(sprite_recno);
	ReadInt8(mobile_type);
	ReadInt8(cur_action);
	ReadInt8(cur_dir);
	ReadInt8(cur_frame);
	ReadInt8(cur_attack);
	ReadInt8(final_dir);
	ReadInt8(turn_delay);
	ReadInt8(guard_count);
	ReadInt8(remain_attack_delay);
	ReadInt8(remain_frames_per_step);
	ReadInt16(cur_x);
	ReadInt16(cur_y);
	ReadInt16(go_x);
	ReadInt16(go_y);
	ReadInt16(next_x);
	ReadInt16(next_y);
	// skip sprite_info

	// Unit
	ReadInt8(unit_id);
	ReadInt8(rank_id);
	ReadInt8(race_id);
	ReadInt8(nation_recno);
	ReadInt8(ai_unit);
	ReadInt16(name_id);
	ReadInt32(unit_group_id);
	ReadInt32(team_id);
	ReadInt8(selected_flag);
	ReadInt8(group_select_id);
	ReadInt8(waiting_term);
	ReadInt8(blocked_by_member);
	ReadInt8(swapping);
	ReadInt16(leader_unit_recno);
	ReadInt8(action_misc);
	ReadInt16(action_misc_para);
	ReadInt8(action_mode);
	ReadInt16(action_para);
	ReadInt16(action_x_loc);
	ReadInt16(action_y_loc);
	ReadInt8(action_mode2);
	ReadInt16(action_para2);
	ReadInt16(action_x_loc2);
	ReadInt16(action_y_loc2);
	ReadInt8Array(blocked_edge, 4);
	ReadInt8(attack_dir);
	ReadInt16(range_attack_x_loc);
	ReadInt16(range_attack_y_loc);
	ReadInt16(move_to_x_loc);
	ReadInt16(move_to_y_loc);
	ReadInt8(loyalty);
	ReadInt8(target_loyalty);
	ReadFloat(hit_points);
	ReadInt16(max_hit_points);
	ReadCall(skill);
	ReadInt8(unit_mode);
	ReadInt16(unit_mode_para);
	ReadInt16(spy_recno);
	ReadInt16(nation_contribution);
	ReadInt16(total_reward);
	// skip attack_info_array
	ReadInt8(attack_count);
	ReadInt8(attack_range);
	ReadInt16(cur_power);
	ReadInt16(max_power);
	result_node_array = r->has_result_node_array ? (ResultNode*)0xdeadbeef : 0;
	ReadInt32(result_node_count);
	ReadInt16(result_node_recno);
	ReadInt16(result_path_dist);
	way_point_array = r->has_way_point_array ? (ResultNode*)0xdeadbeef : 0;
	ReadInt16(way_point_array_size);
	ReadInt16(way_point_count);
	ReadInt16(ai_action_id);
	ReadInt8(original_action_mode);
	ReadInt16(original_action_para);
	ReadInt16(original_action_x_loc);
	ReadInt16(original_action_y_loc);
	ReadInt16(original_target_x_loc);
	ReadInt16(original_target_y_loc);
	ReadInt16(ai_original_target_x_loc);
	ReadInt16(ai_original_target_y_loc);
	ReadInt8(ai_no_suitable_action);
	ReadInt8(can_guard_flag);
	ReadInt8(can_attack_flag);
	ReadInt8(force_move_flag);
	ReadInt16(home_camp_firm_recno);
	ReadInt8(aggressive_mode);
	ReadInt8(seek_path_fail_count);
	ReadInt8(ignore_power_nation);
	team_info = r->has_team_info ? (TeamInfo*)0xdeadbeef : 0;
}

void AIRegion::write_record(AIRegionGF *r)
{
	WriteInt8(region_id);
	WriteInt8(town_count);
	WriteInt8(base_town_count);
}

void AIRegion::read_record(AIRegionGF *r)
{
	ReadInt8(region_id);
	ReadInt8(town_count);
	ReadInt8(base_town_count);
}

void ActionNode::write_record(ActionNodeGF *r)
{
	WriteInt8(action_mode);
	WriteInt8(action_type);
	WriteInt16(action_para);
	WriteInt16(action_para2);
	WriteInt16(action_id);
	WriteInt32(add_date);
	WriteInt16(unit_recno);
	WriteInt16(action_x_loc);
	WriteInt16(action_y_loc);
	WriteInt16(ref_x_loc);
	WriteInt16(ref_y_loc);
	WriteInt8(retry_count);
	WriteInt8(instance_count);
	WriteInt16Array(group_unit_array, MAX_ACTION_GROUP_UNIT);
	WriteInt8(processing_instance_count);
	WriteInt8(processed_instance_count);
	WriteInt32(next_retry_date);
}

void ActionNode::read_record(ActionNodeGF *r)
{
	ReadInt8(action_mode);
	ReadInt8(action_type);
	ReadInt16(action_para);
	ReadInt16(action_para2);
	ReadInt16(action_id);
	ReadInt32(add_date);
	ReadInt16(unit_recno);
	ReadInt16(action_x_loc);
	ReadInt16(action_y_loc);
	ReadInt16(ref_x_loc);
	ReadInt16(ref_y_loc);
	ReadInt8(retry_count);
	ReadInt8(instance_count);
	ReadInt16Array(group_unit_array, MAX_ACTION_GROUP_UNIT);
	ReadInt8(processing_instance_count);
	ReadInt8(processed_instance_count);
	ReadInt32(next_retry_date);
}

void AttackCamp::write_record(AttackCampGF *r)
{
	WriteInt16(firm_recno);
	WriteInt16(combat_level);
	WriteInt16(distance);
	WriteInt32(patrol_date);
}

void AttackCamp::read_record(AttackCampGF *r)
{
	ReadInt16(firm_recno);
	ReadInt16(combat_level);
	ReadInt16(distance);
	ReadInt32(patrol_date);
}

void NationRelation::write_record(NationRelationGF *r)
{
	WriteInt8(has_contact);
	WriteInt8(should_attack);
	WriteInt8(trade_treaty);
	WriteInt8(status);
	WriteInt32(last_change_status_date);
	WriteInt8(ai_relation_level);
	WriteInt8(ai_secret_attack);
	WriteInt8(ai_demand_trade_treaty);
	WriteFloat(good_relation_duration_rating);
	WriteInt16(started_war_on_us_count);
	WriteFloatArray(cur_year_import, IMPORT_TYPE_COUNT);
	WriteFloatArray(last_year_import, IMPORT_TYPE_COUNT);
	WriteFloatArray(lifetime_import, IMPORT_TYPE_COUNT);
	WriteInt32Array(last_talk_reject_date_array, MAX_TALK_TYPE);
	WriteInt32(last_military_aid_date);
	WriteInt32(last_give_gift_date);
	WriteInt16(total_given_gift_amount);
	WriteInt8(contact_msg_flag);
}

void NationRelation::read_record(NationRelationGF *r)
{
	ReadInt8(has_contact);
	ReadInt8(should_attack);
	ReadInt8(trade_treaty);
	ReadInt8(status);
	ReadInt32(last_change_status_date);
	ReadInt8(ai_relation_level);
	ReadInt8(ai_secret_attack);
	ReadInt8(ai_demand_trade_treaty);
	ReadFloat(good_relation_duration_rating);
	ReadInt16(started_war_on_us_count);
	ReadFloatArray(cur_year_import, IMPORT_TYPE_COUNT);
	ReadFloatArray(last_year_import, IMPORT_TYPE_COUNT);
	ReadFloatArray(lifetime_import, IMPORT_TYPE_COUNT);
	ReadInt32Array(last_talk_reject_date_array, MAX_TALK_TYPE);
	ReadInt32(last_military_aid_date);
	ReadInt32(last_give_gift_date);
	ReadInt16(total_given_gift_amount);
	ReadInt8(contact_msg_flag);
}

void Nation::write_record(NationGF *r)
{
	WriteZero(vtp);

	// NationBase
	WriteInt16(nation_recno);
	WriteInt8(nation_type);
	WriteInt8(race_id);
	WriteInt8(color_scheme_id);
	WriteInt8(nation_color);
	WriteInt16(king_unit_recno);
	WriteInt8(king_leadership);
	WriteInt32(nation_name_id);
	WriteInt8Array(nation_name_str, NATION_NAME_LEN+1);
	WriteInt32(player_id);
	WriteInt8(next_frame_ready);
	WriteInt16(last_caravan_id);
	WriteInt16(nation_firm_count);
	WriteInt32(last_build_firm_date);
	WriteInt8Array(know_base_array, MAX_RACE);
	WriteInt8Array(base_count_array, MAX_RACE);
	WriteInt8(is_at_war_today);
	WriteInt8(is_at_war_yesterday);
	WriteInt32(last_war_date);
	WriteInt16(last_attacker_unit_recno);
	WriteInt32(last_independent_unit_join_date);
	WriteInt8(cheat_enabled_flag);
	WriteFloat(cash);
	WriteFloat(food);
	WriteFloat(reputation);
	WriteFloat(kill_monster_score);
	WriteInt16(auto_collect_tax_loyalty);
	WriteInt16(auto_grant_loyalty);
	WriteFloat(cur_year_profit);
	WriteFloat(last_year_profit);
	WriteFloat(cur_year_fixed_income);
	WriteFloat(last_year_fixed_income);
	WriteFloat(cur_year_fixed_expense);
	WriteFloat(last_year_fixed_expense);
	WriteFloatArray(cur_year_income_array, INCOME_TYPE_COUNT);
	WriteFloatArray(last_year_income_array, INCOME_TYPE_COUNT);
	WriteFloat(cur_year_income);
	WriteFloat(last_year_income);
	WriteFloatArray(cur_year_expense_array, EXPENSE_TYPE_COUNT);
	WriteFloatArray(last_year_expense_array, EXPENSE_TYPE_COUNT);
	WriteFloat(cur_year_expense);
	WriteFloat(last_year_expense);
	WriteFloat(cur_year_cheat);
	WriteFloat(last_year_cheat);
	WriteFloat(cur_year_food_in);
	WriteFloat(last_year_food_in);
	WriteFloat(cur_year_food_out);
	WriteFloat(last_year_food_out);
	WriteFloat(cur_year_food_change);
	WriteFloat(last_year_food_change);
	WriteFloat(cur_year_reputation_change);
	WriteFloat(last_year_reputation_change);
	WriteCallArray(relation_array, MAX_NATION);
	WriteInt8Array(relation_status_array, MAX_NATION);
	WriteInt8Array(relation_passable_array, MAX_NATION);
	WriteInt8Array(relation_should_attack_array, MAX_NATION);
	WriteInt8(is_allied_with_player);
	WriteInt32(total_population);
	WriteInt32(total_jobless_population);
	WriteInt32(total_unit_count);
	WriteInt32(total_human_count);
	WriteInt32(total_general_count);
	WriteInt32(total_weapon_count);
	WriteInt32(total_ship_count);
	WriteInt32(total_firm_count);
	WriteInt32(total_spy_count);
	WriteInt32(total_ship_combat_level);
	WriteInt16(largest_town_recno);
	WriteInt16(largest_town_pop);
	WriteInt16Array(raw_count_array, MAX_RAW);
	WriteInt16Array(last_unit_name_id_array, MAX_UNIT_TYPE);
	WriteInt32(population_rating);
	WriteInt32(military_rating);
	WriteInt32(economic_rating);
	WriteInt32(overall_rating);
	WriteInt32(enemy_soldier_killed);
	WriteInt32(own_soldier_killed);
	WriteInt32(enemy_civilian_killed);
	WriteInt32(own_civilian_killed);
	WriteInt32(enemy_weapon_destroyed);
	WriteInt32(own_weapon_destroyed);
	WriteInt32(enemy_ship_destroyed);
	WriteInt32(own_ship_destroyed);
	WriteInt32(enemy_firm_destroyed);
	WriteInt32(own_firm_destroyed);

	// Nation
	WriteZeroBytes(action_array, sizeof(DynArrayGF));
	WriteInt16(last_action_id);
	WriteZero(ai_town_array);
	WriteZero(ai_base_array);
	WriteZero(ai_mine_array);
	WriteZero(ai_factory_array);
	WriteZero(ai_camp_array);
	WriteZero(ai_research_array);
	WriteZero(ai_war_array);
	WriteZero(ai_harbor_array);
	WriteZero(ai_market_array);
	WriteZero(ai_inn_array);
	WriteZero(ai_general_array);
	WriteZero(ai_caravan_array);
	WriteZero(ai_ship_array);
	WriteInt16(ai_town_size);
	WriteInt16(ai_base_size);
	WriteInt16(ai_mine_size);
	WriteInt16(ai_factory_size);
	WriteInt16(ai_camp_size);
	WriteInt16(ai_research_size);
	WriteInt16(ai_war_size);
	WriteInt16(ai_harbor_size);
	WriteInt16(ai_market_size);
	WriteInt16(ai_inn_size);
	WriteInt16(ai_general_size);
	WriteInt16(ai_caravan_size);
	WriteInt16(ai_ship_size);
	WriteInt16(ai_town_count);
	WriteInt16(ai_base_count);
	WriteInt16(ai_mine_count);
	WriteInt16(ai_factory_count);
	WriteInt16(ai_camp_count);
	WriteInt16(ai_research_count);
	WriteInt16(ai_war_count);
	WriteInt16(ai_harbor_count);
	WriteInt16(ai_market_count);
	WriteInt16(ai_inn_count);
	WriteInt16(ai_general_count);
	WriteInt16(ai_caravan_count);
	WriteInt16(ai_ship_count);
	WriteInt16(ai_base_town_count);
	WriteInt16Array(firm_should_close_array, MAX_FIRM_TYPE);
	WriteCallArray(ai_region_array, MAX_AI_REGION);
	WriteInt8(ai_region_count);
	WriteInt8(pref_force_projection);
	WriteInt8(pref_military_development);
	WriteInt8(pref_economic_development);
	WriteInt8(pref_inc_pop_by_capture);
	WriteInt8(pref_inc_pop_by_growth);
	WriteInt8(pref_peacefulness);
	WriteInt8(pref_military_courage);
	WriteInt8(pref_territorial_cohesiveness);
	WriteInt8(pref_trading_tendency);
	WriteInt8(pref_allying_tendency);
	WriteInt8(pref_honesty);
	WriteInt8(pref_town_harmony);
	WriteInt8(pref_loyalty_concern);
	WriteInt8(pref_forgiveness);
	WriteInt8(pref_collect_tax);
	WriteInt8(pref_hire_unit);
	WriteInt8(pref_use_weapon);
	WriteInt8(pref_keep_general);
	WriteInt8(pref_keep_skilled_unit);
	WriteInt8(pref_diplomacy_retry);
	WriteInt8(pref_attack_monster);
	WriteInt8(pref_spy);
	WriteInt8(pref_counter_spy);
	WriteInt8(pref_food_reserve);
	WriteInt8(pref_cash_reserve);
	WriteInt8(pref_use_marine);
	WriteInt8(pref_unit_chase_distance);
	WriteInt8(pref_repair_concern);
	WriteInt8(pref_scout);
	WriteInt16(ai_capture_enemy_town_recno);
	WriteInt32(ai_capture_enemy_town_plan_date);
	WriteInt32(ai_capture_enemy_town_start_attack_date);
	WriteInt8(ai_capture_enemy_town_use_all_camp);
	WriteInt32(ai_last_defend_action_date);
	WriteInt16(ai_attack_target_x_loc);
	WriteInt16(ai_attack_target_y_loc);
	WriteInt16(ai_attack_target_nation_recno);
	WriteCallArray(attack_camp_array, MAX_SUITABLE_ATTACK_CAMP);
	WriteInt16(attack_camp_count);
	WriteInt16(lead_attack_camp_recno);
}

void Nation::read_record(NationGF *r)
{
	// skip vtp

	// NationBase
	ReadInt16(nation_recno);
	ReadInt8(nation_type);
	ReadInt8(race_id);
	ReadInt8(color_scheme_id);
	ReadInt8(nation_color);
	ReadInt16(king_unit_recno);
	ReadInt8(king_leadership);
	ReadInt32(nation_name_id);
	ReadInt8Array(nation_name_str, NATION_NAME_LEN+1);
	ReadInt32(player_id);
	ReadInt8(next_frame_ready);
	ReadInt16(last_caravan_id);
	ReadInt16(nation_firm_count);
	ReadInt32(last_build_firm_date);
	ReadInt8Array(know_base_array, MAX_RACE);
	ReadInt8Array(base_count_array, MAX_RACE);
	ReadInt8(is_at_war_today);
	ReadInt8(is_at_war_yesterday);
	ReadInt32(last_war_date);
	ReadInt16(last_attacker_unit_recno);
	ReadInt32(last_independent_unit_join_date);
	ReadInt8(cheat_enabled_flag);
	ReadFloat(cash);
	ReadFloat(food);
	ReadFloat(reputation);
	ReadFloat(kill_monster_score);
	ReadInt16(auto_collect_tax_loyalty);
	ReadInt16(auto_grant_loyalty);
	ReadFloat(cur_year_profit);
	ReadFloat(last_year_profit);
	ReadFloat(cur_year_fixed_income);
	ReadFloat(last_year_fixed_income);
	ReadFloat(cur_year_fixed_expense);
	ReadFloat(last_year_fixed_expense);
	ReadFloatArray(cur_year_income_array, INCOME_TYPE_COUNT);
	ReadFloatArray(last_year_income_array, INCOME_TYPE_COUNT);
	ReadFloat(cur_year_income);
	ReadFloat(last_year_income);
	ReadFloatArray(cur_year_expense_array, EXPENSE_TYPE_COUNT);
	ReadFloatArray(last_year_expense_array, EXPENSE_TYPE_COUNT);
	ReadFloat(cur_year_expense);
	ReadFloat(last_year_expense);
	ReadFloat(cur_year_cheat);
	ReadFloat(last_year_cheat);
	ReadFloat(cur_year_food_in);
	ReadFloat(last_year_food_in);
	ReadFloat(cur_year_food_out);
	ReadFloat(last_year_food_out);
	ReadFloat(cur_year_food_change);
	ReadFloat(last_year_food_change);
	ReadFloat(cur_year_reputation_change);
	ReadFloat(last_year_reputation_change);
	ReadCallArray(relation_array, MAX_NATION);
	ReadInt8Array(relation_status_array, MAX_NATION);
	ReadInt8Array(relation_passable_array, MAX_NATION);
	ReadInt8Array(relation_should_attack_array, MAX_NATION);
	ReadInt8(is_allied_with_player);
	ReadInt32(total_population);
	ReadInt32(total_jobless_population);
	ReadInt32(total_unit_count);
	ReadInt32(total_human_count);
	ReadInt32(total_general_count);
	ReadInt32(total_weapon_count);
	ReadInt32(total_ship_count);
	ReadInt32(total_firm_count);
	ReadInt32(total_spy_count);
	ReadInt32(total_ship_combat_level);
	ReadInt16(largest_town_recno);
	ReadInt16(largest_town_pop);
	ReadInt16Array(raw_count_array, MAX_RAW);
	ReadInt16Array(last_unit_name_id_array, MAX_UNIT_TYPE);
	ReadInt32(population_rating);
	ReadInt32(military_rating);
	ReadInt32(economic_rating);
	ReadInt32(overall_rating);
	ReadInt32(enemy_soldier_killed);
	ReadInt32(own_soldier_killed);
	ReadInt32(enemy_civilian_killed);
	ReadInt32(own_civilian_killed);
	ReadInt32(enemy_weapon_destroyed);
	ReadInt32(own_weapon_destroyed);
	ReadInt32(enemy_ship_destroyed);
	ReadInt32(own_ship_destroyed);
	ReadInt32(enemy_firm_destroyed);
	ReadInt32(own_firm_destroyed);

	// Nation
	// skip action_array
	ReadInt16(last_action_id);
	// skip ai_town_array
	// skip ai_base_array
	// skip ai_mine_array
	// skip ai_factory_array
	// skip ai_camp_array
	// skip ai_research_array
	// skip ai_war_array
	// skip ai_harbor_array
	// skip ai_market_array
	// skip ai_inn_array
	// skip ai_general_array
	// skip ai_caravan_array
	// skip ai_ship_array
	ReadInt16(ai_town_size);
	ReadInt16(ai_base_size);
	ReadInt16(ai_mine_size);
	ReadInt16(ai_factory_size);
	ReadInt16(ai_camp_size);
	ReadInt16(ai_research_size);
	ReadInt16(ai_war_size);
	ReadInt16(ai_harbor_size);
	ReadInt16(ai_market_size);
	ReadInt16(ai_inn_size);
	ReadInt16(ai_general_size);
	ReadInt16(ai_caravan_size);
	ReadInt16(ai_ship_size);
	ReadInt16(ai_town_count);
	ReadInt16(ai_base_count);
	ReadInt16(ai_mine_count);
	ReadInt16(ai_factory_count);
	ReadInt16(ai_camp_count);
	ReadInt16(ai_research_count);
	ReadInt16(ai_war_count);
	ReadInt16(ai_harbor_count);
	ReadInt16(ai_market_count);
	ReadInt16(ai_inn_count);
	ReadInt16(ai_general_count);
	ReadInt16(ai_caravan_count);
	ReadInt16(ai_ship_count);
	ReadInt16(ai_base_town_count);
	ReadInt16Array(firm_should_close_array, MAX_FIRM_TYPE);
	ReadCallArray(ai_region_array, MAX_AI_REGION);
	ReadInt8(ai_region_count);
	ReadInt8(pref_force_projection);
	ReadInt8(pref_military_development);
	ReadInt8(pref_economic_development);
	ReadInt8(pref_inc_pop_by_capture);
	ReadInt8(pref_inc_pop_by_growth);
	ReadInt8(pref_peacefulness);
	ReadInt8(pref_military_courage);
	ReadInt8(pref_territorial_cohesiveness);
	ReadInt8(pref_trading_tendency);
	ReadInt8(pref_allying_tendency);
	ReadInt8(pref_honesty);
	ReadInt8(pref_town_harmony);
	ReadInt8(pref_loyalty_concern);
	ReadInt8(pref_forgiveness);
	ReadInt8(pref_collect_tax);
	ReadInt8(pref_hire_unit);
	ReadInt8(pref_use_weapon);
	ReadInt8(pref_keep_general);
	ReadInt8(pref_keep_skilled_unit);
	ReadInt8(pref_diplomacy_retry);
	ReadInt8(pref_attack_monster);
	ReadInt8(pref_spy);
	ReadInt8(pref_counter_spy);
	ReadInt8(pref_food_reserve);
	ReadInt8(pref_cash_reserve);
	ReadInt8(pref_use_marine);
	ReadInt8(pref_unit_chase_distance);
	ReadInt8(pref_repair_concern);
	ReadInt8(pref_scout);
	ReadInt16(ai_capture_enemy_town_recno);
	ReadInt32(ai_capture_enemy_town_plan_date);
	ReadInt32(ai_capture_enemy_town_start_attack_date);
	ReadInt8(ai_capture_enemy_town_use_all_camp);
	ReadInt32(ai_last_defend_action_date);
	ReadInt16(ai_attack_target_x_loc);
	ReadInt16(ai_attack_target_y_loc);
	ReadInt16(ai_attack_target_nation_recno);
	ReadCallArray(attack_camp_array, MAX_SUITABLE_ATTACK_CAMP);
	ReadInt16(attack_camp_count);
	ReadInt16(lead_attack_camp_recno);
}

void NationArray::write_record(NationArrayGF *r)
{
	WriteInt16(nation_count);
	WriteInt16(ai_nation_count);
	WriteInt32(last_del_nation_date);
	WriteInt32(last_new_nation_date);
	WriteInt32(max_nation_population);
	WriteInt32(all_nation_population);
	WriteInt16(independent_town_count);
	WriteInt16Array(independent_town_count_race_array, MAX_RACE);
	WriteInt32(max_nation_units);
	WriteInt32(max_nation_humans);
	WriteInt32(max_nation_generals);
	WriteInt32(max_nation_weapons);
	WriteInt32(max_nation_ships);
	WriteInt32(max_nation_spies);
	WriteInt32(max_nation_firms);
	WriteInt32(max_nation_tech_level);
	WriteInt32(max_population_rating);
	WriteInt32(max_military_rating);
	WriteInt32(max_economic_rating);
	WriteInt32(max_reputation);
	WriteInt32(max_kill_monster_score);
	WriteInt32(max_overall_rating);
	WriteInt16(max_population_nation_recno);
	WriteInt16(max_military_nation_recno);
	WriteInt16(max_economic_nation_recno);
	WriteInt16(max_reputation_nation_recno);
	WriteInt16(max_kill_monster_nation_recno);
	WriteInt16(max_overall_nation_recno);
	WriteInt32(last_alliance_id);
	WriteInt32(nation_peace_days);
	WriteInt16(player_recno);
	WriteZero(player_ptr);
	WriteInt8Array(nation_color_array, MAX_NATION+1);
	WriteInt8Array(nation_power_color_array, MAX_NATION+2);
	WriteInt8Array(human_name_array, (HUMAN_NAME_LEN+1)*MAX_NATION);
}

void NationArray::read_record(NationArrayGF *r)
{
	ReadInt16(nation_count);
	ReadInt16(ai_nation_count);
	ReadInt32(last_del_nation_date);
	ReadInt32(last_new_nation_date);
	ReadInt32(max_nation_population);
	ReadInt32(all_nation_population);
	ReadInt16(independent_town_count);
	ReadInt16Array(independent_town_count_race_array, MAX_RACE);
	ReadInt32(max_nation_units);
	ReadInt32(max_nation_humans);
	ReadInt32(max_nation_generals);
	ReadInt32(max_nation_weapons);
	ReadInt32(max_nation_ships);
	ReadInt32(max_nation_spies);
	ReadInt32(max_nation_firms);
	ReadInt32(max_nation_tech_level);
	ReadInt32(max_population_rating);
	ReadInt32(max_military_rating);
	ReadInt32(max_economic_rating);
	ReadInt32(max_reputation);
	ReadInt32(max_kill_monster_score);
	ReadInt32(max_overall_rating);
	ReadInt16(max_population_nation_recno);
	ReadInt16(max_military_nation_recno);
	ReadInt16(max_economic_nation_recno);
	ReadInt16(max_reputation_nation_recno);
	ReadInt16(max_kill_monster_nation_recno);
	ReadInt16(max_overall_nation_recno);
	ReadInt32(last_alliance_id);
	ReadInt32(nation_peace_days);
	ReadInt16(player_recno);
	// skip player_ptr
	ReadInt8Array(nation_color_array, MAX_NATION+1);
	ReadInt8Array(nation_power_color_array, MAX_NATION+2);
	ReadInt8Array(human_name_array, (HUMAN_NAME_LEN+1)*MAX_NATION);
}

void Version_1_Nation::write_record(Version_1_NationGF *r)
{
	WriteZero(vtp);

	// Version_1_NationBase
	WriteInt16(nation_recno);
	WriteInt8(nation_type);
	WriteInt8(race_id);
	WriteInt8(color_scheme_id);
	WriteInt8(nation_color);
	WriteInt16(king_unit_recno);
	WriteInt8(king_leadership);
	WriteInt32(nation_name_id);
	WriteInt8Array(nation_name_str, NATION_NAME_LEN+1);
	WriteInt32(player_id);
	WriteInt8(next_frame_ready);
	WriteInt16(last_caravan_id);
	WriteInt16(nation_firm_count);
	WriteInt32(last_build_firm_date);
	WriteInt8Array(know_base_array, VERSION_1_MAX_RACE);
	WriteInt8Array(base_count_array, VERSION_1_MAX_RACE);
	WriteInt8(is_at_war_today);
	WriteInt8(is_at_war_yesterday);
	WriteInt32(last_war_date);
	WriteInt16(last_attacker_unit_recno);
	WriteInt32(last_independent_unit_join_date);
	WriteInt8(cheat_enabled_flag);
	WriteFloat(cash);
	WriteFloat(food);
	WriteFloat(reputation);
	WriteFloat(kill_monster_score);
	WriteInt16(auto_collect_tax_loyalty);
	WriteInt16(auto_grant_loyalty);
	WriteFloat(cur_year_profit);
	WriteFloat(last_year_profit);
	WriteFloat(cur_year_fixed_income);
	WriteFloat(last_year_fixed_income);
	WriteFloat(cur_year_fixed_expense);
	WriteFloat(last_year_fixed_expense);
	WriteFloatArray(cur_year_income_array, INCOME_TYPE_COUNT);
	WriteFloatArray(last_year_income_array, INCOME_TYPE_COUNT);
	WriteFloat(cur_year_income);
	WriteFloat(last_year_income);
	WriteFloatArray(cur_year_expense_array, EXPENSE_TYPE_COUNT);
	WriteFloatArray(last_year_expense_array, EXPENSE_TYPE_COUNT);
	WriteFloat(cur_year_expense);
	WriteFloat(last_year_expense);
	WriteFloat(cur_year_cheat);
	WriteFloat(last_year_cheat);
	WriteFloat(cur_year_food_in);
	WriteFloat(last_year_food_in);
	WriteFloat(cur_year_food_out);
	WriteFloat(last_year_food_out);
	WriteFloat(cur_year_food_change);
	WriteFloat(last_year_food_change);
	WriteFloat(cur_year_reputation_change);
	WriteFloat(last_year_reputation_change);
	WriteCallArray(relation_array, MAX_NATION);
	WriteInt8Array(relation_status_array, MAX_NATION);
	WriteInt8Array(relation_passable_array, MAX_NATION);
	WriteInt8Array(relation_should_attack_array, MAX_NATION);
	WriteInt8(is_allied_with_player);
	WriteInt32(total_population);
	WriteInt32(total_jobless_population);
	WriteInt32(total_unit_count);
	WriteInt32(total_human_count);
	WriteInt32(total_general_count);
	WriteInt32(total_weapon_count);
	WriteInt32(total_ship_count);
	WriteInt32(total_firm_count);
	WriteInt32(total_spy_count);
	WriteInt32(total_ship_combat_level);
	WriteInt16(largest_town_recno);
	WriteInt16(largest_town_pop);
	WriteInt16Array(raw_count_array, MAX_RAW);
	WriteInt16Array(last_unit_name_id_array, VERSION_1_MAX_UNIT_TYPE);
	WriteInt32(population_rating);
	WriteInt32(military_rating);
	WriteInt32(economic_rating);
	WriteInt32(overall_rating);
	WriteInt32(enemy_soldier_killed);
	WriteInt32(own_soldier_killed);
	WriteInt32(enemy_civilian_killed);
	WriteInt32(own_civilian_killed);
	WriteInt32(enemy_weapon_destroyed);
	WriteInt32(own_weapon_destroyed);
	WriteInt32(enemy_ship_destroyed);
	WriteInt32(own_ship_destroyed);
	WriteInt32(enemy_firm_destroyed);
	WriteInt32(own_firm_destroyed);

	// Version_1_Nation
	WriteZeroBytes(action_array, sizeof(DynArrayGF));
	WriteInt16(last_action_id);
	WriteZero(ai_town_array);
	WriteZero(ai_base_array);
	WriteZero(ai_mine_array);
	WriteZero(ai_factory_array);
	WriteZero(ai_camp_array);
	WriteZero(ai_research_array);
	WriteZero(ai_war_array);
	WriteZero(ai_harbor_array);
	WriteZero(ai_market_array);
	WriteZero(ai_inn_array);
	WriteZero(ai_general_array);
	WriteZero(ai_caravan_array);
	WriteZero(ai_ship_array);
	WriteInt16(ai_town_size);
	WriteInt16(ai_base_size);
	WriteInt16(ai_mine_size);
	WriteInt16(ai_factory_size);
	WriteInt16(ai_camp_size);
	WriteInt16(ai_research_size);
	WriteInt16(ai_war_size);
	WriteInt16(ai_harbor_size);
	WriteInt16(ai_market_size);
	WriteInt16(ai_inn_size);
	WriteInt16(ai_general_size);
	WriteInt16(ai_caravan_size);
	WriteInt16(ai_ship_size);
	WriteInt16(ai_town_count);
	WriteInt16(ai_base_count);
	WriteInt16(ai_mine_count);
	WriteInt16(ai_factory_count);
	WriteInt16(ai_camp_count);
	WriteInt16(ai_research_count);
	WriteInt16(ai_war_count);
	WriteInt16(ai_harbor_count);
	WriteInt16(ai_market_count);
	WriteInt16(ai_inn_count);
	WriteInt16(ai_general_count);
	WriteInt16(ai_caravan_count);
	WriteInt16(ai_ship_count);
	WriteInt16(ai_base_town_count);
	WriteInt16Array(firm_should_close_array, MAX_FIRM_TYPE);
	WriteCallArray(ai_region_array, MAX_AI_REGION);
	WriteInt8(ai_region_count);
	WriteInt8(pref_force_projection);
	WriteInt8(pref_military_development);
	WriteInt8(pref_economic_development);
	WriteInt8(pref_inc_pop_by_capture);
	WriteInt8(pref_inc_pop_by_growth);
	WriteInt8(pref_peacefulness);
	WriteInt8(pref_military_courage);
	WriteInt8(pref_territorial_cohesiveness);
	WriteInt8(pref_trading_tendency);
	WriteInt8(pref_allying_tendency);
	WriteInt8(pref_honesty);
	WriteInt8(pref_town_harmony);
	WriteInt8(pref_loyalty_concern);
	WriteInt8(pref_forgiveness);
	WriteInt8(pref_collect_tax);
	WriteInt8(pref_hire_unit);
	WriteInt8(pref_use_weapon);
	WriteInt8(pref_keep_general);
	WriteInt8(pref_keep_skilled_unit);
	WriteInt8(pref_diplomacy_retry);
	WriteInt8(pref_attack_monster);
	WriteInt8(pref_spy);
	WriteInt8(pref_counter_spy);
	WriteInt8(pref_food_reserve);
	WriteInt8(pref_cash_reserve);
	WriteInt8(pref_use_marine);
	WriteInt8(pref_unit_chase_distance);
	WriteInt8(pref_repair_concern);
	WriteInt8(pref_scout);
	WriteInt16(ai_capture_enemy_town_recno);
	WriteInt32(ai_capture_enemy_town_plan_date);
	WriteInt32(ai_capture_enemy_town_start_attack_date);
	WriteInt8(ai_capture_enemy_town_use_all_camp);
	WriteInt32(ai_last_defend_action_date);
	WriteInt16(ai_attack_target_x_loc);
	WriteInt16(ai_attack_target_y_loc);
	WriteInt16(ai_attack_target_nation_recno);
	WriteCallArray(attack_camp_array, MAX_SUITABLE_ATTACK_CAMP);
	WriteInt16(attack_camp_count);
	WriteInt16(lead_attack_camp_recno);
}

void Version_1_Nation::read_record(Version_1_NationGF *r)
{
	// skip vtp

	// Version_1_NationBase
	ReadInt16(nation_recno);
	ReadInt8(nation_type);
	ReadInt8(race_id);
	ReadInt8(color_scheme_id);
	ReadInt8(nation_color);
	ReadInt16(king_unit_recno);
	ReadInt8(king_leadership);
	ReadInt32(nation_name_id);
	ReadInt8Array(nation_name_str, NATION_NAME_LEN+1);
	ReadInt32(player_id);
	ReadInt8(next_frame_ready);
	ReadInt16(last_caravan_id);
	ReadInt16(nation_firm_count);
	ReadInt32(last_build_firm_date);
	ReadInt8Array(know_base_array, VERSION_1_MAX_RACE);
	ReadInt8Array(base_count_array, VERSION_1_MAX_RACE);
	ReadInt8(is_at_war_today);
	ReadInt8(is_at_war_yesterday);
	ReadInt32(last_war_date);
	ReadInt16(last_attacker_unit_recno);
	ReadInt32(last_independent_unit_join_date);
	ReadInt8(cheat_enabled_flag);
	ReadFloat(cash);
	ReadFloat(food);
	ReadFloat(reputation);
	ReadFloat(kill_monster_score);
	ReadInt16(auto_collect_tax_loyalty);
	ReadInt16(auto_grant_loyalty);
	ReadFloat(cur_year_profit);
	ReadFloat(last_year_profit);
	ReadFloat(cur_year_fixed_income);
	ReadFloat(last_year_fixed_income);
	ReadFloat(cur_year_fixed_expense);
	ReadFloat(last_year_fixed_expense);
	ReadFloatArray(cur_year_income_array, INCOME_TYPE_COUNT);
	ReadFloatArray(last_year_income_array, INCOME_TYPE_COUNT);
	ReadFloat(cur_year_income);
	ReadFloat(last_year_income);
	ReadFloatArray(cur_year_expense_array, EXPENSE_TYPE_COUNT);
	ReadFloatArray(last_year_expense_array, EXPENSE_TYPE_COUNT);
	ReadFloat(cur_year_expense);
	ReadFloat(last_year_expense);
	ReadFloat(cur_year_cheat);
	ReadFloat(last_year_cheat);
	ReadFloat(cur_year_food_in);
	ReadFloat(last_year_food_in);
	ReadFloat(cur_year_food_out);
	ReadFloat(last_year_food_out);
	ReadFloat(cur_year_food_change);
	ReadFloat(last_year_food_change);
	ReadFloat(cur_year_reputation_change);
	ReadFloat(last_year_reputation_change);
	ReadCallArray(relation_array, MAX_NATION);
	ReadInt8Array(relation_status_array, MAX_NATION);
	ReadInt8Array(relation_passable_array, MAX_NATION);
	ReadInt8Array(relation_should_attack_array, MAX_NATION);
	ReadInt8(is_allied_with_player);
	ReadInt32(total_population);
	ReadInt32(total_jobless_population);
	ReadInt32(total_unit_count);
	ReadInt32(total_human_count);
	ReadInt32(total_general_count);
	ReadInt32(total_weapon_count);
	ReadInt32(total_ship_count);
	ReadInt32(total_firm_count);
	ReadInt32(total_spy_count);
	ReadInt32(total_ship_combat_level);
	ReadInt16(largest_town_recno);
	ReadInt16(largest_town_pop);
	ReadInt16Array(raw_count_array, MAX_RAW);
	ReadInt16Array(last_unit_name_id_array, VERSION_1_MAX_UNIT_TYPE);
	ReadInt32(population_rating);
	ReadInt32(military_rating);
	ReadInt32(economic_rating);
	ReadInt32(overall_rating);
	ReadInt32(enemy_soldier_killed);
	ReadInt32(own_soldier_killed);
	ReadInt32(enemy_civilian_killed);
	ReadInt32(own_civilian_killed);
	ReadInt32(enemy_weapon_destroyed);
	ReadInt32(own_weapon_destroyed);
	ReadInt32(enemy_ship_destroyed);
	ReadInt32(own_ship_destroyed);
	ReadInt32(enemy_firm_destroyed);
	ReadInt32(own_firm_destroyed);

	// Version_1_Nation
	// skip action_array
	ReadInt16(last_action_id);
	// skip ai_town_array
	// skip ai_base_array
	// skip ai_mine_array
	// skip ai_factory_array
	// skip ai_camp_array
	// skip ai_research_array
	// skip ai_war_array
	// skip ai_harbor_array
	// skip ai_market_array
	// skip ai_inn_array
	// skip ai_general_array
	// skip ai_caravan_array
	// skip ai_ship_array
	ReadInt16(ai_town_size);
	ReadInt16(ai_base_size);
	ReadInt16(ai_mine_size);
	ReadInt16(ai_factory_size);
	ReadInt16(ai_camp_size);
	ReadInt16(ai_research_size);
	ReadInt16(ai_war_size);
	ReadInt16(ai_harbor_size);
	ReadInt16(ai_market_size);
	ReadInt16(ai_inn_size);
	ReadInt16(ai_general_size);
	ReadInt16(ai_caravan_size);
	ReadInt16(ai_ship_size);
	ReadInt16(ai_town_count);
	ReadInt16(ai_base_count);
	ReadInt16(ai_mine_count);
	ReadInt16(ai_factory_count);
	ReadInt16(ai_camp_count);
	ReadInt16(ai_research_count);
	ReadInt16(ai_war_count);
	ReadInt16(ai_harbor_count);
	ReadInt16(ai_market_count);
	ReadInt16(ai_inn_count);
	ReadInt16(ai_general_count);
	ReadInt16(ai_caravan_count);
	ReadInt16(ai_ship_count);
	ReadInt16(ai_base_town_count);
	ReadInt16Array(firm_should_close_array, MAX_FIRM_TYPE);
	ReadCallArray(ai_region_array, MAX_AI_REGION);
	ReadInt8(ai_region_count);
	ReadInt8(pref_force_projection);
	ReadInt8(pref_military_development);
	ReadInt8(pref_economic_development);
	ReadInt8(pref_inc_pop_by_capture);
	ReadInt8(pref_inc_pop_by_growth);
	ReadInt8(pref_peacefulness);
	ReadInt8(pref_military_courage);
	ReadInt8(pref_territorial_cohesiveness);
	ReadInt8(pref_trading_tendency);
	ReadInt8(pref_allying_tendency);
	ReadInt8(pref_honesty);
	ReadInt8(pref_town_harmony);
	ReadInt8(pref_loyalty_concern);
	ReadInt8(pref_forgiveness);
	ReadInt8(pref_collect_tax);
	ReadInt8(pref_hire_unit);
	ReadInt8(pref_use_weapon);
	ReadInt8(pref_keep_general);
	ReadInt8(pref_keep_skilled_unit);
	ReadInt8(pref_diplomacy_retry);
	ReadInt8(pref_attack_monster);
	ReadInt8(pref_spy);
	ReadInt8(pref_counter_spy);
	ReadInt8(pref_food_reserve);
	ReadInt8(pref_cash_reserve);
	ReadInt8(pref_use_marine);
	ReadInt8(pref_unit_chase_distance);
	ReadInt8(pref_repair_concern);
	ReadInt8(pref_scout);
	ReadInt16(ai_capture_enemy_town_recno);
	ReadInt32(ai_capture_enemy_town_plan_date);
	ReadInt32(ai_capture_enemy_town_start_attack_date);
	ReadInt8(ai_capture_enemy_town_use_all_camp);
	ReadInt32(ai_last_defend_action_date);
	ReadInt16(ai_attack_target_x_loc);
	ReadInt16(ai_attack_target_y_loc);
	ReadInt16(ai_attack_target_nation_recno);
	ReadCallArray(attack_camp_array, MAX_SUITABLE_ATTACK_CAMP);
	ReadInt16(attack_camp_count);
	ReadInt16(lead_attack_camp_recno);
}

void Version_1_NationArray::write_record(Version_1_NationArrayGF *r)
{
	WriteInt16(nation_count);
	WriteInt16(ai_nation_count);
	WriteInt32(last_del_nation_date);
	WriteInt32(last_new_nation_date);
	WriteInt32(max_nation_population);
	WriteInt32(all_nation_population);
	WriteInt16(independent_town_count);
	WriteInt16Array(independent_town_count_race_array, VERSION_1_MAX_RACE);
	WriteInt32(max_nation_units);
	WriteInt32(max_nation_humans);
	WriteInt32(max_nation_generals);
	WriteInt32(max_nation_weapons);
	WriteInt32(max_nation_ships);
	WriteInt32(max_nation_spies);
	WriteInt32(max_nation_firms);
	WriteInt32(max_nation_tech_level);
	WriteInt32(max_population_rating);
	WriteInt32(max_military_rating);
	WriteInt32(max_economic_rating);
	WriteInt32(max_reputation);
	WriteInt32(max_kill_monster_score);
	WriteInt32(max_overall_rating);
	WriteInt16(max_population_nation_recno);
	WriteInt16(max_military_nation_recno);
	WriteInt16(max_economic_nation_recno);
	WriteInt16(max_reputation_nation_recno);
	WriteInt16(max_kill_monster_nation_recno);
	WriteInt16(max_overall_nation_recno);
	WriteInt32(last_alliance_id);
	WriteInt32(nation_peace_days);
	WriteInt16(player_recno);
	WriteZero(player_ptr);
	WriteInt8Array(nation_color_array, MAX_NATION+1);
	WriteInt8Array(nation_power_color_array, MAX_NATION+2);
	WriteInt8Array(human_name_array, (HUMAN_NAME_LEN+1)*MAX_NATION);
}

void Version_1_NationArray::read_record(Version_1_NationArrayGF *r)
{
	ReadInt16(nation_count);
	ReadInt16(ai_nation_count);
	ReadInt32(last_del_nation_date);
	ReadInt32(last_new_nation_date);
	ReadInt32(max_nation_population);
	ReadInt32(all_nation_population);
	ReadInt16(independent_town_count);
	ReadInt16Array(independent_town_count_race_array, VERSION_1_MAX_RACE);
	ReadInt32(max_nation_units);
	ReadInt32(max_nation_humans);
	ReadInt32(max_nation_generals);
	ReadInt32(max_nation_weapons);
	ReadInt32(max_nation_ships);
	ReadInt32(max_nation_spies);
	ReadInt32(max_nation_firms);
	ReadInt32(max_nation_tech_level);
	ReadInt32(max_population_rating);
	ReadInt32(max_military_rating);
	ReadInt32(max_economic_rating);
	ReadInt32(max_reputation);
	ReadInt32(max_kill_monster_score);
	ReadInt32(max_overall_rating);
	ReadInt16(max_population_nation_recno);
	ReadInt16(max_military_nation_recno);
	ReadInt16(max_economic_nation_recno);
	ReadInt16(max_reputation_nation_recno);
	ReadInt16(max_kill_monster_nation_recno);
	ReadInt16(max_overall_nation_recno);
	ReadInt32(last_alliance_id);
	ReadInt32(nation_peace_days);
	ReadInt16(player_recno);
	// skip player_ptr
	ReadInt8Array(nation_color_array, MAX_NATION+1);
	ReadInt8Array(nation_power_color_array, MAX_NATION+2);
	ReadInt8Array(human_name_array, (HUMAN_NAME_LEN+1)*MAX_NATION);
}
