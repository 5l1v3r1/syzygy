// Copyright 2015 Google Inc. All Rights Reserved.
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
// Specialization for the Event interface that admits dependencies between
// events.
#ifndef SYZYGY_BARD_EVENTS_LINKED_EVENT_H_
#define SYZYGY_BARD_EVENTS_LINKED_EVENT_H_

#include <set>

#include "base/synchronization/waitable_event.h"
#include "syzygy/bard/event.h"

namespace bard {
namespace events {

// Specialization for Event interface that admits dependencies between
// events through causal links. However, it is still an abstract class
// that needs extension and implementation of PlayImpl.
class LinkedEvent : public EventInterface {
 public:
  LinkedEvent();

  // LinkedEvent dependencies setter.
  void AddPrequel(LinkedEvent* prequel);

  // @name EventInterface implementation.
  // @{
  bool Play(void* backdrop) override;
  // @}

 protected:
  // Play method to be implemented by classes who inherit a LinkedEvent.
  virtual bool PlayImpl(void* backdrop) = 0;

 private:
  base::WaitableEvent waitable_event_;

  // The prequel events must be played before this one.
  std::set<LinkedEvent*> prequels_;

  DISALLOW_COPY_AND_ASSIGN(LinkedEvent);
};

}  // namespace events
}  // namespace bard

#endif  // SYZYGY_BARD_EVENTS_LINKED_EVENT_H_
