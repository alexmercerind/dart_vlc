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

#include "broadcast/broadcast.h"

#include <sstream>
#include <string>

#include "media_source/media.h"

Broadcast::Broadcast(std::shared_ptr<Media> media,
                     std::unique_ptr<BroadcastConfiguration> configuration)
    : media_(media), configuration_(std::move(configuration)) {}

void Broadcast::Start() {
  std::stringstream sout;
sout << "#duplicate{dst='transcode{vcodec=" << configuration_->vcodec()
     << ", vb=" << configuration_->vb()
     << ", acodec=" << configuration_->acodec()
     << ", ab=" << configuration_->ab()
     << "}:std{access=" << configuration_->access()
     << ", mux=" << configuration_->mux()
     << ", dst=" << configuration_->dst()
     << "}',dst=display}";












  libvlc_media_player_t *media_player = libvlc_media_player_new(vlc_instance_.get());

  // Obtén la ventana de visualización de la pantalla completa (puedes personalizar esto según tus necesidades)
  libvlc_media_player_set_fullscreen(media_player, 1);

  // Agregar la transmisión al servidor de medios y asignarla al reproductor de medios
  libvlc_vlm_add_broadcast(vlc_instance_.get(), "dshow:// ", "dshow://", sout.str().c_str(), 0, nullptr, true, false);
  libvlc_vlm_set_output(media_player, "dshow:// ");  // Asigna la transmisión al reproductor de medios

  // Inicia la transmisión
  libvlc_media_player_play(media_player);
}



Broadcast::~Broadcast() { libvlc_vlm_release(vlc_instance_.get()); }
