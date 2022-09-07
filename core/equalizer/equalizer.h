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

#ifndef EQUALIZER_EQUALIZER_H_
#define EQUALIZER_EQUALIZER_H_

#include <map>
#include <vlcpp/vlc.hpp>

enum EqualizerMode {
  kFlat,
  kClassical,
  kClub,
  kDance,
  kFullBass,
  kFullBassAndTreble,
  kFullTreble,
  kHeadphones,
  kLargeHall,
  kLive,
  kParty,
  kPop,
  kReggae,
  kRock,
  kSka,
  kSoft,
  kSoftRock,
  kTechno
};

class Equalizer {
 public:
  float pre_amp() const { return pre_amp_; }
  const std::map<float, float>& band_amps() const { return band_amps_; }
  VLC::Equalizer* vlc_equalizer() const { return vlc_equalizer_.get(); }

  Equalizer();

  Equalizer(EqualizerMode mode);

  void SetBandAmp(float band, float amps);

  void SetPreAmp(float amp);

 private:
  void Refresh();

  float pre_amp_ = 0.0f;
  std::map<float, float> band_amps_ = {};
  std::unique_ptr<VLC::Equalizer> vlc_equalizer_;
  std::map<float, int32_t> bands_ = {};
};

#endif
