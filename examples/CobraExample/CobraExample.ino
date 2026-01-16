/*
    Copyright 2021-2025 Picovoice Inc.

    You may not use this file except in compliance with the license. A copy of the license is located in the "LICENSE"
    file accompanying this source.

    Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
    an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
    specific language governing permissions and limitations under the License.
*/

#include <Cobra.h>

#define MEMORY_BUFFER_SIZE (70 * 1024)

#define RED (22)

static const char *ACCESS_KEY = "${ACCESS_KEY}"; // AccessKey string obtained from Picovoice Console (https://picovoice.ai/console/)

static int8_t memory_buffer[MEMORY_BUFFER_SIZE] __attribute__((aligned(16)));

static pv_cobra_t *handle = NULL;

static const float THRESHOLD = 0.75f;

static void voice_activity_callback(void) {
    Serial.println("Voice activity detected!");
    digitalWrite(RED, LOW); // turn the LED on (LOW is the voltage level)
}

static void no_voice_activity_callback(void) {
    digitalWrite(RED, HIGH); // turn the LED off (HIGH is the voltage level)
}

static void print_error_message(char **message_stack, int32_t message_stack_depth) {
    for (int32_t i = 0; i < message_stack_depth; i++) {
        Serial.println(message_stack[i]);
    }
}

void setup() {

    Serial.begin(9600);
    while (!Serial);

    pv_status_t status = picovoice::cobra::pv_audio_rec_init();
    if (status != PV_STATUS_SUCCESS) {
        Serial.print("Audio init failed with ");
        Serial.println(pv_status_to_string(status));
        while (1);
    }

    char **message_stack = NULL;
    int32_t message_stack_depth = 0;
    pv_status_t error_status;

    status = pv_cobra_init(
        ACCESS_KEY,
        MEMORY_BUFFER_SIZE,
        memory_buffer,
        &handle);
    if (status != PV_STATUS_SUCCESS) {
        Serial.print("Cobra init failed with ");
        Serial.println(pv_status_to_string(status));

        error_status = pv_get_error_stack(&message_stack, &message_stack_depth);
        if (error_status != PV_STATUS_SUCCESS) {
            Serial.println("Unable to get Cobra error state");
            while (1);
        }
        print_error_message(message_stack, message_stack_depth);
        pv_free_error_stack(message_stack);

        while (1);
    }
}

void loop() {
    const int16_t *buffer = picovoice::cobra::pv_audio_rec_get_new_buffer();
    if (buffer) {
        float voice_probability;
        const pv_status_t status = pv_cobra_process(handle, buffer, &voice_probability);
        if (status != PV_STATUS_SUCCESS) {
            Serial.print("Picovoice process failed with ");
            Serial.println(pv_status_to_string(status));
            char **message_stack = NULL;
            int32_t message_stack_depth = 0;
            pv_get_error_stack(
                &message_stack,
                &message_stack_depth);
            for (int32_t i = 0; i < message_stack_depth; i++) {
                Serial.println(message_stack[i]);
            }
            pv_free_error_stack(message_stack);
            while (1);
        }
        if (voice_probability > THRESHOLD) {
            voice_activity_callback();
        } else {
            no_voice_activity_callback();
        }
    }
}
