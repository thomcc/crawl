/*
 *  Modified for Crawl Reference by $Author$ on $Date$
 */

#ifndef MONDATA_H
#define MONDATA_H

#define AT_NO_ATK       {AT_NONE, AF_PLAIN, 0}

#include "enum.h"

/*
   This whole file was very generously condensed from its initial ugly form
   by Wladimir van der Laan ($pellbinder).
 */

/* ******************************************************************

   (see "mon-util.h" for the gory details)

 - ordering does not matter, because seekmonster() searches the entire
   array ... probably not to most efficient thing to do, but so it goes

 - Here are the rows:
    - row 1: monster id, display character, display colour, name
    - row 2: monster flags
    - row 3: monster resistance flags
    - row 4: mass, experience modifier, genus, species, holiness, resist magic
    - row 5: damage for each of four attacks
    - row 6: hit dice, described by four parameters
    - row 7: AC, evasion, sec(spell), corpse_thingy, zombie size,
              shouts, intel
    - row 8: habitat, speed, energy_usage, gmon_use class, body size


 - Some further explanations:

    - colour: if BLACK, monster uses value of mons_sec
    - name: if an empty string, name generated automagically (see moname)
    - mass: if zero, the monster never leaves a corpse (also corpse_thingy)
    - genus: base monster "type" for a classed monsters (i.e. jackal as hound)
    - species: corpse type of monster (i.e. orc for orc wizard)
    - holiness: 
       MH_HOLY       - irritates some gods when killed, immunity from
                        holy wrath weapons
       MH_NATURAL    - baseline monster type
       MH_UNDEAD     - immunity from draining, pain, torment; extra
                        damage from holy wrath/disruption; affected by
                        repel undead and holy word
       MH_DEMONIC    - similar to undead, but holy wrath does even more
                        damage and repel undead effects are ignored -- *no*
                        automatic hellfire resistance
       MH_NONLIVING  - golems and other constructs
       MH_PLANT      - plants

   exp_mod: see give_adjusted_experience() in monstuff.cc
   - the experience given for killing this monster is calculated something
   like this:
   experience = hp_max * HD * HD * exp_mod / 10
   I think.
   
   Actually it is
    experience = (16 + maxhp) * HD * HD * exp_mod * (100 + diff) * speed
                 / 100000
    with a minimum of 1, and maximum 15000 (jpeg)

   resist_magic: see mons_resist_magic() in mon-util.cc
   - If -x calculate (-x * hit dice * 4/3), else simply x

   damage [4]
   - up to 4 different attacks

   hp_dice [4]
   - hit dice, min hp per HD, extra random hp per HD, fixed HP (unique mons)

   corpse_thingy
   - err, bad name. Describes effects of eating corpses.
     CE_NOCORPSE,        leaves no corpse (mass == 0)
     CE_CLEAN,           can be healthily eaten by non-Ghoul characters
     CE_CONTAMINATED,    occasionally causes sickness
     CE_POISONOUS,       hazardous to characters without poison resistance
     CE_HCL,             causes rotting
     CE_MUTAGEN_RANDOM,  mutagenous
     CE_MUTAGEN_GOOD,  // may be worth implementing {dlb}
     CE_MUTAGEN_BAD,   // may be worth implementing {dlb}
     CE_RANDOM,        // not used, but may be worth implementing {dlb}
     CE_ROTTEN           always causes sickness (good for Ghouls)

   zombie_size
     Z_NOZOMBIE
     Z_SMALL    (z)
     Z_BIG      (Z)

   shouts
   - various things monsters can do upon seeing you

   intel explanation:
   - How smart it is: I_PLANT < I_ANIMAL < I_NORMAL < I_HIGH.
   So far, differences here have little effects except for monster's chance
   of seeing you if stealthy and rudimentary trap handling;
   really stupid monsters will walk through clouds

   speed
   - Increases the store of energy that the monster uses for doing things.
   less = slower. 5 = half speed, 10 = normal, 20 = double speed.

   energy usage
   - How quickly the energy granted by speed is used up.  Most monsters
   should just use DEFAULT_ENERGY, where all the different types of actions
   use 10 energy units.

   gmon_use explanation:
     MONUSE_NOTHING,
     MONUSE_EATS_ITEMS,
     MONUSE_OPEN_DOORS,
     MONUSE_STARTING_EQUIPMENT,
     MONUSE_WEAPONS_ARMOUR,
     MONUSE_MAGIC_ITEMS
     
   size:
     SIZE_TINY,              // rat/bat
     SIZE_LITTLE,            // spriggan
     SIZE_SMALL,             // halfling/kobold/gnome
     SIZE_MEDIUM,            // human/elf/dwarf
     SIZE_LARGE,             // troll/ogre/centaur/naga
     SIZE_BIG,               // large quadrupeds
     SIZE_GIANT,             // giant
     SIZE_HUGE,              // dragon

 */

#define DEFAULT_ENERGY {10, 10, 10, 10, 10, 10, 10, 100}
#define MOVE_ENERGY(x) {(x), (x), 10, 10, 10, 10, 10, 100}

// monster 250: The Thing That Should Not Be(tm)
// do not remove, or seekmonster will crash on unknown mc request
// it is also a good prototype for new monsters
{
    MONS_PROGRAM_BUG, 'B', LIGHTRED, "program bug",
    M_NO_EXP_GAIN,
    MR_NO_FLAGS,
    0, 10, MONS_PROGRAM_BUG, MONS_PROGRAM_BUG, MH_NATURAL, -3,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 0, 0, 0, 0 },
    0, 0, MST_NO_SPELLS, CE_CONTAMINATED, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 0, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_HUGE
},

// real monsters begin here {dlb}:
{
    MONS_GIANT_ANT, 'a', RED, "giant ant",
    M_NO_FLAGS,
    MR_VUL_POISON,
    700, 10, MONS_GIANT_ANT, MONS_GIANT_ANT, MH_NATURAL, -3,
    { {AT_BITE, AF_POISON, 8}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 3, 5, 0 },
    4, 10, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_SILENT, I_INSECT, 
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LITTLE,
},

{
    MONS_GIANT_BAT, 'b', LIGHTGREY, "giant bat",
    M_FLIES | M_SENSE_INVIS | M_WARM_BLOOD,
    MR_NO_FLAGS,
    150, 4, MONS_GIANT_BAT, MONS_GIANT_BAT, MH_NATURAL, -1,
    { {AT_HIT, AF_PLAIN, 1}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 2, 3, 0 },
    1, 14, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_SILENT, I_ANIMAL, 
    HT_LAND, 30, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_CENTAUR, 'c', BROWN, "centaur",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    1500, 10, MONS_CENTAUR, MONS_CENTAUR, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 4, 3, 5, 0 },
    3, 7, MST_NO_SPELLS, CE_CLEAN, Z_BIG, S_SHOUT, I_NORMAL, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_BIG
},

{
    MONS_RED_DEVIL, '4', RED, "red devil",
    M_FIGHTER | M_FLIES | M_EVIL,
    MR_RES_POISON | MR_RES_HELLFIRE | MR_VUL_COLD,
    0, 10, MONS_RED_DEVIL, MONS_RED_DEVIL, MH_DEMONIC, -7,
    { {AT_HIT, AF_PLAIN, 18}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 3, 5, 0 },
    10, 10, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_ETTIN, 'C', BROWN, "ettin",
    M_WARM_BLOOD | M_EVIL | M_TWOWEAPON,
    MR_NO_FLAGS,
    0, 10, MONS_HILL_GIANT, MONS_ETTIN, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 18}, {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK },
    { 7, 3, 5, 0 },
    3, 4, MST_NO_SPELLS, CE_CONTAMINATED, Z_BIG, S_SHOUT2, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_GIANT,
},

{
    MONS_FUNGUS, 'f', LIGHTGREY, "fungus",
    M_NO_EXP_GAIN,
    MR_RES_POISON,
    0, 10, MONS_PLANT, MONS_FUNGUS, MH_PLANT, MAG_IMMUNE,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 8, 3, 5, 0 },
    1, 0, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 0, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_GOBLIN, 'g', LIGHTGREY, "goblin",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    400, 10, MONS_GOBLIN, MONS_GOBLIN, MH_NATURAL, -1,
    { {AT_HIT, AF_PLAIN, 4}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 2, 4, 0 },
    0, 12, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_SMALL
},

{
    MONS_HOUND, 'h', YELLOW, "hound",
    M_SENSE_INVIS | M_WARM_BLOOD,
    MR_NO_FLAGS,
    300, 10, MONS_HOUND, MONS_HOUND, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 3, 5, 0 },
    2, 13, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_BARK, I_ANIMAL, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

// note: these things regenerate
{
    MONS_IMP, '5', RED, "imp",
    M_FLIES | M_SEE_INVIS | M_SPEAKS | M_EVIL | M_SPECIAL_ABILITY,
    MR_RES_POISON | MR_RES_HELLFIRE | MR_VUL_COLD,
    0, 13, MONS_IMP, MONS_IMP, MH_DEMONIC, -9,
    { {AT_HIT, AF_PLAIN, 4}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 3, 3, 0 },
    3, 14, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_LITTLE,
},

{
    MONS_JACKAL, 'h', BROWN, "jackal",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    200, 10, MONS_HOUND, MONS_JACKAL, MH_NATURAL, -1,
    { {AT_BITE, AF_PLAIN, 3}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 3, 5, 0 },
    2, 12, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_BARK, I_ANIMAL, 
    HT_LAND, 14, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

{
    MONS_KILLER_BEE, 'k', YELLOW, "killer bee",
    M_FLIES,
    MR_VUL_POISON,
    150, 11, MONS_KILLER_BEE, MONS_KILLER_BEE, MH_NATURAL, -3,
    { {AT_STING, AF_POISON, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 3, 5, 0 },
    2, 18, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_BUZZ, I_INSECT, 
    HT_LAND, 20, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_KILLER_BEE_LARVA, 'w', YELLOW, "killer bee larva",
    M_NO_SKELETON,
    MR_VUL_POISON,
    150, 5, MONS_KILLER_BEE_LARVA, MONS_KILLER_BEE_LARVA, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 3}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 3, 5, 0 },
    1, 5, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_SILENT, I_INSECT, 
    HT_LAND, 5, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_MANTICORE, 'H', RED, "manticore",
    M_WARM_BLOOD | M_SPECIAL_ABILITY,
    MR_NO_FLAGS,
    1800, 10, MONS_MANTICORE, MONS_MANTICORE, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 14}, {AT_CLAW, AF_PLAIN, 8}, {AT_CLAW, AF_PLAIN, 8}, AT_NO_ATK },
    { 9, 3, 5, 0 },
    5, 7, MST_NO_SPELLS, CE_CONTAMINATED, Z_BIG, S_SILENT, I_NORMAL, 
    HT_LAND, 7, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_BIG
},

// this thing doesn't have nr. 13 for nothing, has it? ($pellbinder)
{
    MONS_NECROPHAGE, 'n', LIGHTGREY, "necrophage",
    M_EVIL,
    MR_RES_POISON | MR_RES_COLD,
    500, 10, MONS_GHOUL, MONS_NECROPHAGE, MH_UNDEAD, -5,
    { {AT_HIT, AF_ROT, 8}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 3, 5, 0 },
    2, 10, MST_NO_SPELLS, CE_HCL, Z_NOZOMBIE, S_SILENT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_ORC, 'o', RED, "orc",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    600, 10, MONS_ORC, MONS_ORC, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 4, 6, 0 },
    0, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

// XP modifier is 5 for these, because they really aren't all that
// dangerous, but still come out at 200+ XP
{
    MONS_PHANTOM, 'p', BLUE, "phantom",
    M_EVIL | M_SPECIAL_ABILITY,
    MR_RES_POISON | mrd(MR_RES_COLD, 2),
    0, 5, MONS_PHANTOM, MONS_PHANTOM, MH_UNDEAD, -4,
    { {AT_HIT, AF_BLINK, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 7, 3, 5, 0 },
    3, 13, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_QUASIT, '5', LIGHTGREY, "quasit",
    M_EVIL,
    MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD,
    0, 10, MONS_QUASIT, MONS_QUASIT, MH_DEMONIC, 5,
    { {AT_BITE, AF_DRAIN_DEX, 3}, {AT_CLAW, AF_DRAIN_DEX, 2}, {AT_CLAW, AF_DRAIN_DEX, 2}, AT_NO_ATK },
    { 3, 2, 6, 0 },
    5, 17, MST_NO_SPELLS, CE_POISONOUS, Z_NOZOMBIE, S_SILENT, I_INSECT, 
    HT_LAND, 13, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

{
    MONS_RAT, 'r', BROWN, "rat",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    200, 10, MONS_RAT, MONS_RAT, MH_NATURAL, -1,
    { {AT_BITE, AF_PLAIN, 3}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 1, 3, 0 },
    1, 10, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_SILENT, I_ANIMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_SCORPION, 's', LIGHTGREY, "scorpion",
    M_NO_FLAGS,
    MR_VUL_POISON,
    500, 10, MONS_SCORPION, MONS_SCORPION, MH_NATURAL, -3,
    { {AT_STING, AF_POISON_MEDIUM, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 3, 5, 0 },
    5, 10, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_SILENT, I_INSECT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

/* ******************************************************************
// the tunneling worm is no more ...
// not until it can be reimplemented safely {dlb}
{
    MONS_TUNNELING_WORM, 't', LIGHTRED, "tunneling worm",
    M_NO_FLAGS,
    MR_RES_POISON,
    0, 10, 19, MH_NATURAL, MAG_IMMUNE,
    { 50, 0, 0, 0 },
    { 10, 5, 5, 0 },
    3, 3, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_INSECT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_HUGE
},

****************************************************************** */

{
    MONS_UGLY_THING, 'u', BROWN, "ugly thing",
    M_WARM_BLOOD | M_AMPHIBIOUS,
    MR_NO_FLAGS,
    600, 10, MONS_UGLY_THING, MONS_UGLY_THING, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 8, 3, 5, 0 },
    3, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_ANIMAL,
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_FIRE_VORTEX, 'v', RED, "fire vortex",
    M_LEVITATE | M_CONFUSED | M_INSUBSTANTIAL,
    MR_RES_POISON | mrd(MR_RES_FIRE, 3) | MR_VUL_COLD | MR_RES_ELEC,
    0, 5, MONS_FIRE_VORTEX, MONS_FIRE_VORTEX, MH_NONLIVING, MAG_IMMUNE,
    { {AT_HIT, AF_FIRE, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 3, 5, 0 },
    0, 5, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

{
    MONS_WORM, 'w', LIGHTRED, "worm",
    M_NO_SKELETON,
    MR_NO_FLAGS,
    350, 4, MONS_WORM, MONS_WORM, MH_NATURAL, -2,
    { {AT_BITE, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 3, 5, 0 },
    1, 5, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_SILENT, I_INSECT, 
    HT_LAND, 6, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_MEDIUM
},

// random
{
    MONS_ABOMINATION_SMALL, 'x', BLACK, "abomination",
    M_EVIL,
    MR_NO_FLAGS,
    0, 10, MONS_ABOMINATION_SMALL, MONS_ABOMINATION_SMALL, MH_DEMONIC, -5,
    { {AT_HIT, AF_PLAIN, 23}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 2, 5, 0 },
    0, 0, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 0, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

{
    MONS_YELLOW_WASP, 'y', YELLOW, "yellow wasp",
    M_FLIES,
    MR_VUL_POISON,
    220, 12, MONS_YELLOW_WASP, MONS_YELLOW_WASP, MH_NATURAL, -3,
    { {AT_STING, AF_PARALYSE, 13}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 4, 3, 5, 0 },
    5, 14, MST_NO_SPELLS, CE_POISONOUS, Z_NOZOMBIE, S_SILENT, I_INSECT, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

// small zombie
{
    MONS_ZOMBIE_SMALL, 'z', BROWN, "small zombie",
    M_EVIL,
    MR_RES_POISON | mrd(MR_RES_COLD, 2),
    0, 6, MONS_ZOMBIE_SMALL, MONS_ZOMBIE_SMALL, MH_UNDEAD, -1,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 5, 5, 0 },
    0, 4, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 5, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

{
    MONS_ANGEL, 'A', WHITE, "Angel",
    M_FIGHTER | M_FLIES | M_SPELLCASTER | M_SEE_INVIS | M_WARM_BLOOD,
    MR_RES_POISON | MR_RES_ELEC,
    0, 10, MONS_ANGEL, MONS_ANGEL, MH_HOLY, -8,
    { {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 12, 6, 5, 0 },
    10, 20, MST_ANGEL, CE_NOCORPSE, Z_NOZOMBIE, S_SHOUT, I_HIGH, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_GIANT_BEETLE, 'B', BLUE, "giant beetle",
    M_NO_FLAGS,
    MR_VUL_POISON,
    1000, 10, MONS_GIANT_BEETLE, MONS_GIANT_BEETLE, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 7, 6, 0 },
    10, 3, MST_NO_SPELLS, CE_POISONOUS, Z_BIG, S_SILENT, I_INSECT, 
    HT_LAND, 5, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_MEDIUM
},

{
    MONS_CYCLOPS, 'C', YELLOW, "cyclops",
    M_WARM_BLOOD | M_EVIL,
    MR_NO_FLAGS,
    2500, 10, MONS_HILL_GIANT, MONS_CYCLOPS, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 35}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 9, 3, 5, 0 },
    5, 3, MST_NO_SPELLS, CE_CLEAN, Z_BIG, S_SHOUT, I_NORMAL, 
    HT_LAND, 7, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_GIANT,
},

{
    MONS_DRAGON, 'D', GREEN, "dragon",
    M_FLIES | M_SPECIAL_ABILITY | M_WARM_BLOOD,
    MR_RES_POISON | mrd(MR_RES_FIRE, 2) | MR_VUL_COLD,
    2200, 12, MONS_DRAGON, MONS_DRAGON, MH_NATURAL, -4,
    { {AT_BITE, AF_PLAIN, 20}, {AT_CLAW, AF_PLAIN, 13}, {AT_CLAW, AF_PLAIN, 13}, AT_NO_ATK },
    { 12, 5, 5, 0 },
    10, 8, MST_NO_SPELLS, CE_CLEAN, Z_BIG, S_SILENT, I_ANIMAL,
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_HUGE
},

// These guys get understated because the experience code can't see
// that they wield two weapons... I'm raising their xp modifier. -- bwr
{
    MONS_TWO_HEADED_OGRE, 'O', LIGHTRED, "two-headed ogre",
    M_WARM_BLOOD | M_TWOWEAPON,
    MR_NO_FLAGS,
    1500, 15, MONS_OGRE, MONS_TWO_HEADED_OGRE, MH_NATURAL, -4,
    { {AT_HIT, AF_PLAIN, 17}, {AT_HIT, AF_PLAIN, 13}, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 5, 0 },
    1, 4, MST_NO_SPELLS, CE_CONTAMINATED, Z_BIG, S_SHOUT2, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_LARGE
},

{
    MONS_FIEND, '1', LIGHTRED, "Fiend",
    M_FLIES | M_SEE_INVIS | M_EVIL,
    MR_RES_POISON | MR_RES_HELLFIRE | MR_VUL_COLD,
    0, 18, MONS_FIEND, MONS_FIEND, MH_DEMONIC, -12,
    { {AT_HIT, AF_PLAIN, 25}, {AT_HIT, AF_PLAIN, 15}, {AT_HIT, AF_PLAIN, 15}, AT_NO_ATK },
    { 18, 3, 5, 0 },
      15, 6, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_ROAR, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_GIANT_SPORE, 'G', GREEN, "giant spore",
    M_LEVITATE | M_INSUBSTANTIAL,
    MR_RES_POISON | MR_RES_ASPHYX,
    0, 10, MONS_PLANT, MONS_GIANT_SPORE, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 1}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 0, 0, 1 },
    0, 10, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

{
    MONS_HOBGOBLIN, 'g', BROWN, "hobgoblin",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    500, 10, MONS_GOBLIN, MONS_HOBGOBLIN, MH_NATURAL, -1,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 4, 3, 0 },
    2, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_ICE_BEAST, 'I', WHITE, "ice beast",
    M_NO_FLAGS,
    MR_RES_POISON | MR_RES_ASPHYX | MR_VUL_FIRE | mrd(MR_RES_COLD, 3),
    0, 12, MONS_ICE_BEAST, MONS_ICE_BEAST, MH_NATURAL, -3,
    { {AT_HIT, AF_COLD, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 3, 5, 0 },
    5, 10, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_ANIMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

{
    MONS_JELLY, 'J', LIGHTRED, "jelly",
    M_SEE_INVIS | M_SPLITS | M_AMPHIBIOUS | M_ACID_SPLASH,
    MR_RES_POISON | MR_RES_ASPHYX | mrd(MR_RES_ACID, 3),
    0, 13, MONS_JELLY, MONS_JELLY, MH_NATURAL, -3,
    { {AT_HIT, AF_ACID, 8}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 5, 5, 0 },
    0, 2, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 9, DEFAULT_ENERGY, MONUSE_EATS_ITEMS, SIZE_MEDIUM
},

{
    MONS_KOBOLD, 'K', BROWN, "kobold",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    400, 10, MONS_KOBOLD, MONS_KOBOLD, MH_NATURAL, -1,
    { {AT_HIT, AF_PLAIN, 4}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 2, 3, 0 },
    2, 12, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_SMALL
},

{
    MONS_LICH, 'L', LIGHTGREY, "lich",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SEE_INVIS | M_EVIL,
    MR_RES_POISON | mrd(MR_RES_COLD, 2) | MR_RES_ELEC,
    0, 16, MONS_LICH, MONS_LICH, MH_UNDEAD, -11,
    { {AT_TOUCH, AF_DRAIN_XP, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 20, 2, 4, 0 },
    10, 10, MST_LICH_I, CE_NOCORPSE, Z_NOZOMBIE, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_MUMMY, 'M', LIGHTGREY, "mummy",
    M_EVIL,
    MR_RES_POISON | MR_VUL_FIRE | MR_RES_COLD,
    0, 10, MONS_MUMMY, MONS_MUMMY, MH_UNDEAD, -5,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 5, 3, 0 },
    3, 6, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_ANIMAL, 
    HT_LAND, 6, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_GUARDIAN_NAGA, 'N', LIGHTGREEN, "guardian naga",
    M_SPELLCASTER | M_SEE_INVIS | M_ACTUAL_SPELLS | M_WARM_BLOOD,
    MR_RES_POISON,
    350, 10, MONS_NAGA, MONS_GUARDIAN_NAGA, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 26}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 8, 3, 5, 0 },
    6, 14, MST_GUARDIAN_NAGA, CE_MUTAGEN_RANDOM, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_BIG
},

{
    MONS_OGRE, 'O', BROWN, "ogre",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    1300, 10, MONS_OGRE, MONS_OGRE, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 17}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 3, 5, 0 },
    1, 6, MST_NO_SPELLS, CE_CONTAMINATED, Z_BIG, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_LARGE
},

{
    MONS_PLANT, 'P', GREEN, "plant",
    M_NO_EXP_GAIN,
    MR_RES_POISON,
    0, 10, MONS_PLANT, MONS_PLANT, MH_PLANT, MAG_IMMUNE,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 3, 5, 0 },
    10, 0, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 0, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

{
    MONS_QUEEN_BEE, 'Q', YELLOW, "queen bee",
    M_FLIES,
    MR_VUL_POISON,
    200, 14, MONS_KILLER_BEE, MONS_QUEEN_BEE, MH_NATURAL, -3,
    { {AT_STING, AF_POISON_NASTY, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 7, 3, 5, 0 },
    10, 10, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_SILENT, I_INSECT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LITTLE,
},

{
    MONS_RAKSHASA, 'R', YELLOW, "rakshasa",
    M_SPELLCASTER | M_SEE_INVIS | M_EVIL | M_ACTUAL_SPELLS,
    MR_RES_POISON,
    0, 15, MONS_RAKSHASA, MONS_RAKSHASA, MH_DEMONIC, -10,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 3, 5, 0 },
    10, 14, MST_RAKSHASA, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_SNAKE, 'S', LIGHTGREEN, "snake",
    M_COLD_BLOOD | M_AMPHIBIOUS,
    MR_NO_FLAGS,
    200, 10, MONS_SNAKE, MONS_SNAKE, MH_NATURAL, -3,
    { {AT_BITE, AF_POISON, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 2, 3, 5, 0 },
    1, 15, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_SILENT, I_INSECT, 
    HT_LAND, 13, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

{
    MONS_TROLL, 'T', BROWN, "troll",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    1500, 10, MONS_TROLL, MONS_TROLL, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 20}, {AT_CLAW, AF_PLAIN, 15}, {AT_CLAW, AF_PLAIN, 15}, AT_NO_ATK },
    { 7, 3, 5, 0 },
    3, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_BIG, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_UNSEEN_HORROR, 'x', MAGENTA, "unseen horror",
    M_SEE_INVIS | M_INVIS,
    MR_NO_FLAGS,
    0, 12, MONS_UNSEEN_HORROR, MONS_UNSEEN_HORROR, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 7, 3, 5, 0 },
    5, 10, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_ANIMAL, 
    HT_LAND, 30, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_MEDIUM
},

{
    MONS_VAMPIRE, 'V', RED, "vampire",
    M_SPELLCASTER | M_SEE_INVIS | M_EVIL,
    MR_RES_POISON | MR_RES_COLD,
    0, 11, MONS_VAMPIRE, MONS_VAMPIRE, MH_UNDEAD, -6,
    { {AT_BITE, AF_VAMPIRIC, 22}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 5, 0 },
    10, 10, MST_VAMPIRE, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_WRAITH, 'W', WHITE, "wraith",
    M_LEVITATE | M_SEE_INVIS | M_EVIL,
    MR_RES_POISON | MR_RES_COLD,
    0, 11, MONS_WRAITH, MONS_WRAITH, MH_UNDEAD, -7,
    { {AT_HIT, AF_DRAIN_XP, 13}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 5, 0 },
    10, 10, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

// Large abom: (the previous one was small)
{
    MONS_ABOMINATION_LARGE, 'X', BLACK, "abomination",
    M_EVIL,
    MR_NO_FLAGS,
    0, 10, MONS_ABOMINATION_SMALL, MONS_ABOMINATION_LARGE, MH_DEMONIC, -7,
    { {AT_HIT, AF_PLAIN, 40}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 11, 2, 5, 0 },
    0, 0, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 0, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_BIG
},

{
    MONS_YAK, 'Y', BROWN, "yak",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    1200, 10, MONS_YAK, MONS_YAK, MH_NATURAL, -3,
    { {AT_BUTT, AF_PLAIN, 18}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 7, 3, 5, 0 },
    4, 7, MST_NO_SPELLS, CE_CLEAN, Z_BIG, S_BELLOW, I_ANIMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_BIG
},

// big zombie
{
    MONS_ZOMBIE_LARGE, 'Z', BROWN, "large zombie",
    M_EVIL,
    MR_RES_POISON | mrd(MR_RES_COLD, 2),
    0, 6, MONS_ZOMBIE_SMALL, MONS_ZOMBIE_LARGE, MH_UNDEAD, -1,
    { {AT_HIT, AF_PLAIN, 23}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 5, 0 },
    8, 5, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 5, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

{
    MONS_ORC_WARRIOR, 'o', LIGHTRED, "orc warrior",
    M_FIGHTER | M_WARM_BLOOD,
    MR_NO_FLAGS,
    0, 10, MONS_ORC, MONS_ORC, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 4, 4, 6, 0 },
    0, 13, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_KOBOLD_DEMONOLOGIST, 'K', MAGENTA, "kobold demonologist",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_WARM_BLOOD | M_EVIL,
    MR_NO_FLAGS,
    0, 10, MONS_KOBOLD, MONS_KOBOLD, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 4}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 4, 3, 5, 0 },
    2, 13, MST_KOBOLD_DEMONOLOGIST, CE_POISONOUS, Z_NOZOMBIE, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_SMALL
},

