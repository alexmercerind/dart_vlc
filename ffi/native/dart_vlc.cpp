/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC &
 * libVLC++.
 *
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 *
 * GNU Lesser General Public License v2.1
 */

#ifndef DLLEXPORT
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __attribute__((used))
#endif
#endif

#include <memory>

#include "eventmanager.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DART_VLC_FFI
#define DART_VLC_FFI

static char** g_metas_ptr = nullptr;
static size_t g_metas_size = 0;
static char** g_devices_ptr = nullptr;
static size_t g_devices_size = 0;
static char** g_equalizer_ptr = nullptr;
static size_t g_equalizer_size = 0;

DLLEXPORT void PlayerCreate(int32_t id, int32_t video_width,
                            int32_t video_height,
                            int32_t commandLineArgumentsCount,
                            const char** commandLineArguments) {
  std::vector<std::string> args{};
  for (int32_t index = 0; index < commandLineArgumentsCount; index++)
    args.emplace_back(commandLineArguments[index]);
  Player* player = g_players->Get(id, args);
  player->SetVideoWidth(video_width);
  player->SetVideoHeight(video_height);
  player->OnPlay([=]() -> void { OnPlayPauseStop(id, player->state()); });
  player->OnPause([=]() -> void { OnPlayPauseStop(id, player->state()); });
  player->OnStop([=]() -> void {
    OnPlayPauseStop(id, player->state());
    OnPosition(id, player->state());
  });
  player->OnComplete([=]() -> void { OnComplete(id, player->state()); });
  player->OnVolume([=](float) -> void { OnVolume(id, player->state()); });
  player->OnRate([=](float) -> void { OnRate(id, player->state()); });
  player->OnPosition([=](int32_t) -> void { OnPosition(id, player->state()); });
  player->OnOpen([=](VLC::Media) -> void { OnOpen(id, player->state()); });
  player->OnPlaylist([=]() -> void { OnOpen(id, player->state()); });
#ifdef _WIN32
/* Windows: Texture & flutter::TextureRegistrar */
#else
  /* Linux: decodeImageFromPixels & NativePorts */
  if (player->video_width() > 0 && player->video_height() > 0) {
    player->OnVideo([=](uint8_t* frame) -> void {
      OnVideo(id, player->video_width() * player->video_height() * 4,
              player->state(), frame);
    });
  }
#endif
}

DLLEXPORT void PlayerDispose(int32_t id) { g_players->Dispose(id); }

DLLEXPORT void PlayerOpen(int32_t id, bool auto_start, const char** source,
                          int32_t source_size) {
  std::vector<std::shared_ptr<Media>> medias{};
  Player* player = g_players->Get(id);
  for (int32_t index = 0; index < 2 * source_size; index += 2) {
    std::shared_ptr<Media> media;
    const char* type = source[index];
    const char* resource = source[index + 1];
    if (strcmp(type, "MediaType.file") == 0)
      media = Media::file(resource, false);
    else if (strcmp(type, "MediaType.network") == 0)
      media = Media::network(resource, false);
    else
      media = Media::directShow(resource);
    medias.emplace_back(media);
  }
  player->Open(std::make_shared<Playlist>(medias), auto_start);
}

DLLEXPORT void PlayerPlay(int32_t id) {
  Player* player = g_players->Get(id);
  player->Play();
}

DLLEXPORT void PlayerPause(int32_t id) {
  Player* player = g_players->Get(id);
  player->Pause();
}

DLLEXPORT void PlayerPlayOrPause(int32_t id) {
  Player* player = g_players->Get(id);
  player->PlayOrPause();
}

DLLEXPORT void PlayerStop(int32_t id) {
  Player* player = g_players->Get(id);
  player->Stop();
}

DLLEXPORT void PlayerNext(int32_t id) {
  Player* player = g_players->Get(id);
  player->Next();
}

DLLEXPORT void PlayerBack(int32_t id) {
  Player* player = g_players->Get(id);
  player->Back();
}

DLLEXPORT void PlayerJump(int32_t id, int32_t index) {
  Player* player = g_players->Get(id);
  player->Jump(index);
}

DLLEXPORT void PlayerSeek(int32_t id, int32_t position) {
  Player* player = g_players->Get(id);
  player->Seek(position);
}

DLLEXPORT void PlayerSetVolume(int32_t id, float volume) {
  Player* player = g_players->Get(id);
  player->SetVolume(volume);
}

DLLEXPORT void PlayerSetRate(int32_t id, float rate) {
  Player* player = g_players->Get(id);
  player->SetRate(rate);
}

