// This file is a part of dart_vlc (https://github.com/alexmercerind/dart_vlc)
//
// Copyright (C) 2021-2022 Hitesh Kumar Saini <saini123hitesh@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef CORE_H_
#define CORE_H_

#include "broadcast/broadcast.h"
#include "chromecast/chromecast.h"
#include "devices/devices.h"
#include "equalizer/equalizer.h"
#include "media_source/media.h"
#include "media_source/playlist.h"
#include "player/player.h"
#include "record/record.h"
#include "utils/instance_handler.h"

#ifndef DLLEXPORT
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

// TODO: Get rid of global variables & linking hack.

extern std::unique_ptr<InstanceHandler<Player>> g_players;
extern std::unique_ptr<InstanceHandler<Equalizer>> g_equalizers;
extern std::unique_ptr<InstanceHandler<Broadcast>> g_broadcasts;
extern std::unique_ptr<InstanceHandler<Record>> g_records;
extern std::unique_ptr<InstanceHandler<Chromecast>> g_chromecasts;

#ifdef __cplusplus
}
#endif

#endif  // CORE_H_