{
    MONS_ORC_WIZARD, 'o', MAGENTA, "orc wizard",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_WARM_BLOOD,
    MR_NO_FLAGS,
    0, 10, MONS_ORC, MONS_ORC, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 3, 4, 0 },
    1, 12, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_ORC_KNIGHT, 'o', CYAN, "orc knight",
    M_FIGHTER | M_WARM_BLOOD,
    MR_NO_FLAGS,
    0, 10, MONS_ORC, MONS_ORC, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 9, 4, 7, 0 },
    2, 13, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

/* ******************************************************************
// the tunneling worm is no more ...
// not until it can be reimplemented safely {dlb}
{
    MONS_WORM_TAIL, '~', LIGHTRED, "worm tail",
    M_NO_EXP_GAIN,
    MR_RES_POISON,
    0, 10, 56, MH_NATURAL, MAG_IMMUNE,
    { 0, 0, 0, 0 },
    { 10, 5, 5, 0 },
    3, 3, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_HUGE
},

****************************************************************** */

{
    MONS_WYVERN, 'D', LIGHTRED, "wyvern",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    2000, 10, MONS_WYVERN, MONS_WYVERN, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 3, 5, 0 },
    5, 10, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_SILENT, I_ANIMAL, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_BIG
},

{
    MONS_BIG_KOBOLD, 'K', YELLOW, "big kobold",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    0, 10, MONS_KOBOLD, MONS_BIG_KOBOLD, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 7}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 3, 5, 0 },
    3, 12, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_SILENT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_SMALL
},

{
    MONS_GIANT_EYEBALL, 'G', WHITE, "giant eyeball",
    M_NO_SKELETON | M_LEVITATE,
    MR_RES_ASPHYX,
    400, 10, MONS_GIANT_EYEBALL, MONS_GIANT_EYEBALL, MH_NATURAL, -3,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 3, 5, 0 },
    0, 1, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_SILENT, I_PLANT, 
    HT_LAND, 3, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_MEDIUM
},

{
    MONS_WIGHT, 'W', LIGHTGREY, "wight",
    M_EVIL,
    MR_RES_POISON | mrd(MR_RES_COLD, 2),
    0, 10, MONS_WRAITH, MONS_WIGHT, MH_UNDEAD, -4,
    { {AT_HIT, AF_DRAIN_XP, 8}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 3, 5, 0 },
    4, 10, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_OKLOB_PLANT, 'P', LIGHTGREEN, "oklob plant",
    M_SPECIAL_ABILITY,
    MR_RES_POISON,
    0, 10, MONS_PLANT, MONS_OKLOB_PLANT, MH_PLANT, -3,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 3, 5, 0 },
    10, 0, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

{
    MONS_WOLF_SPIDER, 's', BROWN, "wolf spider",
    M_NO_FLAGS,
    MR_VUL_POISON,
    800, 10, MONS_WOLF_SPIDER, MONS_WOLF_SPIDER, MH_NATURAL, -3,
    { {AT_BITE, AF_POISON_MEDIUM, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 8, 3, 5, 0 },
    3, 10, MST_NO_SPELLS, CE_POISONOUS, Z_BIG, S_HISS, I_INSECT, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_SHADOW, ' ', BLACK, "shadow",
    M_SEE_INVIS | M_EVIL | M_INSUBSTANTIAL,
    MR_RES_POISON | mrd(MR_RES_COLD, 3),
    0, 10, MONS_WRAITH, MONS_SHADOW, MH_UNDEAD, -5,
    { {AT_HIT, AF_DRAIN_STR, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 3, 5, 0 },
    12, 10, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_ANIMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_SMALL
},

{
    MONS_HUNGRY_GHOST, 'p', GREEN, "hungry ghost",
    M_SENSE_INVIS | M_FLIES | M_EVIL | M_INSUBSTANTIAL,
    MR_RES_POISON | mrd(MR_RES_COLD, 2),
    0, 10, MONS_PHANTOM, MONS_HUNGRY_GHOST, MH_UNDEAD, -4,
    { {AT_HIT, AF_HUNGER, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 7, 3, 5, 0 },
    0, 17, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_EYE_OF_DRAINING, 'G', LIGHTGREY, "eye of draining",
    M_NO_SKELETON | M_LEVITATE | M_SEE_INVIS,
    MR_RES_ASPHYX,
    400, 10, MONS_GIANT_EYEBALL, MONS_EYE_OF_DRAINING, MH_NATURAL, MAG_IMMUNE,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 7, 3, 5, 0 },
    3, 1, MST_NO_SPELLS, CE_MUTAGEN_RANDOM, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 5, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_MEDIUM
},

{
    MONS_BUTTERFLY, 'b', BLACK, "butterfly",
    M_FLIES | M_CONFUSED,
    MR_VUL_POISON | MR_RES_ASPHYX,
    150, 10, MONS_BUTTERFLY, MONS_BUTTERFLY, MH_NATURAL, -3,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 3, 5, 0 },
    0, 25, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_SILENT, I_INSECT, 
    HT_LAND, 25, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_WANDERING_MUSHROOM, 'f', BROWN, "wandering mushroom",
    M_NO_FLAGS,
    MR_RES_POISON,
    0, 10, MONS_PLANT, MONS_WANDERING_MUSHROOM, MH_PLANT, -3,
    { {AT_SPORE, AF_CONFUSE, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 8, 3, 5, 0 },
    5, 0, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_EFREET, 'E', RED, "efreet",
    M_SPELLCASTER | M_LEVITATE | M_EVIL,
    MR_RES_POISON | mrd(MR_RES_FIRE, 3) | MR_VUL_COLD,
    0, 12, MONS_EFREET, MONS_EFREET, MH_DEMONIC, -3,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 7, 3, 5, 0 },
    10, 5, MST_EFREET, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_LARGE
},

{
    MONS_BRAIN_WORM, 'w', LIGHTMAGENTA, "brain worm",
    M_SPELLCASTER,
    MR_NO_FLAGS,
    150, 10, MONS_WORM, MONS_BRAIN_WORM, MH_NATURAL, -3,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 3, 3, 0 },
    1, 5, MST_BRAIN_WORM, CE_POISONOUS, Z_SMALL, S_SILENT, I_INSECT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

{
    MONS_GIANT_ORANGE_BRAIN, 'G', LIGHTRED, "giant orange brain",
    M_NO_SKELETON | M_SPELLCASTER | M_LEVITATE | M_SEE_INVIS | M_WARM_BLOOD,
    MR_RES_ASPHYX,
    1000, 13, MONS_GIANT_ORANGE_BRAIN, MONS_GIANT_ORANGE_BRAIN, MH_NATURAL, -8,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 3, 5, 0 },
    2, 4, MST_GIANT_ORANGE_BRAIN, CE_MUTAGEN_RANDOM, Z_NOZOMBIE, S_SILENT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_SMALL
},

{
    MONS_BOULDER_BEETLE, 'B', LIGHTGREY, "boulder beetle",
    M_NO_FLAGS,
    MR_VUL_POISON,
    2500, 10, MONS_GIANT_BEETLE, MONS_BOULDER_BEETLE, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 35}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 9, 3, 5, 0 },
    20, 2, MST_NO_SPELLS, CE_POISONOUS, Z_BIG, S_SILENT, I_INSECT, 
    HT_LAND, 3, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

{
    MONS_FLYING_SKULL, 'z', WHITE, "flying skull",
    M_LEVITATE,
    MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC,
    0, 10, MONS_SKELETON_SMALL, MONS_FLYING_SKULL, MH_UNDEAD, -3,
    { {AT_HIT, AF_PLAIN, 7}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 2, 3, 5, 0 },
    10, 17, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SCREAM, I_ANIMAL, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_HELL_HOUND, 'h', LIGHTCYAN, "hell hound",
    M_SENSE_INVIS | M_EVIL | M_SPECIAL_ABILITY,
    MR_RES_POISON | MR_RES_HELLFIRE | MR_VUL_COLD,
    0, 10, MONS_HOUND, MONS_HELL_HOUND, MH_DEMONIC, -3,
    { {AT_BITE, AF_PLAIN, 13}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 3, 5, 0 },
    6, 13, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_BARK, I_NORMAL, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_MEDIUM
},

{
    MONS_MINOTAUR, 't', RED, "minotaur",
    M_FIGHTER | M_WARM_BLOOD,
    MR_NO_FLAGS,
    1500, 10, MONS_MINOTAUR, MONS_MINOTAUR, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 35}, {AT_BUTT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK },
    { 13, 3, 5, 0 },
    5, 7, MST_NO_SPELLS, CE_CONTAMINATED, Z_BIG, S_BELLOW, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_ICE_DRAGON, 'D', WHITE, "ice dragon",
    M_FLIES | M_SPECIAL_ABILITY | M_COLD_BLOOD,
    MR_RES_POISON | MR_VUL_FIRE | mrd(MR_RES_COLD, 2),
    2200, 10, MONS_DRAGON, MONS_ICE_DRAGON, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 17}, {AT_CLAW, AF_PLAIN, 17}, {AT_CLAW, AF_PLAIN, 17}, AT_NO_ATK },
    { 12, 5, 5, 0 },
    10, 8, MST_NO_SPELLS, CE_CLEAN, Z_BIG, S_SILENT, I_ANIMAL,
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_HUGE
},

{
    MONS_SLIME_CREATURE, 'J', GREEN, "slime creature",
    M_AMPHIBIOUS,
    MR_RES_POISON,
    0, 5, MONS_SLIME_CREATURE, MONS_SLIME_CREATURE, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 22}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 11, 3, 5, 0 },
    1, 4, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

{
    MONS_FREEZING_WRAITH, 'W', LIGHTBLUE, "freezing wraith",
    M_LEVITATE | M_SEE_INVIS | M_EVIL,
    MR_RES_POISON | MR_VUL_FIRE | mrd(MR_RES_COLD, 3),
    0, 10, MONS_WRAITH, MONS_FREEZING_WRAITH, MH_UNDEAD, -4,
    { {AT_HIT, AF_COLD, 19}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 8, 3, 5, 0 },
    12, 10, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_NORMAL, 
    HT_LAND, 8, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

// fake R - conjured by the R's illusion spell.
{
    MONS_RAKSHASA_FAKE, 'R', YELLOW, "rakshasa",
    M_EVIL,
    MR_RES_POISON,
    0, 10, MONS_RAKSHASA_FAKE, MONS_RAKSHASA_FAKE, MH_DEMONIC, MAG_IMMUNE,
    { {AT_HIT, AF_PLAIN, 0}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 0, 0, 1 },
    0, 30, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_ROAR, I_PLANT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_MEDIUM
},

{
    MONS_GREAT_ORB_OF_EYES, 'G', LIGHTGREEN, "great orb of eyes",
    M_NO_SKELETON | M_SPELLCASTER | M_LEVITATE | M_SEE_INVIS,
    MR_RES_POISON,
    900, 13, MONS_GIANT_EYEBALL, MONS_GREAT_ORB_OF_EYES, MH_NATURAL, MAG_IMMUNE,
    { {AT_BITE, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 12, 3, 5, 0 },
    10, 3, MST_GREAT_ORB_OF_EYES, CE_MUTAGEN_RANDOM, Z_NOZOMBIE, S_SILENT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_HELLION, '3', EC_FIRE, "hellion",
    M_SPELLCASTER | M_EVIL,
    MR_RES_POISON | MR_RES_HELLFIRE | MR_VUL_COLD,
    0, 11, MONS_HELLION, MONS_HELLION, MH_DEMONIC, -7,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 7, 3, 5, 0 },
    5, 10, MST_BURNING_DEVIL, CE_NOCORPSE, Z_NOZOMBIE, S_SCREAM, I_HIGH, 
    HT_LAND, 13, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_ROTTING_DEVIL, '4', GREEN, "rotting devil",
    M_EVIL,
    MR_RES_POISON | MR_RES_COLD,
    0, 10, MONS_ROTTING_DEVIL, MONS_ROTTING_DEVIL, MH_DEMONIC, -7,
    { {AT_HIT, AF_ROT, 8}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 3, 5, 0 },
    2, 10, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_TORMENTOR, '3', YELLOW, "tormentor",
    M_SPELLCASTER | M_FLIES | M_SPEAKS | M_EVIL,
    MR_RES_POISON | MR_RES_FIRE,
    0, 10, MONS_TORMENTOR, MONS_TORMENTOR, MH_DEMONIC, -6,
    { {AT_HIT, AF_PLAIN, 8}, {AT_HIT, AF_PLAIN, 8}, AT_NO_ATK, AT_NO_ATK },
    { 7, 3, 5, 0 },
    12, 12, MST_TORMENTOR, CE_NOCORPSE, Z_NOZOMBIE, S_ROAR, I_HIGH, 
    HT_LAND, 13, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_REAPER, '2', LIGHTGREY, "reaper",
    M_FIGHTER | M_SEE_INVIS | M_EVIL,
    MR_RES_POISON | MR_RES_COLD,
    0, 10, MONS_REAPER, MONS_REAPER, MH_DEMONIC, MAG_IMMUNE,
    { {AT_HIT, AF_PLAIN, 32}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 8, 3, 5, 0 },
    15, 10, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM
},

{
    MONS_SOUL_EATER, '2', MAGENTA, "soul eater",
    M_LEVITATE | M_SEE_INVIS | M_EVIL,
    MR_RES_POISON | MR_RES_COLD,
    0, 12, MONS_SOUL_EATER, MONS_SOUL_EATER, MH_DEMONIC, -10,
    { {AT_HIT, AF_DRAIN_XP, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 11, 3, 5, 0 },
    18, 10, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_HAIRY_DEVIL, '4', LIGHTRED, "hairy devil",
    M_EVIL,
    MR_RES_POISON,
    0, 10, MONS_HAIRY_DEVIL, MONS_HAIRY_DEVIL, MH_DEMONIC, -4,
    { {AT_HIT, AF_PLAIN, 9}, {AT_HIT, AF_PLAIN, 9}, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 5, 0 },
    7, 10, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_SMALL
},

{
    MONS_ICE_DEVIL, '2', WHITE, "ice devil",
    M_EVIL,
    MR_RES_POISON | MR_VUL_FIRE | mrd(MR_RES_COLD, 3),
    0, 11, MONS_ICE_DEVIL, MONS_ICE_DEVIL, MH_DEMONIC, -6,
    { {AT_HIT, AF_COLD, 16}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 11, 3, 5, 0 },
    12, 10, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_BLUE_DEVIL, '3', BLUE, "blue devil",
    M_FLIES | M_EVIL,
    MR_RES_POISON | MR_VUL_FIRE | mrd(MR_RES_COLD, 3),
    0, 10, MONS_BLUE_DEVIL, MONS_BLUE_DEVIL, MH_DEMONIC, -5,
    { {AT_HIT, AF_PLAIN, 21}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 7, 3, 5, 0 },
    14, 10, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

// random
{
    MONS_BEAST, '4', BROWN, "beast",
    M_EVIL | M_FIGHTER,
    MR_NO_FLAGS,
    0, 10, MONS_BEAST, MONS_BEAST, MH_DEMONIC, -3,
    { {AT_BITE, AF_PLAIN, 28}, {AT_CLAW, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK },
    { 7, 9, 6, 0 },
    0, 0, MST_NO_SPELLS, CE_CONTAMINATED, Z_NOZOMBIE, S_RANDOM, I_NORMAL, 
    HT_LAND, 0, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_IRON_DEVIL, '3', CYAN, "iron devil",
    M_EVIL,
    MR_RES_ELEC | MR_RES_POISON | MR_RES_HELLFIRE | MR_RES_COLD,
    0, 10, MONS_IRON_DEVIL, MONS_IRON_DEVIL, MH_DEMONIC, -6,
    { {AT_HIT, AF_PLAIN, 14}, {AT_HIT, AF_PLAIN, 14}, AT_NO_ATK, AT_NO_ATK },
    { 8, 3, 5, 0 },
    16, 8, MST_NO_SPELLS, CE_CONTAMINATED, Z_NOZOMBIE, S_SCREECH, I_HIGH, 
    HT_LAND, 8, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_GLOWING_SHAPESHIFTER, '@', RED, "glowing shapeshifter",
    M_NO_FLAGS,
    MR_NO_FLAGS,
    600, 10, MONS_SHAPESHIFTER, MONS_GLOWING_SHAPESHIFTER, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 3, 5, 0 },
    0, 10, MST_NO_SPELLS, CE_MUTAGEN_RANDOM, Z_NOZOMBIE, S_SILENT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_SHAPESHIFTER, '@', LIGHTRED, "shapeshifter",
    M_NO_FLAGS,
    MR_NO_FLAGS,
    600, 10, MONS_SHAPESHIFTER, MONS_SHAPESHIFTER, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 7, 3, 5, 0 },
    0, 10, MST_NO_SPELLS, CE_MUTAGEN_RANDOM, Z_NOZOMBIE, S_SILENT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_GIANT_MITE, 's', LIGHTRED, "giant mite",
    M_NO_FLAGS,
    MR_VUL_POISON,
    350, 10, MONS_GIANT_MITE, MONS_GIANT_MITE, MH_NATURAL, -1,
    { {AT_BITE, AF_POISON, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 2, 3, 5, 0 },
    1, 7, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_SILENT, I_INSECT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_STEAM_DRAGON, 'D', BLUE, "steam dragon",
    M_SPELLCASTER | M_FLIES | M_WARM_BLOOD,
    MR_RES_STEAM,
    1000, 10, MONS_DRAGON, MONS_STEAM_DRAGON, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 12}, {AT_CLAW, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK },
    { 4, 5, 5, 0 },
    5, 10, MST_STEAM_DRAGON, CE_CLEAN, Z_BIG, S_SILENT, I_ANIMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_GIANT,
},

{
    MONS_VERY_UGLY_THING, 'u', RED, "very ugly thing",
    M_WARM_BLOOD | M_AMPHIBIOUS,
    MR_NO_FLAGS,
    750, 10, MONS_UGLY_THING, MONS_VERY_UGLY_THING, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 17}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 12, 3, 5, 0 },
    4, 8, MST_NO_SPELLS, CE_MUTAGEN_RANDOM, Z_BIG, S_SHOUT, I_ANIMAL,
    HT_LAND, 8, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_ORC_SORCERER, 'o', LIGHTMAGENTA, "orc sorcerer",
    M_SPELLCASTER | M_SEE_INVIS | M_SPEAKS | M_ACTUAL_SPELLS
        | M_WARM_BLOOD | M_EVIL,
    MR_RES_FIRE,
    600, 12, MONS_ORC, MONS_ORC, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 7}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 9, 2, 3, 0 },
    5, 12, MST_ORC_SORCERER, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_HIPPOGRIFF, 'H', BROWN, "hippogriff",
    M_FLIES | M_WARM_BLOOD,
    MR_NO_FLAGS,
    1000, 10, MONS_HIPPOGRIFF, MONS_HIPPOGRIFF, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 10}, {AT_CLAW, AF_PLAIN, 8}, {AT_CLAW, AF_PLAIN, 8}, AT_NO_ATK },
    { 7, 3, 5, 0 },
    2, 7, MST_NO_SPELLS, CE_CLEAN, Z_BIG, S_SCREECH, I_ANIMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_BIG
},

