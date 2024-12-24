boolean WS2812FX::isRunning() {
  return _running;
}

boolean WS2812FX::isTriggered() {
  return _triggered;
}

boolean WS2812FX::isFrame() {
  return isFrame(0);
}

boolean WS2812FX::isFrame(uint8_t seg) {
  uint8_t* ptr = (uint8_t*)memchr(_active_segments, seg, _active_segments_len);
  if(ptr == NULL) return false; // segment not active
  return (_segment_runtimes[ptr - _active_segments].aux_param2 & FRAME);
}

boolean WS2812FX::isCycle() {
  return isCycle(0);
}

boolean WS2812FX::isCycle(uint8_t seg) {
  uint8_t* ptr = (uint8_t*)memchr(_active_segments, seg, _active_segments_len);
  if(ptr == NULL) return false; // segment not active
  return (_segment_runtimes[ptr - _active_segments].aux_param2 & CYCLE);
}

void WS2812FX::setCycle() {
  SET_CYCLE;
}

uint8_t WS2812FX::getMode(void) {
  return getMode(0);
}

uint8_t WS2812FX::getMode(uint8_t seg) {
  return _segments[seg].mode;
}

uint16_t WS2812FX::getSpeed(void) {
  return getSpeed(0);
}

uint16_t WS2812FX::getSpeed(uint8_t seg) {
  return _segments[seg].speed;
}

uint8_t WS2812FX::getOptions(uint8_t seg) {
  return _segments[seg].options;
}

uint16_t WS2812FX::getLength(void) {
  return effectSetting->numLeds;
}

uint16_t WS2812FX::getNumBytes(void) {
  return 0;
}

uint8_t WS2812FX::getNumBytesPerPixel(void) {
  return 0; // 3=RGB, 4=RGBW
}

// uint8_t WS2812FX::getModeCount(void) {
//   return MODE_COUNT;
// }

uint8_t WS2812FX::getNumSegments(void) {
  return _num_segments;
}

void WS2812FX::setNumSegments(uint8_t n) {
  _num_segments = n;
}

uint32_t WS2812FX::getColor(void) {
  return getColor(0);
}

uint32_t WS2812FX::getColor(uint8_t seg) {
  return _segments[seg].colors[0];
}

uint32_t* WS2812FX::getColors(uint8_t seg) {
  return _segments[seg].colors;
}

WS2812FX::Segment* WS2812FX::getSegment(void) {
  return _seg;
}

WS2812FX::Segment* WS2812FX::getSegment(uint8_t seg) {
  return &_segments[seg];
}

WS2812FX::Segment* WS2812FX::getSegments(void) {
  return _segments;
}

WS2812FX::Segment_runtime* WS2812FX::getSegmentRuntime(void) {
  return _seg_rt;
}

WS2812FX::Segment_runtime* WS2812FX::getSegmentRuntime(uint8_t seg) {
  uint8_t* ptr = (uint8_t*)memchr(_active_segments, seg, _active_segments_len);
  if(ptr == NULL) return NULL; // segment not active
  return &_segment_runtimes[ptr - _active_segments];
}

WS2812FX::Segment_runtime* WS2812FX::getSegmentRuntimes(void) {
  return _segment_runtimes;
}

uint8_t* WS2812FX::getActiveSegments(void) {
  return _active_segments;
}

// const __FlashStringHelper* WS2812FX::getModeName(uint8_t m) {
//   if(m < MODE_COUNT) {
//     // return _names[m];
//      return F("");
//   } else {
//     return F("");
//   }
// }

void WS2812FX::setIdleSegment(uint8_t n, uint16_t start, uint16_t stop, uint8_t mode, uint32_t color, uint16_t speed, uint8_t options) {
  uint32_t colors[] = {color, 0, 0};
  setIdleSegment(n, start, stop, mode, colors, speed, options);
}

void WS2812FX::setIdleSegment(uint8_t n, uint16_t start, uint16_t stop, uint8_t mode, const uint32_t colors[], uint16_t speed, uint8_t options) {
  setSegment(n, start, stop, mode, colors, speed, options);
  if(n < _active_segments_len) removeActiveSegment(n);;
}

void WS2812FX::setSegment(uint8_t n, uint16_t start, uint16_t stop, uint8_t mode, uint32_t color,           uint16_t speed, bool reverse) {
  uint32_t colors[] = {color, 0, 0};
  setSegment(n, start, stop, mode, colors, speed, (uint8_t)(reverse ? REVERSE : NO_OPTIONS));
}

