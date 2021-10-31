
// The secrets.h file contains the senstive parameters below.  It is excluded from Git. You can add your own secrets.h
// file or comment this line out and enter the parameters directly below.
#include "secrets.h"

// WiFi credentials
#ifndef WIFI_SSID
#define WIFI_SSID "PUT YOUR SSID"
#endif
#ifndef WIFI_PSWD
#define WIFI_PSWD "PUT YOUR PASS"
#endif

//AUDIO PROCESSING SETTINGS ----------------------------------------------------------

//Uncomment the streamer/intent processor you want to use
//Also use Secrets.h to store the relavent information (port, server)
#define USE_WITAI
//#define USE_UDP_STREAM

// Audio command recognition settings for Wit AI - normally set these in secrets.h
#ifndef COMMAND_RECOGNITION_ACCESS_KEY
#define COMMAND_RECOGNITION_ACCESS_KEY "PUT YOUR ACCESS KEY"
#endif

//Audio command processing setting for UDP Streaming - normally set these in secrets.h
//FOR UDP STREAMEING AUDIO
#ifndef SERVER_IP
#define SERVER_IP "PUT YOUR AUDIO PROCESSING MACHINE IP HERE"
#endif
#ifndef SERVER_PORT
#define SERVER_PORT 4444 //set the port number your machine is listening to UDP broadcasts on
#endif

//AUDIO RECORDING AND PLAYBACK SETTINGS ------------------------------------------------

// are you using an I2S microphone - comment this out if you want to use an analog mic and ADC input
#define USE_I2S_MIC_INPUT

// I2S Microphone Settings

// Which channel is the I2S microphone on? I2S_CHANNEL_FMT_ONLY_LEFT or I2S_CHANNEL_FMT_ONLY_RIGHT
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT
// #define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_RIGHT
#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_26     //SCLK or BCLK
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_22 //LRCL or WS
#define I2S_MIC_SERIAL_DATA GPIO_NUM_21      //SD or DOUT

// Analog Microphone Settings - ADC1_CHANNEL_7 is GPIO35
#define ADC_MIC_CHANNEL ADC1_CHANNEL_7

// speaker settings
#define I2S_SPEAKER_SERIAL_CLOCK GPIO_NUM_19
#define I2S_SPEAKER_LEFT_RIGHT_CLOCK GPIO_NUM_27
#define I2S_SPEAKER_SERIAL_DATA GPIO_NUM_18