{
    MONS_GRIFFON, 'H', YELLOW, "griffon",
    M_FLIES | M_WARM_BLOOD,
    MR_NO_FLAGS,
    1800, 10, MONS_GRIFFON, MONS_GRIFFON, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 18}, {AT_CLAW, AF_PLAIN, 10}, {AT_CLAW, AF_PLAIN, 10}, AT_NO_ATK },
    { 12, 3, 5, 0 },
    4, 6, MST_NO_SPELLS, CE_CLEAN, Z_BIG, S_SCREECH, I_ANIMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_BIG
},

{
    MONS_HYDRA, 'D', LIGHTGREEN, "hydra",
    M_AMPHIBIOUS | M_WARM_BLOOD,  // because it likes the swamp -- bwr
    MR_RES_POISON,
    1800, 11, MONS_HYDRA, MONS_HYDRA, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 18}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 13, 3, 5, 0 },
    0, 5, MST_NO_SPELLS, CE_POISONOUS, Z_NOZOMBIE, S_ROAR, I_INSECT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_BIG
},

// small skeleton
{
    MONS_SKELETON_SMALL, 'z', LIGHTGREY, "small skeleton",
    M_EVIL,
    MR_RES_POISON | mrd(MR_RES_COLD, 2),
    0, 10, MONS_SKELETON_SMALL, MONS_SKELETON_SMALL, MH_UNDEAD, -1,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 0, 0, 0, 0 },
    0, 0, MST_NO_SPELLS, CE_CONTAMINATED, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 0, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

// large skeleton
{
    MONS_SKELETON_LARGE, 'Z', LIGHTGREY, "large skeleton",
    M_EVIL,
    MR_RES_POISON | mrd(MR_RES_COLD, 2),
    0, 10, MONS_SKELETON_SMALL, MONS_SKELETON_LARGE, MH_UNDEAD, -1,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 0, 0, 0, 0 },
    0, 0, MST_NO_SPELLS, CE_CONTAMINATED, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 0, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

{
    MONS_HELL_KNIGHT, '@', RED, "hell knight",
    M_FIGHTER | M_SPELLCASTER | M_ACTUAL_SPELLS | M_WARM_BLOOD | M_EVIL,
    mrd(MR_RES_FIRE, 2),
    550, 10, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 13}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 3, 6, 0 },
    0, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_NECROMANCER, '@', WHITE, "necromancer",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_WARM_BLOOD | M_EVIL,
    MR_NO_FLAGS,
    550, 10, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -4,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 2, 4, 0 },
    0, 13, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_WIZARD, '@', MAGENTA, "wizard",
    M_SPELLCASTER | M_SPEAKS | M_ACTUAL_SPELLS | M_WARM_BLOOD,
    MR_RES_ELEC,
    550, 10, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -4,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 2, 4, 0 },
    0, 13, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_ORC_PRIEST, 'o', GREEN, "orc priest",
    M_SPELLCASTER | M_PRIEST | M_WARM_BLOOD | M_EVIL,
    MR_NO_FLAGS,
    600, 10, MONS_ORC, MONS_ORC, MH_NATURAL, -4,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 3, 4, 0 },
    1, 10, MST_ORC_PRIEST, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_ORC_HIGH_PRIEST, 'o', LIGHTGREEN, "orc high priest",
    M_SPELLCASTER | M_SEE_INVIS | M_SPEAKS | M_PRIEST | M_WARM_BLOOD | M_EVIL,
      MR_RES_HELLFIRE,
      600, 10, MONS_ORC, MONS_ORC, MH_NATURAL, -4,
    { {AT_HIT, AF_PLAIN, 7}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 11, 3, 4, 0 },
    1, 12, MST_ORC_HIGH_PRIEST, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

// this is a dummy monster, used for corpses
// mv:but it can be generated by polymorph spells and because IMO it's
// logical polymorph target so complete monster statistics should exist.
// Same thing for elf dummy monster.

{
    MONS_HUMAN, '@', LIGHTGRAY, "human",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    550, 10, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 3, 5, 0 },
    0, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_GNOLL, 'g', YELLOW, "gnoll",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    750, 10, MONS_GNOLL, MONS_GNOLL, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 9}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 2, 4, 5, 0 },
    2, 9, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_CLAY_GOLEM, '8', BROWN, "clay golem",
    M_SEE_INVIS,
    mrd(MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC, 3),
    0, 10, MONS_CLAY_GOLEM, MONS_CLAY_GOLEM, MH_NONLIVING, MAG_IMMUNE,
    { {AT_HIT, AF_PLAIN, 11}, {AT_HIT, AF_PLAIN, 11}, AT_NO_ATK, AT_NO_ATK },
    { 8, 7, 3, 0 },
    7, 5, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 8, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_WOOD_GOLEM, '8', YELLOW, "wood golem",
    M_NO_FLAGS,
    MR_RES_POISON | MR_VUL_FIRE | MR_RES_COLD | MR_RES_ELEC,
    0, 10, MONS_CLAY_GOLEM, MONS_WOOD_GOLEM, MH_NONLIVING, MAG_IMMUNE,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 6, 3, 0 },
    5, 6, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_STONE_GOLEM, '8', LIGHTGREY, "stone golem",
    M_NO_FLAGS,
    mrd(MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC, 3),
    0, 10, MONS_CLAY_GOLEM, MONS_STONE_GOLEM, MH_NONLIVING, MAG_IMMUNE,
    { {AT_HIT, AF_PLAIN, 28}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 12, 7, 4, 0 },
    12, 4, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 7, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_IRON_GOLEM, '8', CYAN, "iron golem",
    M_SEE_INVIS,
    mrd(MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC, 3),
    0, 10, MONS_CLAY_GOLEM, MONS_IRON_GOLEM, MH_NONLIVING, MAG_IMMUNE,
    { {AT_HIT, AF_PLAIN, 35}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 15, 7, 4, 0 },
    15, 3, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 7, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_CRYSTAL_GOLEM, '8', GREEN, "crystal golem",
    M_SEE_INVIS | M_SPEAKS,
    mrd(MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC, 3),
    0, 10, MONS_CLAY_GOLEM, MONS_CRYSTAL_GOLEM, MH_NONLIVING, MAG_IMMUNE,
    { {AT_HIT, AF_PLAIN, 40}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 13, 7, 4, 0 },
    22, 3, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 7, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_TOENAIL_GOLEM, '8', RED, "toenail golem",
    M_NO_FLAGS,
    MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC,
    0, 10, MONS_CLAY_GOLEM, MONS_TOENAIL_GOLEM, MH_NONLIVING, MAG_IMMUNE,
    { {AT_HIT, AF_PLAIN, 13}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 9, 5, 3, 0 },
    8, 5, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 8, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_MOTTLED_DRAGON, 'D', LIGHTMAGENTA, "mottled dragon",
    M_SPELLCASTER | M_FLIES | M_WARM_BLOOD,
    MR_RES_POISON | MR_RES_FIRE | MR_RES_STICKY_FLAME,
    1100, 10, MONS_DRAGON, MONS_MOTTLED_DRAGON, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 15}, {AT_CLAW, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK },
    { 5, 3, 5, 0 },
    5, 10, MST_MOTTLED_DRAGON, CE_POISONOUS, Z_BIG, S_SILENT, I_ANIMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_GIANT,
},

{
    MONS_EARTH_ELEMENTAL, '#', BROWN, "earth elemental",
    M_NO_FLAGS,
    mrd(MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC, 3),
    0, 10, MONS_EARTH_ELEMENTAL, MONS_EARTH_ELEMENTAL, MH_NONLIVING, MAG_IMMUNE,
    { {AT_HIT, AF_PLAIN, 40}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 5, 5, 0 },
    14, 4, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 6, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_BIG
},

{
    MONS_FIRE_ELEMENTAL, '#', YELLOW, "fire elemental",
    M_FLIES | M_INSUBSTANTIAL,
    MR_RES_POISON | MR_RES_HELLFIRE | MR_VUL_COLD | MR_RES_ELEC,
    0, 10, MONS_EARTH_ELEMENTAL, MONS_FIRE_ELEMENTAL, MH_NONLIVING, MAG_IMMUNE,
    { {AT_HIT, AF_FIRE, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 5, 0 },
    4, 12, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 13, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_BIG
},

{
    MONS_AIR_ELEMENTAL, 'v', LIGHTGREY, "air elemental",
    M_LEVITATE | M_SEE_INVIS | M_FLIES | M_INSUBSTANTIAL,
    mrd(MR_RES_ELEC, 3) | MR_RES_POISON,
    0, 5, MONS_EARTH_ELEMENTAL, MONS_AIR_ELEMENTAL, MH_NONLIVING, MAG_IMMUNE,
    { {AT_HIT, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 5, 0 },
    2, 18, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 25, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_BIG
},

// water elementals are later (with the other water monsters)

{
    MONS_ICE_FIEND, '1', WHITE, "Ice Fiend",
    M_SPELLCASTER | M_FLIES | M_SEE_INVIS | M_EVIL,
    MR_RES_POISON | MR_VUL_FIRE | mrd(MR_RES_COLD, 3),
    0, 10, MONS_FIEND, MONS_ICE_FIEND, MH_DEMONIC, -12,
    { {AT_CLAW, AF_COLD, 25}, {AT_CLAW, AF_COLD, 25}, AT_NO_ATK, AT_NO_ATK },
    { 18, 3, 5, 0 },
    15, 6, MST_ICE_FIEND, CE_CONTAMINATED, Z_NOZOMBIE, S_ROAR, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_SHADOW_FIEND, '1', MAGENTA, "Shadow Fiend",
    M_SPELLCASTER | M_LEVITATE | M_SEE_INVIS | M_EVIL,
    MR_RES_POISON | mrd(MR_RES_COLD, 2) | MR_RES_ELEC,
    0, 10, MONS_FIEND, MONS_SHADOW_FIEND, MH_DEMONIC, -13,
    { {AT_HIT, AF_DRAIN_XP, 25}, {AT_HIT, AF_DRAIN_XP, 15}, {AT_HIT, AF_DRAIN_XP, 15}, AT_NO_ATK },
    { 18, 3, 5, 0 },
    15, 6, MST_SHADOW_FIEND, CE_CONTAMINATED, Z_NOZOMBIE, S_ROAR, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_BROWN_SNAKE, 'S', BROWN, "brown snake",
    M_COLD_BLOOD | M_AMPHIBIOUS,
    MR_RES_POISON,
    300, 10, MONS_SNAKE, MONS_BROWN_SNAKE, MH_NATURAL, -3,
    { {AT_BITE, AF_POISON_MEDIUM, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 4, 3, 5, 0 },
    2, 15, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_HISS, I_INSECT, 
    HT_LAND, 14, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_MEDIUM
},

{
    MONS_GIANT_LIZARD, 'l', GREEN, "giant lizard",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    600, 10, MONS_GIANT_LIZARD, MONS_GIANT_LIZARD, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 3, 5, 0 },
    4, 10, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_SILENT, I_INSECT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LITTLE,
},

{
    MONS_SPECTRAL_WARRIOR, 'W', LIGHTGREEN, "spectral warrior",
    M_LEVITATE | M_SEE_INVIS | M_EVIL,
    MR_RES_POISON | MR_RES_COLD,
    0, 13, MONS_WRAITH, MONS_SPECTRAL_WARRIOR, MH_UNDEAD, -6,
    { {AT_HIT, AF_DRAIN_XP, 18}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 9, 3, 5, 0 },
    12, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_NOZOMBIE, S_SILENT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_PULSATING_LUMP, 'J', RED, "pulsating lump",
    M_SENSE_INVIS,
    MR_RES_POISON | MR_RES_ASPHYX,
    0, 3, MONS_JELLY, MONS_PULSATING_LUMP, MH_NATURAL, -3,
    { {AT_HIT, AF_MUTATE, 13}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 3, 5, 0 },
    2, 6, MST_NO_SPELLS, CE_CONTAMINATED, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 5, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LITTLE,
},

{
    MONS_STORM_DRAGON, 'D', LIGHTBLUE, "storm dragon",
    M_SPELLCASTER | M_FLIES | M_WARM_BLOOD,
    mrd(MR_RES_ELEC, 3) | MR_RES_COLD,
    2800, 12, MONS_DRAGON, MONS_STORM_DRAGON, MH_NATURAL, -5,
    { {AT_BITE, AF_PLAIN, 25}, {AT_CLAW, AF_PLAIN, 15}, {AT_CLAW, AF_PLAIN, 15}, AT_NO_ATK },
    { 14, 5, 5, 0 },
    13, 10, MST_STORM_DRAGON, CE_CLEAN, Z_BIG, S_ROAR, I_ANIMAL,
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_HUGE
},

{
    MONS_YAKTAUR, 'c', RED, "yaktaur",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    2000, 10, MONS_YAKTAUR, MONS_YAKTAUR, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 8, 3, 5, 0 },
    4, 4, MST_NO_SPELLS, CE_CLEAN, Z_BIG, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_BIG
},

{
    MONS_DEATH_YAK, 'Y', YELLOW, "death yak",
    M_WARM_BLOOD | M_EVIL,
    MR_NO_FLAGS,
    1500, 10, MONS_YAK, MONS_DEATH_YAK, MH_NATURAL, -5,
    { {AT_BUTT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 14, 3, 5, 0 },
    9, 5, MST_NO_SPELLS, CE_POISONOUS, Z_BIG, S_BELLOW, I_ANIMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_BIG
},

{
    MONS_ROCK_TROLL, 'T', LIGHTGREY, "rock troll",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    2200, 11, MONS_TROLL, MONS_ROCK_TROLL, MH_NATURAL, -4,
    { {AT_BITE, AF_PLAIN, 30}, {AT_CLAW, AF_PLAIN, 20}, {AT_CLAW, AF_PLAIN, 20}, AT_NO_ATK },
    { 11, 3, 5, 0 },
    13, 6, MST_NO_SPELLS, CE_CONTAMINATED, Z_BIG, S_SHOUT, I_NORMAL, 
    HT_LAND, 8, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_STONE_GIANT, 'C', LIGHTGREY, "stone giant",
    M_WARM_BLOOD | M_EVIL,
    MR_NO_FLAGS,
    3000, 10, MONS_HILL_GIANT, MONS_STONE_GIANT, MH_NATURAL, -4,
    { {AT_HIT, AF_PLAIN, 45}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 16, 3, 5, 0 },
    12, 2, MST_NO_SPELLS, CE_CONTAMINATED, Z_BIG, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_GIANT,
},

{
    MONS_FLAYED_GHOST, 'p', RED, "flayed ghost",
    M_FLIES | M_EVIL,
    MR_RES_POISON,
    0, 10, MONS_PHANTOM, MONS_FLAYED_GHOST, MH_UNDEAD, -4,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 11, 3, 5, 0 },
    0, 14, MST_NO_SPELLS, CE_CONTAMINATED, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_BUMBLEBEE, 'k', RED, "bumblebee",
    M_FLIES,
    MR_VUL_POISON,
    300, 10, MONS_KILLER_BEE, MONS_BUMBLEBEE, MH_NATURAL, -3,
    { {AT_STING, AF_POISON_MEDIUM, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 7, 3, 5, 0 },
    4, 15, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_BUZZ, I_INSECT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_REDBACK, 's', RED, "redback",
    M_NO_FLAGS,
    MR_VUL_POISON,
    1000, 14, MONS_WOLF_SPIDER, MONS_REDBACK, MH_NATURAL, -3,
    { {AT_BITE, AF_POISON_STRONG, 18}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 5, 0 },
    2, 12, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_SILENT, I_INSECT, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_INSUBSTANTIAL_WISP, 'p', LIGHTGREY, "insubstantial wisp",
    M_LEVITATE | M_SPECIAL_ABILITY | M_INSUBSTANTIAL,
    mrd(MR_RES_ELEC | MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD, 2),
    0, 17, MONS_INSUBSTANTIAL_WISP, MONS_INSUBSTANTIAL_WISP, MH_NONLIVING, MAG_IMMUNE,
    { {AT_HIT, AF_BLINK, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 1, 2, 0 },
    20, 20, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_MOAN, I_PLANT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_VAPOUR, '#', LIGHTGREY, "vapour",
    M_SPELLCASTER | M_LEVITATE | M_SEE_INVIS | M_INVIS | M_CONFUSED | M_INSUBSTANTIAL,
    mrd(MR_RES_ELEC, 3) | MR_RES_POISON,
    0, 21, MONS_VAPOUR, MONS_VAPOUR, MH_NONLIVING, MAG_IMMUNE,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 12, 2, 3, 0 },
    0, 12, MST_STORM_DRAGON, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LITTLE,
},

{
    MONS_OGRE_MAGE, 'O', MAGENTA, "ogre-mage",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SEE_INVIS | M_WARM_BLOOD,
    MR_RES_ELEC,
    0, 16, MONS_OGRE, MONS_OGRE, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 3, 5, 0 },
    1, 7, MST_NO_SPELLS, CE_CONTAMINATED, Z_BIG, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_LARGE
},

{
    MONS_SPINY_WORM, 'w', LIGHTGREEN, "spiny worm",
    M_NO_FLAGS,
    MR_VUL_POISON | MR_RES_ACID,
    1300, 13, MONS_WORM, MONS_SPINY_WORM, MH_NATURAL, -3,
    { {AT_STING, AF_ACID, 32}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 12, 3, 5, 0 },
    10, 6, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_SILENT, I_PLANT, 
    HT_LAND, 9, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_BIG
},

// These are named more explicitly when they attack, also when you use 'x'
// to examine them.
{
    MONS_DANCING_WEAPON, '(', BLACK, "dancing weapon",
    M_FIGHTER | M_LEVITATE,
    mrd(MR_RES_POISON | MR_RES_HELLFIRE | MR_RES_COLD, 3) | MR_RES_ELEC,
    0, 10, MONS_DANCING_WEAPON, MONS_DANCING_WEAPON, MH_NONLIVING, MAG_IMMUNE,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 15, 0, 0, 15 },
    10, 20, MST_NO_SPELLS, CE_CONTAMINATED, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

{
    MONS_TITAN, 'C', MAGENTA, "titan",
    M_FIGHTER | M_SPELLCASTER | M_WARM_BLOOD | M_SENSE_INVIS | M_EVIL,
    mrd(MR_RES_ELEC, 2),
    3500, 12, MONS_HILL_GIANT, MONS_TITAN, MH_NATURAL, -7,
    { {AT_HIT, AF_PLAIN, 55}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 20, 3, 5, 0 },
    10, 3, MST_TITAN, CE_CLEAN, Z_BIG, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_GIANT,
},

{
    MONS_GOLDEN_DRAGON, 'D', YELLOW, "golden dragon",
    M_SPELLCASTER | M_FLIES | M_SENSE_INVIS | M_WARM_BLOOD,
    MR_RES_ELEC | MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD,
    3000, 17, MONS_DRAGON, MONS_GOLDEN_DRAGON, MH_NATURAL, -8,
    { {AT_BITE, AF_PLAIN, 40}, {AT_CLAW, AF_PLAIN, 20}, {AT_CLAW, AF_PLAIN, 20}, AT_NO_ATK },
    { 18, 4, 4, 0 },
    15, 7, MST_GOLDEN_DRAGON, CE_POISONOUS, Z_BIG, S_ROAR, I_ANIMAL,
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_HUGE
},

// 147 - dummy monster, used for corpses etc.
//mv: have to exist because it's (and should be) valid polymorph target.
{
    MONS_ELF, 'e', RED, "elf",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    450, 10, MONS_ELF, MONS_ELF, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 3, 3, 0 },
    0, 12, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SILENT, I_NORMAL,
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

// Used to be "lindworm" and a GREEN 'l'...  I'm hoping that by 
// making it a 'd' and using an alternate spelling people will 
// more intuitively know that this isn't a regular lizard. -- bwr
// Changed back to 'l' for differentiation as the 'd' glyph is
// packed with draconians already, and a lightcyan l is unique -- jpeg, Feb 08
{
    MONS_LINDWURM, 'l', LIGHTCYAN, "lindwurm",
    M_SPECIAL_ABILITY | M_WARM_BLOOD,
    MR_NO_FLAGS,
    1000, 11, MONS_DRAGON, MONS_LINDWURM, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 20}, {AT_CLAW, AF_PLAIN, 10}, {AT_CLAW, AF_PLAIN, 10}, AT_NO_ATK },
    { 9, 3, 5, 0 },
    8, 6, MST_NO_SPELLS, CE_CLEAN, Z_BIG, S_ROAR, I_INSECT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_GIANT,
},