void WS2812FX::setSegment(uint8_t n, uint16_t start, uint16_t stop, uint8_t mode, const uint32_t colors[],  uint16_t speed, bool reverse) {
  setSegment(n, start, stop, mode, colors, speed, (uint8_t)(reverse ? REVERSE : NO_OPTIONS));
}

void WS2812FX::setSegment(uint8_t n, uint16_t start, uint16_t stop, uint8_t mode, uint32_t color,           uint16_t speed, uint8_t options) {
  uint32_t colors[] = {color, 0, 0};
  setSegment(n, start, stop, mode, colors, speed, options);
}

void WS2812FX::setSegment(uint8_t n, uint16_t start, uint16_t stop, uint8_t mode, const uint32_t colors[],  uint16_t speed, uint8_t options) {
  if(n < _segments_len) {
    if(n + 1 > _num_segments) _num_segments = n + 1;
    _segments[n].start = start;
    _segments[n].stop = stop;
    _segments[n].mode = mode;
    _segments[n].speed = speed;
    _segments[n].options = options;

    setColors(n, (uint32_t*)colors);

    if(n < _active_segments_len) addActiveSegment(n);
  }
}

void WS2812FX::addActiveSegment(uint8_t seg) {
  uint8_t* ptr = (uint8_t*)memchr(_active_segments, seg, _active_segments_len);
  if(ptr != NULL) return; // segment already active
  for(uint8_t i=0; i<_active_segments_len; i++) {
    if(_active_segments[i] == INACTIVE_SEGMENT) {
      _active_segments[i] = seg;
      resetSegmentRuntime(seg);
      break;
    }
  }
}

void WS2812FX::removeActiveSegment(uint8_t seg) {
  for(uint8_t i=0; i<_active_segments_len; i++) {
    if(_active_segments[i] == seg) {
      _active_segments[i] = INACTIVE_SEGMENT;
    }
  }
}

void WS2812FX::swapActiveSegment(uint8_t oldSeg, uint8_t newSeg) {
  uint8_t* ptr = (uint8_t*)memchr(_active_segments, newSeg, _active_segments_len);
  if(ptr != NULL) return; // if newSeg is already active, don't swap
  for(uint8_t i=0; i<_active_segments_len; i++) {
    if(_active_segments[i] == oldSeg) {
      _active_segments[i] = newSeg;

      // reset all runtime parameters EXCEPT next_time,
      // allowing the current animation frame to complete
      // segment_runtime seg_rt = _segment_runtimes[i];
      _segment_runtimes[i].counter_mode_step = 0;
      _segment_runtimes[i].counter_mode_call = 0;
      _segment_runtimes[i].aux_param = 0;
      _segment_runtimes[i].aux_param2 = 0;
      _segment_runtimes[i].aux_param3 = 0;
      break;
    }
  }
}

boolean WS2812FX::isActiveSegment(uint8_t seg) {
  uint8_t* ptr = (uint8_t*)memchr(_active_segments, seg, _active_segments_len);
  if(ptr != NULL) return true;
  return false;
}

void WS2812FX::resetSegments() {
  resetSegmentRuntimes();
  memset(_segments, 0, _segments_len * sizeof(Segment));
  memset(_active_segments, INACTIVE_SEGMENT, _active_segments_len);
  _num_segments = 0;
}

void WS2812FX::resetSegmentRuntimes() {
  // memset(_segment_runtimes, 0, _active_segments_len * sizeof(Segment_runtime));
  for(uint8_t i=0; i<_segments_len; i++) {
    resetSegmentRuntime(i);
  };
}

void WS2812FX::resetSegmentRuntime(uint8_t seg) {
  uint8_t* ptr = (uint8_t*)memchr(_active_segments, seg, _active_segments_len);
  if(ptr == NULL) return; // segment not active
  _segment_runtimes[seg].next_time = 0;
  _segment_runtimes[seg].counter_mode_step = 0;
  _segment_runtimes[seg].counter_mode_call = 0;
  _segment_runtimes[seg].aux_param = 0;
  _segment_runtimes[seg].aux_param2 = 0;
  _segment_runtimes[seg].aux_param3 = 0;
  // don't reset any external data source
}