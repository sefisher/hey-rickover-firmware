#include "config.h"
#include "ChunkedUploader.h"

#ifdef USE_WITAI
#include "WiFiClientSecure.h"
#endif

#ifdef USE_UDP_STREAM
#include <WiFiUdp.h>
#endif

//TODO - fix to remove need for access_key

ChunkedUploader::ChunkedUploader(const char *access_key)
{
#ifdef USE_WITAI
    Serial.printf("Connecting...");
    m_wifi_client = new WiFiClientSecure();
    Serial.println(m_wifi_client->connected());
    m_wifi_client->setInsecure();
    m_wifi_client->connect("api.wit.ai", 443);
    Serial.println(m_wifi_client->connected());
    char authorization_header[100];
    snprintf(authorization_header, 100, "authorization: Bearer %s", access_key);
    m_wifi_client->println("POST /speech?v=20200927 HTTP/1.1");
    m_wifi_client->println("host: api.wit.ai");
    m_wifi_client->println(authorization_header);
    m_wifi_client->println("content-type: audio/raw; encoding=signed-integer; bits=16; rate=16000; endian=little");
    m_wifi_client->println("transfer-encoding: chunked");
    m_wifi_client->println();
#endif
#ifdef USE_UDP_STREAM
    m_wifi_client = new WiFiUDP();
    m_wifi_client->begin(SERVER_PORT);
#endif
    //Serial.begin(115200);
}

bool ChunkedUploader::connected()
{
#ifdef USE_WITAI
    Serial.printf("WITAI - Testing connection.");
    return m_wifi_client->connected();

#endif
#ifdef USE_UDP_STREAM
    return true;
#endif
    return true;
}

void ChunkedUploader::startChunk(int size_in_bytes)
{
#ifdef USE_WITAI
    Serial.printf("WITAI - Start Chunk - ");
    m_wifi_client->printf("%X\r\n", size_in_bytes);
#endif
#ifdef USE_UDP_STREAM
    //m_wifi_client->beginPacket(SERVER_IP, SERVER_PORT);
    //Serial.println("Started UDP.");
#endif
}

void ChunkedUploader::sendChunkData(const uint8_t *data, int size_in_bytes)
{
#ifdef USE_WITAI
    Serial.printf(" data - ");
    m_wifi_client->write(data, size_in_bytes);
#endif
#ifdef USE_UDP_STREAM
    m_wifi_client->beginPacket(SERVER_IP, SERVER_PORT);
    m_wifi_client->write(data, size_in_bytes);
    m_wifi_client->endPacket();
    //Serial.print(".");
#endif
}

void ChunkedUploader::finishChunk()
{
#ifdef USE_WITAI
    Serial.printf(" ending.\r\n");
    m_wifi_client->print("\r\n");
#endif
#ifdef USE_UDP_STREAM
    //m_wifi_client->endPacket();
    //Serial.println("Ended UDP.");
#endif
}

Intent ChunkedUploader::getResults()
{
#ifdef USE_WITAI
    Serial.printf("WITAI - get results.\r\n ");
    // finish the chunked request by sending a zero length chunk
    m_wifi_client->print("0\r\n");
    m_wifi_client->print("\r\n");
    // get the headers and the content length
    int status = -1;
    int content_length = 0;
    while (m_wifi_client->connected())
    {
        char buffer[255];
        int read = m_wifi_client->readBytesUntil('\n', buffer, 255);
        if (read > 0)
        {
            buffer[read] = '\0';
            // blank line indicates the end of the headers
            if (buffer[0] == '\r')
            {
                break;
            }
            if (strncmp("HTTP", buffer, 4) == 0)
            {
                sscanf(buffer, "HTTP/1.1 %d", &status);
            }
            else if (strncmp("Content-Length:", buffer, 15) == 0)
            {
                sscanf(buffer, "Content-Length: %d", &content_length);
            }
        }
    }
    Serial.printf("Http status is %d with content length of %d\n", status, content_length);
    if (status == 200)
    {
        StaticJsonDocument<500> filter;
        filter["entities"]["device:device"][0]["value"] = true;
        filter["entities"]["device:device"][0]["confidence"] = true;
        filter["text"] = true;
        filter["intents"][0]["name"] = true;
        filter["intents"][0]["confidence"] = true;
        filter["traits"]["wit$on_off"][0]["value"] = true;
        filter["traits"]["wit$on_off"][0]["confidence"] = true;
        StaticJsonDocument<500> doc;
        deserializeJson(doc, *m_wifi_client, DeserializationOption::Filter(filter));

        const char *text = doc["text"];
        const char *intent_name = doc["intents"][0]["name"];
        float intent_confidence = doc["entities"]["device:device"][0]["confidence"];
        const char *device_name = doc["entities"]["device:device"][0]["value"];
        float device_confidence = doc["entities"]["device:device"][0]["confidence"];
        const char *trait_value = doc["traits"]["wit$on_off"][0]["value"];
        float trait_confidence = doc["traits"]["wit$on_off"][0]["confidence"];

        return Intent{
            .text = (text ? text : ""),
            .intent_name = (intent_name ? intent_name : ""),
            .intent_confidence = intent_confidence,
            .device_name = (device_name ? device_name : ""),
            .device_confidence = device_confidence,
            .trait_value = (trait_value ? trait_value : ""),
            .trait_confidence = trait_confidence};
    }
#endif

#ifdef USE_UDP_STREAM
    return Intent{.text = "UDP"};
#endif
    return Intent{};
}

ChunkedUploader::~ChunkedUploader()
{
#ifdef USE_WITAI
    delete m_wifi_client;
#endif
#ifdef USE_UDP_STREAM
    delete m_wifi_client;
#endif
}