DLLEXPORT void PlayerSetUserAgent(int32_t id, const char* userAgent) {
  Player* player = g_players->Get(id);
  player->SetUserAgent(userAgent);
}

DLLEXPORT void PlayerSetDevice(int32_t id, const char* device_id,
                               const char* device_name) {
  Player* player = g_players->Get(id);
  Device device(device_id, device_name);
  player->SetDevice(device);
}

DLLEXPORT void PlayerSetEqualizer(int32_t id, int32_t equalizer_id) {
  Player* player = g_players->Get(id);
  Equalizer* equalizer = g_equalizers->Get(equalizer_id);
  player->SetEqualizer(*equalizer);
}

DLLEXPORT void PlayerSetPlaylistMode(int32_t id, const char* mode) {
  Player* player = g_players->Get(id);
  PlaylistMode playlistMode;
  if (strcmp(mode, "PlaylistMode.repeat") == 0)
    playlistMode = PlaylistMode::repeat;
  else if (strcmp(mode, "PlaylistMode.loop") == 0)
    playlistMode = PlaylistMode::loop;
  else
    playlistMode = PlaylistMode::single;
  player->SetPlaylistMode(playlistMode);
}

DLLEXPORT void PlayerAdd(int32_t id, const char* type, const char* resource) {
  Player* player = g_players->Get(id);
  std::shared_ptr<Media> media;
  if (strcmp(type, "MediaType.file") == 0)
    media = Media::file(resource, false);
  else if (strcmp(type, "MediaType.network") == 0)
    media = Media::network(resource, false);
  else
    media = Media::directShow(resource);
  player->Add(media);
}

DLLEXPORT void PlayerRemove(int32_t id, int32_t index) {
  Player* player = g_players->Get(id);
  player->Remove(index);
}

DLLEXPORT void PlayerInsert(int32_t id, int32_t index, const char* type,
                            const char* resource) {
  Player* player = g_players->Get(id);
  std::shared_ptr<Media> media;
  if (strcmp(type, "MediaType.file") == 0)
    media = Media::file(resource, false);
  else if (strcmp(type, "MediaType.network") == 0)
    media = Media::network(resource, false);
  else
    media = Media::directShow(resource);
  player->Insert(index, media);
}

DLLEXPORT void Player_move(int32_t id, int32_t initial_index,
                           int32_t final_index) {
  Player* player = g_players->Get(id);
  player->Move(initial_index, final_index);
}

// TODO: respect timeout
DLLEXPORT char** MediaParse(const char* type, const char* resource,
                            int32_t timeout) {
  std::shared_ptr<Media> media = Media::create(type, resource, true);
  g_metas_ptr = new char*[media->metas().size()];
  g_metas_size = media->metas().size();
  int32_t index = 0;
  for (const auto& [key, value] : media->metas()) {
    g_metas_ptr[index] = new char[200];
    strncpy(g_metas_ptr[index], value.data(), 200);
    index++;
  }
  return g_metas_ptr;
}

DLLEXPORT void MediaClear() {
  if (g_metas_ptr != nullptr) {
    for (size_t i = 0; i < g_metas_size; i++) {
      delete g_metas_ptr[i];
    }
    delete[] g_metas_ptr;
    g_metas_ptr = nullptr;
    g_metas_size = 0;
  }
}

DLLEXPORT void BroadcastCreate(int32_t id, const char* type,
                               const char* resource, const char* access,
                               const char* mux, const char* dst,
                               const char* vcodec, int32_t vb,
                               const char* acodec, int32_t ab) {
  std::shared_ptr<Media> media = Media::create(type, resource);

  std::unique_ptr<BroadcastConfiguration> configuration =
      std::make_unique<BroadcastConfiguration>(access, mux, dst, vcodec, vb,
                                               acodec, ab);
  g_broadcasts->Get(id, std::move(media), std::move(configuration));
}

DLLEXPORT void BroadcastStart(int32_t id) {
  Broadcast* broadcast = g_broadcasts->Get(id, nullptr, nullptr);
  broadcast->Start();
}

DLLEXPORT void BroadcastDispose(int32_t id) { g_broadcasts->Dispose(id); }

DLLEXPORT void ChromecastCreate(int32_t id, const char* type,
                                const char* resource, const char* ip_address) {
  std::shared_ptr<Media> media = Media::create(type, resource);
  chromecasts->Get(id, std::move(media), ip_address);
}

DLLEXPORT void ChromecastStart(int32_t id) {
  Chromecast* chromecast = chromecasts->Get(id, nullptr, "");
  chromecast->Start();
}

DLLEXPORT void ChromecastDispose(int32_t id) { chromecasts->Dispose(id); }