{
    MONS_ELEPHANT_SLUG, 'j', LIGHTGREY, "elephant slug",
    M_NO_SKELETON,
    MR_VUL_POISON,
    1500, 10, MONS_GIANT_SLUG, MONS_ELEPHANT_SLUG, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 40}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 20, 5, 3, 0 },
    2, 1, MST_NO_SPELLS, CE_POISONOUS, Z_BIG, S_SILENT, I_INSECT, 
    HT_LAND, 4, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_BIG
},

{
    MONS_WAR_DOG, 'h', CYAN, "war dog",
    M_SENSE_INVIS | M_WARM_BLOOD,
    MR_NO_FLAGS,
    350, 10, MONS_HOUND, MONS_WAR_DOG, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 4, 3, 5, 0 },
    4, 15, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_BARK, I_ANIMAL, 
    HT_LAND, 17, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_MEDIUM
},

{
    MONS_GREY_RAT, 'r', LIGHTGREY, "grey rat",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    250, 10, MONS_RAT, MONS_GREY_RAT, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 3, 6, 0 },
    2, 12, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SILENT, I_ANIMAL, 
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_GREEN_RAT, 'r', LIGHTGREEN, "green rat",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    250, 10, MONS_RAT, MONS_GREEN_RAT, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 2, 3, 5, 0 },
    5, 11, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_SILENT, I_ANIMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_ORANGE_RAT, 'r', LIGHTRED, "orange rat",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    250, 10, MONS_RAT, MONS_ORANGE_RAT, MH_NATURAL, -3,
    { {AT_BITE, AF_DRAIN_XP, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 3, 5, 0 },
    7, 10, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_ROAR, I_ANIMAL, 
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LITTLE,
},

{
    MONS_BLACK_SNAKE, 'S', BLUE, "black snake",
    M_COLD_BLOOD,
    MR_RES_POISON,
    500, 12, MONS_SNAKE, MONS_BLACK_SNAKE, MH_NATURAL, -3,
    { {AT_BITE, AF_POISON_MEDIUM, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 7, 3, 5, 0 },
    4, 15, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_HISS, I_INSECT, 
    HT_LAND, 18, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_MEDIUM
},

{
    MONS_SHEEP, 'Y', LIGHTGREY, "sheep",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    1200, 10, MONS_SHEEP, MONS_SHEEP, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 13}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 3, 5, 0 },
    2, 7, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_BELLOW, I_ANIMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

{
    MONS_GHOUL, 'n', RED, "ghoul",
    M_EVIL,
    MR_RES_POISON | MR_RES_COLD,
    500, 12, MONS_GHOUL, MONS_GHOUL, MH_UNDEAD, -5,
    { {AT_CLAW, AF_ROT, 9}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 4, 3, 5, 0 },
    4, 10, MST_NO_SPELLS, CE_HCL, Z_NOZOMBIE, S_SILENT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_HOG, 'h', RED, "hog",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    700, 10, MONS_HOG, MONS_HOG, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 14}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 5, 0 },
    2, 9, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_SILENT, I_ANIMAL, 
    HT_LAND, 13, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

{
    MONS_GIANT_MOSQUITO, 'y', WHITE, "giant mosquito",
    M_FLIES,
    MR_VUL_POISON,
    100, 10, MONS_GIANT_MOSQUITO, MONS_GIANT_MOSQUITO, MH_NATURAL, -3,
    { {AT_BITE, AF_DISEASE, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 3, 5, 0 },
    0, 13, MST_NO_SPELLS, CE_CONTAMINATED, Z_NOZOMBIE, S_WHINE, I_PLANT, 
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_GIANT_CENTIPEDE, 's', GREEN, "giant centipede",
    M_NO_FLAGS,
    MR_VUL_POISON,
    350, 10, MONS_GIANT_CENTIPEDE, MONS_GIANT_CENTIPEDE, MH_NATURAL, -3,
    { {AT_STING, AF_POISON_NASTY, 2}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 2, 3, 3, 0 },
    2, 14, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_SILENT, I_INSECT, 
    HT_LAND, 13, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_IRON_TROLL, 'T', CYAN, "iron troll",
    M_WARM_BLOOD,
    MR_RES_FIRE | MR_RES_COLD,
    2400, 10, MONS_TROLL, MONS_IRON_TROLL, MH_NATURAL, -5,
    { {AT_BITE, AF_PLAIN, 35}, {AT_CLAW, AF_PLAIN, 25}, {AT_CLAW, AF_PLAIN, 25}, AT_NO_ATK },
    { 16, 3, 5, 0 },
    20, 4, MST_NO_SPELLS, CE_POISONOUS, Z_BIG, S_ROAR, I_NORMAL, 
    HT_LAND, 7, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_NAGA, 'N', GREEN, "naga",
    M_SPELLCASTER | M_SEE_INVIS | M_WARM_BLOOD,
    MR_RES_POISON,
    750, 10, MONS_NAGA, MONS_NAGA, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 17}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 3, 5, 0 },
    6, 10, MST_NAGA, CE_POISONOUS, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 8, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_BIG
},

{
    MONS_FIRE_GIANT, 'C', RED, "fire giant",
    M_FIGHTER | M_SPELLCASTER | M_WARM_BLOOD | M_SENSE_INVIS | M_EVIL,
    mrd(MR_RES_FIRE, 2),
    2400, 11, MONS_HILL_GIANT, MONS_FIRE_GIANT, MH_NATURAL, -4,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 16, 3, 6, 0 },
    8, 4, MST_EFREET, CE_CONTAMINATED, Z_BIG, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_GIANT,
},

{
    MONS_FROST_GIANT, 'C', LIGHTBLUE, "frost giant",
    M_FIGHTER | M_SPELLCASTER | M_WARM_BLOOD | M_SENSE_INVIS | M_EVIL,
    mrd(MR_RES_COLD, 2),
    2600, 11, MONS_HILL_GIANT, MONS_FROST_GIANT, MH_NATURAL, -4,
    { {AT_HIT, AF_PLAIN, 35}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 16, 4, 5, 0 },
    9, 3, MST_FROST_GIANT, CE_CONTAMINATED, Z_BIG, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_GIANT,
},

{
    MONS_FIREDRAKE, 'l', RED, "firedrake",
    M_FLIES | M_SPECIAL_ABILITY | M_WARM_BLOOD,
    MR_RES_FIRE,
    900, 10, MONS_DRAGON, MONS_FIREDRAKE, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 8}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 5, 0 },
    3, 12, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SILENT, I_ANIMAL, 
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_HUGE
},

{
    MONS_SHADOW_DRAGON, 'D', MAGENTA, "shadow dragon",
    M_SPELLCASTER | M_FLIES | M_SEE_INVIS | M_EVIL | M_COLD_BLOOD,
    MR_RES_POISON | mrd(MR_RES_COLD, 2),
    2000, 12, MONS_DRAGON, MONS_SHADOW_DRAGON, MH_NATURAL, -5,
    { {AT_BITE, AF_DRAIN_XP, 20}, {AT_CLAW, AF_PLAIN, 15}, {AT_CLAW, AF_PLAIN, 15}, AT_NO_ATK },
    { 17, 5, 5, 0 },
    15, 10, MST_SHADOW_DRAGON, CE_CLEAN, Z_BIG, S_ROAR, I_ANIMAL,
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_HUGE
},

{
    MONS_YELLOW_SNAKE, 'S', YELLOW, "yellow snake",
    M_COLD_BLOOD,
    MR_RES_POISON,
    400, 10, MONS_SNAKE, MONS_YELLOW_SNAKE, MH_NATURAL, -3,
    { {AT_BITE, AF_POISON_MEDIUM, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 5, 0 },
    4, 14, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_HISS, I_INSECT, 
    HT_LAND, 13, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_MEDIUM
},

{
    MONS_GREY_SNAKE, 'S', LIGHTGREY, "grey snake",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    600, 10, MONS_SNAKE, MONS_GREY_SNAKE, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 11, 3, 5, 0 },
    4, 16, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_HISS, I_INSECT, 
    HT_LAND, 18, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

{
    MONS_DEEP_TROLL, 'T', YELLOW, "deep troll",
    M_WARM_BLOOD | M_SENSE_INVIS | M_EVIL,
    MR_NO_FLAGS,
    1500, 12, MONS_TROLL, MONS_DEEP_TROLL, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 27}, {AT_CLAW, AF_PLAIN, 20}, {AT_CLAW, AF_PLAIN, 20}, AT_NO_ATK },
    { 10, 3, 5, 0 },
    6, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_BIG, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_BIG
},

{
    MONS_GIANT_BLOWFLY, 'y', LIGHTGREY, "giant blowfly",
    M_FLIES,
    MR_VUL_POISON,
    200, 10, MONS_GIANT_BLOWFLY, MONS_GIANT_BLOWFLY, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 13}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 3, 5, 0 },
    2, 15, MST_NO_SPELLS, CE_CONTAMINATED, Z_NOZOMBIE, S_BUZZ, I_PLANT, 
    HT_LAND, 19, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_RED_WASP, 'y', RED, "red wasp",
    M_FLIES,
    MR_VUL_POISON,
    400, 14, MONS_YELLOW_WASP, MONS_RED_WASP, MH_NATURAL, -3,
    { {AT_STING, AF_PARALYSE, 23}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 8, 3, 5, 0 },
    7, 14, MST_NO_SPELLS, CE_POISONOUS, Z_NOZOMBIE, S_BUZZ, I_PLANT, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_SWAMP_DRAGON, 'D', BROWN, "swamp dragon",
    M_SPELLCASTER | M_FLIES | M_WARM_BLOOD,
    MR_RES_POISON,
    1900, 11, MONS_DRAGON, MONS_SWAMP_DRAGON, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 18}, {AT_CLAW, AF_PLAIN, 9}, {AT_CLAW, AF_PLAIN, 9}, AT_NO_ATK },
    { 9, 5, 5, 0 },
    7, 7, MST_SWAMP_DRAGON, CE_CONTAMINATED, Z_BIG, S_ROAR, I_ANIMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_GIANT,
},

{
    MONS_SWAMP_DRAKE, 'l', BROWN, "swamp drake",
    M_SPELLCASTER | M_FLIES | M_WARM_BLOOD,
    MR_RES_POISON,
    900, 11, MONS_DRAGON, MONS_SWAMP_DRAKE, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 14}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 4, 5, 5, 0 },
    3, 11, MST_SWAMP_DRAKE, CE_CONTAMINATED, Z_SMALL, S_ROAR, I_ANIMAL, 
    HT_LAND, 11, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_BIG
},

{
    MONS_DEATH_DRAKE, 'l', LIGHTGREY, "death drake",
    M_SPELLCASTER | M_FLIES | M_EVIL | M_COLD_BLOOD,
    MR_RES_POISON,
    900, 11, MONS_DRAGON, MONS_DEATH_DRAKE, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 9, 5, 7, 0 },
    6, 14, MST_DEATH_DRAKE, CE_HCL, Z_BIG, S_ROAR, I_ANIMAL,
    HT_LAND, 13, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_BIG
},

{
    MONS_SOLDIER_ANT, 'a', LIGHTGREY, "soldier ant",
    M_NO_FLAGS,
    MR_VUL_POISON,
    900, 10, MONS_GIANT_ANT, MONS_SOLDIER_ANT, MH_NATURAL, -3,
    { {AT_STING, AF_POISON_NASTY, 14}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 5, 0 },
    8, 10, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_SILENT, I_INSECT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LITTLE,
},

{
    MONS_HILL_GIANT, 'C', LIGHTRED, "hill giant",
    M_WARM_BLOOD | M_EVIL,
    MR_NO_FLAGS,
    1600, 10, MONS_HILL_GIANT, MONS_HILL_GIANT, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 11, 3, 5, 0 },
    3, 4, MST_NO_SPELLS, CE_CONTAMINATED, Z_BIG, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_GIANT,
},

{
    MONS_QUEEN_ANT, 'Q', LIGHTGREY, "queen ant",
    M_NO_FLAGS,
    MR_VUL_POISON,
    1200, 10, MONS_GIANT_ANT, MONS_QUEEN_ANT, MH_NATURAL, -3,
    { {AT_STING, AF_POISON_NASTY, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 13, 3, 5, 0 },
    14, 3, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_SILENT, I_INSECT, 
    HT_LAND, 7, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

{
    MONS_ANT_LARVA, 'w', LIGHTGREY, "ant larva",
    M_NO_SKELETON,
    MR_VUL_POISON,
    350, 5, MONS_GIANT_ANT, MONS_ANT_LARVA, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 2, 3, 5, 0 },
    2, 6, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_SILENT, I_INSECT, 
    HT_LAND, 6, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_GIANT_FROG, 'F', GREEN, "giant frog",
    M_COLD_BLOOD | M_AMPHIBIOUS,
    MR_NO_FLAGS,
    500, 10, MONS_GIANT_FROG, MONS_GIANT_FROG, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 9}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 4, 3, 5, 0 },
    0, 12, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_CROAK, I_ANIMAL, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

{
    MONS_GIANT_BROWN_FROG, 'F', BROWN, "giant brown frog",
    M_COLD_BLOOD | M_AMPHIBIOUS,
    MR_NO_FLAGS,
    890, 10, MONS_GIANT_FROG, MONS_GIANT_BROWN_FROG, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 14}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 8, 3, 5, 0 },
    2, 11, MST_NO_SPELLS, CE_CLEAN, Z_BIG, S_CROAK, I_ANIMAL, 
    HT_LAND, 13, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_MEDIUM
},

