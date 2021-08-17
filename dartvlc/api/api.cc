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

#include <memory>
#include "api.h"

#include "broadcast.h"
#include "chromecast.h"
#include "device.h"
#include "equalizer.h"
#include "player.h"
#include "record.h"

#ifdef __cplusplus
extern "C" {
#endif

void PlayerCreate(int32_t id, int32_t video_width, int32_t video_height,
                  int32_t commandLineArgumentsCount,
                  const char** commandLineArguments) {
  std::vector<std::string> args{};
  for (int32_t index = 0; index < commandLineArgumentsCount; index++)
    args.emplace_back(commandLineArguments[index]);
  Player* player = g_players->Get(id, args);
  if (video_width > 0 && video_height > 0) {
    player->SetVideoWidth(video_width);
    player->SetVideoHeight(video_height);
  }
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
  player->OnVideo([=](uint8_t* frame, int32_t width, int32_t height) -> void {
    OnVideo(id, player->video_width() * player->video_height() * 4,
            player->state(), frame);
  });
#endif
  player->OnVideoDimensions(
      [=](int32_t video_width, int32_t video_height) -> void {
        OnVideoDimensions(id, video_width, video_height);
      });
}

void PlayerDispose(int32_t id) { g_players->Dispose(id); }

void PlayerOpen(int32_t id, bool auto_start, const char** source,
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

void PlayerPlay(int32_t id) {
  Player* player = g_players->Get(id);
  player->Play();
}

void PlayerPause(int32_t id) {
  Player* player = g_players->Get(id);
  player->Pause();
}

void PlayerPlayOrPause(int32_t id) {
  Player* player = g_players->Get(id);
  player->PlayOrPause();
}

void PlayerStop(int32_t id) {
  Player* player = g_players->Get(id);
  player->Stop();
}

void PlayerNext(int32_t id) {
  Player* player = g_players->Get(id);
  player->Next();
}

void PlayerBack(int32_t id) {
  Player* player = g_players->Get(id);
  player->Back();
}

void PlayerJump(int32_t id, int32_t index) {
  Player* player = g_players->Get(id);
  player->Jump(index);
}

void PlayerSeek(int32_t id, int32_t position) {
  Player* player = g_players->Get(id);
  player->Seek(position);
}

void PlayerSetVolume(int32_t id, float volume) {
  Player* player = g_players->Get(id);
  player->SetVolume(volume);
}

void PlayerSetRate(int32_t id, float rate) {
  Player* player = g_players->Get(id);
  player->SetRate(rate);
}

void PlayerSetUserAgent(int32_t id, const char* userAgent) {
  Player* player = g_players->Get(id);
  player->SetUserAgent(userAgent);
}

void PlayerSetDevice(int32_t id, const char* device_id,
                     const char* device_name) {
  Player* player = g_players->Get(id);
  Device device(device_id, device_name);
  player->SetDevice(device);
}

void PlayerSetEqualizer(int32_t id, int32_t equalizer_id) {
  Player* player = g_players->Get(id);
  Equalizer* equalizer = g_equalizers->Get(equalizer_id);
  player->SetEqualizer(*equalizer);
}

void PlayerSetPlaylistMode(int32_t id, const char* mode) {
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

void PlayerAdd(int32_t id, const char* type, const char* resource) {
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

void PlayerRemove(int32_t id, int32_t index) {
  Player* player = g_players->Get(id);
  player->Remove(index);
}

void PlayerInsert(int32_t id, int32_t index, const char* type,
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

void PlayerMove(int32_t id, int32_t initial_index, int32_t final_index) {
  Player* player = g_players->Get(id);
  player->Move(initial_index, final_index);
}

void MediaClearMap(void*, void* peer) {
  delete reinterpret_cast<std::map<std::string, std::string>*>(peer);
}

void MediaClearVector(void*, void* peer) {
  delete reinterpret_cast<std::vector<const char*>*>(peer);
}

const char** MediaParse(Dart_Handle object, const char* type,
                        const char* resource, int32_t timeout) {
  std::shared_ptr<Media> media = Media::create(type, resource, true, timeout);
  auto metas = new std::map<std::string, std::string>(media->metas());
  auto values = new std::vector<const char*>();
  Dart_NewFinalizableHandle_DL(
      object, reinterpret_cast<void*>(metas), sizeof(metas),
      static_cast<Dart_HandleFinalizer>(MediaClearMap));
  Dart_NewFinalizableHandle_DL(
      object, reinterpret_cast<void*>(values), sizeof(values),
      static_cast<Dart_HandleFinalizer>(MediaClearVector));
  for (const auto & [ key, value ] : *metas) {
    values->emplace_back(value.c_str());
  }
  return values->data();
}

void BroadcastCreate(int32_t id, const char* type, const char* resource,
                     const char* access, const char* mux, const char* dst,
                     const char* vcodec, int32_t vb, const char* acodec,
                     int32_t ab) {
  std::shared_ptr<Media> media = Media::create(type, resource);

  std::unique_ptr<BroadcastConfiguration> configuration =
      std::make_unique<BroadcastConfiguration>(access, mux, dst, vcodec, vb,
                                               acodec, ab);
  g_broadcasts->Get(id, std::move(media), std::move(configuration));
}

void BroadcastStart(int32_t id) {
  Broadcast* broadcast = g_broadcasts->Get(id, nullptr, nullptr);
  broadcast->Start();
}

void BroadcastDispose(int32_t id) { g_broadcasts->Dispose(id); }

void ChromecastCreate(int32_t id, const char* type, const char* resource,
                      const char* ip_address) {
  std::shared_ptr<Media> media = Media::create(type, resource);
  chromecasts->Get(id, std::move(media), ip_address);
}

void ChromecastStart(int32_t id) {
  Chromecast* chromecast = chromecasts->Get(id, nullptr, "");
  chromecast->Start();
}

void ChromecastDispose(int32_t id) { chromecasts->Dispose(id); }

void RecordCreate(int32_t id, const char* saving_file, const char* type,
                  const char* resource) {
  std::shared_ptr<Media> media = Media::create(type, resource);
  g_records->Get(id, media, saving_file);
}

void RecordStart(int32_t id) {
  Record* record = g_records->Get(id, nullptr, "");
  record->Start();
}

void RecordDispose(int32_t id) { g_records->Dispose(id); }

void DevicesClear(void*, void* peer) {
  delete reinterpret_cast<std::vector<const char*>*>(peer);
}

struct DevicesStruct DevicesAll(Dart_Handle object) {
  auto devices = new std::vector<Device>(Devices::All());
  auto ids = new std::vector<const char*>();
  auto names = new std::vector<const char*>();
  for (int i = 0; i < devices->size(); i++) {
    ids->emplace_back((*devices)[i].id().c_str());
    names->emplace_back((*devices)[i].name().c_str());
  }
  Dart_NewFinalizableHandle_DL(object, reinterpret_cast<void*>(devices),
                               sizeof(devices),
                               static_cast<Dart_HandleFinalizer>(DevicesClear));
  Dart_NewFinalizableHandle_DL(object, reinterpret_cast<void*>(ids),
                               sizeof(ids),
                               static_cast<Dart_HandleFinalizer>(DevicesClear));
  Dart_NewFinalizableHandle_DL(object, reinterpret_cast<void*>(names),
                               sizeof(names),
                               static_cast<Dart_HandleFinalizer>(DevicesClear));
  struct DevicesStruct devices_struct;
  devices_struct.device_ids = ids->data();
  devices_struct.device_names = names->data();
  devices_struct.size = devices->size();
  return devices_struct;
}

void EqualizerClear(void*, void* peer) {
  delete reinterpret_cast<std::vector<float>*>(peer);
}

struct EqualizerStruct EqualizerCreateEmpty(Dart_Handle object) {
  int32_t id = g_equalizers->CreateEmpty();
  Equalizer* equalizer = g_equalizers->Get(id);
  auto bands = new std::vector<float>();
  auto amps = new std::vector<float>();
  Dart_NewFinalizableHandle_DL(
      object, reinterpret_cast<void*>(bands), sizeof(bands),
      static_cast<Dart_HandleFinalizer>(EqualizerClear));
  Dart_NewFinalizableHandle_DL(
      object, reinterpret_cast<void*>(amps), sizeof(amps),
      static_cast<Dart_HandleFinalizer>(EqualizerClear));
  for (const auto & [ band, amp ] : equalizer->band_amps()) {
    bands->emplace_back(band);
    amps->emplace_back(amp);
  }
  struct EqualizerStruct equalizer_struct;
  equalizer_struct.id = id;
  equalizer_struct.pre_amp = equalizer->pre_amp();
  equalizer_struct.bands = bands->data();
  equalizer_struct.amps = amps->data();
  equalizer_struct.size = equalizer->band_amps().size();
  return equalizer_struct;
}

struct EqualizerStruct EqualizerCreateMode(Dart_Handle object, int32_t mode) {
  int32_t id = g_equalizers->CreateMode(static_cast<EqualizerMode>(mode));
  Equalizer* equalizer = g_equalizers->Get(id);
  auto bands = new std::vector<float>();
  auto amps = new std::vector<float>();
  Dart_NewFinalizableHandle_DL(
      object, reinterpret_cast<void*>(bands), sizeof(bands),
      static_cast<Dart_HandleFinalizer>(EqualizerClear));
  Dart_NewFinalizableHandle_DL(
      object, reinterpret_cast<void*>(amps), sizeof(amps),
      static_cast<Dart_HandleFinalizer>(EqualizerClear));
  for (const auto & [ band, amp ] : equalizer->band_amps()) {
    bands->emplace_back(band);
    amps->emplace_back(amp);
  }
  struct EqualizerStruct equalizer_struct;
  equalizer_struct.id = id;
  equalizer_struct.pre_amp = equalizer->pre_amp();
  equalizer_struct.bands = bands->data();
  equalizer_struct.amps = amps->data();
  equalizer_struct.size = equalizer->band_amps().size();
  return equalizer_struct;
}

void EqualizerSetBandAmp(int32_t id, float band, float amp) {
  g_equalizers->Get(id)->SetBandAmp(band, amp);
}

void EqualizerSetPreAmp(int32_t id, float amp) {
  g_equalizers->Get(id)->SetPreAmp(amp);
}

#ifdef __cplusplus
}
#endif