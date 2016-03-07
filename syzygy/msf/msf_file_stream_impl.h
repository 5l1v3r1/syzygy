// Copyright 2012 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Internal implementation details for msf_file_stream.h. Not meant to be
// included directly.

#ifndef SYZYGY_MSF_MSF_FILE_STREAM_IMPL_H_
#define SYZYGY_MSF_MSF_FILE_STREAM_IMPL_H_

#include <algorithm>
#include <cstdio>

#include "base/logging.h"
#include "syzygy/msf/msf_decl.h"

namespace msf {
namespace detail {

template <MsfFileType T>
MsfFileStreamImpl<T>::MsfFileStreamImpl(RefCountedFILE* file,
                                        size_t length,
                                        const uint32_t* pages,
                                        size_t page_size)
    : MsfStreamImpl(length), file_(file), page_size_(page_size) {
  size_t num_pages = (length + page_size - 1) / page_size;
  pages_.assign(pages, pages + num_pages);
}

template <MsfFileType T>
MsfFileStreamImpl<T>::~MsfFileStreamImpl() {
}

template <MsfFileType T>
bool MsfFileStreamImpl<T>::ReadBytes(void* dest,
                                     size_t count,
                                     size_t* bytes_read) {
  DCHECK(dest != NULL);
  DCHECK(bytes_read != NULL);

  // Return 0 once we've reached the end of the stream.
  if (pos() == length()) {
    *bytes_read = 0;
    return true;
  }

  // Don't read beyond the end of the known stream length.
  count = std::min(count, length() - pos());
  *bytes_read = count;

  // Read the stream.
  while (count > 0) {
    size_t page_index = pos() / page_size_;
    size_t offset = pos() % page_size_;
    size_t chunk_size = std::min(count, page_size_ - (pos() % page_size_));
    if (!ReadFromPage(dest, pages_[page_index], offset, chunk_size))
      return false;

    count -= chunk_size;
    Seek(pos() + chunk_size);
    dest = reinterpret_cast<uint8_t*>(dest) + chunk_size;
  }

  return true;
}

template <MsfFileType T>
bool MsfFileStreamImpl<T>::ReadFromPage(void* dest,
                                        uint32_t page_num,
                                        size_t offset,
                                        size_t count) {
  DCHECK(dest != NULL);
  DCHECK(offset + count <= page_size_);

  size_t page_offset = page_size_ * page_num;
  if (fseek(file_->file(), page_offset + offset, SEEK_SET) != 0) {
    LOG(ERROR) << "Page seek failed";
    return false;
  }

  if (fread(dest, 1, count, file_->file()) != static_cast<size_t>(count)) {
    LOG(ERROR) << "Page read failed";
    return false;
  }

  return true;
}

}  // namespace detail
}  // namespace msf

#endif  // SYZYGY_MSF_MSF_FILE_STREAM_IMPL_H_
