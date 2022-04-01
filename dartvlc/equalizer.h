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

#ifndef EQUALIZER_H_
#define EQUALIZER_H_

#include <map>
#include <vlcpp/vlc.hpp>

enum EqualizerMode {
  flat,
  classical,
  club,
  dance,
  fullBass,
  fullBassAndTreble,
  fullTreble,
  headphones,
  largeHall,
  live,
  party,
  pop,
  reggae,
  rock,
  ska,
  soft,
  softRock,
  techno
};

class Equalizer {
 public:
  float pre_amp() const { return pre_amp_; }
  const std::map<float, float>& band_amps() const { return band_amps_; }

  Equalizer() {
    vlc_equalizer_ = VLC::Equalizer();
    for (uint32_t index = 0; index < vlc_equalizer_.bandCount(); index++)
      bands_[vlc_equalizer_.bandFrequency(index)] = index;
    Refresh();
  }

  Equalizer(EqualizerMode mode) {
    vlc_equalizer_ = VLC::Equalizer(static_cast<int32_t>(mode));
    for (uint32_t index = 0; index < vlc_equalizer_.bandCount(); index++)
      bands_[vlc_equalizer_.bandFrequency(index)] = index;
    Refresh();
  }

  void SetBandAmp(float band, float amps) {
    vlc_equalizer_.setAmp(amps, bands_[band]);
    Refresh();
  }

  void SetPreAmp(float amp) {
    vlc_equalizer_.setPreamp(amp);
    Refresh();
  }

 private:
  float pre_amp_;
  std::map<float, float> band_amps_;
  VLC::Equalizer vlc_equalizer_;
  std::map<float, int32_t> bands_;

  void Refresh() {
    for (uint32_t index = 0; index < vlc_equalizer_.bandCount(); index++)
      band_amps_[vlc_equalizer_.bandFrequency(index)] =
          vlc_equalizer_.amp(index);
    pre_amp_ = vlc_equalizer_.preamp();
  }

  friend class PlayerSetters;
};

class Equalizers {
 public:
  Equalizer* Get(int id) { return equalizers_[id]; }

  int CreateEmpty() {
    int id = static_cast<int>(equalizers_.size());
    equalizers_[id] = new Equalizer();
    return id;
  }

  int CreateMode(EqualizerMode mode) {
    int id = static_cast<int>(equalizers_.size());
    equalizers_[id] = new Equalizer(mode);
    return id;
  }

  void Dispose(int id) { equalizers_.erase(id); }

 private:
  std::map<int, Equalizer*> equalizers_;
};

extern std::unique_ptr<Equalizers> g_equalizers;

#endif
