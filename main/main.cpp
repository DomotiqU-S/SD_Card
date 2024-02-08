
#include "sd_card.cpp"

extern "C" void app_main()
{
    Sd_Card *sd_card = new Sd_Card();
    if(sd_card->GetRet() != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize SD card");
        return;
    }
    const char *file_hello = MOUNT_POINT "/hello.txt";
    char data[EXAMPLE_MAX_CHAR_SIZE];
    snprintf(data, EXAMPLE_MAX_CHAR_SIZE, "Hello World!\n");
    sd_card->WriteFile(file_hello, data);
    if (sd_card->GetRet() != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to write file");
        return;
    }
    ESP_LOGI(TAG, "File written");

    sd_card->ReadFile(file_hello);
    if (sd_card->GetRet() != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to read file");
        return;
    }
    ESP_LOGI(TAG, "File Read");
    delete sd_card;
}
