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

uint32_t readUINT32(std::ifstream& stream)
{
    uint32_t value;
    char* buff = reinterpret_cast<char*>(&value);
    stream.read(buff, 4);
    std::reverse(buff, buff + 4);
    return value;
}

int32_t readINT32(std::ifstream& stream)
{
    return readUINT32(stream);
}


uint8_t readUINT8(std::ifstream& stream)
{
    uint8_t value;
    char* buff = reinterpret_cast<char*>(&value);
    stream.read(buff, 1);
    return value;
}

void usage(std::string binaryName)
{
    std::cout << "PRO to JSON converter v0.0.2" << std::endl;
    std::cout << "Copyright (c) 2015 Falltergeist developers" << std::endl;
    std::cout << "Usage: " << binaryName << " <PRO filename>" << std::endl;
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
        std::cout << "Can't open file: " << filename << std::endl;
        return 1;
    }

    std::cout << "{" << std::endl;

    uint32_t PID = readUINT32(in);
    uint32_t type = (PID & 0xFF000000) >> 24;
    std::cout << "    \"PID\": {" << std::endl;
    std::cout << "        \"type\": " << type << "," << std::endl;
    std::cout << "        \"id\": " << (PID & 0x00FFFFFF) << std::endl;
    std::cout << "    }," << std::endl;
    std::cout << "    \"textId\": " << readUINT32(in) << "," <<std::endl;
    uint32_t FID = readUINT32(in);
    std::cout << "    \"FID\": {" << std::endl;
    std::cout << "        \"type\": " << ((FID & 0xFF000000) >> 24) << "," << std::endl;
    std::cout << "        \"id\": " << (FID & 0x00FFFFFF) << std::endl;
    std::cout << "    }," << std::endl;
    std::cout << "    \"lightRadius\": " << readUINT32(in) << "," <<std::endl;
    std::cout << "    \"lightIntencity\": " << readUINT32(in) << "," <<std::endl;
    uint32_t flags = readUINT32(in);
    std::cout << "    \"flags\": {" << std::endl;
    std::cout << "        \"FLAT\": "         << (flags&0x00000008 ? "true" : "false") << "," << std::endl;
    std::cout << "        \"NO_BLOCK\": "     << (flags&0x00000010 ? "true" : "false") << "," << std::endl;
    std::cout << "        \"MULTIHEX\": "     << (flags&0x00000800 ? "true" : "false") << "," << std::endl;
    std::cout << "        \"NO_HIGHLIGHT\": " << (flags&0x00001000 ? "true" : "false") << "," << std::endl;
    std::cout << "        \"TRANS_RED\": "    << (flags&0x00004000 ? "true" : "false") << "," << std::endl;
    std::cout << "        \"TRANS_NONE\": "   << (flags&0x00008000 ? "true" : "false") << "," << std::endl;
    std::cout << "        \"TRANS_WALL\": "   << (flags&0x00010000 ? "true" : "false") << "," << std::endl;
    std::cout << "        \"TRANS_GLASS\": "  << (flags&0x00020000 ? "true" : "false") << "," << std::endl;
    std::cout << "        \"TRANS_STREAM\": " << (flags&0x00080000 ? "true" : "false") << "," << std::endl;
    std::cout << "        \"TRANS_ENERGY\": " << (flags&0x00040000 ? "true" : "false") << "," << std::endl;
    std::cout << "        \"NO_EGG\": "       << (flags&0x10000000 ? "true" : "false") << "," << std::endl;
    std::cout << "        \"LIGHT_THRU\": "   << (flags&0x20000000 ? "true" : "false") << "," << std::endl;
    std::cout << "        \"SHOOT_THRU\": "   << (flags&0x80000000 ? "true" : "false") << std::endl;
    std::cout << "    }," <<std::endl;

    switch(type)
    {
        case 0: // items
        {
            std::cout << "    \"itemFlags\": {" << std::endl;
            uint32_t flags = readUINT32(in);
            std::cout << "        \"HIDDEN\": "            << (flags&0x08000000 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"CAN_USE\": "           << (flags&0x00000800 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"CAN_USE_ON\": "        << (flags&0x00001000 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"CAN_PICK_UP\": "       << (flags&0x00008000 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"WEAPON_BIG\": "        << (flags&0x00000100 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"WEAPON_TWO_HANDED\": " << (flags&0x00000200 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"WEAPON_ATTACK_1\": "   << (flags&0x0000000F) << "," << std::endl;
            std::cout << "        \"WEAPON_ATTACK_2\": "   << ((flags&0x000000F0) >> 4) << std::endl;
            std::cout << "    }," << std::endl;

            int32_t SID = readINT32(in);

            if (SID > 0)
            {
                std::cout << "    \"SID\": {" << std::endl;
                std::cout << "        \"type\": " << ((SID&0XFF000000) >> 24) << "," << std::endl;
                std::cout << "        \"id\":" << (SID&0x00FFFFFF) << std::endl;
                std::cout << "    }," << std::endl;
            }
            else
            {
                std::cout << "    \"SID\": false," << std::endl;
            }

            uint32_t subtype = readUINT32(in);
            std::cout << "    \"subtype\": " << subtype << "," << std::endl;
            std::cout << "    \"materialId\": " << readUINT32(in) << "," << std::endl;
            std::cout << "    \"inventorySize\": " << readUINT32(in) << "," << std::endl;
            std::cout << "    \"weight\": " << readUINT32(in) << "," << std::endl;
            std::cout << "    \"cost\": " << readUINT32(in) << "," << std::endl;
            uint32_t FID = readUINT32(in);
            std::cout << "    \"inventoryFID\": {" << std::endl;
            std::cout << "        \"type\": " << ((FID&0XFF000000) >> 24) << "," << std::endl;
            std::cout << "        \"id\": " << (FID&0x00FFFFFF) << std::endl;
            std::cout << "    }," << std::endl;
            std::cout << "    \"soundId\": " << (int) readUINT8(in) << "," << std::endl;

            switch(subtype)
            {
                case 0: // armor
                {
                    std::cout << "    \"armorClass\": " << readUINT32(in) << "," << std::endl;
                    std::cout << "    \"resistance\": {" << std::endl;
                    std::cout << "        \"normal\": " << readUINT32(in) << "," << std::endl;
                    std::cout << "        \"laser\": " << readUINT32(in) << "," << std::endl;
                    std::cout << "        \"fire\": " << readUINT32(in) << "," << std::endl;
                    std::cout << "        \"plasma\": " << readUINT32(in) << "," << std::endl;
                    std::cout << "        \"electrical\": " << readUINT32(in) << "," << std::endl;
                    std::cout << "        \"emp\": " << readUINT32(in) << "," << std::endl;
                    std::cout << "        \"explosion\": " << readUINT32(in) << std::endl;
                    std::cout << "    }," << std::endl;
                    std::cout << "    \"threshold\": {" << std::endl;
                    std::cout << "        \"normal\": " << readUINT32(in) << "," << std::endl;
                    std::cout << "        \"laser\": " << readUINT32(in) << "," << std::endl;
                    std::cout << "        \"fire\": " << readUINT32(in) << "," << std::endl;
                    std::cout << "        \"plasma\": " << readUINT32(in) << "," << std::endl;
                    std::cout << "        \"electrical\": " << readUINT32(in) << "," << std::endl;
                    std::cout << "        \"emp\": " << readUINT32(in) << "," << std::endl;
                    std::cout << "        \"explosion\": " << readUINT32(in) << std::endl;
                    std::cout << "    }," << std::endl;

                    int32_t perk = readINT32(in);

                    std::cout << "    \"perk\": " << (perk > 0 ? std::to_string(perk) : "false") << "," << std::endl;

                    FID = readUINT32(in);
                    std::cout << "    \"maleFID\": {" << std::endl;
                    std::cout << "        \"type\": " << ((FID&0XFF000000) >> 24) << "," << std::endl;
                    std::cout << "        \"id\": " << (FID&0x00FFFFFF) << std::endl;
                    std::cout << "    }," << std::endl;

                    FID = readUINT32(in);
                    std::cout << "    \"femaleFID\": {" << std::endl;
                    std::cout << "        \"type\": " << ((FID&0XFF000000) >> 24) << "," << std::endl;
                    std::cout << "        \"id\": " << (FID&0x00FFFFFF) << std::endl;
                    std::cout << "    }" << std::endl;


                    break;
                }
                case 1: // container
                {
                    std::cout << "    \"containerCapacity\": " << readUINT32(in) << "," << std::endl;
                    uint32_t flags = readUINT32(in);
                    std::cout << "    \"containerFlags\": {" << std::endl;
                    std::cout << "        \"CANT_PICK_UP\": "       << (flags&0x00000001 ? "true" : "false") << "," << std::endl;
                    std::cout << "        \"MAGIC_HANDS_GROUND\": " << (flags&0x00000008 ? "true" : "false") << std::endl;
                    std::cout << "    }" << std::endl;
                    break;
                }
                case 2: // drug
                {
                    std::cout << "    \"stat0\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"stat1\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"stat2\": " << readINT32(in) << "," << std::endl;
                    // instant effect
                    std::cout << "    \"amount0\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"amount1\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"amount2\": " << readINT32(in) << "," << std::endl;
                    // first delayed effect
                    std::cout << "    \"delayed_1_duration\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"delayed_1_amount0\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"delayed_1_amount1\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"delayed_1_amount2\": " << readINT32(in) << "," << std::endl;
                    // second delayed effect
                    std::cout << "    \"delayed_2_duration\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"delayed_2_amount0\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"delayed_2_amount1\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"delayed_2_amount2\": " << readINT32(in) << "," << std::endl;

                    std::cout << "    \"addictionRate\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"addictionPerk\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"addictionOnset\": " << readINT32(in) << std::endl;
                    break;
                }
                case 3: // weapon
                {
                    std::cout << "    \"animationCode\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"damageMin\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"damageMax\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"damageType\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"primaryRange\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"secondaryRange\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"projPID\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"requiredStrength\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"primaryActionPoints\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"secondaryActionPoints\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"critFail\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"perk\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"burstRounds\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"caliber\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"ammoPID\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"maxAmmo\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"soundId\": " << (int) readUINT8(in) << std::endl;
                    break;
                }
                case 4: // ammo
                {
                    std::cout << "    \"caliber\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"quantity\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"ac_modifier\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"dr_modifier\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"damMult\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"damDiv\": " << readINT32(in) << std::endl;
                    break;
                }
                case 5: // misc
                {
                    std::cout << "    \"powerPID\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"powerType\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"charges\": " << readINT32(in) << std::endl;
                    break;
                }
                case 6: // key
                {
                    std::cout << "    \"unknown\": " << readINT32(in) << std::endl;
                    break;
                }
            }
            break;
        }
        case 1:
        {
            // critters
            std::cout << "    \"critterFlags\": {" << std::endl;
            uint32_t flags = readUINT32(in);
            std::cout << "        \"0x2000\": " << (flags&0x00002000 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"0x4000\": " << (flags&0x00004000 ? "true" : "false") << std::endl;
            std::cout << "    }," << std::endl;

            int32_t SID = readINT32(in);

            if (SID > 0)
            {
                std::cout << "    \"SID\": {" << std::endl;
                std::cout << "        \"type\": " << ((SID&0XFF000000) >> 24) << "," << std::endl;
                std::cout << "        \"id\":" << (SID&0x00FFFFFF) << std::endl;
                std::cout << "    }," << std::endl;
            }
            else
            {
                std::cout << "    \"SID\": false," << std::endl;
            }

            int32_t FID = readINT32(in);
            if (FID > 0)
            {
                std::cout << "    \"headFID\": {" << std::endl;
                std::cout << "        \"type\": " << ((FID&0XFF000000) >> 24) << "," << std::endl;
                std::cout << "        \"id\": " << (FID&0x00FFFFFF) << std::endl;
                std::cout << "    }," << std::endl;
            }
            else
            {
                std::cout << "    \"headFID\": false," << std::endl;
            }
            std::cout << "    \"aiPacket\": " << readINT32(in) << "," << std::endl;
            std::cout << "    \"teamNumber\": " << readINT32(in) << "," << std::endl;
            flags = readUINT32(in);
            std::cout << "    \"critterFlags2\": {" << std::endl;
            std::cout << "        \"CAN_TRADE\": "      << (flags&0x00000002 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"CANT_STEAL\": "     << (flags&0x00000020 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"NO_DROP\": "        << (flags&0x00000040 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"NO_LIMBS\": "       << (flags&0x00000080 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"NO_AGE\": "         << (flags&0x00000100 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"NO_HEAL\": "        << (flags&0x00000200 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"NO_DAMAGE\": "      << (flags&0x00000400 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"NO_BODY\": "        << (flags&0x00000800 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"SPECIAL_DEATH\": "  << (flags&0x00001000 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"RANGE\": "          << (flags&0x00002000 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"NO_KNOCKDOWN\": "   << (flags&0x00004000 ? "true" : "false") << std::endl;
            std::cout << "    }," << std::endl;
            std::cout << "    \"stats\": {" << std::endl;
            std::cout << "        \"strength\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"perception\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"endurance\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"charisma\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"intelligence\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"agility\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"luck\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"hp\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"ap\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"ac\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"damageUnarmed\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"damageMelee\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"carryWeight\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"sequence\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"healingRate\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"criticalChance\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"betterCriticals\": " << readINT32(in) << std::endl;
            std::cout << "    }," << std::endl;
            std::cout << "    \"damageThreshold\": {" << std::endl;
            std::cout << "        \"normal\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"laser\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"fire\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"plasma\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"electrical\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"emp\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"explosive\": " << readINT32(in) << std::endl;
            std::cout << "    }," << std::endl;
            std::cout << "    \"damageResist\": {" << std::endl;
            std::cout << "        \"normal\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"laser\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"fire\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"plasma\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"electrical\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"emp\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"explosive\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"radiation\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"poison\": " << readINT32(in) << std::endl;
            std::cout << "    }," << std::endl;
            std::cout << "    \"age\": " << readINT32(in) << "," << std::endl;
            std::cout << "    \"gender\": " << readINT32(in) << "," << std::endl;
            std::cout << "    \"statsBonus\": {" << std::endl;
            std::cout << "        \"strength\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"perception\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"endurance\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"charisma\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"intelligence\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"agility\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"luck\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"hp\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"ap\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"ac\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"damageUnarmed\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"damageMelee\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"carryWeight\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"sequence\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"healingRate\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"criticalChance\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"betterCriticals\": " << readINT32(in) << std::endl;
            std::cout << "    }," << std::endl;
            std::cout << "    \"damageThresholdBonus\": {" << std::endl;
            std::cout << "        \"normal\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"laser\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"fire\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"plasma\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"electrical\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"emp\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"explosive\": " << readINT32(in) << std::endl;
            std::cout << "    }," << std::endl;
            std::cout << "    \"damageResistBonus\": {" << std::endl;
            std::cout << "        \"normal\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"laser\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"fire\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"plasma\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"electrical\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"emp\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"explosive\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"radiation\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"poison\": " << readINT32(in) << std::endl;
            std::cout << "    }," << std::endl;
            std::cout << "    \"ageBonus\": " << readINT32(in) << "," << std::endl;
            std::cout << "    \"genderBonus\": " << readINT32(in) << "," << std::endl;
            std::cout << "    \"skills\": {" << std::endl;
            std::cout << "        \"smallGuns\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"bigGuns\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"energyWeapons\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"unarmed\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"melee\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"throwing\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"firstAid\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"doctor\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"sneak\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"lockpick\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"steal\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"traps\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"science\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"repair\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"speech\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"barter\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"gambling\": " << readINT32(in) << "," << std::endl;
            std::cout << "        \"outdoorsman\": " << readINT32(in) << std::endl;
            std::cout << "    }," << std::endl;
            std::cout << "    \"bodyType\": " << readINT32(in) << "," << std::endl;
            std::cout << "    \"experience\": " << readINT32(in) << "," << std::endl;
            std::cout << "    \"killType\": " << readINT32(in) << "," << std::endl;
            std::cout << "    \"damageType\": " << readINT32(in) << std::endl;
            break;
        }
        case 2: // scenery
        {
            uint32_t flags = readUINT32(in);

            uint16_t lightFlags = flags >> 16;

            std::cout << "    \"lightFlags\": {" << std::endl;
            std::cout << "        \"NORTH_SOUTH\": "  << (lightFlags == 0 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"EAST_WEST\": "    << (lightFlags == 0x0800 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"NORTH_CORNER\": " << (lightFlags == 0x1000 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"SOUTH_CORNER\": " << (lightFlags == 0x2000 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"EAST_CORNER\": "  << (lightFlags == 0x4000 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"WEST_CORNER\": "  << (lightFlags == 0x8000 ? "true" : "false") << std::endl;
            std::cout << "    }," << std::endl;

            std::cout << "    \"actionFlags\": {" << std::endl;
            std::cout << "        \"KNEEL_DOWN\": " << (flags & 0x0001 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"CAN_USE\": "    << (flags & 0x0008 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"CAN_USE_ON\": " << (flags & 0x0010 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"LOOK\": "       << (flags & 0x0020 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"TALK\": "       << (flags & 0x0040 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"PICK_UP\": "    << (flags & 0x0080 ? "true" : "false") << std::endl;
            std::cout << "    }," << std::endl;

            int32_t SID = readINT32(in);
            if (SID > 0)
            {
                std::cout << "    \"SID\": {" << std::endl;
                std::cout << "        \"type\": " << ((SID&0XFF000000) >> 24) << "," << std::endl;
                std::cout << "        \"id\":" << (SID&0x00FFFFFF) << std::endl;
                std::cout << "    }," << std::endl;
            }
            else
            {
                std::cout << "    \"SID\": false," << std::endl;
            }

            uint32_t subtype = readUINT32(in);
            std::cout << "    \"subtype\": " << subtype << "," << std::endl;
            std::cout << "    \"materialId\": " << readINT32(in) << "," << std::endl;
            std::cout << "    \"soundId\": " << (int) readUINT8(in) << "," << std::endl;

            switch(subtype)
            {
                case 0: // door

                    std::cout << "    \"walkThru\": " << (readUINT32(in) & 0x0000000F ? "true" : "false") << "," << std::endl;
                    std::cout << "    \"unknown\": " << readINT32(in)  << std::endl;
                    break;
                case 1: // stairs
                {
                    int32_t destination = readINT32(in);
                    if (destination > 0)
                    {
                        unsigned elevation = 0;
                        if (destination & 0x02000000) elevation = 1;
                        if (destination & 0x04000000) elevation = 2;
                        //unsigned orientation = (0x00FF0000 & destination) >> 16;
                        unsigned hexagon = 0x0000FFFF & destination;
                        std::cout << "    \"destination\": {" << std::endl;
                        std::cout << "        \"elevation\": " << elevation << "," << std::endl;
                        // @todo: test if this is orientation
                        //std::cout << "        \"orientation\": " << orientation << "," << std::endl;
                        std::cout << "        \"hexagon\":" << hexagon << std::endl;
                        std::cout << "    }," << std::endl;
                        break;
                    }
                    else
                    {
                        std::cout << "    \"destination\": false," << std::endl;
                    }
                    std::cout << "    \"map\": " << readINT32(in) << std::endl;
                    break;
                }
                case 2: // elevator
                    std::cout << "    \"elevatorType\": " << readINT32(in) << "," << std::endl;
                    std::cout << "    \"elevatorLevel\": " << readINT32(in) << std::endl;
                    break;
                case 3: // ladder bottom
                case 4: // ladder top
                {
                    int32_t destination = readINT32(in);
                    if (destination > 0)
                    {
                        unsigned elevation = 0;
                        if (destination & 0x02000000) elevation = 1;
                        if (destination & 0x04000000) elevation = 2;
                        //unsigned orientation = (0x00FF0000 & destination) >> 16;
                        unsigned hexagon = 0x0000FFFF & destination;
                        std::cout << "    \"destination\": {" << std::endl;
                        std::cout << "        \"elevation\": " << elevation << "," << std::endl;
                        // @todo: test if this is orientation
                        //std::cout << "        \"orientation\": " << orientation << "," << std::endl;
                        std::cout << "        \"hexagon\":" << hexagon << std::endl;
                        std::cout << "    }" << std::endl;
                        break;
                    }
                    else
                    {
                        std::cout << "    \"destination\": false" << std::endl;
                    }
                }
                case 5: // generic scenery
                    std::cout << "    \"unknown\": " << readINT32(in) << std::endl;
                    break;
            }


            break;
        }
        case 3: // walls
        {
            uint32_t flags = readUINT32(in);

            uint16_t lightFlags = flags >> 16;

            std::cout << "    \"lightFlags\": {" << std::endl;
            std::cout << "        \"NORTH_SOUTH\": "  << (lightFlags == 0 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"EAST_WEST\": "    << (lightFlags == 0x0800 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"NORTH_CORNER\": " << (lightFlags == 0x1000 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"SOUTH_CORNER\": " << (lightFlags == 0x2000 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"EAST_CORNER\": "  << (lightFlags == 0x4000 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"WEST_CORNER\": "  << (lightFlags == 0x8000 ? "true" : "false") << std::endl;
            std::cout << "    }," << std::endl;

            std::cout << "    \"actionFlags\": {" << std::endl;
            std::cout << "        \"KNEEL_DOWN\": " << (flags & 0x0001 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"CAN_USE\": "    << (flags & 0x0008 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"CAN_USE_ON\": " << (flags & 0x0010 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"LOOK\": "       << (flags & 0x0020 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"TALK\": "       << (flags & 0x0040 ? "true" : "false") << "," << std::endl;
            std::cout << "        \"PICK_UP\": "    << (flags & 0x0080 ? "true" : "false") << std::endl;
            std::cout << "    }," << std::endl;


            int32_t SID = readINT32(in);
            if (SID > 0)
            {
                std::cout << "    \"SID\": {" << std::endl;
                std::cout << "        \"type\": " << ((SID&0XFF000000) >> 24) << "," << std::endl;
                std::cout << "        \"id\":" << (SID&0x00FFFFFF) << std::endl;
                std::cout << "    }," << std::endl;
            }
            else
            {
                std::cout << "    \"SID\": false," << std::endl;
            }
            std::cout << "    \"materialId\": " << readINT32(in) << std::endl;

            break;
        }
        case 4: // tiles
        {
            std::cout << "    \"materialId\": " << readINT32(in) << std::endl;
            break;
        }
        case 5: // misc
            break;
    }


    std::cout << "}" << std::endl;

    in.close();
    return 0;
}
