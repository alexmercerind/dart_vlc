/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC &
 * libVLC++.
 *
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * saini123hitesh@gmail.com; alexmercerind@gmail.com
 *
 * GNU Lesser General Public License v2.1
 */

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
