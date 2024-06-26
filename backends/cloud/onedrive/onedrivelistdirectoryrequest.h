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

#ifndef BACKENDS_CLOUD_ONEDRIVE_ONEDRIVELISTDIRECTORYREQUEST_H
#define BACKENDS_CLOUD_ONEDRIVE_ONEDRIVELISTDIRECTORYREQUEST_H

#include "backends/cloud/storage.h"
#include "backends/networking/curl/curljsonrequest.h"
#include "backends/networking/curl/request.h"
#include "common/callback.h"

namespace Cloud {
namespace OneDrive {

class OneDriveStorage;

class OneDriveListDirectoryRequest: public Networking::Request {
	Common::String _requestedPath;
	bool _requestedRecursive;
	OneDriveStorage *_storage;
	Storage::ListDirectoryCallback _listDirectoryCallback;
	Common::Array<StorageFile> _files;
	Common::Array<Common::String> _directoriesQueue;
	Common::String _currentDirectory;
	Request *_workingRequest;
	bool _ignoreCallback;
	Common::String _date;

	void start();
	void listNextDirectory();
	void listedDirectoryCallback(const Networking::JsonResponse &response);
	void listedDirectoryErrorCallback(const Networking::ErrorResponse &error);
	void makeRequest(const Common::String &url);
	void finishListing(const Common::Array<StorageFile> &files);
public:
	OneDriveListDirectoryRequest(OneDriveStorage *storage, const Common::String &path, Storage::ListDirectoryCallback cb, Networking::ErrorCallback ecb, bool recursive = false);
	~OneDriveListDirectoryRequest() override;

	void handle() override;
	void restart() override;
	Common::String date() const override;
};

} // End of namespace OneDrive
} // End of namespace Cloud

#endif
