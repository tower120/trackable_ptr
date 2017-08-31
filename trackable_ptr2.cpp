#include "trackable_ptr2.h"

TrackerBase::TrackerBase(TrackableBase* trackable) {
    this->trackable = trackable;

    if (trackable->tracker_head == nullptr){
        trackable->tracker_head = this;
        return;
    }

    TrackerBase* prev_head = trackable->tracker_head;
    this->next = prev_head;

    prev_head->prev = this;

    trackable->tracker_head = this;

}

TrackerBase::~TrackerBase() {
    if(!trackable) return;

    if (!prev && !next) {
        trackable->tracker_head = nullptr;
        return;
    }
    else if (!prev){
        next->prev = nullptr;
        trackable->tracker_head = next;
    }
    else if (!next){
        prev->next = nullptr;
    }
    else {
        prev->next = next->prev;
    }
}

TrackerBase::TrackerBase(TrackerBase &&other) {
    next = other.next;
    prev = other.prev;

    trackable = other.trackable;

    other.trackable = nullptr;

    if (prev){
        prev->next = this;
    }

    if (next){
        next->prev = this;
    }
}