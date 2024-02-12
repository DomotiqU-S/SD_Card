
#include "sd_card.hpp"
/**
 * @brief Constructor for the SdCard class.
 * 
 * this constructor initializes the SD card and mounts the filesystem.
 * 
 * @return N/A
*/
SdCard::SdCard()
{
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };
    const char mount_point[] = MOUNT_POINT;
    ESP_LOGI(TAG, "Initializing SD card");
    ESP_LOGI(TAG, "Using SPI peripheral");
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    this->host = host;
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };
    this->ret = spi_bus_initialize((spi_host_device_t)this->host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (this->ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize bus.");
        return;
    }
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = PIN_NUM_CS;
    slot_config.host_id = (spi_host_device_t)this->host.slot;
    ESP_LOGI(TAG, "Mounting filesystem");
    this->ret = esp_vfs_fat_sdspi_mount(mount_point, &this->host, &slot_config, &mount_config, &this->card);

    if (this->ret != ESP_OK)
    {
        if (this->ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount filesystem (%s).", esp_err_to_name(this->ret));
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize the card (%s).", esp_err_to_name(this->ret));
        }
        return;
    }
    ESP_LOGI(TAG, "Filesystem mounted");
    sdmmc_card_print_info(stdout, this->card);
    return;
}
/**
 * @brief Destructor for the SdCard class.
 * 
 * this destructor unmounts the filesystem and frees the SPI bus.
 * 
 * @return N/A
*/
SdCard::~SdCard()
{
    esp_vfs_fat_sdcard_unmount(MOUNT_POINT, card);
    spi_bus_free((spi_host_device_t)this->host.slot);
}
/**
 * @brief Writes data to a file.
 * 
 * This function writes data to a file.
 * 
 * @param path the path to the file.
 * @param data the data to write to the file.
 * 
 * @return N/A
*/
void SdCard::WriteFile(const char *path, char *data)
{
    ESP_LOGI(TAG, "Opening file %s", path);
    FILE *f = fopen(path, "w");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing");
        this->ret = ESP_FAIL;
    }
    fprintf(f, data);
    fclose(f);
    ESP_LOGI(TAG, "File written");
    this->ret = ESP_OK;
}
/**
 * @brief Reads data from a file.
 * 
 * This function reads data from a file.
 * 
 * @param path the path to the file.
 * 
 * @return N/A
*/
void SdCard::ReadFile(const char *path)
{
    ESP_LOGI(TAG, "Reading file %s", path);
    FILE *f = fopen(path, "r");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for reading");
        this->ret = ESP_FAIL;
    }
    char line[EXAMPLE_MAX_CHAR_SIZE];
    fgets(line, sizeof(line), f);
    fclose(f);

    // strip newline
    char *pos = strchr(line, '\n');
    if (pos)
    {
        *pos = '\0';
    }
    ESP_LOGI(TAG, "Read from file: '%s'", line);
    this->ret = ESP_OK;
}
/**
 * @brief Gets the return value of the last operation.
 * 
 * This function returns the return value of the last operation.
 * 
 * @return the return value of the last operation.
*/
esp_err_t SdCard::GetRet()
{
    return this->ret;
}