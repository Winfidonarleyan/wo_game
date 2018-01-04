/*
 * Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// This is where scripts' loading functions should be declared:
void AddSC_Transmogrification();
void AddSC_arena_spectator();
void AddSC_vip_account();
void AddSC_FakePlayers();
void AddSC_Custom_SpellScripts();
void AddSC_Custom_Npc_Scripts();
void AddSC_LevelSpellLearn();
void AddSC_PlayedTimeRewards();
void AddSC_custom_misc_playerscripts();
void AddSC_custom_gameobjects();
void AddSC_custom_commands();
void AddSC_PVPZone();
void AddSC_npc_solo_queue();
void AddSC_npc_enchantment();
void AddSC_NPC_VisualWeapon();
void AddSC_Professions_NPC();
void AddSC_npc_morph();
void AddSC_npc_mount();
void AddSC_npc_Teleporter();
void AddSC_npc_title();
void AddSC_npc_welcome();
void AddSC_VAS_AutoBalance();
void AddSC_forbidden_texts();
void AddSC_npc_all();

// The name of this function should match:
// void Add${NameOfDirectory}Scripts()
void AddCustomScripts()
{
    AddSC_Transmogrification();
    AddSC_arena_spectator();
    AddSC_vip_account();
    AddSC_Custom_SpellScripts();
    AddSC_Custom_Npc_Scripts();
    AddSC_FakePlayers();
    AddSC_LevelSpellLearn();
    AddSC_PlayedTimeRewards();
    AddSC_custom_misc_playerscripts();
    AddSC_custom_gameobjects();
    AddSC_custom_commands();
    AddSC_PVPZone();
    AddSC_npc_solo_queue();
    AddSC_npc_enchantment();
    AddSC_NPC_VisualWeapon();
    AddSC_Professions_NPC();
    AddSC_npc_morph();
    AddSC_npc_mount();
    AddSC_npc_Teleporter();
    AddSC_npc_title();
    AddSC_npc_welcome();
    AddSC_VAS_AutoBalance();
	AddSC_forbidden_texts();
	AddSC_npc_all();
}