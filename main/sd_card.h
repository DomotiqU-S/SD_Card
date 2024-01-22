#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

#define EXAMPLE_MAX_CHAR_SIZE 64

static const char *TAG = "SD";

#define MOUNT_POINT "/sdcard"

// Pin assignments can be set in menuconfig, see "SD SPI Example Configuration" menu.
// You can also change the pin assignments here by changing the following 4 lines.
#define PIN_NUM_MISO 37
#define PIN_NUM_MOSI 35
#define PIN_NUM_CLK 36
#define PIN_NUM_CS 34

typedef struct SdCard
{
   sdmmc_host_t host;
   sdmmc_card_t *card;
}SdCard;

static esp_err_t SdCard_WriteFile(const char *path, char *data);
static esp_err_t SdCard_ReadFile(const char *path);
static esp_err_t SdCard_Init(struct SdCard* this);
static void SdCard_Deinit(struct SdCard* this);
static esp_err_t SdCard_Test(void);