{
    MONS_SPINY_FROG, 'F', YELLOW, "spiny frog",
    M_COLD_BLOOD | M_AMPHIBIOUS,
    MR_RES_POISON,
    1000, 10, MONS_GIANT_FROG, MONS_SPINY_FROG, MH_NATURAL, -3,
    { {AT_STING, AF_POISON_MEDIUM, 26}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 7, 3, 5, 0 },
    6, 9, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_CROAK, I_ANIMAL, 
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

{
    MONS_BLINK_FROG, 'F', LIGHTGREEN, "blink frog",
    M_COLD_BLOOD | M_AMPHIBIOUS | M_SPECIAL_ABILITY,
    MR_NO_FLAGS,
    800, 12, MONS_GIANT_FROG, MONS_BLINK_FROG, MH_NATURAL, -5,
    { {AT_HIT, AF_BLINK, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 5, 0 },
    3, 12, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_CROAK, I_ANIMAL, 
    HT_LAND, 14, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LITTLE,
},

{
    MONS_GIANT_COCKROACH, 'a', BROWN, "giant cockroach",
    M_NO_FLAGS,
    MR_NO_FLAGS,
    250, 10, MONS_GIANT_COCKROACH, MONS_GIANT_COCKROACH, MH_NATURAL, -1,
    { {AT_BITE, AF_PLAIN, 2}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 3, 4, 0 },
    3, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SILENT, I_INSECT, 
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_SMALL_SNAKE, 'S', GREEN, "small snake",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    100, 13, MONS_SNAKE, MONS_SMALL_SNAKE, MH_NATURAL, -1,
    { {AT_BITE, AF_POISON, 2}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 2, 3, 0 },
    0, 11, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_SILENT, I_INSECT, 
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LITTLE,
},

{
    MONS_WHITE_IMP, '5', WHITE, "white imp",
    M_SPELLCASTER | M_FLIES | M_SPEAKS | M_EVIL,
    mrd(MR_RES_COLD, 2),
    0, 10, MONS_IMP, MONS_WHITE_IMP, MH_DEMONIC, -3,
    { {AT_HIT, AF_COLD, 4}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 2, 3, 5, 0 },
    4, 10, MST_WHITE_IMP, CE_CONTAMINATED, Z_NOZOMBIE, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LITTLE,
},

{
    MONS_LEMURE, '5', YELLOW, "lemure",
    M_EVIL,
    MR_RES_POISON,
    0, 10, MONS_LEMURE, MONS_LEMURE, MH_DEMONIC, -3,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 2, 3, 5, 0 },
    1, 12, MST_NO_SPELLS, CE_CONTAMINATED, Z_NOZOMBIE, S_MOAN, I_NORMAL, 
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_SMALL
},

{
    MONS_UFETUBUS, '5', LIGHTCYAN, "ufetubus",
    M_EVIL,
    MR_VUL_FIRE | MR_RES_COLD,
    0, 10, MONS_UFETUBUS, MONS_UFETUBUS, MH_DEMONIC, -3,
    { {AT_HIT, AF_PLAIN, 5}, {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK },
    { 1, 4, 6, 0 },
    2, 15, MST_NO_SPELLS, CE_CONTAMINATED, Z_NOZOMBIE, S_SHOUT, I_NORMAL, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_MANES, '5', LIGHTRED, "manes",
    M_EVIL,
    MR_RES_ELEC | MR_RES_FIRE | MR_RES_COLD | MR_RES_POISON,
    0, 10, MONS_MANES, MONS_MANES, MH_DEMONIC, -3,
    { {AT_HIT, AF_PLAIN, 5}, {AT_HIT, AF_PLAIN, 3}, {AT_HIT, AF_PLAIN, 3}, AT_NO_ATK },
    { 3, 3, 5, 0 },
    2, 8, MST_NO_SPELLS, CE_CONTAMINATED, Z_NOZOMBIE, S_SHOUT, I_NORMAL, 
    HT_LAND, 8, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_SMALL
},

{
    MONS_MIDGE, '5', LIGHTGREEN, "midge",
    M_FLIES | M_EVIL,
    MR_RES_POISON,
    0, 10, MONS_MIDGE, MONS_MIDGE, MH_DEMONIC, -3,
    { {AT_HIT, AF_BLINK, 8}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 2, 3, 5, 0 },
    4, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_NOZOMBIE, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_SMALL
},

{
    MONS_NEQOXEC, '3', LIGHTMAGENTA, "neqoxec",
    M_SPELLCASTER | M_LEVITATE | M_EVIL,
    MR_RES_POISON,
    0, 12, MONS_NEQOXEC, MONS_NEQOXEC, MH_DEMONIC, -6,
    { {AT_HIT, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 5, 0 },
    4, 12, MST_NEQOXEC, CE_CONTAMINATED, Z_NOZOMBIE, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_ORANGE_DEMON, '3', LIGHTRED, "orange demon",
    M_EVIL,
    MR_NO_FLAGS,
    0, 12, MONS_ORANGE_DEMON, MONS_ORANGE_DEMON, MH_DEMONIC, -6,
    { {AT_STING, AF_POISON_STR, 10}, {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK },
    { 8, 4, 5, 0 },
    3, 7, MST_NO_SPELLS, CE_POISONOUS, Z_NOZOMBIE, S_SCREECH, I_NORMAL, 
    HT_LAND, 7, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_MEDIUM
},

{
    MONS_HELLWING, '3', LIGHTGREY, "hellwing",
    M_SPELLCASTER | M_FLIES | M_EVIL,
    MR_RES_POISON,
    0, 12, MONS_HELLWING, MONS_HELLWING, MH_DEMONIC, -6,
    { {AT_HIT, AF_PLAIN, 17}, {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK },
    { 7, 4, 5, 0 },
    8, 10, MST_HELLWING, CE_CONTAMINATED, Z_NOZOMBIE, S_MOAN, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_SMOKE_DEMON, '4', LIGHTGREY, "smoke demon",
    M_SPELLCASTER | M_FLIES | M_EVIL | M_INSUBSTANTIAL,
    MR_RES_POISON | mrd(MR_RES_FIRE, 2),
    0, 12, MONS_SMOKE_DEMON, MONS_SMOKE_DEMON, MH_DEMONIC, -6,
    { {AT_HIT, AF_PLAIN, 8}, {AT_HIT, AF_PLAIN, 5}, {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK },
    { 7, 3, 5, 0 },
    5, 9, MST_SMOKE_DEMON, CE_NOCORPSE, Z_NOZOMBIE, S_ROAR, I_NORMAL, 
    HT_LAND, 9, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_SMALL
},

{
    MONS_YNOXINUL, '3', CYAN, "ynoxinul",
    M_SPELLCASTER | M_FLIES | M_SENSE_INVIS | M_EVIL,
    MR_RES_ELEC | MR_RES_POISON | MR_RES_COLD,
    0, 12, MONS_YNOXINUL, MONS_YNOXINUL, MH_DEMONIC, -6,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 5, 0 },
    3, 10, MST_YNOXINUL, CE_CONTAMINATED, Z_NOZOMBIE, S_BELLOW, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_EXECUTIONER, '1', LIGHTGREY, "Executioner",
    M_FIGHTER | M_SPELLCASTER | M_SEE_INVIS | M_EVIL,
    MR_RES_ELEC | MR_RES_FIRE | MR_RES_COLD | MR_RES_POISON,
    0, 14, MONS_EXECUTIONER, MONS_EXECUTIONER, MH_DEMONIC, -9,
    { {AT_HIT, AF_PLAIN, 30}, {AT_HIT, AF_PLAIN, 10}, {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK },
    { 12, 3, 5, 0 },
    10, 15, MST_HELL_KNIGHT_I, CE_CONTAMINATED, Z_NOZOMBIE, S_SCREAM, I_HIGH, 
    HT_LAND, 20, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_GREEN_DEATH, '1', GREEN, "Green Death",
    M_SPELLCASTER | M_SEE_INVIS | M_EVIL,
    MR_RES_POISON,
    0, 14, MONS_GREEN_DEATH, MONS_GREEN_DEATH, MH_DEMONIC, -9,
    { {AT_HIT, AF_PLAIN, 32}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 13, 3, 5, 0 },
    5, 7, MST_GREEN_DEATH, CE_POISONOUS, Z_NOZOMBIE, S_ROAR, I_HIGH, 
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_BLUE_DEATH, '1', BLUE, "Blue Death",
    M_SPELLCASTER | M_FLIES | M_SEE_INVIS | M_EVIL,
    MR_RES_POISON | MR_VUL_FIRE | mrd(MR_RES_COLD, 2) | MR_RES_ELEC,
    0, 14, MONS_BLUE_DEATH, MONS_BLUE_DEATH, MH_DEMONIC, -9,
    { {AT_HIT, AF_PLAIN, 20}, {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK },
    { 12, 3, 5, 0 },
    10, 10, MST_BLUE_DEATH, CE_CONTAMINATED, Z_NOZOMBIE, S_SHOUT, I_HIGH, 
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_BALRUG, '1', RED, "Balrug",
    M_FIGHTER | M_SPELLCASTER | M_FLIES | M_SEE_INVIS | M_EVIL,
    MR_RES_POISON | MR_RES_HELLFIRE | MR_VUL_COLD,
    0, 14, MONS_BALRUG, MONS_BALRUG, MH_DEMONIC, -9,
    { {AT_HIT, AF_FIRE, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 14, 3, 5, 0 },
    5, 12, MST_BALRUG, CE_CONTAMINATED, Z_NOZOMBIE, S_SHOUT, I_HIGH, 
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_BIG
},

{
    MONS_CACODEMON, '1', YELLOW, "Cacodemon",
    M_SPELLCASTER | M_LEVITATE | M_SEE_INVIS | M_EVIL,
    MR_RES_POISON | MR_RES_ELEC,
    0, 14, MONS_CACODEMON, MONS_CACODEMON, MH_DEMONIC, -9,
    { {AT_HIT, AF_PLAIN, 22}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 13, 3, 5, 0 },
    11, 10, MST_CACODEMON, CE_CONTAMINATED, Z_NOZOMBIE, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_DEMONIC_CRAWLER, '3', GREEN, "demonic crawler",
    M_SEE_INVIS | M_EVIL,
    MR_RES_ELEC | MR_RES_POISON | MR_RES_COLD | MR_RES_FIRE,
    0, 12, MONS_DEMONIC_CRAWLER, MONS_DEMONIC_CRAWLER, MH_DEMONIC, -6,
    { {AT_HIT, AF_PLAIN, 13}, {AT_HIT, AF_PLAIN, 13}, {AT_HIT, AF_PLAIN, 13} },
    { 9, 3, 5, 0 },
    10, 6, MST_NO_SPELLS, CE_CONTAMINATED, Z_NOZOMBIE, S_SCREAM, I_NORMAL, 
    HT_LAND, 9, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_BIG
},

{
    MONS_SUN_DEMON, '2', YELLOW, "sun demon",
    M_SENSE_INVIS | M_LEVITATE | M_EVIL,
    MR_RES_ELEC | MR_RES_POISON | MR_VUL_COLD | MR_RES_HELLFIRE,
    0, 14, MONS_SUN_DEMON, MONS_SUN_DEMON, MH_DEMONIC, -6,
    { {AT_HIT, AF_FIRE, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 3, 5, 0 },
    10, 12, MST_NO_SPELLS, CE_CONTAMINATED, Z_NOZOMBIE, S_SHOUT, I_NORMAL, 
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_SHADOW_IMP, '5', MAGENTA, "shadow imp",
    M_SPELLCASTER | M_FLIES | M_SPEAKS | M_EVIL,
    mrd(MR_RES_COLD, 2) | MR_RES_POISON,
    0, 11, MONS_IMP, MONS_SHADOW_IMP, MH_DEMONIC, -3,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 2, 3, 5, 0 },
    3, 11, MST_SHADOW_IMP, CE_CONTAMINATED, Z_NOZOMBIE, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LITTLE,
},

{
    MONS_SHADOW_DEMON, '3', MAGENTA, "shadow demon",
    M_SEE_INVIS | M_INVIS | M_EVIL,
    MR_RES_POISON | mrd(MR_RES_COLD, 2),
    0, 12, MONS_SHADOW_DEMON, MONS_SHADOW_DEMON, MH_DEMONIC, -7,
    { {AT_HIT, AF_PLAIN, 21}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 5, 0 },
    7, 12, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_CROAK, I_HIGH, 
    HT_LAND, 11, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_SMALL
},

{
    MONS_LOROCYPROCA, '2', BLUE, "Lorocyproca",
    M_SENSE_INVIS | M_INVIS | M_EVIL,
    MR_RES_POISON | MR_RES_COLD | MR_RES_FIRE | MR_RES_ELEC,
    0, 12, MONS_LOROCYPROCA, MONS_LOROCYPROCA, MH_DEMONIC, -7,
    { {AT_HIT, AF_PLAIN, 25}, {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK },
    { 12, 3, 5, 0 },
    10, 12, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_MOAN, I_HIGH, 
    HT_LAND, 9, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_SHADOW_WRAITH, 'W', BLUE, "shadow wraith",
    M_LEVITATE | M_SEE_INVIS | M_INVIS | M_EVIL | M_INSUBSTANTIAL,
    MR_RES_POISON,
    0, 15, MONS_WRAITH, MONS_SHADOW_WRAITH, MH_UNDEAD, -8,
    { {AT_HIT, AF_DRAIN_XP, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 3, 5, 0 },
    7, 7, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_MOAN, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_GIANT_AMOEBA, 'J', BLUE, "giant amoeba",
    M_NO_SKELETON | M_SENSE_INVIS | M_AMPHIBIOUS,
    MR_RES_POISON,
    1000, 10, MONS_GIANT_AMOEBA, MONS_GIANT_AMOEBA, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 12, 3, 5, 0 },
    0, 4, MST_NO_SPELLS, CE_POISONOUS, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LITTLE,
},

{
    MONS_GIANT_SLUG, 'j', GREEN, "giant slug",
    M_NO_SKELETON | M_AMPHIBIOUS,
    MR_NO_FLAGS,
    700, 10, MONS_GIANT_SLUG, MONS_GIANT_SLUG, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 23}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 5, 3, 0 },
    0, 2, MST_NO_SPELLS, CE_POISONOUS, Z_BIG, S_SILENT, I_INSECT, 
    HT_LAND, 6, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

{
    MONS_GIANT_SNAIL, 'j', LIGHTGREEN, "giant snail",
    M_NO_SKELETON | M_AMPHIBIOUS,
    MR_NO_FLAGS,
    900, 10, MONS_GIANT_SLUG, MONS_GIANT_SNAIL, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 18}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 14, 5, 3, 0 },
    7, 2, MST_NO_SPELLS, CE_POISONOUS, Z_BIG, S_SILENT, I_INSECT, 
    HT_LAND, 4, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

{
    MONS_SPATIAL_VORTEX, 'v', BLACK, "spatial vortex",
    M_LEVITATE | M_CONFUSED | M_INSUBSTANTIAL,
    MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC,
    0, 5, MONS_FIRE_VORTEX, MONS_SPATIAL_VORTEX, MH_NONLIVING, MAG_IMMUNE,
    { {AT_HIT, AF_DISTORT, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 6, 6, 0 },
    0, 5, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

{
    MONS_PIT_FIEND, '1', BROWN, "Pit Fiend",
    M_FLIES | M_SEE_INVIS | M_EVIL | M_SPECIAL_ABILITY,
    MR_RES_POISON | MR_RES_HELLFIRE | MR_RES_COLD | MR_RES_ELEC,
    0, 18, MONS_FIEND, MONS_PIT_FIEND, MH_DEMONIC, -12,
    { {AT_HIT, AF_PLAIN, 28}, {AT_HIT, AF_PLAIN, 21}, {AT_HIT, AF_PLAIN, 21}, AT_NO_ATK },
    { 19, 4, 5, 0 },
    17, 5, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_ROAR, I_HIGH, 
    HT_LAND, 8, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_BORING_BEETLE, 'B', BROWN, "boring beetle",
    M_NO_FLAGS,
    MR_VUL_POISON,
    1300, 10, MONS_GIANT_BEETLE, MONS_BORING_BEETLE, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 26}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 8, 3, 5, 0 },
    13, 4, MST_NO_SPELLS, CE_POISONOUS, Z_BIG, S_SILENT, I_INSECT, 
    HT_LAND, 6, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

{
    MONS_GARGOYLE, '9', LIGHTGREY, "gargoyle",
    M_FLIES,
    MR_RES_POISON | MR_RES_ELEC,
    0, 12, MONS_GARGOYLE, MONS_GARGOYLE, MH_NONLIVING, -6,
    { {AT_BITE, AF_PLAIN, 10}, {AT_CLAW, AF_PLAIN, 6}, {AT_CLAW, AF_PLAIN, 6}, AT_NO_ATK },
    { 4, 3, 5, 0 },
    18, 6, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

// only appear in Dis castle
{
    MONS_METAL_GARGOYLE, '9', CYAN, "metal gargoyle",
    M_FLIES,
    MR_RES_POISON | MR_RES_ELEC,
    0, 12, MONS_GARGOYLE, MONS_METAL_GARGOYLE, MH_NONLIVING, -6,
    { {AT_BITE, AF_PLAIN, 19}, {AT_CLAW, AF_PLAIN, 10}, {AT_CLAW, AF_PLAIN, 10}, AT_NO_ATK },
    { 8, 3, 5, 0 },
    20, 4, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_HIGH, 
    HT_LAND, 7, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

// only appear in Gehenna castle & one minivault
{
    MONS_MOLTEN_GARGOYLE, '9', RED, "molten gargoyle",
    M_FLIES,
    MR_RES_POISON | MR_RES_ELEC | mrd(MR_RES_FIRE, 3),
    0, 12, MONS_GARGOYLE, MONS_MOLTEN_GARGOYLE, MH_NONLIVING, -6,
    { {AT_BITE, AF_FIRE, 12}, {AT_CLAW, AF_PLAIN, 8}, {AT_CLAW, AF_PLAIN, 8}, AT_NO_ATK },
    { 5, 3, 5, 0 },
    14, 7, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

// 250 can't exist -> program bug


{
    MONS_MNOLEG, '&', LIGHTGREEN, "Mnoleg",
    M_FIGHTER | M_SEE_INVIS | M_SPELLCASTER | M_SPEAKS | M_EVIL | M_UNIQUE,
    MR_RES_ELEC | MR_RES_POISON | MR_RES_FIRE,
    0, 15, MONS_MNOLEG, MONS_MNOLEG, MH_DEMONIC, MAG_IMMUNE,
    { {AT_HIT, AF_PLAIN, 35}, {AT_HIT, AF_PLAIN, 23}, AT_NO_ATK, AT_NO_ATK },
    { 17, 0, 0, 250 },
    10, 25, MST_MNOLEG, CE_CONTAMINATED, Z_NOZOMBIE, S_BUZZ, I_HIGH, 
    HT_LAND, 13, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_LOM_LOBON, '&', LIGHTBLUE, "Lom Lobon",
    M_FIGHTER | M_LEVITATE | M_SEE_INVIS | M_SPELLCASTER | M_SPEAKS | M_EVIL | M_UNIQUE,
    MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC,
    0, 15, MONS_LOM_LOBON, MONS_LOM_LOBON, MH_DEMONIC, MAG_IMMUNE,
    { {AT_HIT, AF_PLAIN, 40}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 19, 0, 0, 250 },
    10, 20, MST_LOM_LOBON, CE_CONTAMINATED, Z_NOZOMBIE, S_SCREAM, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_CEREBOV, '&', RED, "Cerebov",
    M_FIGHTER | M_SPELLCASTER | M_SEE_INVIS | M_SPEAKS | M_EVIL | M_UNIQUE,
    MR_RES_ELEC | MR_RES_POISON | MR_RES_HELLFIRE,
    0, 15, MONS_CEREBOV, MONS_CEREBOV, MH_DEMONIC, -6,
    { {AT_HIT, AF_PLAIN, 60}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 21, 0, 0, 650 },
    30, 8, MST_CEREBOV, CE_CONTAMINATED, Z_NOZOMBIE, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_GIANT,
},

{
    MONS_GLOORX_VLOQ, '&', LIGHTGREY, "Gloorx Vloq",
    M_FIGHTER | M_LEVITATE | M_SEE_INVIS | M_SPELLCASTER | M_SPEAKS | M_EVIL | M_UNIQUE,
    MR_RES_POISON | MR_RES_COLD | MR_RES_ELEC,
    0, 15, MONS_GLOORX_VLOQ, MONS_GLOORX_VLOQ, MH_DEMONIC, -14,
    { {AT_HIT, AF_PLAIN, 45}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 16, 0, 0, 350 },
    10, 10, MST_GLOORX_VLOQ, CE_CONTAMINATED, Z_NOZOMBIE, S_MOAN, I_HIGH, 
    HT_LAND, 20, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

/* ******************************************************************
{MONS_MOLLUSC_LORD, 'U', GREEN, "The Mollusc Lord", M_NO_FLAGS,
   MR_RES_POISON,
   0, 25, 255, MH_DEMONIC, -3, {30,0,0,0},
   {16,0,0,100}, 10, 10, 10, 7, 93, CE_POISONOUS, Z_NOZOMBIE, S_SILENT, I_HIGH, 1},
****************************************************************** */

{
    MONS_NAGA_MAGE, 'N', RED, "naga mage",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SEE_INVIS | M_WARM_BLOOD,
    MR_RES_POISON,
    750, 13, MONS_NAGA, MONS_NAGA, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 17}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 7, 3, 5, 0 },
    6, 10, MST_NAGA_MAGE, CE_POISONOUS, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 8, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_BIG
},

{
    MONS_NAGA_WARRIOR, 'N', BLUE, "naga warrior",
    M_FIGHTER | M_SPELLCASTER | M_SEE_INVIS | M_WARM_BLOOD,
    MR_RES_POISON,
    750, 8, MONS_NAGA, MONS_NAGA, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 28}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 9, 2, 0 },
    6, 10, MST_NAGA, CE_POISONOUS, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 8, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_BIG
},

{
    MONS_ORC_WARLORD, 'o', LIGHTCYAN, "orc warlord",
    M_FIGHTER | M_WARM_BLOOD,
    MR_NO_FLAGS,
    600, 15, MONS_ORC, MONS_ORC, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 32}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 15, 4, 7, 0 },
    3, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_DEEP_ELF_SOLDIER, 'e', RED, "deep elf soldier",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_WARM_BLOOD | M_FIGHTER,
    MR_NO_FLAGS,
    450, 10, MONS_ELF, MONS_ELF, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 3, 3, 0 },
    0, 12, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_DEEP_ELF_FIGHTER, 'e', LIGHTRED, "deep elf fighter",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_WARM_BLOOD | M_FIGHTER,
    MR_NO_FLAGS,
    450, 10, MONS_ELF, MONS_ELF, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 9}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 3, 0 },
    0, 13, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_DEEP_ELF_KNIGHT, 'e', CYAN, "deep elf knight",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_WARM_BLOOD | M_FIGHTER,
    MR_NO_FLAGS,
    450, 10, MONS_ELF, MONS_ELF, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 14}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 11, 3, 3, 0 },
    0, 15, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 11, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_DEEP_ELF_BLADEMASTER, 'e', LIGHTCYAN, "deep elf blademaster",
    M_WARM_BLOOD | M_FIGHTER | M_TWOWEAPON,
    MR_NO_FLAGS,
    450, 10, MONS_ELF, MONS_ELF, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 25}, {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK },
    { 16, 5, 3, 0 },
    0, 25, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_DEEP_ELF_MASTER_ARCHER, 'e', LIGHTGREY, "deep elf master archer",
    M_WARM_BLOOD | M_ARCHER,
    MR_NO_FLAGS,
    450, 10, MONS_ELF, MONS_ELF, MH_NATURAL, -5,
    // Attack damage gets rolled into their ranged attacks.
    { {AT_SHOOT, AF_PLAIN, 25}, {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK },
    { 15, 4, 2, 0 },
    0, 15, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 11, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

    

{
    MONS_DEEP_ELF_MAGE, 'e', MAGENTA, "deep elf mage",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_WARM_BLOOD,
    MR_RES_ELEC,
    450, 10, MONS_ELF, MONS_ELF, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 4, 3, 3, 0 },
    0, 13, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_DEEP_ELF_SUMMONER, 'e', BROWN, "deep elf summoner",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_WARM_BLOOD,
    MR_NO_FLAGS,
    450, 10, MONS_ELF, MONS_ELF, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 3, 0 },
    0, 13, MST_DEEP_ELF_SUMMONER, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_DEEP_ELF_CONJURER, 'e', BLUE, "deep elf conjurer",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_WARM_BLOOD,
    MR_RES_ELEC,
    450, 10, MONS_ELF, MONS_ELF, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 3, 0 },
    0, 13, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_DEEP_ELF_PRIEST, 'e', GREEN, "deep elf priest",
    M_SPELLCASTER | M_PRIEST | M_WARM_BLOOD | M_EVIL,
    MR_NO_FLAGS,
    450, 10, MONS_ELF, MONS_ELF, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 9}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 3, 3, 0 },
    0, 13, MST_DEEP_ELF_PRIEST, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_DEEP_ELF_HIGH_PRIEST, 'e', LIGHTGREEN, "deep elf high priest",
    M_SPELLCASTER | M_SPEAKS | M_PRIEST | M_WARM_BLOOD | M_SEE_INVIS | M_EVIL,
    MR_NO_FLAGS,
    450, 10, MONS_ELF, MONS_ELF, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 14}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 11, 3, 3, 0 },
    3, 13, MST_DEEP_ELF_HIGH_PRIEST, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_DEEP_ELF_DEMONOLOGIST, 'e', YELLOW, "deep elf demonologist",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_WARM_BLOOD | M_SEE_INVIS | M_EVIL,
    MR_NO_FLAGS,
    450, 10, MONS_ELF, MONS_ELF, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 12, 3, 3, 0 },
    0, 13, MST_DEEP_ELF_DEMONOLOGIST, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_DEEP_ELF_ANNIHILATOR, 'e', LIGHTBLUE, "deep elf annihilator",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_WARM_BLOOD | M_SEE_INVIS | M_EVIL,
    MR_RES_ELEC,
    450, 10, MONS_ELF, MONS_ELF, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 15, 3, 3, 0 },
    0, 13, MST_DEEP_ELF_ANNIHILATOR, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_DEEP_ELF_SORCERER, 'e', LIGHTMAGENTA, "deep elf sorcerer",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_WARM_BLOOD | M_SEE_INVIS | M_SPEAKS
        | M_EVIL,
    MR_NO_FLAGS,
    450, 10, MONS_ELF, MONS_ELF, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 14, 3, 3, 0 },
    0, 13, MST_DEEP_ELF_SORCERER, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_DEEP_ELF_DEATH_MAGE, 'e', WHITE, "deep elf death mage",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_WARM_BLOOD | M_SEE_INVIS | M_EVIL,
    MR_NO_FLAGS,
    450, 10, MONS_ELF, MONS_ELF, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 15, 3, 3, 0 },
    0, 13, MST_DEEP_ELF_DEATH_MAGE, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_BROWN_OOZE, 'J', BROWN, "brown ooze",
    M_NO_SKELETON | M_SENSE_INVIS | M_ACID_SPLASH,
    MR_RES_POISON | MR_RES_ASPHYX | MR_RES_ACID,
    0, 11, MONS_JELLY, MONS_BROWN_OOZE, MH_NATURAL, -7,
    { {AT_HIT, AF_ACID, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 7, 3, 5, 0 },
    10, 1, MST_NO_SPELLS, CE_POISONOUS, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_EATS_ITEMS, SIZE_LITTLE,
},

