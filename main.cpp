/*
 * Copyright (c) 2015 Falltergeist developers
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

// C++ standard includes
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

// pro2json includes

// Third party includes

using namespace std;

std::string flagValue(uint32_t value, uint32_t flag)
{
    return value&flag ? "true" : "false";
}

uint32_t uint32(std::ifstream& stream)
{
    uint32_t value;
    char* buff = reinterpret_cast<char*>(&value);
    stream.read(buff, 4);
    std::reverse(buff, buff + 4);
    return value;
}

int32_t int32(std::ifstream& stream)
{
    return uint32(stream);
}


uint8_t uint8(std::ifstream& stream)
{
    uint8_t value;
    char* buff = reinterpret_cast<char*>(&value);
    stream.read(buff, 1);
    return value;
}

void usage(std::string binaryName)
{
    cout << "PRO to JSON converter v0.0.3" << endl;
    cout << "Copyright (c) 2015 Falltergeist developers" << endl;
    cout << "Usage: " << binaryName << " <PRO filename>" << endl;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        usage(argv[0]);
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream in;

    in.open(filename.c_str(), std::ios_base::binary | std::ios_base::in);
    if (!in.is_open())
    {
        cout << "Can't open file: " << filename << endl;
        return 1;
    }

    uint32_t PID = uint32(in);
    cout << "{"                                                     << endl
         << "    \"pid\": "    << PID        << ","                 << endl
         << "    \"text\": "   << uint32(in) << ","                 << endl
         << "    \"fid\": "    << uint32(in) << ","                 << endl   
         << "    \"light\": {"                                      << endl
         << "        \"radius\": "    << uint32(in) << ","          << endl
         << "        \"intencity\": " << uint32(in)                 << endl
         << "    },"                                                << endl;
    uint32_t flags = uint32(in);
    cout << "    \"flags\": {"                                                  << endl
         << "        \"FLAT\": "         << flagValue(flags, 0x00000008) << "," << endl
         << "        \"NO_BLOCK\": "     << flagValue(flags, 0x00000010) << "," << endl
         << "        \"MULTIHEX\": "     << flagValue(flags, 0x00000800) << "," << endl
         << "        \"NO_HIGHLIGHT\": " << flagValue(flags, 0x00001000) << "," << endl
         << "        \"TRANS_RED\": "    << flagValue(flags, 0x00004000) << "," << endl
         << "        \"TRANS_NONE\": "   << flagValue(flags, 0x00008000) << "," << endl
         << "        \"TRANS_WALL\": "   << flagValue(flags, 0x00010000) << "," << endl
         << "        \"TRANS_GLASS\": "  << flagValue(flags, 0x00020000) << "," << endl
         << "        \"TRANS_STREAM\": " << flagValue(flags, 0x00080000) << "," << endl
         << "        \"TRANS_ENERGY\": " << flagValue(flags, 0x00040000) << "," << endl
         << "        \"NO_EGG\": "       << flagValue(flags, 0x10000000) << "," << endl
         << "        \"LIGHT_THRU\": "   << flagValue(flags, 0x20000000) << "," << endl
         << "        \"SHOOT_THRU\": "   << flagValue(flags, 0x80000000)        << endl
         << "    },"                                                            << endl;

    switch(PID >> 24)
    {
        case 0: // items
        {
            uint32_t flags = uint32(in);
            cout << "    \"item\": {"                                                             << endl
                 << "        \"flags\": {"                                                        << endl
                 << "            \"HIDDEN\": "            << flagValue(flags, 0x08000000 ) << "," << endl
                 << "            \"CAN_USE\": "           << flagValue(flags, 0x00000800 ) << "," << endl
                 << "            \"CAN_USE_ON\": "        << flagValue(flags, 0x00001000 ) << "," << endl
                 << "            \"CAN_PICK_UP\": "       << flagValue(flags, 0x00008000 ) << "," << endl
                 << "            \"WEAPON_BIG\": "        << flagValue(flags, 0x00000100 ) << "," << endl
                 << "            \"WEAPON_TWO_HANDED\": " << flagValue(flags, 0x00000200 ) << "," << endl
                 << "            \"WEAPON_ATTACK_1\": "   << (flags & 0x0000000F)          << "," << endl
                 << "            \"WEAPON_ATTACK_2\": "   << ((flags & 0x000000F0) >> 4)          << endl
                 << "        },"                                                                  << endl
                 << "        \"sid\": "                   << int32(in) << ","                     << endl;

            uint32_t subtype = uint32(in);
            cout << "        \"material\": "      << uint32(in) << "," << endl
                 << "        \"inventory\": "     << uint32(in) << "," << endl
                 << "        \"weight\": "        << uint32(in) << "," << endl
                 << "        \"cost\": "          << uint32(in) << "," << endl
                 << "        \"fid\": "           << int32(in)  << "," << endl
                 << "        \"sound\": "         << (int) uint8(in)   << endl;

            switch(subtype)
            {
                case 0: // armor
                {
                    cout << "        \"armor\": {"                                  << endl
                         << "            \"class\": "          << uint32(in) << "," << endl
                         << "            \"resistance\": {"                         << endl
                         << "                \"normal\": "     << uint32(in) << "," << endl
                         << "                \"laser\": "      << uint32(in) << "," << endl
                         << "                \"fire\": "       << uint32(in) << "," << endl
                         << "                \"plasma\": "     << uint32(in) << "," << endl
                         << "                \"electrical\": " << uint32(in) << "," << endl
                         << "                \"emp\": "        << uint32(in) << "," << endl
                         << "                \"explosion\": "  << uint32(in)        << endl
                         << "            },"                                        << endl
                         << "            \"threshold\": {"                          << endl
                         << "                \"normal\": "     << uint32(in) << "," << endl
                         << "                \"laser\": "      << uint32(in) << "," << endl
                         << "                \"fire\": "       << uint32(in) << "," << endl
                         << "                \"plasma\": "     << uint32(in) << "," << endl
                         << "                \"electrical\": " << uint32(in) << "," << endl
                         << "                \"emp\": "        << uint32(in) << "," << endl
                         << "                \"explosion\": "  << uint32(in)        << endl
                         << "            },"                                        << endl
                         << "            \"perk\": "           << int32(in)  << "," << endl 
                         << "            \"fid\": {"                                << endl 
                         << "                \"male\": "       << int32(in)  << "," << endl
                         << "                \"female\": "     << int32(in)         << endl
                         << "             }"                                        << endl
                         << "        }"                                             << endl;
                    break;
                }
                case 1: // container
                {
                    cout << "        \"container\" {"                         << endl
                         << "            \"capacity\": " << uint32(in) << "," << endl;
                    uint32_t flags = uint32(in);
                    cout << "            \"flags\": {"                                                        << endl
                         << "                \"CANT_PICK_UP\": "       << flagValue(flags, 0x00000001) << "," << endl
                         << "                \"MAGIC_HANDS_GROUND\": " << flagValue(flags, 0x00000008)        << endl
                         << "            }"                                                                   << endl
                         << "        }"                                                                       << endl;
                    break;
                }
                case 2: // drug
                {
                    int32_t stat0, stat1, stat2;
                    stat0 = int32(in);
                    stat1 = int32(in);
                    stat2 = int32(in);
                    cout << "        \"drug\": {"                                  << endl;
                    cout << "            \"instant\": ["                           << endl;                    
                    cout << "                {"                                    << endl;
                    cout << "                   \"stat\": "    << stat0     << "," << endl;
                    cout << "                   \"ammount\": " << int32(in) << "," << endl;
                    cout << "                },"                                   << endl;
                    cout << "                {"                                    << endl;
                    cout << "                   \"stat\": "    << stat1     << "," << endl;
                    cout << "                   \"ammount\": " << int32(in) << "," << endl;
                    cout << "                },"                                   << endl;
                    cout << "                {"                                    << endl;
                    cout << "                   \"stat\": "    << stat2     << "," << endl;
                    cout << "                   \"ammount\": " << int32(in) << "," << endl;
                    cout << "                }"                                    << endl;
                    cout << "            ],"                                       << endl;
                    cout << "            \"delayed\": ["                            << endl;
                    cout << "                {"                                     << endl;
                    cout << "                   \"duration\": " << int32(in) << "," << endl;
                    cout << "                   \"ammount\": ["                     << endl;
                    cout << "                       " << int32(in) << ","           << endl;
                    cout << "                       " << int32(in) << ","           << endl;
                    cout << "                       " << int32(in)                  << endl;
                    cout << "                   ]"                                  << endl;
                    cout << "                },"                                    << endl;
                    cout << "                {"                                     << endl;
                    cout << "                   \"duration\": " << int32(in) << "," << endl;
                    cout << "                   \"ammount\": ["                     << endl;
                    cout << "                       " << int32(in) << ","           << endl;
                    cout << "                       " << int32(in) << ","           << endl;
                    cout << "                       " << int32(in)                  << endl;
                    cout << "                   ]"                                  << endl;
                    cout << "                }"                                     << endl;
                    cout << "            ],"                                        << endl;
                    cout << "            \"addiction\": {"                          << endl;
                    cout << "                \"chance\": "  << int32(in) << ","     << endl;
                    cout << "                \"perk\": "    << int32(in) << ","     << endl;
                    cout << "                \"timeout\": " << int32(in)            << endl;
                    cout << "            }"                                         << endl;
                    cout << "        }"                                             << endl;
                    break;
                }
                case 3: // weapon
                {
                    cout << "    \"animationCode\": " << int32(in) << "," << endl;
                    cout << "    \"damageMin\": " << int32(in) << "," << endl;
                    cout << "    \"damageMax\": " << int32(in) << "," << endl;
                    cout << "    \"damageType\": " << int32(in) << "," << endl;
                    cout << "    \"primaryRange\": " << int32(in) << "," << endl;
                    cout << "    \"secondaryRange\": " << int32(in) << "," << endl;
                    cout << "    \"projPID\": " << int32(in) << "," << endl;
                    cout << "    \"requiredStrength\": " << int32(in) << "," << endl;
                    cout << "    \"primaryActionPoints\": " << int32(in) << "," << endl;
                    cout << "    \"secondaryActionPoints\": " << int32(in) << "," << endl;
                    cout << "    \"critFail\": " << int32(in) << "," << endl;
                    cout << "    \"perk\": " << int32(in) << "," << endl;
                    cout << "    \"burstRounds\": " << int32(in) << "," << endl;
                    cout << "    \"caliber\": " << int32(in) << "," << endl;
                    cout << "    \"ammoPID\": " << int32(in) << "," << endl;
                    cout << "    \"maxAmmo\": " << int32(in) << "," << endl;
                    cout << "    \"soundId\": " << (int) uint8(in) << endl;
                    break;
                }
                case 4: // ammo
                {
                    cout << "    \"caliber\": " << int32(in) << "," << endl;
                    cout << "    \"quantity\": " << int32(in) << "," << endl;
                    cout << "    \"ac_modifier\": " << int32(in) << "," << endl;
                    cout << "    \"dr_modifier\": " << int32(in) << "," << endl;
                    cout << "    \"damMult\": " << int32(in) << "," << endl;
                    cout << "    \"damDiv\": " << int32(in) << endl;
                    break;
                }
                case 5: // misc
                {
                    cout << "    \"powerPID\": " << int32(in) << "," << endl;
                    cout << "    \"powerType\": " << int32(in) << "," << endl;
                    cout << "    \"charges\": " << int32(in) << endl;
                    break;
                }
                case 6: // key
                {
                    cout << "    \"unknown\": " << int32(in) << endl;
                    break;
                }
            }
            cout << "    }" << endl;
            break;
        }
        case 1:
        {
            // critters
            cout << "    \"critterFlags\": {" << endl;
            uint32_t flags = uint32(in);
            cout << "        \"0x2000\": " << (flags&0x00002000 ? "true" : "false") << "," << endl;
            cout << "        \"0x4000\": " << (flags&0x00004000 ? "true" : "false") << endl;
            cout << "    }," << endl;

            int32_t SID = int32(in);

            if (SID > 0)
            {
                cout << "    \"SID\": {" << endl;
                cout << "        \"type\": " << ((SID&0XFF000000) >> 24) << "," << endl;
                cout << "        \"id\":" << (SID&0x00FFFFFF) << endl;
                cout << "    }," << endl;
            }
            else
            {
                cout << "    \"SID\": false," << endl;
            }

            int32_t FID = int32(in);
            if (FID > 0)
            {
                cout << "    \"headFID\": {" << endl;
                cout << "        \"type\": " << ((FID&0XFF000000) >> 24) << "," << endl;
                cout << "        \"id\": " << (FID&0x00FFFFFF) << endl;
                cout << "    }," << endl;
            }
            else
            {
                cout << "    \"headFID\": false," << endl;
            }
            cout << "    \"aiPacket\": " << int32(in) << "," << endl;
            cout << "    \"teamNumber\": " << int32(in) << "," << endl;
            flags = uint32(in);
            cout << "    \"critterFlags2\": {" << endl;
            cout << "        \"CAN_TRADE\": "      << (flags&0x00000002 ? "true" : "false") << "," << endl;
            cout << "        \"CANT_STEAL\": "     << (flags&0x00000020 ? "true" : "false") << "," << endl;
            cout << "        \"NO_DROP\": "        << (flags&0x00000040 ? "true" : "false") << "," << endl;
            cout << "        \"NO_LIMBS\": "       << (flags&0x00000080 ? "true" : "false") << "," << endl;
            cout << "        \"NO_AGE\": "         << (flags&0x00000100 ? "true" : "false") << "," << endl;
            cout << "        \"NO_HEAL\": "        << (flags&0x00000200 ? "true" : "false") << "," << endl;
            cout << "        \"NO_DAMAGE\": "      << (flags&0x00000400 ? "true" : "false") << "," << endl;
            cout << "        \"NO_BODY\": "        << (flags&0x00000800 ? "true" : "false") << "," << endl;
            cout << "        \"SPECIAL_DEATH\": "  << (flags&0x00001000 ? "true" : "false") << "," << endl;
            cout << "        \"RANGE\": "          << (flags&0x00002000 ? "true" : "false") << "," << endl;
            cout << "        \"NO_KNOCKDOWN\": "   << (flags&0x00004000 ? "true" : "false") << endl;
            cout << "    }," << endl;
            cout << "    \"stats\": {" << endl;
            cout << "        \"strength\": " << int32(in) << "," << endl;
            cout << "        \"perception\": " << int32(in) << "," << endl;
            cout << "        \"endurance\": " << int32(in) << "," << endl;
            cout << "        \"charisma\": " << int32(in) << "," << endl;
            cout << "        \"intelligence\": " << int32(in) << "," << endl;
            cout << "        \"agility\": " << int32(in) << "," << endl;
            cout << "        \"luck\": " << int32(in) << "," << endl;
            cout << "        \"hp\": " << int32(in) << "," << endl;
            cout << "        \"ap\": " << int32(in) << "," << endl;
            cout << "        \"ac\": " << int32(in) << "," << endl;
            cout << "        \"damageUnarmed\": " << int32(in) << "," << endl;
            cout << "        \"damageMelee\": " << int32(in) << "," << endl;
            cout << "        \"carryWeight\": " << int32(in) << "," << endl;
            cout << "        \"sequence\": " << int32(in) << "," << endl;
            cout << "        \"healingRate\": " << int32(in) << "," << endl;
            cout << "        \"criticalChance\": " << int32(in) << "," << endl;
            cout << "        \"betterCriticals\": " << int32(in) << endl;
            cout << "    }," << endl;
            cout << "    \"damageThreshold\": {" << endl;
            cout << "        \"normal\": " << int32(in) << "," << endl;
            cout << "        \"laser\": " << int32(in) << "," << endl;
            cout << "        \"fire\": " << int32(in) << "," << endl;
            cout << "        \"plasma\": " << int32(in) << "," << endl;
            cout << "        \"electrical\": " << int32(in) << "," << endl;
            cout << "        \"emp\": " << int32(in) << "," << endl;
            cout << "        \"explosive\": " << int32(in) << endl;
            cout << "    }," << endl;
            cout << "    \"damageResist\": {" << endl;
            cout << "        \"normal\": " << int32(in) << "," << endl;
            cout << "        \"laser\": " << int32(in) << "," << endl;
            cout << "        \"fire\": " << int32(in) << "," << endl;
            cout << "        \"plasma\": " << int32(in) << "," << endl;
            cout << "        \"electrical\": " << int32(in) << "," << endl;
            cout << "        \"emp\": " << int32(in) << "," << endl;
            cout << "        \"explosive\": " << int32(in) << "," << endl;
            cout << "        \"radiation\": " << int32(in) << "," << endl;
            cout << "        \"poison\": " << int32(in) << endl;
            cout << "    }," << endl;
            cout << "    \"age\": " << int32(in) << "," << endl;
            cout << "    \"gender\": " << int32(in) << "," << endl;
            cout << "    \"statsBonus\": {" << endl;
            cout << "        \"strength\": " << int32(in) << "," << endl;
            cout << "        \"perception\": " << int32(in) << "," << endl;
            cout << "        \"endurance\": " << int32(in) << "," << endl;
            cout << "        \"charisma\": " << int32(in) << "," << endl;
            cout << "        \"intelligence\": " << int32(in) << "," << endl;
            cout << "        \"agility\": " << int32(in) << "," << endl;
            cout << "        \"luck\": " << int32(in) << "," << endl;
            cout << "        \"hp\": " << int32(in) << "," << endl;
            cout << "        \"ap\": " << int32(in) << "," << endl;
            cout << "        \"ac\": " << int32(in) << "," << endl;
            cout << "        \"damageUnarmed\": " << int32(in) << "," << endl;
            cout << "        \"damageMelee\": " << int32(in) << "," << endl;
            cout << "        \"carryWeight\": " << int32(in) << "," << endl;
            cout << "        \"sequence\": " << int32(in) << "," << endl;
            cout << "        \"healingRate\": " << int32(in) << "," << endl;
            cout << "        \"criticalChance\": " << int32(in) << "," << endl;
            cout << "        \"betterCriticals\": " << int32(in) << endl;
            cout << "    }," << endl;
            cout << "    \"damageThresholdBonus\": {" << endl;
            cout << "        \"normal\": " << int32(in) << "," << endl;
            cout << "        \"laser\": " << int32(in) << "," << endl;
            cout << "        \"fire\": " << int32(in) << "," << endl;
            cout << "        \"plasma\": " << int32(in) << "," << endl;
            cout << "        \"electrical\": " << int32(in) << "," << endl;
            cout << "        \"emp\": " << int32(in) << "," << endl;
            cout << "        \"explosive\": " << int32(in) << endl;
            cout << "    }," << endl;
            cout << "    \"damageResistBonus\": {" << endl;
            cout << "        \"normal\": " << int32(in) << "," << endl;
            cout << "        \"laser\": " << int32(in) << "," << endl;
            cout << "        \"fire\": " << int32(in) << "," << endl;
            cout << "        \"plasma\": " << int32(in) << "," << endl;
            cout << "        \"electrical\": " << int32(in) << "," << endl;
            cout << "        \"emp\": " << int32(in) << "," << endl;
            cout << "        \"explosive\": " << int32(in) << "," << endl;
            cout << "        \"radiation\": " << int32(in) << "," << endl;
            cout << "        \"poison\": " << int32(in) << endl;
            cout << "    }," << endl;
            cout << "    \"ageBonus\": " << int32(in) << "," << endl;
            cout << "    \"genderBonus\": " << int32(in) << "," << endl;
            cout << "    \"skills\": {" << endl;
            cout << "        \"smallGuns\": " << int32(in) << "," << endl;
            cout << "        \"bigGuns\": " << int32(in) << "," << endl;
            cout << "        \"energyWeapons\": " << int32(in) << "," << endl;
            cout << "        \"unarmed\": " << int32(in) << "," << endl;
            cout << "        \"melee\": " << int32(in) << "," << endl;
            cout << "        \"throwing\": " << int32(in) << "," << endl;
            cout << "        \"firstAid\": " << int32(in) << "," << endl;
            cout << "        \"doctor\": " << int32(in) << "," << endl;
            cout << "        \"sneak\": " << int32(in) << "," << endl;
            cout << "        \"lockpick\": " << int32(in) << "," << endl;
            cout << "        \"steal\": " << int32(in) << "," << endl;
            cout << "        \"traps\": " << int32(in) << "," << endl;
            cout << "        \"science\": " << int32(in) << "," << endl;
            cout << "        \"repair\": " << int32(in) << "," << endl;
            cout << "        \"speech\": " << int32(in) << "," << endl;
            cout << "        \"barter\": " << int32(in) << "," << endl;
            cout << "        \"gambling\": " << int32(in) << "," << endl;
            cout << "        \"outdoorsman\": " << int32(in) << endl;
            cout << "    }," << endl;
            cout << "    \"bodyType\": " << int32(in) << "," << endl;
            cout << "    \"experience\": " << int32(in) << "," << endl;
            cout << "    \"killType\": " << int32(in) << "," << endl;
            cout << "    \"damageType\": " << int32(in) << endl;
            break;
        }
        case 2: // scenery
        {
            uint32_t flags = uint32(in);

            uint16_t lightFlags = flags >> 16;

            cout << "    \"lightFlags\": {" << endl;
            cout << "        \"NORTH_SOUTH\": "  << (lightFlags == 0 ? "true" : "false") << "," << endl;
            cout << "        \"EAST_WEST\": "    << (lightFlags == 0x0800 ? "true" : "false") << "," << endl;
            cout << "        \"NORTH_CORNER\": " << (lightFlags == 0x1000 ? "true" : "false") << "," << endl;
            cout << "        \"SOUTH_CORNER\": " << (lightFlags == 0x2000 ? "true" : "false") << "," << endl;
            cout << "        \"EAST_CORNER\": "  << (lightFlags == 0x4000 ? "true" : "false") << "," << endl;
            cout << "        \"WEST_CORNER\": "  << (lightFlags == 0x8000 ? "true" : "false") << endl;
            cout << "    }," << endl;

            cout << "    \"actionFlags\": {" << endl;
            cout << "        \"KNEEL_DOWN\": " << (flags & 0x0001 ? "true" : "false") << "," << endl;
            cout << "        \"CAN_USE\": "    << (flags & 0x0008 ? "true" : "false") << "," << endl;
            cout << "        \"CAN_USE_ON\": " << (flags & 0x0010 ? "true" : "false") << "," << endl;
            cout << "        \"LOOK\": "       << (flags & 0x0020 ? "true" : "false") << "," << endl;
            cout << "        \"TALK\": "       << (flags & 0x0040 ? "true" : "false") << "," << endl;
            cout << "        \"PICK_UP\": "    << (flags & 0x0080 ? "true" : "false") << endl;
            cout << "    }," << endl;

            int32_t SID = int32(in);
            if (SID > 0)
            {
                cout << "    \"SID\": {" << endl;
                cout << "        \"type\": " << ((SID&0XFF000000) >> 24) << "," << endl;
                cout << "        \"id\":" << (SID&0x00FFFFFF) << endl;
                cout << "    }," << endl;
            }
            else
            {
                cout << "    \"SID\": false," << endl;
            }

            uint32_t subtype = uint32(in);
            cout << "    \"subtype\": " << subtype << "," << endl;
            cout << "    \"materialId\": " << int32(in) << "," << endl;
            cout << "    \"soundId\": " << (int) uint8(in) << "," << endl;

            switch(subtype)
            {
                case 0: // door

                    cout << "    \"walkThru\": " << (uint32(in) & 0x0000000F ? "true" : "false") << "," << endl;
                    cout << "    \"unknown\": " << int32(in)  << endl;
                    break;
                case 1: // stairs
                {
                    int32_t destination = int32(in);
                    if (destination > 0)
                    {
                        unsigned elevation = 0;
                        if (destination & 0x02000000) elevation = 1;
                        if (destination & 0x04000000) elevation = 2;
                        //unsigned orientation = (0x00FF0000 & destination) >> 16;
                        unsigned hexagon = 0x0000FFFF & destination;
                        cout << "    \"destination\": {" << endl;
                        cout << "        \"elevation\": " << elevation << "," << endl;
                        // @todo: test if this is orientation
                        //cout << "        \"orientation\": " << orientation << "," << endl;
                        cout << "        \"hexagon\":" << hexagon << endl;
                        cout << "    }," << endl;
                        break;
                    }
                    else
                    {
                        cout << "    \"destination\": false," << endl;
                    }
                    cout << "    \"map\": " << int32(in) << endl;
                    break;
                }
                case 2: // elevator
                    cout << "    \"elevatorType\": " << int32(in) << "," << endl;
                    cout << "    \"elevatorLevel\": " << int32(in) << endl;
                    break;
                case 3: // ladder bottom
                case 4: // ladder top
                {
                    int32_t destination = int32(in);
                    if (destination > 0)
                    {
                        unsigned elevation = 0;
                        if (destination & 0x02000000) elevation = 1;
                        if (destination & 0x04000000) elevation = 2;
                        //unsigned orientation = (0x00FF0000 & destination) >> 16;
                        unsigned hexagon = 0x0000FFFF & destination;
                        cout << "    \"destination\": {" << endl;
                        cout << "        \"elevation\": " << elevation << "," << endl;
                        // @todo: test if this is orientation
                        //cout << "        \"orientation\": " << orientation << "," << endl;
                        cout << "        \"hexagon\":" << hexagon << endl;
                        cout << "    }" << endl;
                        break;
                    }
                    else
                    {
                        cout << "    \"destination\": false" << endl;
                    }
                }
                case 5: // generic scenery
                    cout << "    \"unknown\": " << int32(in) << endl;
                    break;
            }


            break;
        }
        case 3: // walls
        {
            uint32_t flags = uint32(in);

            uint16_t lightFlags = flags >> 16;

            cout << "    \"lightFlags\": {" << endl;
            cout << "        \"NORTH_SOUTH\": "  << (lightFlags == 0 ? "true" : "false") << "," << endl;
            cout << "        \"EAST_WEST\": "    << (lightFlags == 0x0800 ? "true" : "false") << "," << endl;
            cout << "        \"NORTH_CORNER\": " << (lightFlags == 0x1000 ? "true" : "false") << "," << endl;
            cout << "        \"SOUTH_CORNER\": " << (lightFlags == 0x2000 ? "true" : "false") << "," << endl;
            cout << "        \"EAST_CORNER\": "  << (lightFlags == 0x4000 ? "true" : "false") << "," << endl;
            cout << "        \"WEST_CORNER\": "  << (lightFlags == 0x8000 ? "true" : "false") << endl;
            cout << "    }," << endl;

            cout << "    \"actionFlags\": {" << endl;
            cout << "        \"KNEEL_DOWN\": " << (flags & 0x0001 ? "true" : "false") << "," << endl;
            cout << "        \"CAN_USE\": "    << (flags & 0x0008 ? "true" : "false") << "," << endl;
            cout << "        \"CAN_USE_ON\": " << (flags & 0x0010 ? "true" : "false") << "," << endl;
            cout << "        \"LOOK\": "       << (flags & 0x0020 ? "true" : "false") << "," << endl;
            cout << "        \"TALK\": "       << (flags & 0x0040 ? "true" : "false") << "," << endl;
            cout << "        \"PICK_UP\": "    << (flags & 0x0080 ? "true" : "false") << endl;
            cout << "    }," << endl;


            int32_t SID = int32(in);
            if (SID > 0)
            {
                cout << "    \"SID\": {" << endl;
                cout << "        \"type\": " << ((SID&0XFF000000) >> 24) << "," << endl;
                cout << "        \"id\":" << (SID&0x00FFFFFF) << endl;
                cout << "    }," << endl;
            }
            else
            {
                cout << "    \"SID\": false," << endl;
            }
            cout << "    \"materialId\": " << int32(in) << endl;

            break;
        }
        case 4: // tiles
        {
            cout << "    \"materialId\": " << int32(in) << endl;
            break;
        }
        case 5: // misc
            break;
    }


    cout << "}" << endl;

    in.close();
    return 0;
}
