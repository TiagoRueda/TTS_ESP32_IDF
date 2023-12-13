#include "main.h"

static void init_i2s(void)
{
    static const i2s_config_t i2s_config = {
        .mode = I2S_MODE_TX,
        .sample_rate = 44100,
        .bits_per_sample = 16,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false
    };

    static const i2s_pin_config_t pin_config = {
        .bck_io_num = CONFIG_I2S_BCK_PIN,
        .ws_io_num = CONFIG_I2S_WS_PIN,
        .data_out_num = CONFIG_I2S_DATA_PIN
    };

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
}

int i2s_asc(const cst_wave *wave, int start, int size, int last, cst_audio_streaming_info *audio_info){
	if (start == 0){
		i2s_set_sample_rates(I2S_NUM_0, wave->sample_rate);
	}

	size_t bytes = 0;

    i2s_write(I2S_NUM_0, &(wave->samples[start]), size*sizeof(uint16_t), &bytes, 100);

    ESP_LOGI(TAG, "Transmitted %d bytes", bytes);

    return CST_AUDIO_STREAM_CONT;
}


void task_audio(void *pvParameters){
	flite_init();
    cst_voice *voice = register_cmu_us_kal(NULL);
    cst_audio_streaming_info *audio_info = cst_alloc(struct cst_audio_streaming_info_struct,1);


    audio_info->min_buffsize = 256;
    audio_info->asc = i2s_asc;
    audio_info->userdata = NULL;

    feat_set(voice->features,"streaming_info",audio_streaming_info_val(audio_info));
    for (;;){
        char *text_to_synth = NULL;

        if (xQueueReceive(text_queue, &text_to_synth, portMAX_DELAY) != pdTRUE){
            ESP_LOGI(TAG, "Failed to receive data from the queue");
        }
        else{
            ESP_LOGI(TAG, "String %s received in task_audio", text_to_synth);

            cst_wave *wave = flite_text_to_wave(text_to_synth, voice);
            delete_wave(wave);

            free(text_to_synth);
        }
    }
}

void set_text_to_synth(const char *text){
    char *point = strdup(text);

    if (xQueueSend(text_queue, (void *)(&point), portMAX_DELAY) != pdTRUE){
        ESP_LOGI(TAG, "Failed to send the text string to the synthesis task queue");
        free(point);
    }
}

void app_main()
{
	init_i2s();

    text_queue = xQueueCreate(32, sizeof(char *));

    ESP_LOGI(TAG, "Queue create %s", (text_queue == NULL) ? "Failed" : "OK");

    BaseType_t code = xTaskCreatePinnedToCore(task_audio, "audio task", 10000, NULL, 20, NULL, 0);

    ESP_LOGI(TAG, "Task create %s", (code != pdPASS) ? "Failed" : "OK");

    set_text_to_synth("   Text-to-Speech ESP32   ");
}