{
    MONS_AZURE_JELLY, 'J', LIGHTBLUE, "azure jelly",
    M_NO_SKELETON | M_SENSE_INVIS,
    MR_RES_POISON | MR_RES_COLD | MR_VUL_FIRE | MR_RES_ELEC | MR_RES_ASPHYX
         | MR_RES_ACID,
    0, 11, MONS_JELLY, MONS_AZURE_JELLY, MH_NATURAL, -4,
    { {AT_HIT, AF_COLD, 12}, {AT_HIT, AF_COLD, 12}, {AT_HIT, AF_PLAIN, 12}, {AT_HIT, AF_PLAIN, 12} },
    { 15, 3, 5, 0 },
    5, 10, MST_NO_SPELLS, CE_POISONOUS, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_EATS_ITEMS, SIZE_LITTLE,
},

{
    MONS_DEATH_OOZE, 'J', MAGENTA, "death ooze",
    M_NO_SKELETON | M_SENSE_INVIS | M_EVIL,
    MR_RES_POISON | MR_RES_COLD | MR_RES_ASPHYX | MR_RES_ACID,
    0, 13, MONS_JELLY, MONS_DEATH_OOZE, MH_UNDEAD, -8,
    { {AT_HIT, AF_ROT, 32}, {AT_HIT, AF_PLAIN, 32}, AT_NO_ATK, AT_NO_ATK },
    { 11, 3, 3, 0 },
    2, 4, MST_NO_SPELLS, CE_POISONOUS, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_EATS_ITEMS, SIZE_LITTLE,
},

{
    MONS_ACID_BLOB, 'J', LIGHTGREEN, "acid blob",
    M_NO_SKELETON | M_SENSE_INVIS | M_SPECIAL_ABILITY | M_ACID_SPLASH,
    MR_RES_POISON | MR_RES_ASPHYX | MR_RES_ACID,
    0, 12, MONS_JELLY, MONS_ACID_BLOB, MH_NATURAL, -7,
    { {AT_HIT, AF_ACID, 42}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 18, 3, 5, 0 },
    1, 3, MST_NO_SPELLS, CE_POISONOUS, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 14, DEFAULT_ENERGY, MONUSE_EATS_ITEMS, SIZE_LITTLE,
},

{
    MONS_ROYAL_JELLY, 'J', YELLOW, "royal jelly",
    M_NO_SKELETON | M_SENSE_INVIS | M_ACID_SPLASH,
    MR_RES_POISON | MR_RES_ASPHYX | MR_RES_ACID,
    0, 20, MONS_JELLY, MONS_ROYAL_JELLY, MH_NATURAL, -7,
    { {AT_HIT, AF_ACID, 50}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 21, 0, 0, 111 },
    8, 4, MST_NO_SPELLS, CE_CLEAN, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_EATS_ITEMS, SIZE_SMALL
},

{
    MONS_TERENCE, '@', LIGHTCYAN, "Terence",
    M_WARM_BLOOD | M_SPEAKS | M_UNIQUE,
    MR_NO_FLAGS,
    0, 20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 0, 0, 20 },
    0, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_JESSICA, '@', LIGHTGREY, "Jessica",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SPEAKS | M_WARM_BLOOD | M_UNIQUE,
    MR_NO_FLAGS,
    0, 20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 0, 0, 10 },
    0, 10, MST_ORC_WIZARD_I, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_IJYB, 'g', BLUE, "Ijyb",
    M_WARM_BLOOD | M_SPEAKS | M_UNIQUE,
    MR_NO_FLAGS,
    0, 5, MONS_GOBLIN, MONS_GOBLIN, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 4}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 0, 0, 28 },
    2, 12, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_SMALL
},

{
    MONS_SIGMUND, '@', YELLOW, "Sigmund",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SPEAKS | M_WARM_BLOOD | M_UNIQUE,
    MR_NO_FLAGS,
    0, 20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 0, 0, 30 },
    0, 11, MST_ORC_WIZARD_II, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_BLORK_THE_ORC, 'o', BROWN, "Blork the orc",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SPEAKS | M_WARM_BLOOD | M_UNIQUE,
    MR_NO_FLAGS,
    0, 20, MONS_ORC, MONS_ORC, MH_NATURAL, -4,
    { {AT_HIT, AF_PLAIN, 7}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 0, 0, 32 },
    0, 9, MST_ORC_WIZARD_III, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 8, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_EDMUND, '@', RED, "Edmund",
    M_FIGHTER | M_WARM_BLOOD | M_SPEAKS | M_UNIQUE,
    MR_NO_FLAGS,
    0, 20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -4,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 4, 0, 0, 44 },
    0, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_PSYCHE, '@', LIGHTMAGENTA, "Psyche",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SPEAKS | M_WARM_BLOOD | M_UNIQUE,
    MR_NO_FLAGS,
    0, 20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -4,
    { {AT_HIT, AF_PLAIN, 7}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 0, 0, 39 },
    0, 12, MST_ORC_WIZARD_III, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 13, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_EROLCHA, 'O', LIGHTBLUE, "Erolcha",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SEE_INVIS | M_WARM_BLOOD 
        | M_SPEAKS | M_EVIL | M_UNIQUE,
    MR_RES_ELEC,
    0, 20, MONS_OGRE, MONS_OGRE, MH_NATURAL, -7,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 0, 0, 54 },
    3, 7, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_LARGE
},

{
    MONS_DONALD, '@', BLUE, "Donald",
    M_FIGHTER | M_WARM_BLOOD | M_SPEAKS | M_UNIQUE,
    MR_NO_FLAGS,
    0, 20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 8}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 0, 0, 54 },
    0, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_URUG, 'o', YELLOW, "Urug",
    M_FIGHTER | M_WARM_BLOOD | M_SPEAKS | M_UNIQUE,
    MR_NO_FLAGS,
    0, 20, MONS_ORC, MONS_ORC, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 0, 0, 66 },
    0, 11, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_MICHAEL, '@', LIGHTGREY, "Michael",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SPEAKS | M_WARM_BLOOD | M_UNIQUE,
    MR_NO_FLAGS,
    0, 20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 9}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 0, 0, 50 },
    0, 10, MST_ORC_WIZARD_III, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_JOSEPH, '@', CYAN, "Joseph",
    M_FIGHTER | M_WARM_BLOOD | M_SPEAKS | M_UNIQUE,
    MR_NO_FLAGS,
    0, 20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 9}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 7, 0, 0, 66 },
    0, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_SNORG, 'T', GREEN, "Snorg",
    M_WARM_BLOOD | M_SPEAKS | M_EVIL | M_UNIQUE,
    MR_NO_FLAGS,
    0, 20, MONS_TROLL, MONS_TROLL, MH_NATURAL, -6,
    { {AT_BITE, AF_PLAIN, 20}, {AT_CLAW, AF_PLAIN, 15}, {AT_CLAW, AF_PLAIN, 15}, AT_NO_ATK },
    { 8, 0, 0, 96 },
    0, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_ERICA, '@', MAGENTA, "Erica",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SPEAKS | M_WARM_BLOOD | M_UNIQUE,
    MR_NO_FLAGS,
    0, 20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 9, 0, 0, 64 },
    0, 11, MST_WIZARD_II, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 11, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_JOSEPHINE, '@', WHITE, "Josephine",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SPEAKS | M_WARM_BLOOD | M_UNIQUE,
    MR_NO_FLAGS,
    0, 20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 11}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 9, 0, 0, 69 },
    0, 10, MST_NECROMANCER_I, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_HAROLD, '@', LIGHTGREEN, "Harold",
    M_FIGHTER | M_SPELLCASTER | M_ACTUAL_SPELLS | M_SPEAKS | M_WARM_BLOOD | M_UNIQUE,
    MR_NO_FLAGS,
    0, 20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 9, 0, 0, 76 },
    0, 8, MST_HELL_KNIGHT_II, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_NORBERT, '@', BROWN, "Norbert",
    M_FIGHTER | M_WARM_BLOOD | M_SPEAKS | M_UNIQUE,
    MR_NO_FLAGS,
    0, 20, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 0, 0, 105 },
    0, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_JOZEF, '@', LIGHTMAGENTA, "Jozef",
    M_FIGHTER | M_SPELLCASTER | M_ACTUAL_SPELLS | M_SPEAKS | M_WARM_BLOOD | M_UNIQUE,
    MR_NO_FLAGS,
    0, 15, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 18}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 11, 0, 0, 90 },
    0, 9, MST_GUARDIAN_NAGA, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_AGNES, '@', LIGHTBLUE, "Agnes",
    M_FIGHTER | M_WARM_BLOOD | M_SPEAKS | M_UNIQUE,
    MR_NO_FLAGS,
    0, 15, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 11, 0, 0, 140 },
    0, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_MAUD, '@', RED, "Maud",
    M_FIGHTER | M_WARM_BLOOD | M_SPEAKS | M_UNIQUE,
    MR_NO_FLAGS,
    0, 15, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 24}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 13, 0, 0, 118 },
    0, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_LOUISE, '@', BLUE, "Louise",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SPEAKS | M_WARM_BLOOD | M_UNIQUE,
    MR_RES_ELEC,
    0, 15, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 17}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 13, 0, 0, 106 },
    0, 10, MST_WIZARD_IV, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_FRANCIS, '@', YELLOW, "Francis",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SPEAKS | M_WARM_BLOOD 
        | M_SEE_INVIS | M_EVIL | M_UNIQUE,
    MR_NO_FLAGS,
    0, 15, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 19}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 14, 0, 0, 110 },
    0, 10, MST_ORC_HIGH_PRIEST, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_FRANCES, '@', YELLOW, "Frances",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SPEAKS | M_WARM_BLOOD 
        | M_SEE_INVIS | M_EVIL | M_UNIQUE,
    MR_NO_FLAGS,
    0, 15, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 29}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 14, 0, 0, 121 },
    0, 10, MST_ORC_HIGH_PRIEST, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_RUPERT, '@', RED, "Rupert",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SPEAKS | M_WARM_BLOOD 
        | M_SEE_INVIS | M_UNIQUE,
    MR_RES_ELEC,
    0, 12, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 21}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 16, 0, 0, 123 },
    0, 10, MST_RUPERT, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_WAYNE, '@', YELLOW, "Wayne",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SPEAKS | M_WARM_BLOOD 
        | M_SEE_INVIS | M_UNIQUE,
    MR_NO_FLAGS,
    0, 12, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 22}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 17, 0, 0, 140 },
    1, 10, MST_ORC_PRIEST, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 7, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_DUANE, '@', YELLOW, "Duane",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SPEAKS | M_WARM_BLOOD 
        | M_SEE_INVIS | M_UNIQUE,
    MR_NO_FLAGS,
    0, 10, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 22}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 18, 0, 0, 136 },
    0, 10, MST_ORC_WIZARD_I, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_XTAHUA, 'D', RED, "Xtahua",
    M_SEE_INVIS | M_FLIES | M_SPECIAL_ABILITY | M_UNIQUE | M_WARM_BLOOD,
    MR_RES_POISON | mrd(MR_RES_FIRE, 2) | MR_VUL_COLD,
    0, 18, MONS_DRAGON, MONS_DRAGON, MH_NATURAL, -7,
    { {AT_BITE, AF_PLAIN, 35}, {AT_CLAW, AF_PLAIN, 20}, {AT_CLAW, AF_PLAIN, 17}, AT_NO_ATK },
    { 19, 0, 0, 133 },
    15, 7, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_ROAR, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_HUGE
},

{
    MONS_NORRIS, '@', LIGHTRED, "Norris",
    M_FIGHTER | M_SPELLCASTER | M_ACTUAL_SPELLS | M_SPEAKS | M_WARM_BLOOD 
        | M_SEE_INVIS | M_EVIL | M_UNIQUE,
    MR_NO_FLAGS,
    0, 10, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 36}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 20, 0, 0, 214 },
    1, 9, MST_MYSTIC, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 9, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_FREDERICK, '@', GREEN, "Frederick",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SPEAKS | M_WARM_BLOOD
        | M_SEE_INVIS | M_EVIL | M_UNIQUE,
    MR_NO_FLAGS,
    0, 12, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 27}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 21, 0, 0, 159 },
    0, 10, MST_LICH_IV, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_MARGERY, '@', LIGHTRED, "Margery",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SPEAKS | M_WARM_BLOOD 
        | M_SEE_INVIS | M_UNIQUE,
    MR_NO_FLAGS,
    0, 15, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 22, 0, 0, 164 },
    0, 10, MST_EFREET, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_BORIS, 'L', RED, "Boris",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SEE_INVIS | M_SPEAKS | M_EVIL
        | M_UNIQUE,
    MR_RES_POISON | MR_RES_COLD | MR_RES_ELEC,
    0, 15, MONS_LICH, MONS_LICH, MH_UNDEAD, -11,
    { {AT_HIT, AF_PLAIN, 25}, {AT_TOUCH, AF_DRAIN_XP, 15}, AT_NO_ATK, AT_NO_ATK },
    { 22, 0, 0, 154 },
    12, 10, MST_LICH_IV, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM
},

{
    MONS_GERYON, '&', GREEN, "Geryon",
    M_FIGHTER | M_SPELLCASTER | M_SEE_INVIS | M_SPEAKS | M_EVIL | M_FLIES
      | M_UNIQUE,
    MR_NO_FLAGS,
    0, 15, MONS_GERYON, MONS_GERYON, MH_DEMONIC, -6,
    { {AT_HIT, AF_PLAIN, 35}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 15, 0, 0, 300 },
    15, 6, MST_GERYON, CE_CONTAMINATED, Z_NOZOMBIE, S_ROAR, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_GIANT,
},

{
    MONS_DISPATER, '&', MAGENTA, "Dispater",
    M_FIGHTER | M_SPELLCASTER | M_SEE_INVIS | M_SPEAKS | M_EVIL | M_UNIQUE,
    MR_RES_ELEC | MR_RES_POISON | MR_RES_HELLFIRE | MR_RES_COLD,
    0, 15, MONS_DISPATER, MONS_DISPATER, MH_DEMONIC, -10,
    { {AT_HIT, AF_PLAIN, 50}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 16, 0, 0, 450 },
    40, 3, MST_DISPATER, CE_CONTAMINATED, Z_NOZOMBIE, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_LARGE
},

{
    MONS_ASMODEUS, '&', LIGHTMAGENTA, "Asmodeus",
    M_FIGHTER | M_SPELLCASTER | M_FLIES | M_SEE_INVIS | M_SPEAKS | M_EVIL | M_UNIQUE,
    MR_RES_ELEC | MR_RES_POISON | MR_RES_HELLFIRE,
    0, 25, MONS_ASMODEUS, MONS_ASMODEUS, MH_DEMONIC, -12,
    { {AT_HIT, AF_PLAIN, 50}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 17, 0, 0, 450 },
    30, 7, MST_ASMODEUS, CE_CONTAMINATED, Z_NOZOMBIE, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_LARGE
},

// Antaeus is now demonic so that he'll resist torment. -- bwr
{
    MONS_ANTAEUS, 'C', LIGHTCYAN, "Antaeus",
    M_FIGHTER | M_SPELLCASTER | M_SPEAKS | M_UNIQUE,
    MR_RES_ELEC | MR_VUL_FIRE | MR_RES_COLD,
    0, 15, MONS_HILL_GIANT, MONS_ANTAEUS, MH_DEMONIC, -9,
    { {AT_HIT, AF_COLD, 75}, {AT_HIT, AF_COLD, 30}, AT_NO_ATK, AT_NO_ATK },
    { 22, 0, 0, 700 },
    28, 4, MST_ANTAEUS, CE_CONTAMINATED, Z_NOZOMBIE, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_GIANT,
},

{
    MONS_ERESHKIGAL, '&', WHITE, "Ereshkigal",
    M_SPELLCASTER | M_SEE_INVIS | M_SPEAKS | M_EVIL | M_UNIQUE,
    MR_RES_ELEC | MR_RES_POISON | MR_RES_COLD,
    0, 15, MONS_ERESHKIGAL, MONS_ERESHKIGAL, MH_DEMONIC, -10,
    { {AT_HIT, AF_PLAIN, 40}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 18, 0, 0, 250 },
    10, 30, MST_ERESHKIGAL, CE_CONTAMINATED, Z_NOZOMBIE, S_SHOUT, I_HIGH, 
    HT_LAND, 14, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_LARGE
},

{
    MONS_ANCIENT_LICH, 'L', WHITE, "ancient lich",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SEE_INVIS | M_EVIL,
    MR_RES_POISON | mrd(MR_RES_COLD, 2) | MR_RES_FIRE | MR_RES_ELEC,
    0, 20, MONS_LICH, MONS_LICH, MH_UNDEAD, -14,
    { {AT_TOUCH, AF_DRAIN_XP, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 27, 2, 4, 0 },
    20, 10, MST_LICH_I, CE_NOCORPSE, Z_NOZOMBIE, S_SHOUT, I_HIGH, 
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

/* number is set in define_monster */

{
    MONS_OOZE, 'J', LIGHTGREY, "ooze",
    M_NO_SKELETON | M_SENSE_INVIS,
    MR_RES_POISON | MR_RES_ASPHYX | MR_RES_ACID,
    0, 5, MONS_JELLY, MONS_OOZE, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 3, 5, 0 },
    1, 3, MST_NO_SPELLS, CE_POISONOUS, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 8, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LITTLE,
},

{
    MONS_VAULT_GUARD, '@', CYAN, "vault guard",
    M_FIGHTER | M_WARM_BLOOD | M_SENSE_INVIS,
    MR_NO_FLAGS,
    0, 12, MONS_HUMAN, MONS_HUMAN, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 13, 3, 5, 0 },
    1, 13, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

/* These nasties are never randomly generated, only sometimes specially
   placed in the Crypt. */
{
    MONS_CURSE_SKULL, 'z', LIGHTCYAN, "curse skull",
    M_LEVITATE | M_SPELLCASTER | M_SEE_INVIS | M_EVIL,
    MR_RES_ELEC | MR_RES_POISON | MR_RES_HELLFIRE | mrd(MR_RES_COLD, 2),
    0, 50, MONS_LICH, MONS_CURSE_SKULL, MH_UNDEAD, MAG_IMMUNE,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 13, 0, 0, 66 },
    40, 3, MST_CURSE_SKULL, CE_NOCORPSE, Z_NOZOMBIE, S_MOAN, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_VAMPIRE_KNIGHT, 'V', CYAN, "vampire knight",
    M_FIGHTER | M_SPELLCASTER | M_SEE_INVIS | M_EVIL,
    MR_RES_POISON | MR_RES_COLD,
    0, 13, MONS_VAMPIRE, MONS_VAMPIRE, MH_UNDEAD, -6,
    { {AT_HIT, AF_PLAIN, 33}, {AT_BITE, AF_VAMPIRIC, 15}, AT_NO_ATK, AT_NO_ATK },
    { 11, 3, 7, 0 },
    10, 10, MST_VAMPIRE_KNIGHT, CE_NOCORPSE, Z_NOZOMBIE, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_VAMPIRE_MAGE, 'V', MAGENTA, "vampire mage",
    M_SPELLCASTER | M_SEE_INVIS | M_FLIES | M_EVIL,
    MR_RES_POISON | MR_RES_COLD,
    0, 15, MONS_VAMPIRE, MONS_VAMPIRE, MH_UNDEAD, -6,
    { {AT_BITE, AF_VAMPIRIC, 22}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 8, 3, 4, 0 },
    10, 10, MST_VAMPIRE_MAGE, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_SHINING_EYE, 'G', LIGHTMAGENTA, "shining eye",
    M_NO_SKELETON | M_LEVITATE | M_SPELLCASTER | M_SEE_INVIS,
    MR_RES_ASPHYX,
    0, 14, MONS_SHINING_EYE, MONS_SHINING_EYE, MH_NATURAL, MAG_IMMUNE,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 3, 5, 0 },
    3, 1, MST_SHINING_EYE, CE_POISONOUS, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 7, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_MEDIUM
},

{
    MONS_ORB_GUARDIAN, 'X', MAGENTA, "Orb Guardian",
    M_FIGHTER | M_NO_SKELETON | M_SEE_INVIS,
    MR_NO_FLAGS,
    0, 20, MONS_ORB_GUARDIAN, MONS_ORB_GUARDIAN, MH_NATURAL, -6,
    { {AT_HIT, AF_PLAIN, 45}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 15, 3, 5, 0 },
    13, 13, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 14, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_GIANT,
},

