// sherpa-onnx/csrc/sherpa-alsa.h
//
// Copyright (c)  2022-2023  Xiaomi Corporation

#ifndef SHERPA_ONNX_CSRC_ALSA_H_
#define SHERPA_ONNX_CSRC_ALSA_H_

#include <memory>
#include <vector>

#include "alsa/asoundlib.h"
#include "sherpa-onnx/csrc/resample.h"

namespace sherpa_onnx {

class Alsa {
 public:
  explicit Alsa(
    const char *device_name, 
    int32_t period_size = 170, 
    int32_t buffer_size = 1365
  );
  
  ~Alsa();

  // This is a blocking read.
  //
  // @param num_samples  Number of samples to read.
  //
  // The returned value is valid until the next call to Read().
  const std::vector<float> &Read(int32_t num_samples);

  int32_t GetExpectedSampleRate() const { return expected_sample_rate_; }
  int32_t GetActualSampleRate() const { return actual_sample_rate_; }

 private:
  snd_pcm_t *capture_handle_;
  int32_t expected_sample_rate_ = 16000;
  int32_t actual_sample_rate_;

  int32_t actual_channel_count_ = 1;
  int32_t period_size_;  // 默认170
  int32_t buffer_size_;  // 默认1365

  std::unique_ptr<LinearResample> resampler_;
  std::vector<int16_t> samples_;  // directly from the microphone
  std::vector<float> samples1_;   // normalized version of samples_
  std::vector<float> samples2_;   // possibly resampled from samples1_
};

}  // namespace sherpa_onnx

#endif  // SHERPA_ONNX_CSRC_ALSA_H_
