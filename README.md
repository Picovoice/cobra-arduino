# Cobra SDK for Arduino boards

Made in Vancouver, Canada by [Picovoice](https://picovoice.ai)

Cobra is a highly-accurate and lightweight voice activity detection (VAD) engine. It enables building always-listening voice-enabled
applications. It is

- using deep neural networks trained in real-world environments.
- compact and computationally-efficient. It is perfect for IoT.
- scalable. It can detect voice activities with no added runtime footprint.

Cobra VAD for MCU is currently in Beta and available for evaluation for 30 minutes per device, (i.e., user). If you are working on a commercial project or require extended access, please contact [Picovoice](https://picovoice.ai/contact) with your project details.

## Compatibility

- [Arduino Nano 33 BLE Sense](https://docs.arduino.cc/hardware/nano-33-ble)

## Dependency

- LibPrintf

## AccessKey

The Cobra SDK requires a valid `AccessKey` at initialization. `AccessKey`s act as your credentials when using Cobra SDKs.
You can create your `AccessKey` for free. Make sure to keep your `AccessKey` secret.

To obtain your `AccessKey`:

1. Login or Signup for a free account on the [Picovoice Console](https://picovoice.ai/console/).
2. Once logged in, go to the [`AccessKey` tab](https://console.picovoice.ai/access_key) to create one or use an existing `AccessKey`.

## Integration

1. Define all the necessary variables before `setup()`:

```c
#include <Cobra.h>

#define MEMORY_BUFFER_SIZE ...
static uint8_t memory_buffer[MEMORY_BUFFER_SIZE] __attribute__((aligned(16));

static const char* ACCESS_KEY = ...; //AccessKey string obtained from [Picovoice Console](https://picovoice.ai/console/)

pv_cobra_t *handle = NULL;
```

`handle` is an instance of Cobra runtime engine that detects voice activities.

2. Put the following code block inside `setup()` in order to initialize the Cobra engine:

```c
const pv_status_t status = pv_cobra_init(
        ACCESS_KEY,
        MEMORY_BUFFER_SIZE,
        memory_buffer,
        &handle);

if (status != PV_STATUS_SUCCESS) {
    // error handling logic
}
```

Cobra accepts single channel, 16-bit PCM audio. The sample rate can be retrieved using `pv_sample_rate()`. Cobra accepts input audio in consecutive chunks (aka frames); the length of each frame can be retrieved using `pv_cobra_frame_length()`. Inside the `loop()` function in the sketch, pass the recorded audio to the Cobra engine:

```cpp
const int16_t *pcm = picovoice::cobra::pv_audio_rec_get_new_buffer()
float voice_probability;
const pv_status_t status = pv_cobra_process(handle, pcm, &voice_probability);
if (status != PV_STATUS_SUCCESS) {
    // error handling logic
}
```