{
    MONS_DAEVA, 'A', YELLOW, "Daeva",
    M_FIGHTER | M_LEVITATE | M_SPELLCASTER | M_SEE_INVIS | M_WARM_BLOOD,
    MR_RES_POISON,
    0, 12, MONS_ANGEL, MONS_DAEVA, MH_HOLY, -8,
    { {AT_HIT, AF_PLAIN, 25}, {AT_HIT, AF_PLAIN, 10}, {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK },
    { 14, 6, 5, 0 },
    10, 13, MST_DAEVA, CE_NOCORPSE, Z_NOZOMBIE, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

/* spectral thing - similar to zombies/skeletons */
{
    MONS_SPECTRAL_THING, 'W', GREEN, "spectral thing",
    M_LEVITATE | M_SEE_INVIS,
    MR_RES_POISON | MR_RES_COLD,
    0, 11, MONS_WRAITH, MONS_SPECTRAL_THING, MH_UNDEAD, MAG_IMMUNE,
    { {AT_HIT, AF_DRAIN_XP, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 8, 3, 5, 0 },
    8, 5, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 7, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_GREATER_NAGA, 'N', LIGHTRED, "greater naga",
    M_FIGHTER | M_SPELLCASTER | M_ACTUAL_SPELLS | M_SEE_INVIS | M_WARM_BLOOD,
    MR_RES_POISON,
    750, 10, MONS_NAGA, MONS_NAGA, MH_NATURAL, MAG_IMMUNE,
    { {AT_HIT, AF_PLAIN, 27}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 15, 3, 5, 0 },
    6, 10, MST_NAGA_MAGE, CE_POISONOUS, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 8, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_BIG
},

{
    MONS_SKELETAL_DRAGON, 'D', LIGHTGREY, "skeletal dragon",
    M_SENSE_INVIS | M_EVIL,
    MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC,
    0, 12, MONS_SKELETAL_WARRIOR, MONS_SKELETAL_DRAGON, MH_UNDEAD, -4,
    { {AT_BITE, AF_PLAIN, 30}, {AT_CLAW, AF_PLAIN, 20}, {AT_CLAW, AF_PLAIN, 20}, AT_NO_ATK },
    { 20, 8, 8, 0 },
    20, 4, MST_NO_SPELLS, CE_CLEAN, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 8, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_HUGE
},

{
    MONS_TENTACLED_MONSTROSITY, 'X', GREEN, "tentacled monstrosity",
    M_SEE_INVIS | M_AMPHIBIOUS,
    MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC,
    0, 10, MONS_TENTACLED_MONSTROSITY, MONS_TENTACLED_MONSTROSITY, MH_DEMONIC, -5,
    { {AT_HIT, AF_PLAIN, 22}, {AT_HIT, AF_PLAIN, 17}, {AT_HIT, AF_PLAIN, 13} },
    { 25, 3, 5, 0 },
    5, 5, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_NORMAL, 
    HT_LAND, 9, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_GIANT,
},

{
    MONS_SPHINX, 'H', LIGHTGREY, "sphinx",
    M_FLIES | M_SENSE_INVIS | M_SPELLCASTER | M_ACTUAL_SPELLS | M_WARM_BLOOD,
    MR_NO_FLAGS,
    0, 10, MONS_SPHINX, MONS_SPHINX, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 25}, {AT_HIT, AF_PLAIN, 12}, {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK },
    { 16, 3, 5, 0 },
    5, 5, MST_SPHINX, CE_CLEAN, Z_NOZOMBIE, S_SHOUT, I_HIGH, 
    HT_LAND, 13, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_BIG
},

{
    MONS_ROTTING_HULK, 'n', BROWN, "rotting hulk",
    M_EVIL,
    MR_RES_POISON | MR_RES_COLD,
    0, 12, MONS_GHOUL, MONS_ROTTING_HULK, MH_UNDEAD, -5,
    { {AT_HIT, AF_DISEASE, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 3, 5, 0 },
    5, 7, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_NORMAL, 
    HT_LAND, 8, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_GUARDIAN_MUMMY, 'M', YELLOW, "guardian mummy",
    M_FIGHTER | M_SEE_INVIS | M_EVIL,
    MR_RES_POISON | MR_RES_COLD,
    0, 13, MONS_MUMMY, MONS_GUARDIAN_MUMMY, MH_UNDEAD, -5,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 7, 5, 3, 0 },
    6, 9, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 9, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_BIG
},

{
    MONS_GREATER_MUMMY, 'M', WHITE, "greater mummy",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SEE_INVIS | M_EVIL,
    MR_RES_POISON | MR_RES_COLD | MR_RES_ELEC,
    0, 20, MONS_MUMMY, MONS_MUMMY, MH_UNDEAD, MAG_IMMUNE,
    { {AT_HIT, AF_PLAIN, 35}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 15, 5, 3, 100 },
    10, 6, MST_MUMMY, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_MUMMY_PRIEST, 'M', RED, "mummy priest",
    M_SPELLCASTER | M_PRIEST | M_SEE_INVIS | M_EVIL,
    MR_RES_POISON | MR_RES_COLD | MR_RES_ELEC,
    0, 16, MONS_MUMMY, MONS_MUMMY, MH_UNDEAD, MAG_IMMUNE,
    { {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 5, 3, 0 },
    8, 7, MST_MUMMY, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_HIGH, 
    HT_LAND, 9, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_CENTAUR_WARRIOR, 'c', YELLOW, "centaur warrior",
    M_WARM_BLOOD | M_FIGHTER,
    MR_NO_FLAGS,
    1500, 12, MONS_CENTAUR, MONS_CENTAUR, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 16}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 9, 3, 5, 0 },
    4, 8, MST_NO_SPELLS, CE_CLEAN, Z_BIG, S_SHOUT, I_HIGH, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_BIG
},

{
    MONS_YAKTAUR_CAPTAIN, 'c', LIGHTRED, "yaktaur captain",
    M_WARM_BLOOD | M_FIGHTER,
    MR_NO_FLAGS,
    2000, 10, MONS_YAKTAUR, MONS_YAKTAUR, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 23}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 14, 3, 5, 0 },
    5, 5, MST_NO_SPELLS, CE_CLEAN, Z_BIG, S_SHOUT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{   // Base draconian -- for use like MONS_HUMAN, MONS_ELF although we 
    // now store the draconian subspecies in the high byte of mon->number
    // for those listed as species MONS_DRACONIAN.
    MONS_DRACONIAN, 'd', BROWN, "draconian",
    M_HUMANOID | M_COLD_BLOOD,
    MR_NO_FLAGS,
    900, 10, MONS_DRACONIAN, MONS_DRACONIAN, MH_NATURAL, -1,
    { {AT_HIT, AF_PLAIN, 15}, {AT_HIT, AF_PLAIN, 0}, {AT_HIT, AF_PLAIN, 0} },
    { 3, 6, 4, 0 },
    7, 8, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_ROAR, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM
},

{
    MONS_BLACK_DRACONIAN, 'd', BLUE, "black draconian",
    M_HUMANOID | M_COLD_BLOOD,
    MR_RES_ELEC,
    900, 10, MONS_DRACONIAN, MONS_BLACK_DRACONIAN, MH_NATURAL, -2,
    { {AT_HIT, AF_PLAIN, 20}, {AT_HIT, AF_PLAIN, 0}, {AT_HIT, AF_PLAIN, 0} },
    { 14, 5, 4, 0 },
    9, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_ROAR, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM
},

{ 
    MONS_YELLOW_DRACONIAN, 'd', YELLOW, "yellow draconian", 
    M_HUMANOID | M_COLD_BLOOD | M_SPECIAL_ABILITY,
    MR_RES_ACID,
    900, 10, MONS_DRACONIAN, MONS_YELLOW_DRACONIAN, MH_NATURAL, -2,
    { {AT_HIT, AF_PLAIN, 20}, {AT_HIT, AF_PLAIN, 0}, {AT_HIT, AF_PLAIN, 0} },
    { 14, 5, 4, 0 },
    9, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_ROAR, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM
},

{
    MONS_PALE_DRACONIAN, 'd', LIGHTGREY, "pale draconian", 
    M_HUMANOID | M_COLD_BLOOD,
    MR_RES_STEAM, 
    900, 10, MONS_DRACONIAN, MONS_PALE_DRACONIAN, MH_NATURAL, -2,
    { {AT_HIT, AF_PLAIN, 20}, {AT_HIT, AF_PLAIN, 0}, {AT_HIT, AF_PLAIN, 0} },
    { 14, 5, 4, 0 },
    9, 14, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_ROAR, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM
},

{
    MONS_GREEN_DRACONIAN, 'd', GREEN, "green draconian", 
    M_HUMANOID | M_COLD_BLOOD,
    MR_RES_POISON,
    900, 10, MONS_DRACONIAN, MONS_GREEN_DRACONIAN, MH_NATURAL, -2,
    { {AT_HIT, AF_PLAIN, 20}, {AT_HIT, AF_PLAIN, 0}, {AT_HIT, AF_PLAIN, 0} },
    { 14, 5, 4, 0 },
    9, 10, MST_NO_SPELLS, CE_POISONOUS, Z_SMALL, S_ROAR, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM
},

{
    MONS_PURPLE_DRACONIAN, 'd', MAGENTA, "purple draconian", 
    M_HUMANOID | M_COLD_BLOOD,
    MR_NO_FLAGS,
    900, 10, MONS_DRACONIAN, MONS_PURPLE_DRACONIAN, MH_NATURAL, -2,
    { {AT_HIT, AF_PLAIN, 20}, {AT_HIT, AF_PLAIN, 0}, {AT_HIT, AF_PLAIN, 0} },
    { 14, 5, 4, 0 },
    8, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_ROAR, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM
},

{ 
    MONS_RED_DRACONIAN, 'd', RED, "red draconian", 
    M_HUMANOID | M_COLD_BLOOD | M_SPECIAL_ABILITY,
    MR_RES_FIRE,
    900, 10, MONS_DRACONIAN, MONS_RED_DRACONIAN, MH_NATURAL, -2,
    { {AT_HIT, AF_PLAIN, 20}, {AT_HIT, AF_PLAIN, 0}, {AT_HIT, AF_PLAIN, 0} },
    { 14, 5, 4, 0 },
    9, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_ROAR, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM
},

{ 
    MONS_WHITE_DRACONIAN, 'd', WHITE, "white draconian",
    M_HUMANOID | M_COLD_BLOOD | M_SPECIAL_ABILITY,
    MR_RES_COLD,
    900, 10, MONS_DRACONIAN, MONS_WHITE_DRACONIAN, MH_NATURAL, -2,
    { {AT_HIT, AF_PLAIN, 20}, {AT_HIT, AF_PLAIN, 0}, {AT_HIT, AF_PLAIN, 0} },
    { 14, 5, 4, 0 },
    9, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_ROAR, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM
},

{
    MONS_MOTTLED_DRACONIAN, 'd', LIGHTMAGENTA, "mottled draconian", 
    M_HUMANOID | M_COLD_BLOOD,
    MR_RES_FIRE | MR_RES_STICKY_FLAME,
    900, 10, MONS_DRACONIAN, MONS_MOTTLED_DRACONIAN, MH_NATURAL, -2,
    { {AT_HIT, AF_PLAIN, 20}, {AT_HIT, AF_PLAIN, 0}, {AT_HIT, AF_PLAIN, 0} },
    { 14, 5, 4, 0 },
    9, 10, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_ROAR, I_HIGH,
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM
},

{
    MONS_DRACONIAN_CALLER, 'd', BROWN, "draconian caller",  
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_HUMANOID | M_COLD_BLOOD,
    MR_NO_FLAGS,
    900, 10, MONS_DRACONIAN, MONS_DRACONIAN, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 20}, {AT_HIT, AF_PLAIN, 0}, {AT_HIT, AF_PLAIN, 0} },
    { 16, 4, 3, 0 },
    9, 10, MST_DRAC_CALLER, CE_CONTAMINATED, Z_SMALL, S_ROAR, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM
},

{
    MONS_DRACONIAN_MONK, 'd', BLUE, "draconian monk", 
    M_FIGHTER | M_HUMANOID | M_COLD_BLOOD,
    MR_NO_FLAGS,
    900, 10, MONS_DRACONIAN, MONS_DRACONIAN, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 35}, {AT_HIT, AF_PLAIN, 20},
                              {AT_TAIL_SLAP, AF_PLAIN, 15} },
    { 16, 6, 3, 0 },
    6, 20, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_ROAR, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM
},

{ 
    MONS_DRACONIAN_ZEALOT, 'd', LIGHTGREEN, "draconian zealot",
    M_SPELLCASTER | M_HUMANOID | M_PRIEST | M_COLD_BLOOD | M_EVIL,
    MR_NO_FLAGS,
    900, 10, MONS_DRACONIAN, MONS_DRACONIAN, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 15}, {AT_HIT, AF_PLAIN, 0}, {AT_HIT, AF_PLAIN, 0} },
    { 16, 4, 2, 0 },
    12, 10, MST_DEEP_ELF_HIGH_PRIEST, CE_CONTAMINATED, Z_SMALL, S_ROAR, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM
},

{
    MONS_DRACONIAN_SHIFTER, 'd', LIGHTCYAN, "draconian shifter", 
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_HUMANOID | M_COLD_BLOOD,
    MR_NO_FLAGS,
    900, 10, MONS_DRACONIAN, MONS_DRACONIAN, MH_NATURAL, -4,
    { {AT_HIT, AF_PLAIN, 15}, {AT_HIT, AF_PLAIN, 0}, {AT_HIT, AF_PLAIN, 0} },
    { 16, 4, 4, 0 },
    8, 16, MST_DRAC_SHIFTER, CE_CONTAMINATED, Z_SMALL, S_ROAR, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM
},

{
    MONS_DRACONIAN_ANNIHILATOR, 'd', LIGHTBLUE, "draconian annihilator",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_HUMANOID | M_COLD_BLOOD,
    MR_NO_FLAGS,
    900, 10, MONS_DRACONIAN, MONS_DRACONIAN, MH_NATURAL, -4,
    { {AT_HIT, AF_PLAIN, 15}, {AT_HIT, AF_PLAIN, 0}, {AT_HIT, AF_PLAIN, 0} },
    { 16, 4, 2, 0 },
    8, 10, MST_DEEP_ELF_ANNIHILATOR, CE_CONTAMINATED, Z_SMALL, S_ROAR, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM
},

{
    MONS_DRACONIAN_KNIGHT, 'd', CYAN, "draconian knight", 
    M_SPELLCASTER | M_HUMANOID | M_FIGHTER | M_COLD_BLOOD,
    MR_NO_FLAGS,
    900, 10, MONS_DRACONIAN, MONS_DRACONIAN, MH_NATURAL, -4,
    { {AT_HIT, AF_PLAIN, 15}, {AT_HIT, AF_PLAIN, 0}, {AT_HIT, AF_PLAIN, 0} },
    { 16, 6, 4, 0 },
    12, 12, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_ROAR, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM
},

{
    MONS_DRACONIAN_SCORCHER, 'd', LIGHTRED, "draconian scorcher", 
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_HUMANOID | M_COLD_BLOOD | M_EVIL,
    MR_RES_FIRE | MR_RES_HELLFIRE,
    900, 10, MONS_DRACONIAN, MONS_DRACONIAN, MH_NATURAL, -4,
    { {AT_HIT, AF_PLAIN, 15}, {AT_HIT, AF_PLAIN, 0}, {AT_HIT, AF_PLAIN, 0} },
    { 16, 4, 2, 0 },
    8, 12, MST_DRAC_SCORCHER, CE_CONTAMINATED, Z_SMALL, S_ROAR, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM
},

