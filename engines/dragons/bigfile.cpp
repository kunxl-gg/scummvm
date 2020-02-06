/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "dragons/bigfile.h"

namespace Dragons {
typedef struct FileInfo {
	const char filename[16];
	uint32 offset;
	uint32 size;
} FileInfo;

#define TOTAL_FILES 576
//TODO add support for other game types. get data directly from DRAGON.EXE
FileInfo fileInfo[TOTAL_FILES] = {
		{"loadpic.tim", 0x0, 0x1F414},
		{"fntfiles.dat", 0x1F800, 0x6E44},
		{"tfont.map", 0x26800, 0xC6A},
		{"tfont.chr", 0x27800, 0x16C0},
		{"tfont1.map", 0x29000, 0xC6A},
		{"tfont1.chr", 0x2A000, 0x16C0},
		{"sfont.map", 0x2B800, 0xC6A},
		{"sfont.chr", 0x2C800, 0x16C0},
		{"boxfont.chr", 0x2E000, 0x4C0},
		{"bag.scr", 0x2E800, 0xA2E4},
		{"igfiles.dat", 0x39000, 0x29208},
		{"dragon.flg", 0x62800, 0x24},
		{"dragon.var", 0x63000, 0x1E},
		{"dragon.ini", 0x63800, 0x6028},
		{"dragon.img", 0x6A000, 0x441C},
		{"dragon.opt", 0x6E800, 0x16A0},
		{"dragon.ipt", 0x70000, 0x5D8},
		{"dragon.rms", 0x70800, 0x63C},
		{"dragon.obd", 0x71000, 0x1C9DC},
		{"dragon.txt", 0x8E000, 0x4E8A4},
		{"dragon.spt", 0xDD000, 0xD0},
		{"musx.vh", 0xDD800, 0x2E20},
		{"musx.vb", 0xE0800, 0x402A0},
		{"glob.vh", 0x121000, 0xC20},
		{"glob.vb", 0x122000, 0xBB90},
		{"glob.vab", 0x12E000, 0xC7B0},
		{"cursor.act", 0x13A800, 0x1863C},
		{"invenicn.act", 0x153000, 0x3D3C},
		{"shadow.act", 0x157000, 0x508},
		{"flicker.act", 0x157800, 0x3CDF0},
		{"flickert.act", 0x194800, 0x1A4A0},
		{"flkg.act", 0x1AF000, 0x4D298},
		{"flkspec.act", 0x1FC800, 0xE008},
		{"credits.txt", 0x20B000, 0x249A},
		{"titles.act", 0x20D800, 0x15214},
		{"drag0000.txt", 0x223000, 0x1000},
		{"drag0001.txt", 0x224000, 0x1000},
		{"drag0002.txt", 0x225000, 0x1000},
		{"drag0003.txt", 0x226000, 0x1000},
		{"drag0004.txt", 0x227000, 0x1000},
		{"drag0005.txt", 0x228000, 0x1000},
		{"drag0006.txt", 0x229000, 0x1000},
		{"drag0007.txt", 0x22A000, 0x1000},
		{"drag0008.txt", 0x22B000, 0x1000},
		{"drag0009.txt", 0x22C000, 0x1000},
		{"drag0010.txt", 0x22D000, 0x1000},
		{"drag0011.txt", 0x22E000, 0x1000},
		{"drag0012.txt", 0x22F000, 0x1000},
		{"drag0013.txt", 0x230000, 0x1000},
		{"drag0014.txt", 0x231000, 0x1000},
		{"drag0015.txt", 0x232000, 0x1000},
		{"drag0016.txt", 0x233000, 0x1000},
		{"drag0017.txt", 0x234000, 0x1000},
		{"drag0018.txt", 0x235000, 0x1000},
		{"drag0019.txt", 0x236000, 0x1000},
		{"drag0020.txt", 0x237000, 0x1000},
		{"drag0021.txt", 0x238000, 0x1000},
		{"drag0022.txt", 0x239000, 0x1000},
		{"drag0023.txt", 0x23A000, 0x1000},
		{"drag0024.txt", 0x23B000, 0x1000},
		{"drag0025.txt", 0x23C000, 0x1000},
		{"drag0026.txt", 0x23D000, 0x1000},
		{"drag0027.txt", 0x23E000, 0x1000},
		{"drag0028.txt", 0x23F000, 0x1000},
		{"drag0029.txt", 0x240000, 0x1000},
		{"drag0030.txt", 0x241000, 0x1000},
		{"drag0031.txt", 0x242000, 0x1000},
		{"drag0032.txt", 0x243000, 0x1000},
		{"drag0033.txt", 0x244000, 0x1000},
		{"drag0034.txt", 0x245000, 0x1000},
		{"drag0035.txt", 0x246000, 0x1000},
		{"drag0036.txt", 0x247000, 0x1000},
		{"drag0037.txt", 0x248000, 0x1000},
		{"drag0038.txt", 0x249000, 0x1000},
		{"drag0039.txt", 0x24A000, 0x1000},
		{"drag0040.txt", 0x24B000, 0x1000},
		{"drag0041.txt", 0x24C000, 0x1000},
		{"drag0042.txt", 0x24D000, 0x1000},
		{"drag0043.txt", 0x24E000, 0x1000},
		{"drag0044.txt", 0x24F000, 0x1000},
		{"drag0045.txt", 0x250000, 0x1000},
		{"drag0046.txt", 0x251000, 0x1000},
		{"drag0047.txt", 0x252000, 0x1000},
		{"drag0048.txt", 0x253000, 0x1000},
		{"drag0049.txt", 0x254000, 0x1000},
		{"drag0050.txt", 0x255000, 0x1000},
		{"drag0051.txt", 0x256000, 0x1000},
		{"drag0052.txt", 0x257000, 0x1000},
		{"drag0053.txt", 0x258000, 0x1000},
		{"drag0054.txt", 0x259000, 0x1000},
		{"drag0055.txt", 0x25A000, 0x1000},
		{"drag0056.txt", 0x25B000, 0x1000},
		{"drag0057.txt", 0x25C000, 0x1000},
		{"drag0058.txt", 0x25D000, 0x1000},
		{"drag0059.txt", 0x25E000, 0x1000},
		{"drag0060.txt", 0x25F000, 0x1000},
		{"drag0061.txt", 0x260000, 0x1000},
		{"drag0062.txt", 0x261000, 0x1000},
		{"drag0063.txt", 0x262000, 0x1000},
		{"drag0064.txt", 0x263000, 0x1000},
		{"drag0065.txt", 0x264000, 0x1000},
		{"drag0066.txt", 0x265000, 0x1000},
		{"drag0067.txt", 0x266000, 0x1000},
		{"drag0068.txt", 0x267000, 0x1000},
		{"drag0069.txt", 0x268000, 0x1000},
		{"drag0070.txt", 0x269000, 0x1000},
		{"drag0071.txt", 0x26A000, 0x1000},
		{"drag0072.txt", 0x26B000, 0x1000},
		{"drag0073.txt", 0x26C000, 0x1000},
		{"drag0074.txt", 0x26D000, 0x1000},
		{"drag0075.txt", 0x26E000, 0x1000},
		{"drag0076.txt", 0x26F000, 0x1000},
		{"drag0077.txt", 0x270000, 0x1000},
		{"drag0078.txt", 0x271000, 0x1000},
		{"drag0079.txt", 0x272000, 0x1000},
		{"drag0080.txt", 0x273000, 0x1000},
		{"dem1.msf", 0x274000, 0x128C0},
		{"dem1.scr", 0x287000, 0x1934C},
		{"dem11.act", 0x2A0800, 0x573FC},
		{"dem1z15.msq", 0x2F8000, 0x3A03},
		{"dem2.msf", 0x2FC000, 0x23C50},
		{"dem2.scr", 0x320000, 0x160F4},
		{"dem2z10.msq", 0x336800, 0x192B},
		{"dem2z13.msq", 0x338800, 0x3D20},
		{"demoz00.msq", 0x33C800, 0x1D7A},
		{"demoz01.msq", 0x33E800, 0x136D},
		{"demoz02.msq", 0x340000, 0x2F1D},
		{"demoz03.msq", 0x343000, 0x246A},
		{"demoz04.msq", 0x345800, 0xFA1},
		{"demoz05.msq", 0x346800, 0x61C},
		{"demoz06.msq", 0x347000, 0x3A4B},
		{"demoz07.msq", 0x34B000, 0x3909},
		{"demoz08.msq", 0x34F000, 0x25B8},
		{"demoz09.msq", 0x351800, 0x673F},
		{"demoz10.msq", 0x358000, 0x192B},
		{"demoz11.msq", 0x35A000, 0x2A5A},
		{"demoz12.msq", 0x35D000, 0xE7B},
		{"demoz13.msq", 0x35E000, 0x3D20},
		{"demoz14.msq", 0x362000, 0x1B9B},
		{"demoz15.msq", 0x364000, 0x3A03},
		{"demoz16.msq", 0x368000, 0x2B6B},
		{"demoz17.msq", 0x36B000, 0x1D7A},
		{"demoz18.msq", 0x36D000, 0x33FE},
		{"demoz19.msq", 0x370800, 0x20D7},
		{"demoz20.msq", 0x373000, 0x673F},
		{"demoz21.msq", 0x379800, 0x2EA7},
		{"demoz22.msq", 0x37C800, 0x286C},
		{"demoz23.msq", 0x37F800, 0x246A},
		{"demoz24.msq", 0x382000, 0x2463},
		{"demoz25.msq", 0x384800, 0x245B},
		{"demoz26.msq", 0x387000, 0x2AE},
		{"demoz27.msq", 0x387800, 0xB6E},
		{"demoz99.msq", 0x388800, 0x2EA7},
		{"s00b.msf", 0x38B800, 0x1CD0},
		{"s00b.scr", 0x38D800, 0xEF4},
		{"s00b1.act", 0x38E800, 0x3DE24},
		{"s00bz00.msq", 0x3CC800, 0x1D7A},
		{"s00f.msf", 0x3CE800, 0x158D0},
		{"s00f.scr", 0x3E4800, 0x138F4},
		{"s00f1.act", 0x3F8800, 0x2CBB0},
		{"s00f2.act", 0x425800, 0x16738},
		{"s00f3.act", 0x43C000, 0x32C78},
		{"s00f4.act", 0x46F000, 0xB78},
		{"s00fz01.msq", 0x470000, 0x136D},
		{"s00g.msf", 0x471800, 0x3840},
		{"s00g.scr", 0x475800, 0x106F4},
		{"s00g1.act", 0x486000, 0xAA50},
		{"s00g2.act", 0x491000, 0x4DDC},
		{"s00g3.act", 0x496000, 0x1944},
		{"s00gz00.msq", 0x498000, 0x1D7A},
		{"s00h.msf", 0x49A000, 0x4FB0},
		{"s00h.scr", 0x49F000, 0x17AF4},
		{"s00h1.act", 0x4B7000, 0x34D94},
		{"s00h2.act", 0x4EC000, 0x1BF4C},
		{"s00h3.act", 0x508000, 0xA92C},
		{"s00h4.act", 0x513000, 0x3F58},
		{"s00h5.act", 0x517000, 0xC850},
		{"s00h6.act", 0x524000, 0x32174},
		{"s00h7.act", 0x556800, 0x3BC4},
		{"s00h8.act", 0x55A800, 0x2D78},
		{"s00h9.act", 0x55D800, 0x28B0},
		{"s00ha.act", 0x560800, 0x6F0},
		{"s00hz10.msq", 0x561000, 0x192B},
		{"s00i.msf", 0x563000, 0x4A20},
		{"s00i.scr", 0x568000, 0x61EA0},
		{"s00i1.act", 0x5CA000, 0x4708},
		{"s00i2.act", 0x5CE800, 0x32B4},
		{"s00i3.act", 0x5D2000, 0x2530},
		{"s00i4.act", 0x5D4800, 0x222C},
		{"s00iz06.msq", 0x5D7000, 0x3A4B},
		{"s00j.msf", 0x5DB000, 0x1D2A0},
		{"s00j.scr", 0x5F8800, 0x1934C},
		{"s00j1.act", 0x612000, 0x50914},
		{"s00j2.act", 0x663000, 0x43FCC},
		{"s00jz15.msq", 0x6A7000, 0x3A03},
		{"s00k.msf", 0x6AB000, 0x144F0},
		{"s00k.scr", 0x6BF800, 0x200F4},
		{"s00kz21.msq", 0x6E0000, 0x2EA7},
		{"s00l.scr", 0x6E3000, 0xBDF4},
		{"s00lz16.msq", 0x6EF000, 0x2B6B},
		{"s00lz21.msq", 0x6F2000, 0x2EA7},
		{"s01a.msf", 0x6F5000, 0x10590},
		{"s01a.scr", 0x705800, 0x1B0F4},
		{"s01a1.act", 0x721000, 0xFBF0},
		{"s01a2.act", 0x731000, 0x591C},
		{"s01az00.msq", 0x737000, 0x1D7A},
		{"s01az08.msq", 0x739000, 0x25B8},
		{"s01az17.msq", 0x73B800, 0x1D7A},
		{"s01b.msf", 0x73D800, 0xCDF0},
		{"s01b.scr", 0x74A800, 0x1DAF4},
		{"s01b1.act", 0x768800, 0xAF78},
		{"s01b2.act", 0x773800, 0xAA8},
		{"s01bz00.msq", 0x774800, 0x1D7A},
		{"s01c.msf", 0x776800, 0x7D70},
		{"s01c.scr", 0x77E800, 0x1CCF4},
		{"s01c1.act", 0x79B800, 0x76CBC},
		{"s01c2.act", 0x812800, 0x4D0},
		{"s01c3.act", 0x813000, 0xEE4},
		{"s01cz01.msq", 0x814000, 0x136D},
		{"s01d.msf", 0x815800, 0xE740},
		{"s01d.scr", 0x824000, 0x5E0AC},
		{"s01d1.act", 0x882800, 0x90A2C},
		{"s01d2.act", 0x913800, 0x11AB4},
		{"s01d3.act", 0x925800, 0x20FD8},
		{"s01d4.act", 0x946800, 0x17D2C},
		{"s01d5.act", 0x95E800, 0x1BAF4},
		{"s01d7.act", 0x97A800, 0x6A974},
		{"s01d8.act", 0x9E5800, 0x2852C},
		{"s01d9.act", 0xA0E000, 0x958C},
		{"s01d20.act", 0xA17800, 0x1B9D8},
		{"s01d21.act", 0xA33800, 0x1D460},
		{"s01d22.act", 0xA51000, 0x287C8},
		{"s01d23.act", 0xA79800, 0x16734},
		{"s01d24.act", 0xA90000, 0x138A8},
		{"s01d25.act", 0xAA4000, 0x1A290},
		{"s01da.act", 0xABE800, 0x702C},
		{"s01dz00.msq", 0xAC6000, 0x1D7A},
		{"s01dz17.msq", 0xAC8000, 0x1D7A},
		{"s01e.scr", 0xACA000, 0x189F4},
		{"s01e1.act", 0xAE3000, 0x7004},
		{"s01ez00.msq", 0xAEA800, 0x1D7A},
		{"s01f.msf", 0xAEC800, 0x11210},
		{"s01f.scr", 0xAFE000, 0x195F4},
		{"s01f1.act", 0xB17800, 0x33868},
		{"s01f2.act", 0xB4B800, 0x734C},
		{"s01fz00.msq", 0xB53000, 0x1D7A},
		{"s01fz03.msq", 0xB55000, 0x246A},
		{"s01fz06.msq", 0xB57800, 0x3A4B},
		{"s01fz23.msq", 0xB5B800, 0x246A},
		{"s01g.msf", 0xB5E000, 0x1E010},
		{"s01g.scr", 0xB7C800, 0x1BBF4},
		{"s01g1.act", 0xB98800, 0xD30},
		{"s01g2.act", 0xB99800, 0x18794},
		{"s01g3.act", 0xBB2000, 0x38FC},
		{"s01g4.act", 0xBB6000, 0x39784},
		{"s01g5.act", 0xBEF800, 0x20694},
		{"s01g6.act", 0xC10000, 0x10A18},
		{"s01g7.act", 0xC21000, 0x13AD4},
		{"s01g8.act", 0xC35000, 0x1A750},
		{"s01g9.act", 0xC4F800, 0x12958},
		{"s01gz00.msq", 0xC62800, 0x1D7A},
		{"s01gz01.msq", 0xC64800, 0x136D},
		{"s01gz08.msq", 0xC66000, 0x25B8},
		{"s01h.msf", 0xC68800, 0xFBF0},
		{"s01h.scr", 0xC78800, 0x10CF4},
		{"s01h1.act", 0xC89800, 0x5E5A8},
		{"s01h2.act", 0xCE8000, 0x52FC0},
		{"s01h3.act", 0xD3B000, 0x2CC68},
		{"s01h4.act", 0xD68000, 0xC3CC},
		{"s01h5.act", 0xD74800, 0x8D8},
		{"s01h6.act", 0xD75800, 0x1A044},
		{"s01hz00.msq", 0xD90000, 0x1D7A},
		{"s01hz01.msq", 0xD92000, 0x136D},
		{"s01i.msf", 0xD93800, 0x3840},
		{"s01i.scr", 0xD97800, 0x25FDC},
		{"s01i1.act", 0xDBD800, 0x19CF0},
		{"s01i2.act", 0xDD7800, 0x514},
		{"s01i3.act", 0xDD8000, 0x3BC8},
		{"s01iz00.msq", 0xDDC000, 0x1D7A},
		{"s01j.msf", 0xDDE000, 0xBBA0},
		{"s01j.scr", 0xDEA000, 0x15BF4},
		{"s01j0.act", 0xE00000, 0x245B8},
		{"s01j1.act", 0xE24800, 0x8A34},
		{"s01j2.act", 0xE2D800, 0x3C0A0},
		{"s01j3.act", 0xE6A000, 0x2944},
		{"s01jz00.msq", 0xE6D000, 0x1D7A},
		{"s01k.msf", 0xE6F000, 0x12FA0},
		{"s01k.scr", 0xE82000, 0x189F4},
		{"s01k1.act", 0xE9B000, 0x4394},
		{"s01k2.act", 0xE9F800, 0x75068},
		{"s01k3.act", 0xF15000, 0x9A0},
		{"s01kz00.msq", 0xF16000, 0x1D7A},
		{"s01l.msf", 0xF18000, 0x9A70},
		{"s01l.scr", 0xF22000, 0x12BF4},
		{"s01l1.act", 0xF35000, 0x1CDC},
		{"s01lz00.msq", 0xF37000, 0x1D7A},
		{"s02a.msf", 0xF39000, 0x11240},
		{"s02a.scr", 0xF4A800, 0x197F4},
		{"s02a1.act", 0xF64000, 0x17110},
		{"s02a2.act", 0xF7B800, 0x13EB4},
		{"s02a3.act", 0xF8F800, 0x8D20},
		{"s02a4.act", 0xF98800, 0x36F8},
		{"s02a5.act", 0xF9C000, 0x22F58},
		{"s02a6.act", 0xFBF000, 0xE3C},
		{"s02az02.msq", 0xFC0000, 0x2F1D},
		{"s02az11.msq", 0xFC3000, 0x2A5A},
		{"s02b.msf", 0xFC6000, 0x170D0},
		{"s02b.scr", 0xFDD800, 0x14EF4},
		{"s02b1.act", 0xFF2800, 0x49F08},
		{"s02b2.act", 0x103C800, 0xDE78},
		{"s02b3.act", 0x104A800, 0x3768},
		{"s02bz02.msq", 0x104E000, 0x2F1D},
		{"s02bz11.msq", 0x1051000, 0x2A5A},
		{"s02bz13.msq", 0x1054000, 0x3D20},
		{"s02bz20.msq", 0x1058000, 0x673F},
		{"s02c.msf", 0x105E800, 0x20240},
		{"s02c.scr", 0x107F000, 0x277F4},
		{"s02c1.act", 0x10A6800, 0x30448},
		{"s02c2.act", 0x10D7000, 0x15828},
		{"s02c3.act", 0x10ED000, 0x1708C},
		{"s02c4.act", 0x1104800, 0x12D1C},
		{"s02c5.act", 0x1117800, 0x39970},
		{"s02c6.act", 0x1151800, 0x1B114},
		{"s02c7.act", 0x116D000, 0x5F7C},
		{"s02c8.act", 0x1173000, 0x82368},
		{"s02c9.act", 0x11F5800, 0x3D02C},
		{"s02c11.act", 0x1233000, 0x2A934},
		{"s02c51.act", 0x125E000, 0x3004},
		{"s02ca.act", 0x1261800, 0x7F0D0},
		{"s02cb.act", 0x12E1000, 0x125A8},
		{"s02cc.act", 0x12F3800, 0x4150},
		{"s02cz11.msq", 0x12F8000, 0x2A5A},
		{"s02cz12.msq", 0x12FB000, 0xE7B},
		{"s02cz13.msq", 0x12FC000, 0x3D20},
		{"s02cz20.msq", 0x1300000, 0x673F},
		{"s02d.msf", 0x1306800, 0x21940},
		{"s02d.scr", 0x1328800, 0x160F4},
		{"s02d1.act", 0x133F000, 0x2E09C},
		{"s02d2.act", 0x136D800, 0x26D74},
		{"s02d3.act", 0x1394800, 0x14EA0},
		{"s02d4.act", 0x13A9800, 0x2294},
		{"s02d5.act", 0x13AC000, 0x4A48},
		{"s02dz10.msq", 0x13B1000, 0x192B},
		{"s02dz13.msq", 0x13B3000, 0x3D20},
		{"s02e.msf", 0x13B7000, 0x5EB0},
		{"s02e.scr", 0x13BD000, 0x107F4},
		{"s02e1.act", 0x13CD800, 0x62BC8},
		{"s02ez18.msq", 0x1430800, 0x33FE},
		{"s03a.msf", 0x1434000, 0x12070},
		{"s03a.scr", 0x1446800, 0x18BF4},
		{"s03a1.act", 0x145F800, 0x49E0},
		{"s03a2.act", 0x1464800, 0x3DF4},
		{"s03a3.act", 0x1468800, 0x45774},
		{"s03a4.act", 0x14AE000, 0x23D4},
		{"s03a5.act", 0x14B0800, 0x13E64},
		{"s03a6.act", 0x14C4800, 0x46BC},
		{"s03az03.msq", 0x14C9000, 0x246A},
		{"s03az06.msq", 0x14CB800, 0x3A4B},
		{"s03az23.msq", 0x14CF800, 0x246A},
		{"s04a.msf", 0x14D2000, 0x11D60},
		{"s04a.scr", 0x14E4000, 0x12AF4},
		{"s04a1.act", 0x14F7000, 0x2AC40},
		{"s04a2.act", 0x1522000, 0x6544},
		{"s04a3.act", 0x1528800, 0x46C8},
		{"s04a4.act", 0x152D000, 0x2240},
		{"s04az03.msq", 0x152F800, 0x246A},
		{"s04az10.msq", 0x1532000, 0x192B},
		{"s04az24.msq", 0x1534000, 0x2463},
		{"s05a.msf", 0x1536800, 0x5D60},
		{"s05a.scr", 0x153C800, 0x127F4},
		{"s05a1.act", 0x154F000, 0x3E1CC},
		{"s05az04.msq", 0x158D800, 0xFA1},
		{"s05b.msf", 0x158E800, 0x28640},
		{"s05b.scr", 0x15B7000, 0x2C3C4},
		{"s05b0.act", 0x15E3800, 0x16474},
		{"s05b1.act", 0x15FA000, 0x1F2F4},
		{"s05b2.act", 0x1619800, 0x113E8},
		{"s05b3.act", 0x162B000, 0x1D424},
		{"s05b4.act", 0x1648800, 0x501C4},
		{"s05b5.act", 0x1699000, 0x1F510},
		{"s05b6.act", 0x16B8800, 0x8860},
		{"s05bz05.msq", 0x16C1800, 0x61C},
		{"s05bz06.msq", 0x16C2000, 0x3A4B},
		{"s05bz07.msq", 0x16C6000, 0x3909},
		{"s06a.msf", 0x16CA000, 0x1C750},
		{"s06a.scr", 0x16E6800, 0x151F4},
		{"s06a1.act", 0x16FC000, 0x24654},
		{"s06a2.act", 0x1720800, 0x25D0},
		{"s06az19.msq", 0x1723000, 0x20D7},
		{"s06b.msf", 0x1725800, 0x1BF60},
		{"s06b.scr", 0x1741800, 0x182F4},
		{"s06b1.act", 0x175A000, 0x6CE68},
		{"s06bz19.msq", 0x17C7000, 0x20D7},
		{"s07a.msf", 0x17C9800, 0x15240},
		{"s07a.scr", 0x17DF000, 0x1ACF4},
		{"s07a1.act", 0x17FA000, 0xE5A0},
		{"s07a2.act", 0x1808800, 0x425D0},
		{"s07a3.act", 0x184B000, 0x18810},
		{"s07az03.msq", 0x1864000, 0x246A},
		{"s07az08.msq", 0x1866800, 0x25B8},
		{"s07az25.msq", 0x1869000, 0x245B},
		{"s07b.msf", 0x186B800, 0x14960},
		{"s07b.scr", 0x1880800, 0x15CF4},
		{"s07b1.act", 0x1896800, 0x419C8},
		{"s07b2.act", 0x18D8800, 0x1A560},
		{"s07b3.act", 0x18F3000, 0x76F4},
		{"s07b4.act", 0x18FA800, 0x76C8},
		{"s07bz03.msq", 0x1902000, 0x246A},
		{"s07bz08.msq", 0x1904800, 0x25B8},
		{"s07bz25.msq", 0x1907000, 0x245B},
		{"s07c.scr", 0x1909800, 0x143F4},
		{"s07c1.act", 0x191E000, 0xA25C},
		{"s08a.msf", 0x1928800, 0xB9E0},
		{"s08a.scr", 0x1934800, 0x322F4},
		{"s08a1.act", 0x1967000, 0xA668},
		{"s08a2.act", 0x1971800, 0x2C828},
		{"s08a3.act", 0x199E800, 0x6CF0},
		{"s08az03.msq", 0x19A5800, 0x246A},
		{"s08az06.msq", 0x19A8000, 0x3A4B},
		{"s08az23.msq", 0x19AC000, 0x246A},
		{"s08b.scr", 0x19AE800, 0x399F4},
		{"s08b1.act", 0x19E8800, 0x1F0A0},
		{"s08bz03.msq", 0x1A08000, 0x246A},
		{"s08bz06.msq", 0x1A0A800, 0x3A4B},
		{"s08bz23.msq", 0x1A0E800, 0x246A},
		{"s09a.msf", 0x1A11000, 0x14190},
		{"s09a.scr", 0x1A25800, 0x1B4F4},
		{"s09a1.act", 0x1A41000, 0x634B8},
		{"s09az14.msq", 0x1AA4800, 0x1B9B},
		{"s09b.msf", 0x1AA6800, 0x1F2E0},
		{"s09b.scr", 0x1AC6000, 0x1B4F4},
		{"s09b1.act", 0x1AE1800, 0x71B8},
		{"s09b2.act", 0x1AE9000, 0x2299C},
		{"s09b3.act", 0x1B0C000, 0x1B360},
		{"s09b4.act", 0x1B27800, 0x14B64},
		{"s09b5.act", 0x1B3C800, 0x5E4},
		{"s09b6.act", 0x1B3D000, 0xAA78},
		{"s09b7.act", 0x1B48000, 0x9B24},
		{"s09bz14.msq", 0x1B52000, 0x1B9B},
		{"s09bz15.msq", 0x1B54000, 0x3A03},
		{"s09c.msf", 0x1B58000, 0x2D640},
		{"s09c.scr", 0x1B85800, 0x200F4},
		{"s09c1.act", 0x1BA6000, 0x10F68},
		{"s09c2.act", 0x1BB7000, 0x25B1C},
		{"s09c3.act", 0x1BDD000, 0x15DF0},
		{"s09c4.act", 0x1BF3000, 0x11310},
		{"s09c5.act", 0x1C04800, 0x14F8C},
		{"s09c6.act", 0x1C19800, 0x3810},
		{"s09cz16.msq", 0x1C1D800, 0x2B6B},
		{"s09d.msf", 0x1C20800, 0xF2F0},
		{"s09d.scr", 0x1C30000, 0x1C0F4},
		{"s09d1.act", 0x1C4C800, 0x2FE78},
		{"s09dz16.msq", 0x1C7C800, 0x2B6B},
		{"s09e.msf", 0x1C7F800, 0xCBD0},
		{"s09e.scr", 0x1C8C800, 0x152F4},
		{"s09e1.act", 0x1CA2000, 0x32538},
		{"s09e2.act", 0x1CD4800, 0xADE4},
		{"s09ez16.msq", 0x1CDF800, 0x2B6B},
		{"s09f.msf", 0x1CE2800, 0x29380},
		{"s09f.scr", 0x1D0C000, 0x177F4},
		{"s09f1.act", 0x1D23800, 0x13B84},
		{"s09f2.act", 0x1D37800, 0x5106C},
		{"s09f3.act", 0x1D89000, 0xBDB4},
		{"s09fz16.msq", 0x1D95000, 0x2B6B},
		{"s09h.msf", 0x1D98000, 0x20F20},
		{"s09h.scr", 0x1DB9000, 0x1EDF4},
		{"s09h1.act", 0x1DD8000, 0x11A80},
		{"s09h2.act", 0x1DEA000, 0x23FC0},
		{"s09h3.act", 0x1E0E000, 0x2CC88},
		{"s09h4.act", 0x1E3B000, 0x54534},
		{"s09h5.act", 0x1E8F800, 0x2BFF4},
		{"s09hz16.msq", 0x1EBB800, 0x2B6B},
		{"s09i.msf", 0x1EBE800, 0x3840},
		{"s09i.scr", 0x1EC2800, 0x229C4},
		{"s09i1.act", 0x1EE5800, 0x2DD38},
		{"s09i2.act", 0x1F13800, 0x11A24},
		{"s09i3.act", 0x1F25800, 0x2CCA8},
		{"s09i4.act", 0x1F52800, 0x343C},
		{"s09iz16.msq", 0x1F56000, 0x2B6B},
		{"s10a.msf", 0x1F59000, 0x270F0},
		{"s10a.scr", 0x1F80800, 0x406F4},
		{"s10a1.act", 0x1FC1000, 0x1F82C},
		{"s10a2.act", 0x1FE1000, 0x5D690},
		{"s10a3.act", 0x203E800, 0x5E800},
		{"s10a4.act", 0x209D000, 0xBAF8},
		{"s10a5.act", 0x20A9000, 0x181C},
		{"s10a6.act", 0x20AB000, 0x36404},
		{"s10a7.act", 0x20E1800, 0x51C44},
		{"s10az17.msq", 0x2133800, 0x1D7A},
		{"s10az18.msq", 0x2135800, 0x33FE},
		{"s10b.msf", 0x2139000, 0xD5E0},
		{"s10b.scr", 0x2146800, 0x14DF4},
		{"s10b1.act", 0x215B800, 0x27B20},
		{"s10b2.act", 0x2183800, 0x2B900},
		{"s10b3.act", 0x21AF800, 0x8D78},
		{"s10b4.act", 0x21B8800, 0x1A78},
		{"s10bz17.msq", 0x21BA800, 0x1D7A},
		{"s10bz18.msq", 0x21BC800, 0x33FE},
		{"s10d.msf", 0x21C0000, 0x16D20},
		{"s10d.scr", 0x21D7000, 0x220F4},
		{"s10d1.act", 0x21F9800, 0x4DFB8},
		{"s11a.msf", 0x2247800, 0xD650},
		{"s11a.scr", 0x2255000, 0x16CF4},
		{"s11a1.act", 0x226C000, 0x13BB4},
		{"s11a2.act", 0x2280000, 0x4794},
		{"s11a3.act", 0x2284800, 0x186E0},
		{"s11az03.msq", 0x229D000, 0x246A},
		{"s11az17.msq", 0x229F800, 0x1D7A},
		{"s12a.msf", 0x22A1800, 0x22B60},
		{"s12a.scr", 0x22C4800, 0x21AAA},
		{"s12a1.act", 0x22E6800, 0xFE24},
		{"s12a2.act", 0x22F6800, 0x123D4},
		{"s12a3.act", 0x2309000, 0x39510},
		{"s12a4.act", 0x2342800, 0x48474},
		{"s12a5.act", 0x238B000, 0x35D24},
		{"s12a6.act", 0x23C1000, 0x12850},
		{"s12az04.msq", 0x23D4000, 0xFA1},
		{"s12az26.msq", 0x23D5000, 0x2AE},
		{"s12b.msf", 0x23D5800, 0xA720},
		{"s12b.scr", 0x23E0000, 0x117F4},
		{"s12b1.act", 0x23F1800, 0x50C0},
		{"s12b2.act", 0x23F7000, 0xAE2C},
		{"s12b3.act", 0x2402000, 0x6A4},
		{"s12c.scr", 0x2402800, 0x106F4},
		{"s12c1.act", 0x2413000, 0xA21B4},
		{"s13a.msf", 0x24B5800, 0x251A0},
		{"s13a.scr", 0x24DB000, 0x117F4},
		{"s13a0.act", 0x24EC800, 0x74C},
		{"s13a1.act", 0x24ED000, 0x7784},
		{"s13a2.act", 0x24F4800, 0x923A8},
		{"s13a3.act", 0x2587000, 0x381F4},
		{"s13a4.act", 0x25BF800, 0x6E140},
		{"s13a5.act", 0x262E000, 0x4B18},
		{"s13az20.msq", 0x2633000, 0x673F},
		{"s13b.msf", 0x2639800, 0xAB70},
		{"s13b.scr", 0x2644800, 0xEF4},
		{"s13b1.act", 0x2645800, 0x7784},
		{"s13b2.act", 0x264D000, 0x4FAB0},
		{"s13bz20.msq", 0x269D000, 0x673F},
		{"s14a.msf", 0x26A3800, 0xF310},
		{"s14a.scr", 0x26B3000, 0x30A94},
		{"s14a0.act", 0x26E4000, 0xD320},
		{"s14a1.act", 0x26F1800, 0x13FC8},
		{"s14a2.act", 0x2705800, 0xD5E30},
		{"s14a3.act", 0x27DB800, 0x13274},
		{"s14a5.act", 0x27EF000, 0x5A6A4},
		{"s14a15.act", 0x2849800, 0x6D50},
		{"s14a25.act", 0x2850800, 0x7B564},
		{"s14az21.msq", 0x28CC000, 0x2EA7},
		{"s14b.msf", 0x28CF000, 0x187E0},
		{"s14b.scr", 0x28E7800, 0x106F4},
		{"s14bz21.msq", 0x28F8000, 0x2EA7},
		{"s14d.msf", 0x28FB000, 0xC0E0},
		{"s14d.scr", 0x2907800, 0x23E34},
		{"s14d1.act", 0x292B800, 0x2D11C},
		{"s14dz10.msq", 0x2959000, 0x192B},
		{"s14dz11.msq", 0x295B000, 0x2A5A},
		{"s14e.msf", 0x295E000, 0x8600},
		{"s14e.scr", 0x2966800, 0x106F4},
		{"s14e1.act", 0x2977000, 0xAA00},
		{"s14e2.act", 0x2982000, 0x1F848},
		{"s14ez03.msq", 0x29A2000, 0x246A},
		{"s14ez20.msq", 0x29A4800, 0x673F},
		{"s14ez22.msq", 0x29AB000, 0x286C},
		{"s14f.scr", 0x29AE000, 0x106F4},
		{"s14f1.act", 0x29BE800, 0x27A2C},
		{"s14fz22.msq", 0x29E6800, 0x286C},
		{"s14fz27.msq", 0x29E9800, 0xB6E},
		{"s14g.msf", 0x29EA800, 0x2500},
		{"s14g.scr", 0x29ED000, 0x1C3C4},
		{"s14g1.act", 0x2A09800, 0x140FC},
		{"s14g2.act", 0x2A1E000, 0x943F8},
		{"s14g3.act", 0x2AB2800, 0x1E9A8},
		{"s14gz22.msq", 0x2AD1800, 0x286C},
		{"s14h.scr", 0x2AD4800, 0x106F4},
		{"s14hz22.msq", 0x2AE5000, 0x286C}
};

uint32 getResourceId(const char *filename) {
	for (uint32 i=0; i < TOTAL_FILES; i++) {
		if (scumm_stricmp(fileInfo[i].filename, filename) == 0) {
			return i;
		}
	}

	return TOTAL_FILES;
}

BigfileArchive::BigfileArchive(const char *filename, Common::Language language) :_fd(0), _language(language) {
	_fd = new Common::File();
	if (!_fd->open(filename))
		error("BigfileArchive::BigfileArchive() Could not open %s", filename);
}

BigfileArchive::~BigfileArchive() {
	_fd->close();
	delete _fd;
}

byte *BigfileArchive::load(const char *filename, uint32 &dataSize) {
	uint32 id = getResourceId(filename);
	if (id >= TOTAL_FILES) {
		error("Invalid resourceID for input filename: %s", filename);
	}

	dataSize = fileInfo[id].size;
	_fd->seek(fileInfo[id].offset);
	byte *buf = (byte *)malloc(dataSize);
	if (!buf) {
		error("Failed to malloc %d bytes for '%s'", dataSize, filename);
	}
	_fd->read(buf, dataSize);
	return buf;
}

bool BigfileArchive::doesFileExist(const char *filename) {
	uint32 id = getResourceId(filename);
	return (id < TOTAL_FILES);
}


} // End of namespace Dragons
