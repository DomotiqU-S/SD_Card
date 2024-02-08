#ifndef SD_CARD_HPP
#define SD_CARD_HPP

#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

#define EXAMPLE_MAX_CHAR_SIZE 64    // Maximum size of the data to write to the file.
#define PIN_NUM_MISO (gpio_num_t)37 // The pin number for the MISO line.
#define PIN_NUM_MOSI (gpio_num_t)35 // The pin number for the MOSI line.
#define PIN_NUM_CLK  (gpio_num_t)36 // The pin number for the clock line.
#define PIN_NUM_CS   (gpio_num_t)34 // The pin number for the chip select line.
#define MOUNT_POINT "/sdcard"       // The mount point for the SD card.

static const char *TAG = "SD";      // The tag for the ESP_LOGI macro.

class Sd_Card{
private:
   sdmmc_host_t host;
   sdmmc_card_t *card;
   esp_err_t ret;
public:
   Sd_Card();
   ~Sd_Card();
   void WriteFile(const char *path, char *data);
   void ReadFile(const char *path);
   esp_err_t GetRet();
};

#endif // SD_CARD_HPP