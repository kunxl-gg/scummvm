# ScummVM - Graphic Adventure Engine
# Copyright (C) 2020 Stryzhniou Fiodar

# ScummVM is the legal property of its developers, whose names
# are too numerous to list here. Please refer to the COPYRIGHT
# file distributed with this source distribution.

# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.


from __future__ import with_statement
import os
from common_names import *


# ---------------------------
# Expect: reg_rss_template %(UID3, ordnum, ordnum)
reg_rss_template = """
/*
 * Warning: autogenerated file
 */
// All registration files need to #include appinfo.rh.
#include <AppInfo.rh>

// All registration files must define UID2, which is always
// KUidAppRegistrationResourceFile, and UID3, which is the application's UID.
UID2 KUidAppRegistrationResourceFile
UID3 %s // application UID

// Registration file need to containo an APP_REGISTRATION_INFO resource that
// minimally needs to provide the name of the application binary (using the
// app_file statement).
RESOURCE APP_REGISTRATION_INFO
	{
	app_file = "ScummVM%s"; // filename of application binary (minus extension)
	// Specify the location of the localisable icon/caption definition file
	localisable_resource_file = "\\Resource\\Apps\\ScummVM%s_loc";
	}
"""

# ---------------------------
# Expect:  loc_rss_template %UID3
loc_rss_template = """
/*
 * Warning: autogenerated file
 */
#include <AppInfo.rh>

// This file localise the applications icons and caption
RESOURCE LOCALISABLE_APP_INFO
	{
	caption_and_icon =
		{
		CAPTION_AND_ICON_INFO
			{
			// The caption text is defined in the rls file
			caption = "ScummVM %s";
			// Icons are used to represent applications in the
			// application launcher and application title bar.
			// The number_of_icons value identifies how many icons
			// that exist in the icon_file.
			number_of_icons = 1;
			// Using the application icons.
			icon_file = "\\Resource\\Apps\\ScummVM.mif";
			}
		};
	}
"""

# ---------------------------
# Expect: rss_template %(UID3, UID3)
rss_template = """
/*
 * Warning: autogenerated file
 */

// ScummVM%s.rss

NAME SCUM

// Include definitions of resource STRUCTS used by this
// resource script
#include <eikon.rh>


RESOURCE RSS_SIGNATURE
	{
	}

RESOURCE TBUF16 { buf=""; }

RESOURCE EIK_APP_INFO
	{
		menubar = r_scum_menubar;
	}

RESOURCE MENU_BAR r_scum_menubar                                         // *** Menu bar
{
      titles =
    {
		MENU_TITLE { menu_pane = r_scum_menu; txt = "ScummVM%s"; }
	};
}

RESOURCE MENU_PANE r_scum_menu                                  // *** Submenu
{
	items =
	{

		MENU_ITEM{command = EEikCmdExit;txt = "Exit";}
    };
}

"""

# ---------------------------
# Expect: mmp_template %(ordnum, UID3, ordnum, ordnum, ordnum, ordnum)
mmp_template = """
/*
 * Warning: autogenerated file
 */

//
// EPOC S60 MMP makefile project for ScummVM
//

// *** Definitions


TARGET          ScummVM%s.exe
TARGETPATH      sys\\bin
TARGETTYPE      exe
OPTION			GCCE -Wno-multichar -Wno-reorder -Wno-unused -Wno-format -fsigned-char \
 -fno-asynchronous-unwind-tables -std=c++11 // -ffreestanding -fno-sized-deallocation //--verbose //-flto -fuse-linker-plugin //-Wl, -v  -fbuiltin 
// fixes error "section .data loaded at [...] overlaps section .text loaded at [...]"
LINKEROPTION 	GCCE -Tdata 0xAA00000 // -v -flto -fuse-linker-plugin
//--print-gc-sections --stats --gc-sections --strip-all// removes unused code

UID             0x100039ce %s

bytepaircompresstarget

START RESOURCE  ScummVM%s.rss
HEADER
TARGETPATH              \Resource\Apps
LANG                    SC
END

START RESOURCE  ScummVM%s_reg.rss
TARGETPATH              \private\\10003a3f\\apps
END

START RESOURCE  ScummVM%s_loc.rss
TARGETPATH              \Resource\Apps
LANG                    SC
END

EPOCSTACKSIZE	80000
EPOCHEAPSIZE	5000000 64000000


#define SCUMMVM_PT_%s
#include "../mmp/macros.mmh"
#include "ScummVM_common.mmh" // must be above engines.mmh

#include "../mmp/engines.mmh"

// implicitly compiled if links with engines kyra, sci, scumm and saga
SOURCEPATH ..\..\..\..\graphics
SOURCE sjis.cpp
"""
# ---------------------------
bld_inf_template = """
PRJ_PLATFORMS
GCCE WINSCW

PRJ_MMPFILES

gnumakefile icons.mk
gnumakefile ..\help\build_help.mk

"""
# ---------------------------

def SafeWriteFile(path, data, mode = 'w'):
   """Save list elments as strings. Save strings as is"""
   with open(path, mode) as f:
      if type(data) is list:
         for s in data:
            f.write(s + '\n')
      else:
         f.write(data)
 
def create_mmps(build, path = ''):
   uids = get_UIDs(build)
   bld_inf = os.path.join(path, bld_inf_template)
   SafeWriteFile(bld_inf, "PRJ_MMPFILES\n")
   for i in range(len(uids)):
      UID3 = uids[i]
      idx = i + 1
      rss_name = "ScummVM%s.rss" %idx
      data = rss_template %(idx, idx)
      SafeWriteFile(os.path.join(path, rss_name), data)
      data = loc_rss_template %idx
      rss_loc_name = "ScummVM%s_loc.rss" %idx
      SafeWriteFile(os.path.join(path, rss_loc_name), data)
      data = reg_rss_template %(UID3, idx, idx)
      rss_reg_name = "ScummVM%s_reg.rss" %idx
      SafeWriteFile(os.path.join(path, rss_reg_name), data)
      data = mmp_template %(idx, UID3, idx, idx, idx, idx)
      mmp_name = "ScummVM%s.mmp" %idx
      SafeWriteFile(os.path.join(path, mmp_name), data)
      SafeWriteFile(bld_inf, mmp_name + "\n", mode = 'a')

if __name__ == "__main__":
   create_mmps(build = 'release')
