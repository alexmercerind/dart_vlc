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

#include "api.h"

#include "broadcast.h"
#include "chromecast.h"
#include "device.h"
#include "equalizer.h"
#include "player.h"
#include "record.h"

namespace DartObjects {

struct DeviceList {
  // The device list that gets exposed to Dart.
  DartDeviceList dart_object;

  // Backing data
  std::vector<DartDeviceList::Device> device_infos;
  std::vector<Device> devices;
};

struct Equalizer {
  // The equalizer that gets exposed to Dart.
  DartEqualizer dart_object;

  // Backing data
  std::vector<float> bands;
  std::vector<float> amps;
};

static void DestroyObject(void*, void* peer) { delete peer; }

}  // namespace DartObjects

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
  player->OnBuffering(
      [=](float buffering) -> void { OnBuffering(id, buffering); });
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

void PlayerTakeSnapshot(int32_t id, const char* file_path, int32_t width,
                        int32_t height) {
  Player* player = g_players->Get(id);
  player->TakeSnapshot(file_path, width, height);
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
  for (const auto& [key, value] : *metas) {
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

DartDeviceList* DevicesAll(Dart_Handle object) {
  auto wrapper = new DartObjects::DeviceList();
  wrapper->devices = Devices::All();

  for (const auto& device : wrapper->devices) {
    wrapper->device_infos.emplace_back(device.name().c_str(),
                                       device.id().c_str());
  }

  wrapper->dart_object.size = wrapper->device_infos.size();
  wrapper->dart_object.device_infos = wrapper->device_infos.data();

  Dart_NewFinalizableHandle_DL(
      object, wrapper, sizeof(*wrapper),
      static_cast<Dart_HandleFinalizer>(DartObjects::DestroyObject));
  return &wrapper->dart_object;
}

static DartEqualizer* EqualizerToDart(const Equalizer* equalizer, int32_t id,
                                      Dart_Handle dart_handle) {
  auto wrapper = new DartObjects::Equalizer();
  for (const auto& [band, amp] : equalizer->band_amps()) {
    wrapper->bands.emplace_back(band);
    wrapper->amps.emplace_back(amp);
  }

  wrapper->dart_object.id = id;
  wrapper->dart_object.pre_amp = equalizer->pre_amp();
  wrapper->dart_object.bands = wrapper->bands.data();
  wrapper->dart_object.amps = wrapper->amps.data();
  wrapper->dart_object.size = wrapper->amps.size();

  Dart_NewFinalizableHandle_DL(
      dart_handle, wrapper, sizeof(*wrapper),
      static_cast<Dart_HandleFinalizer>(DartObjects::DestroyObject));

  return &wrapper->dart_object;
}

struct DartEqualizer* EqualizerCreateEmpty(Dart_Handle object) {
  int32_t id = g_equalizers->CreateEmpty();
  Equalizer* equalizer = g_equalizers->Get(id);
  return EqualizerToDart(equalizer, id, object);
}

struct DartEqualizer* EqualizerCreateMode(Dart_Handle object, int32_t mode) {
  int32_t id = g_equalizers->CreateMode(static_cast<EqualizerMode>(mode));
  Equalizer* equalizer = g_equalizers->Get(id);
  return EqualizerToDart(equalizer, id, object);
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