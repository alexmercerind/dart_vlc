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

#ifndef UTILS_INSTANCE_HANDLER_H_
#define UTILS_INSTANCE_HANDLER_H_

#include <memory>
#include <mutex>
#include <unordered_map>

template <typename type>
class InstanceHandler {
 public:
  type* Get(int32_t id);

  void Create(int32_t id, std::unique_ptr<type> instance);

  size_t Count();

  void Dispose(int32_t id);

 private:
  std::mutex mutex_;
  std::unordered_map<int32_t, std::unique_ptr<type>> instances;
};

template <typename type>
type* InstanceHandler<type>::Get(int32_t id) {
  std::lock_guard<std::mutex> lock(mutex_);
  auto it = instances.find(id);
  if (it == instances.end()) {
    return nullptr;
  }
  return it->second.get();
}

template <typename type>
void InstanceHandler<type>::Create(int32_t id, std::unique_ptr<type> instance) {
  std::lock_guard<std::mutex> lock(mutex_);
  instances.insert(std::make_pair(id, std::move(instance)));
}

template <typename type>
size_t InstanceHandler<type>::Count() {
  std::lock_guard<std::mutex> lock(mutex_);
  return instances.size();
}

template <typename type>
void InstanceHandler<type>::Dispose(int32_t id) {
  std::lock_guard<std::mutex> lock(mutex_);
  instances.erase(id);
}

#endif