{
    MONS_KILLER_KLOWN, '@', BLACK, "Killer Klown",
    M_SEE_INVIS | M_SPEAKS | M_WARM_BLOOD | M_SPECIAL_ABILITY,
    MR_NO_FLAGS,
    0, 15, MONS_HUMAN, MONS_KILLER_KLOWN, MH_NATURAL, -6,
    { {AT_HIT, AF_KLOWN, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 20, 5, 5, 0 },
    10, 15, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_HIGH, 
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

{
    MONS_ELECTRIC_GOLEM, '8', LIGHTCYAN, "electric golem",
    M_SPELLCASTER | M_SEE_INVIS | M_SPEAKS,
    mrd(MR_RES_ELEC | MR_RES_POISON, 3) | MR_RES_FIRE | MR_RES_COLD,
    0, 10, MONS_CLAY_GOLEM, MONS_ELECTRIC_GOLEM, MH_NONLIVING, -8,
    { {AT_HIT, AF_ELEC, 15}, {AT_HIT, AF_ELEC, 15}, {AT_HIT, AF_PLAIN, 15}, {AT_HIT, AF_PLAIN, 15} },
    { 15, 7, 4, 0 },
    5, 20, MST_ELECTRIC_GOLEM, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 20, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

{
    MONS_BALL_LIGHTNING, '*', LIGHTCYAN, "ball lightning",
    M_FLIES | M_CONFUSED | M_SPELLCASTER | M_SPECIAL_ABILITY | M_INSUBSTANTIAL,
    mrd(MR_RES_ELEC | MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD, 3),
    0, 20, MONS_BALL_LIGHTNING, MONS_BALL_LIGHTNING, MH_NONLIVING, MAG_IMMUNE,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 12, 0, 0, 1 },
    0, 10, MST_STORM_DRAGON, CE_NOCORPSE, Z_NOZOMBIE, S_SHOUT, I_PLANT, 
    HT_LAND, 20, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LITTLE,
},

{
    MONS_ORB_OF_FIRE, '*', RED, "orb of fire",
    M_SPELLCASTER | M_FLIES | M_SEE_INVIS | M_INSUBSTANTIAL,
    MR_RES_ELEC | MR_RES_FIRE | MR_RES_HELLFIRE | MR_RES_COLD | MR_RES_POISON,
    0, 10, MONS_ORB_OF_FIRE, MONS_ORB_OF_FIRE, MH_NONLIVING, MAG_IMMUNE,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 30, 0, 0, 150 },
    20, 20, MST_ORB_OF_FIRE, CE_NOCORPSE, Z_NOZOMBIE, S_SHOUT, I_NORMAL, 
    HT_LAND, 20, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LITTLE,
},

{
    MONS_QUOKKA, 'r', WHITE, "quokka",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    300, 10, MONS_QUOKKA, MONS_QUOKKA, MH_NATURAL, -1,
    { {AT_BITE, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 3, 5, 0 },
    2, 13, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_SILENT, I_ANIMAL,
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_EYE_OF_DEVASTATION, 'G', YELLOW, "eye of devastation",
    M_NO_SKELETON | M_LEVITATE | M_SPELLCASTER | M_SEE_INVIS,
    MR_RES_ASPHYX,
    0, 11, MONS_GIANT_EYEBALL, MONS_EYE_OF_DEVASTATION, MH_NATURAL, MAG_IMMUNE,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 3, 5, 0 },
    12, 1, MST_EYE_OF_DEVASTATION, CE_POISONOUS, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 7, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_BIG
},

{
    MONS_MOTH_OF_WRATH, 'y', BROWN, "moth of wrath",
    M_FLIES | M_SPECIAL_ABILITY,
    MR_NO_FLAGS,
    0, 10, MONS_MOTH_OF_WRATH, MONS_MOTH_OF_WRATH, MH_NATURAL, -3,
    { {AT_BITE, AF_RAGE, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 9, 3, 5, 0 },
    0, 10, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_SHOUT, I_HIGH,
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_MEDIUM
},

{
    MONS_DEATH_COB, '%', YELLOW, "death cob",
    M_SPEAKS | M_EVIL,
    MR_RES_POISON | MR_RES_COLD,
    0, 10, MONS_DEATH_COB, MONS_DEATH_COB, MH_UNDEAD, -3,
    { {AT_HIT, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 4, 5, 0 },
    10, 15, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_MOAN, I_NORMAL, 
    HT_LAND, 25, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_TINY
},

// Curse toes move at half the speed with which they attack.
{
    MONS_CURSE_TOE, 'z', YELLOW, "curse toe", 
    M_LEVITATE | M_SPELLCASTER | M_SEE_INVIS | M_EVIL,
    MR_RES_ELEC | MR_RES_POISON | MR_RES_HELLFIRE | MR_RES_COLD,
    0, 60, MONS_LICH, MONS_CURSE_TOE, MH_UNDEAD, MAG_IMMUNE,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 14, 0, 0, 77 },
    50, 1, MST_CURSE_TOE, CE_NOCORPSE, Z_NOZOMBIE, S_MOAN, I_HIGH, 
    HT_LAND, 12, MOVE_ENERGY(20), MONUSE_NOTHING, SIZE_TINY
},

{
    // gold mimics are the only mimics that actually use their name -- bwr
    MONS_GOLD_MIMIC, '$', YELLOW, "pile of gold coins",
    M_NO_SKELETON,
    MR_RES_POISON | MR_RES_ELEC | MR_RES_FIRE | MR_RES_COLD,
    0, 13, MONS_GOLD_MIMIC, MONS_GOLD_MIMIC, MH_NONLIVING, -3,
    { {AT_HIT, AF_POISON, 12}, {AT_HIT, AF_PLAIN, 12}, {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK },
    { 8, 3, 5, 0 },
    5, 1, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_WEAPON_MIMIC, ')', BLACK, "mimic",
    M_NO_SKELETON,
    MR_RES_POISON | MR_RES_ELEC | MR_RES_FIRE | MR_RES_COLD,
    0, 13, MONS_GOLD_MIMIC, MONS_GOLD_MIMIC, MH_NONLIVING, -3,
    { {AT_HIT, AF_POISON, 17}, {AT_HIT, AF_PLAIN, 17}, {AT_HIT, AF_PLAIN, 17}, AT_NO_ATK },
    { 8, 3, 5, 0 },
    5, 1, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LITTLE,
},

{
    MONS_ARMOUR_MIMIC, '[', BLACK, "mimic",
    M_NO_SKELETON,
    MR_RES_POISON | MR_RES_ELEC | MR_RES_FIRE | MR_RES_COLD,
    0, 13, MONS_GOLD_MIMIC, MONS_GOLD_MIMIC, MH_NONLIVING, -3,
    { {AT_HIT, AF_POISON, 12}, {AT_HIT, AF_PLAIN, 12}, {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK },
    { 8, 3, 5, 0 },
    15, 1, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

{
    MONS_SCROLL_MIMIC, '?', LIGHTGREY, "mimic",
    M_NO_SKELETON,
    MR_RES_POISON | MR_RES_ELEC | MR_RES_FIRE | MR_RES_COLD,
    0, 13, MONS_GOLD_MIMIC, MONS_GOLD_MIMIC, MH_NONLIVING, -3,
    { {AT_HIT, AF_POISON, 12}, {AT_HIT, AF_PLAIN, 12}, {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK },
    { 8, 3, 5, 0 },
    5, 1, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_POTION_MIMIC, '!', BLACK, "mimic",
    M_NO_SKELETON,
    MR_RES_POISON | MR_RES_ELEC | MR_RES_FIRE | MR_RES_COLD,
    0, 13, MONS_GOLD_MIMIC, MONS_GOLD_MIMIC, MH_NONLIVING, -3,
    { {AT_HIT, AF_POISON, 12}, {AT_HIT, AF_PLAIN, 12}, {AT_HIT, AF_PLAIN, 12}, AT_NO_ATK },
    { 8, 3, 5, 0 },
    5, 1, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_HELL_HOG, 'h', LIGHTRED, "hell-hog",
    M_SPELLCASTER | M_THICK_SKIN | M_EVIL,
    MR_NO_FLAGS,
    0, 10, MONS_HELL_HOG, MONS_HELL_HOG, MH_DEMONIC, -3,
    { {AT_BITE, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 11, 3, 5, 0 },
    2, 9, MST_HELL_HOG, CE_CLEAN, Z_NOZOMBIE, S_SILENT, I_ANIMAL, 
    HT_LAND, 14, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_MEDIUM
},

{
    MONS_SERPENT_OF_HELL, 'D', RED, "Serpent of Hell",
    M_SPELLCASTER | M_FLIES | M_SENSE_INVIS | M_EVIL,
    MR_RES_POISON | MR_RES_HELLFIRE,
    0, 18, MONS_SERPENT_OF_HELL, MONS_SERPENT_OF_HELL, MH_DEMONIC, -13,
    { {AT_BITE, AF_PLAIN, 35}, {AT_CLAW, AF_PLAIN, 15}, {AT_CLAW, AF_PLAIN, 15}, AT_NO_ATK },
    { 20, 4, 4, 0 },
    12, 9, MST_SERPENT_OF_HELL, CE_CLEAN, Z_NOZOMBIE, S_ROAR, I_HIGH, 
    HT_LAND, 14, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_HUGE
},

{
    MONS_BOGGART, 'g', MAGENTA, "boggart",
    M_SPELLCASTER | M_ACTUAL_SPELLS | M_SEE_INVIS | M_WARM_BLOOD,
    MR_NO_FLAGS,
    0, 14, MONS_BOGGART, MONS_BOGGART, MH_NATURAL, -7,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 2, 3, 5, 0 },
    0, 12, MST_BOGGART, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL, 
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_LITTLE,
},

{
    MONS_QUICKSILVER_DRAGON, 'D', LIGHTCYAN, "quicksilver dragon",
    M_SPELLCASTER | M_FLIES | M_SENSE_INVIS | M_WARM_BLOOD,
    MR_NO_FLAGS,
    0, 14, MONS_DRAGON, MONS_QUICKSILVER_DRAGON, MH_NATURAL, -7,
    { {AT_BITE, AF_PLAIN, 25}, {AT_CLAW, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK },
    { 16, 3, 5, 0 },
    10, 15, MST_QUICKSILVER_DRAGON, CE_CONTAMINATED, Z_SMALL, S_ROAR, I_ANIMAL,
    HT_LAND, 15, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_GIANT,
},

{
    MONS_IRON_DRAGON, 'D', CYAN, "iron dragon",
    M_SPELLCASTER | M_SENSE_INVIS | M_WARM_BLOOD,
    MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD,
    0, 14, MONS_DRAGON, MONS_IRON_DRAGON, MH_NATURAL, -7,
    { {AT_BITE, AF_PLAIN, 25}, {AT_CLAW, AF_PLAIN, 25}, {AT_CLAW, AF_PLAIN, 25}, AT_NO_ATK },
    { 18, 5, 3, 0 },
    20, 6, MST_IRON_DRAGON, CE_CONTAMINATED, Z_SMALL, S_ROAR, I_ANIMAL,
    HT_LAND, 8, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_HUGE
},

{
    MONS_SKELETAL_WARRIOR, 'z', CYAN, "skeletal warrior",
    M_FIGHTER | M_SPELLCASTER | M_ACTUAL_SPELLS | M_EVIL,
    MR_RES_POISON | MR_RES_COLD,
    0, 10, MONS_SKELETAL_WARRIOR, MONS_SKELETAL_WARRIOR, MH_UNDEAD, -7,
    { {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 5, 3, 0 },
    15, 10, MST_SKELETAL_WARRIOR, CE_CONTAMINATED, Z_SMALL, S_SILENT, I_NORMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

/* player ghost - stats are stored in ghost struct */
{
    MONS_PLAYER_GHOST, 'p', WHITE, "player ghost",
    M_FIGHTER | M_SPEAKS | M_SPELLCASTER | M_ACTUAL_SPELLS | M_FLIES | M_UNIQUE | M_INSUBSTANTIAL,
    MR_RES_POISON,
    0, 15, MONS_PHANTOM, MONS_PLAYER_GHOST, MH_UNDEAD, -5,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 4, 2, 3, 0 },
    1, 2, MST_GHOST, CE_NOCORPSE, Z_NOZOMBIE, S_SHOUT, I_HIGH,
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_MEDIUM
},

/* random demon in pan - only one per level. stats are stored in ghost struct */
{
    MONS_PANDEMONIUM_DEMON, '&', BLACK, "pandemonium lord",
    M_FIGHTER | M_SPELLCASTER | M_SPEAKS | M_EVIL,
    MR_RES_POISON,
    0, 14, MONS_PANDEMONIUM_DEMON, MONS_PANDEMONIUM_DEMON, MH_DEMONIC, -5,
    { {AT_HIT, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 4, 2, 3, 0 },
    1, 2, MST_GHOST, CE_CONTAMINATED, Z_NOZOMBIE, S_RANDOM, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_LARGE
},

// begin lava monsters {dlb}
{
    MONS_LAVA_WORM, 'w', RED, "lava worm",
    M_NO_FLAGS,
    MR_RES_FIRE | MR_VUL_COLD,
    0, 10, MONS_LAVA_WORM, MONS_LAVA_WORM, MH_NATURAL, -3,
    { {AT_BITE, AF_FIRE, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 3, 5, 0 },
    1, 10, MST_NO_SPELLS, CE_NOCORPSE, Z_SMALL, S_SILENT, I_ANIMAL, 
    HT_LAVA, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

{
    MONS_LAVA_FISH, ';', RED, "lava fish",
    M_WARM_BLOOD,
    MR_RES_FIRE | MR_VUL_COLD,
    0, 10, MONS_BIG_FISH, MONS_LAVA_FISH, MH_NATURAL, -3,
    { {AT_BITE, AF_FIRE, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 4, 3, 5, 0 },
    4, 15, MST_NO_SPELLS, CE_NOCORPSE, Z_SMALL, S_SILENT, I_ANIMAL, 
    HT_LAVA, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LITTLE,
},

{
    MONS_LAVA_SNAKE, 'S', RED, "lava snake",
    M_SPECIAL_ABILITY,
    mrd(MR_RES_FIRE, 3) | MR_VUL_COLD,
    0, 10, MONS_SNAKE, MONS_LAVA_SNAKE, MH_NATURAL, -3,
    { {AT_BITE, AF_FIRE, 7}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 3, 5, 0 },
    2, 17, MST_NO_SPELLS, CE_NOCORPSE, Z_SMALL, S_HISS, I_ANIMAL, 
    HT_LAVA, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_MEDIUM
},

{  // mv: was another lava thing
    MONS_SALAMANDER, 'S', LIGHTRED, "salamander",
    M_FIGHTER | M_WARM_BLOOD,
    mrd(MR_RES_FIRE, 3) | MR_VUL_COLD,
    0, 10, MONS_SALAMANDER, MONS_SALAMANDER, MH_NATURAL, -3,
    { {AT_HIT, AF_FIRE, 23}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 14, 3, 5, 0 },
    5, 5, MST_NO_SPELLS, CE_NOCORPSE, Z_SMALL, S_SILENT, I_HIGH, 
    HT_LAVA, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

// end lava monsters {dlb}

// begin water monsters {dlb}
{
    MONS_BIG_FISH, ';', LIGHTGREEN, "big fish",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    0, 10, MONS_BIG_FISH, MONS_BIG_FISH, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 8}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 4, 3, 5, 0 },
    1, 12, MST_NO_SPELLS, CE_NOCORPSE, Z_SMALL, S_SILENT, I_ANIMAL, 
    HT_WATER, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

{
    MONS_GIANT_GOLDFISH, ';', LIGHTRED, "giant goldfish",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    0, 10, MONS_BIG_FISH, MONS_GIANT_GOLDFISH, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 7, 3, 5, 0 },
    5, 7, MST_NO_SPELLS, CE_NOCORPSE, Z_SMALL, S_SILENT, I_ANIMAL, 
    HT_WATER, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LITTLE,
},

{
    MONS_ELECTRICAL_EEL, ';', LIGHTBLUE, "electrical eel",
    M_COLD_BLOOD | M_SPECIAL_ABILITY,
    MR_RES_ELEC,
    0, 10, MONS_ELECTRICAL_EEL, MONS_ELECTRICAL_EEL, MH_NATURAL, -3,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 3, 5, 0 },
    1, 15, MST_NO_SPELLS, CE_NOCORPSE, Z_SMALL, S_SILENT, I_ANIMAL, 
    HT_WATER, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

{
    MONS_JELLYFISH, 'J', CYAN, "jellyfish",
    M_NO_FLAGS,
    MR_RES_POISON,
    0, 10, MONS_JELLYFISH, MONS_JELLYFISH, MH_NATURAL, -3,
    { {AT_STING, AF_POISON_STR, 1}, {AT_HIT, AF_PLAIN, 1}, AT_NO_ATK, AT_NO_ATK },
    { 4, 3, 5, 0 },
    0, 5, MST_NO_SPELLS, CE_NOCORPSE, Z_SMALL, S_SILENT, I_PLANT, 
    HT_WATER, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LITTLE,
},

{
    MONS_WATER_ELEMENTAL, '{', LIGHTBLUE, "water elemental",
    M_FLIES | M_AMPHIBIOUS,
    MR_RES_POISON | MR_VUL_FIRE | MR_RES_ELEC,
    0, 10, MONS_EARTH_ELEMENTAL, MONS_WATER_ELEMENTAL, MH_NONLIVING, MAG_IMMUNE,
    { {AT_HIT, AF_PLAIN, 25}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 5, 3, 0 },
    0, 7, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_WATER, 10, DEFAULT_ENERGY, MONUSE_OPEN_DOORS, SIZE_BIG
},

{
    MONS_SWAMP_WORM, 'w', BROWN, "swamp worm",
    M_NO_FLAGS,
    MR_NO_FLAGS,
    0, 10, MONS_WORM, MONS_SWAMP_WORM, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 5, 5, 0 },
    3, 12, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_WATER, 12, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

// end water monsters {dlb}

// begin "move through rock" monsters {mpc}
{
    MONS_ROCK_WORM, 'w', BROWN, "rock worm",
    M_NO_FLAGS,
    MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC,
    0, 10, MONS_WORM, MONS_ROCK_WORM, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 5, 5, 0 },
    3, 12, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_ROCK, 12, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},
// end "move through rock" monsters {mpc}

{
    MONS_WOLF, 'h', LIGHTGREY, "wolf",
    M_WARM_BLOOD | M_SENSE_INVIS,
    MR_NO_FLAGS,
    450, 10, MONS_HOUND, MONS_WOLF, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 8}, {AT_CLAW, AF_PLAIN, 2}, {AT_CLAW, AF_PLAIN, 2}, AT_NO_ATK },
    { 4, 3, 5, 0 },
    3, 15, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_BARK, I_ANIMAL, 
    HT_LAND, 17, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

{
    MONS_WARG, 'h', WHITE, "warg",
    M_SENSE_INVIS | M_WARM_BLOOD,
    MR_RES_POISON,
    600, 12, MONS_HOUND, MONS_WARG, MH_NATURAL, -6,
    { {AT_BITE, AF_PLAIN, 12}, {AT_CLAW, AF_PLAIN, 3}, {AT_CLAW, AF_PLAIN, 3}, AT_NO_ATK },
    { 4, 4, 5, 0 },
    4, 12, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_BARK, I_ANIMAL, 
    HT_LAND, 13, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

{
    MONS_BEAR, 'U', BROWN, "bear",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    2000, 10, MONS_BEAR, MONS_BEAR, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 10}, {AT_CLAW, AF_PLAIN, 6}, {AT_CLAW, AF_PLAIN, 6}, AT_NO_ATK },
    { 7, 3, 3, 0 },
    4, 4, MST_NO_SPELLS, CE_CLEAN, Z_BIG, S_GROWL, I_ANIMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

{
    MONS_GRIZZLY_BEAR, 'U', LIGHTGREY, "grizzly bear",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    2500, 10, MONS_BEAR, MONS_GRIZZLY_BEAR, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 12}, {AT_CLAW, AF_PLAIN, 8}, {AT_CLAW, AF_PLAIN, 8}, AT_NO_ATK },
    { 7, 4, 4, 0 },
    5, 8, MST_NO_SPELLS, CE_CLEAN, Z_BIG, S_GROWL, I_ANIMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

{
    MONS_POLAR_BEAR, 'U', WHITE, "polar bear",
    M_WARM_BLOOD | M_AMPHIBIOUS,
    MR_RES_COLD,
    2500, 10, MONS_BEAR, MONS_POLAR_BEAR, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 20}, {AT_CLAW, AF_PLAIN, 5}, {AT_CLAW, AF_PLAIN, 5}, AT_NO_ATK },
    { 7, 5, 3, 0 },
    7, 8, MST_NO_SPELLS, CE_CLEAN, Z_BIG, S_GROWL, I_ANIMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_BIG
},

{
    MONS_BLACK_BEAR, 'U', BLUE, "black bear",
    M_WARM_BLOOD,
    MR_NO_FLAGS,
    1800, 10, MONS_BEAR, MONS_BLACK_BEAR, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 4}, {AT_CLAW, AF_PLAIN, 4}, {AT_CLAW, AF_PLAIN, 4}, AT_NO_ATK },
    { 6, 3, 3, 0 },
    2, 8, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_GROWL, I_ANIMAL, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_MEDIUM
},

// small simulacrum
{
    MONS_SIMULACRUM_SMALL, 'z', WHITE, "small simulacrum",
    M_EVIL,
    MR_RES_POISON | MR_VUL_FIRE | mrd(MR_RES_COLD, 3),
    0, 6, MONS_SIMULACRUM_SMALL, MONS_SIMULACRUM_SMALL, MH_UNDEAD, -1,
    { {AT_HIT, AF_PLAIN, 6}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 2, 3, 5, 0 },
    10, 4, MST_NO_SPELLS, CE_CONTAMINATED, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 7, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_SMALL
},

// large simulacrum
{
    MONS_SIMULACRUM_LARGE, 'Z', WHITE, "large simulacrum",
    M_EVIL,
    MR_RES_POISON | MR_VUL_FIRE | mrd(MR_RES_COLD, 3),
    0, 6, MONS_SIMULACRUM_SMALL, MONS_SIMULACRUM_LARGE, MH_UNDEAD, -1,
    { {AT_HIT, AF_PLAIN, 14}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 3, 5, 0 },
    10, 5, MST_NO_SPELLS, CE_CONTAMINATED, Z_NOZOMBIE, S_SILENT, I_PLANT, 
    HT_LAND, 7, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

{
    MONS_MERFOLK, 'm', LIGHTBLUE, "merfolk fighter",
    M_HUMANOID | M_WARM_BLOOD | M_AMPHIBIOUS,
    MR_RES_POISON | MR_RES_COLD,
    500, 10, MONS_MERFOLK, MONS_MERFOLK, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 14}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 8, 2, 4, 0 },
    4, 12, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL,
    HT_WATER, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_MERMAID, 'm', LIGHTCYAN, "mermaid",
    M_SPELLCASTER | M_HUMANOID | M_WARM_BLOOD | M_AMPHIBIOUS,
    MR_RES_POISON | MR_RES_COLD,
    500, 10, MONS_MERMAID, MONS_MERMAID, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 10}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 8, 2, 4, 0 },
    4, 12, MST_NO_SPELLS, CE_CONTAMINATED, Z_SMALL, S_SHOUT, I_NORMAL,
    HT_WATER, 10, DEFAULT_ENERGY, MONUSE_WEAPONS_ARMOUR, SIZE_MEDIUM
},

{
    MONS_GIANT_NEWT, 'l', LIGHTGREEN, "giant newt",
    M_COLD_BLOOD | M_AMPHIBIOUS,
    MR_NO_FLAGS,
    150, 10, MONS_GIANT_LIZARD, MONS_GIANT_NEWT, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 3}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 1, 2, 0 },
    0, 15, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_SILENT, I_INSECT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_GIANT_GECKO, 'l', YELLOW, "giant gecko",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    250, 10, MONS_GIANT_LIZARD, MONS_GIANT_GECKO, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 5}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 1, 3, 5, 0 },
    1, 14, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_SILENT, I_INSECT, 
    HT_LAND, 12, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_GIANT_IGUANA, 'l', BLUE, "giant iguana",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    400, 10, MONS_GIANT_LIZARD, MONS_GIANT_IGUANA, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 15}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 3, 3, 5, 0 },
    5, 9, MST_NO_SPELLS, CE_CLEAN, Z_SMALL, S_HISS, I_INSECT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LITTLE,
},

{
    // gila monsters colours: lightmagenta, magenta, lightred, yellow
    MONS_GILA_MONSTER, 'l', BLACK, "gila monster",
    M_COLD_BLOOD,
    MR_NO_FLAGS,
    500, 10, MONS_GIANT_LIZARD, MONS_GILA_MONSTER, MH_NATURAL, -3,
    { {AT_BITE, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 5, 4, 4, 0 },
    3, 12, MST_NO_SPELLS, CE_POISONOUS, Z_BIG, S_HISS, I_INSECT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LITTLE,
},

{
    MONS_KOMODO_DRAGON, 'l', LIGHTRED, "komodo dragon",
    M_COLD_BLOOD | M_AMPHIBIOUS,
    MR_NO_FLAGS,
    800, 10, MONS_GIANT_LIZARD, MONS_KOMODO_DRAGON, MH_NATURAL, -3,
    { {AT_BITE, AF_DISEASE, 30}, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 8, 3, 5, 0 },
    7, 8, MST_NO_SPELLS, CE_CONTAMINATED, Z_BIG, S_HISS, I_INSECT, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_MEDIUM
},

{
    MONS_ORANGE_STATUE, '8', LIGHTRED, "orange crystal statue",
    M_SPECIAL_ABILITY | M_SPEAKS,
    mrd(MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC, 2),
    0, 10, MONS_CLAY_GOLEM, MONS_ORANGE_STATUE, MH_NONLIVING, MAG_IMMUNE,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 10, 0, 0, 160 },
    20, 1, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_HIGH, 
    HT_LAND, 6, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

{
    MONS_SILVER_STATUE, '8', WHITE, "silver statue",
    M_SPECIAL_ABILITY | M_SPEAKS,
    mrd(MR_RES_POISON | MR_RES_FIRE | MR_RES_COLD | MR_RES_ELEC, 2),
    0, 10, MONS_CLAY_GOLEM, MONS_SILVER_STATUE, MH_NONLIVING, MAG_IMMUNE,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 6, 0, 0, 150 },
    15, 1, MST_NO_SPELLS, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

{
    MONS_ICE_STATUE, '8', LIGHTBLUE, "ice statue",
    M_SPELLCASTER | M_SPEAKS,
    MR_RES_POISON | MR_VUL_FIRE | mrd(MR_RES_COLD, 3) | MR_RES_ELEC,
    0, 10, MONS_CLAY_GOLEM, MONS_ICE_STATUE, MH_NONLIVING, MAG_IMMUNE,
    { AT_NO_ATK, AT_NO_ATK, AT_NO_ATK, AT_NO_ATK },
    { 8, 0, 0, 70 },
    12, 1, MST_ICE_STATUE, CE_NOCORPSE, Z_NOZOMBIE, S_SILENT, I_HIGH, 
    HT_LAND, 16, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_LARGE
},

{
    MONS_MURRAY, 'z', LIGHTRED, "Murray",
    M_SPELLCASTER | M_SEE_INVIS | M_EVIL | M_SPEAKS | M_UNIQUE,
    MR_RES_ELEC | MR_RES_POISON | MR_RES_HELLFIRE | mrd(MR_RES_COLD, 2),
    0, 10, MONS_LICH, MONS_CURSE_SKULL, MH_UNDEAD, MAG_IMMUNE,
    { {AT_BITE, AF_PLAIN, 20}, {AT_BITE, AF_PLAIN, 20}, AT_NO_ATK, AT_NO_ATK },
    { 14, 0, 0, 180 },
    30, 10, MST_CURSE_SKULL, CE_NOCORPSE, Z_NOZOMBIE, S_MOAN, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_NOTHING, SIZE_TINY
},

{
    MONS_TIAMAT, 'd', MAGENTA, "Tiamat", 
    M_HUMANOID | M_SEE_INVIS | M_EVIL | M_UNIQUE | M_FLIES | M_COLD_BLOOD,
    MR_RES_POISON,
    0, 10, MONS_DRACONIAN, MONS_DRACONIAN, MH_NATURAL, -5,
    { {AT_HIT, AF_PLAIN, 35}, {AT_TAIL_SLAP, AF_PLAIN, 25},
      AT_NO_ATK, AT_NO_ATK },
    { 22, 0, 0, 200 },
    8, 10, MST_NO_SPELLS, CE_NOCORPSE, Z_SMALL, S_ROAR, I_HIGH, 
    HT_LAND, 10, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_MEDIUM
},

{
    MONS_POLYPHEMUS, 'C', GREEN, "Polyphemus",
    M_WARM_BLOOD | M_EVIL | M_UNIQUE,
    MR_NO_FLAGS,
    2500, 10, MONS_HILL_GIANT, MONS_CYCLOPS, MH_NATURAL, -3,
    { {AT_HIT, AF_PLAIN, 35}, {AT_HIT, AF_PLAIN, 30}, AT_NO_ATK, AT_NO_ATK },
    { 12, 0, 0, 105 },
    7, 3, MST_NO_SPELLS, CE_CLEAN, Z_BIG, S_SHOUT, I_HIGH, 
    HT_LAND, 8, DEFAULT_ENERGY, MONUSE_STARTING_EQUIPMENT, SIZE_GIANT,
},
#endif
