/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

/*
 * This code is based on original Hugo Trilogy source code
 *
 * Copyright (c) 1989-1995 David P. Gray
 *
 */

#include "common/system.h"
#include "common/util.h"
#include "gui/message.h"

#include "hugo/dialogs.h"
#include "hugo/util.h"

namespace Hugo {

namespace Utils {

/**
 * Returns index (0 to 7) of first 1 in supplied byte, or 8 if not found
 */
int firstBit(byte data) {
	if (!data)
		return 8;

	int i;
	for (i = 0; i < 8; i++) {
		if ((data << i) & 0x80)
			break;
	}

	return i;
}

/**
 * Returns index (0 to 7) of last 1 in supplied byte, or 8 if not found
 */
int lastBit(byte data) {
	if (!data)
		return 8;

	int i;
	for (i = 7; i >= 0; i--) {
		if ((data << i) & 0x80)
			break;
	}

	return i;
}

/**
 * Reverse the bit order in supplied byte
 */
void reverseByte(byte *data) {
	byte maskIn = 0x80;
	byte maskOut = 0x01;
	byte result = 0;

	for (byte i = 0; i < 8; i++, maskIn >>= 1, maskOut <<= 1) {
		if (*data & maskIn)
			result |= maskOut;
	}

	*data = result;
}

void notifyBox(const Common::String &msg) {
	notifyBox(Common::U32String(msg));
}

void notifyBox(const Common::U32String &msg) {
	if (msg.empty())
		return;

	GUI::MessageDialog dialog(msg);
	dialog.runModal();
}

Common::String promptBox(const Common::String &msg) {
	if (msg.empty())
		return Common::String();

	EntryDialog dialog(msg, "OK", "");

	dialog.runModal();

	return dialog.getEditString();
}

bool yesNoBox(const Common::String &msg) {
	return yesNoBox(Common::U32String(msg));
}

bool yesNoBox(const Common::U32String &msg) {
	if (msg.empty())
		return 0;

	GUI::MessageDialog dialog(msg, Common::U32String("YES"), Common::U32String("NO"));
	return (dialog.runModal() == GUI::kMessageOK);
}

char *hugo_strlwr(char *buffer) {
	char *result = buffer;

	while (*buffer != '\0') {
		if (Common::isUpper(*buffer))
			*buffer = tolower(*buffer);
		buffer++;
	}

	return result;
}

} // End of namespace Utils

} // End of namespace Hugo
