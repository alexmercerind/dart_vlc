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

#ifndef BROADCAST_BROADCAST_H_
#define BROADCAST_BROADCAST_H_

#include <sstream>
#include <string>
#include <vlcpp/vlc.hpp>

#include "broadcast/broadcast_configuration.h"
#include "media_source/media.h"

// TODO: Finalize |Broadcast| API.
// Current API is very bad & doesn't allow proper change in |Media| once the
// |Broadcast| has been started.

class Broadcast {
 public:
  BroadcastConfiguration* configuration() const { return configuration_.get(); }

  Broadcast(std::shared_ptr<Media> media,
            std::unique_ptr<BroadcastConfiguration> configuration);

  void Start();

  ~Broadcast();

 private:
  //VLC::Instance vlc_instance_ = VLC::Instance(0, nullptr);
  //const char* mi_argumento = "--dshow-size=d1";
  //VLC::Instance vlc_instance_ = VLC::Instance(1, &mi_argumento);
  // const char* argumento1 = "--dshow-size=d1";
  //const char* argumento2 = "--dshow-fps=30";
  //const char* argumento3 = "--live-caching=100";
  //const char* argumentos[] = {argumento1, argumento2, argumento3};
  //VLC::Instance vlc_instance_ = VLC::Instance(3, &argumentos);
  //std::vector<const char*> argumentos;
  //argumentos.push_back("--dshow-size=d1");
  //argumentos.push_back("--dshow-fps=30");
  //argumentos.push_back("--live-caching=100");
  // Llamar a la constructora de VLC::Instance con los argumentos del vector.
  //VLC::Instance vlc_instance_ = VLC::Instance(static_cast<int32_t>(argumentos.size()), argumentos.data());
 std::vector<std::string> cmd_arguments = {
    "--dshow-size=d1",
    "--dshow-fps=30",
    "--live-caching=100"
};

const char** c_array = nullptr;
if (!cmd_arguments.empty()) {
    c_array = new const char*[cmd_arguments.size()];
    for (size_t i = 0; i < cmd_arguments.size(); i++) {
        c_array[i] = cmd_arguments[i].c_str();
    }
}

VLC::Instance vlc_instance(static_cast<int32_t>(cmd_arguments.size()), c_array);

  std::shared_ptr<Media> media_ = nullptr;
  std::unique_ptr<BroadcastConfiguration> configuration_ = nullptr;
};


#endif  // BROADCAST_BROADCAST_H_
