#ifndef _witai_chunked_uploaded_h_
#define _witai_chunked_uploaded_h_

#include <stdint.h>
#include <string>

#ifdef USE_WITAI
class WiFiClientSecure;
#endif

typedef struct
{
    std::string text;
    std::string intent_name;
    float intent_confidence;
    std::string device_name;
    float device_confidence;
    std::string trait_value;
    float trait_confidence;
} Intent;

class ChunkedUploader
{
private:
#ifdef USE_WITAI
    WiFiClientSecure *m_wifi_client;
#endif
#ifdef USE_UDP_STREAM
    WiFiUDP *m_wifi_client;
#endif

public:
    ChunkedUploader(const char *access_key);
    ~ChunkedUploader();
    bool connected();
    void startChunk(int size_in_bytes);
    void sendChunkData(const uint8_t *data, int size_in_bytes);
    void finishChunk();
    Intent getResults();
};

#endif