DLLEXPORT void RecordCreate(int32_t id, const char* saving_file,
                            const char* type, const char* resource) {
  std::shared_ptr<Media> media = Media::create(type, resource);
  g_records->Get(id, media, saving_file);
}

DLLEXPORT void RecordStart(int32_t id) {
  Record* record = g_records->Get(id, nullptr, "");
  record->Start();
}

DLLEXPORT void RecordDispose(int32_t id) { g_records->Dispose(id); }

DLLEXPORT char** DevicesAll() {
  std::vector<Device> devices = Devices::All();
  g_devices_ptr = new char*[(devices.size() * 2) + 1];
  g_devices_size = (devices.size() * 2) + 1;
  g_devices_ptr[0] = new char[200];
  strncpy(g_devices_ptr[0], std::to_string(devices.size()).data(), 200);
  int32_t index = 0;
  for (Device& device : devices) {
    g_devices_ptr[index + 1] = new char[200];
    strncpy(g_devices_ptr[index + 1], device.id().data(), 200);
    g_devices_ptr[index + 2] = new char[200];
    strncpy(g_devices_ptr[index + 2], device.name().data(), 200);
    index += 2;
  }
  return g_devices_ptr;
}

DLLEXPORT void DevicesClear() {
  if (g_devices_ptr != nullptr) {
    for (size_t i = 0; i < g_devices_size; i++) {
      delete g_devices_ptr[i];
    }
    delete[] g_devices_ptr;
    g_devices_ptr = nullptr;
    g_devices_size = 0;
  }
}

DLLEXPORT char** EqualizerCreateEmpty() {
  int32_t id = g_equalizers->CreateEmpty();
  Equalizer* equalizer = g_equalizers->Get(id);
  g_equalizer_ptr = new char*[2 * equalizer->band_amps().size() + 2];
  g_equalizer_size = 2 * equalizer->band_amps().size() + 2;
  g_equalizer_ptr[0] = new char[200];
  strncpy(g_equalizer_ptr[0], std::to_string(id).data(), 200);
  g_equalizer_ptr[1] = new char[200];
  strncpy(g_equalizer_ptr[1], std::to_string(equalizer->pre_amp()).data(), 200);
  int32_t index = 0;
  for (const auto& [band, amp] : equalizer->band_amps()) {
    g_equalizer_ptr[index + 2] = new char[200];
    strncpy(g_equalizer_ptr[index + 2], std::to_string(band).data(), 200);
    g_equalizer_ptr[index + 3] = new char[200];
    strncpy(g_equalizer_ptr[index + 3], std::to_string(amp).data(), 200);
    index += 2;
  }
  return g_equalizer_ptr;
}

DLLEXPORT char** EqualizerCreateMode(int32_t mode) {
  int32_t id = g_equalizers->CreateMode(static_cast<EqualizerMode>(mode));
  Equalizer* equalizer = g_equalizers->Get(id);
  g_equalizer_ptr = new char*[2 * equalizer->band_amps().size() + 2];
  g_equalizer_size = 2 * equalizer->band_amps().size() + 2;
  g_equalizer_ptr[0] = new char[200];
  strncpy(g_equalizer_ptr[0], std::to_string(id).data(), 200);
  g_equalizer_ptr[1] = new char[200];
  strncpy(g_equalizer_ptr[1], std::to_string(equalizer->pre_amp()).data(), 200);
  int32_t index = 0;
  for (const auto& [band, amp] : equalizer->band_amps()) {
    g_equalizer_ptr[index + 2] = new char[200];
    strncpy(g_equalizer_ptr[index + 2], std::to_string(band).data(), 200);
    g_equalizer_ptr[index + 3] = new char[200];
    strncpy(g_equalizer_ptr[index + 3], std::to_string(amp).data(), 200);
    index += 2;
  }
  return g_equalizer_ptr;
}

DLLEXPORT void EqualizerClear() {
  if (g_equalizer_ptr != nullptr) {
    for (size_t i = 0; i < g_equalizer_size; i++) {
      delete g_equalizer_ptr[i];
    }
    delete[] g_equalizer_ptr;
    g_equalizer_ptr = nullptr;
    g_equalizer_size = 0;
  }
}

DLLEXPORT void EqualizerSetBandAmp(int32_t id, float band, float amp) {
  g_equalizers->Get(id)->SetBandAmp(band, amp);
}

DLLEXPORT void EqualizerSetPreAmp(int32_t id, float amp) {
  g_equalizers->Get(id)->SetPreAmp(amp);
}

#endif
#ifdef __cplusplus
}
#endif

