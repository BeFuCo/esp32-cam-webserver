// code snippets taken from 
// https://raydiy.de/esp32-cam-platformio-ueberwachungskamera-mit-live-video-stream-per-webserver/
// René Aye: Thank's for your great tutorial

#include <Arduino.h>
#include <WiFi.h>

#include <esp_camera.h>       // ESP-Functions for fetching camera picuteres
#include <esp_http_server.h>  // ESP-Functions for webserver to display camera pictures

// Replace with your network credentials
const char* ssid = "gateway03";
const char* password = "65505213651803934831";

// change the hostname 
// code snippet taken from https://deepbluembedded.com/esp32-hostname-change-arduino/
// To change the ESP32 hostname, you need to call the WiFi.setHostname() function before calling 
// WiFi.mode() and then WiFi.begin() in this exact same order.
// Functions for setting the new WiFi host name shall be performed in this order
// Thanks for this great tutorial
const char* hostname = "esp32cam01";

// definition of strings required for HTTP-protocol
#define PART_BOUNDARY "123456789000000000000987654321"
static const char *_STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char *_STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char *_STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\nX-Timestamp: %d.%06d\r\n\r\n";

httpd_handle_t stream_httpd = NULL;

static esp_err_t stream_handler(httpd_req_t *req)
{
    camera_fb_t *fb = NULL;
    struct timeval _timestamp;
    esp_err_t res = ESP_OK;
    size_t _jpg_buf_len = 0;
    uint8_t *_jpg_buf = NULL;
    char *part_buf[128];

    static int64_t last_frame = 0;
    if (!last_frame)
    {
        last_frame = esp_timer_get_time();
    }

    res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
    if (res != ESP_OK)
    {
        return res;
    }

    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_hdr(req, "X-Framerate", "60");

    while (true)
    {
        fb = esp_camera_fb_get();
        if (!fb)
        {
            Serial.println("Camera capture failed");
            res = ESP_FAIL;
        }
        else
        {
            _timestamp.tv_sec = fb->timestamp.tv_sec;
            _timestamp.tv_usec = fb->timestamp.tv_usec;
            if (fb->format != PIXFORMAT_JPEG)
            {
                bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
                esp_camera_fb_return(fb);
                fb = NULL;
                if (!jpeg_converted)
                {
                    Serial.println("JPEG compression failed");
                    res = ESP_FAIL;
                }
            }
            else
            {
                _jpg_buf_len = fb->len;
                _jpg_buf = fb->buf;
            }
        }

        if (res == ESP_OK)
        {
            res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
        }
        if (res == ESP_OK)
        {
            size_t hlen = snprintf((char *)part_buf, 128, _STREAM_PART, _jpg_buf_len, _timestamp.tv_sec, _timestamp.tv_usec);
            res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
        }
        if (res == ESP_OK)
        {
            res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
        }
        if (fb)
        {
            esp_camera_fb_return(fb);
            fb = NULL;
            _jpg_buf = NULL;
        }
        else if (_jpg_buf)
        {
            free(_jpg_buf);
            _jpg_buf = NULL;
        }
        if (res != ESP_OK)
        {
            Serial.println("Send frame failed");
            break;
        }

        int64_t fr_end = esp_timer_get_time();
        int64_t frame_time = fr_end - last_frame;
        frame_time /= 1000;
    }

    return res;
}

void startCameraServer()
{
    Serial.println("startCameraServer()");

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;

    httpd_uri_t index_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = stream_handler,
        .user_ctx = NULL};

    Serial.printf("Starting stream server on port: '%d'", config.server_port);
    if (httpd_start(&stream_httpd, &config) == ESP_OK)
    {
        httpd_register_uri_handler(stream_httpd, &index_uri);
    }
}

//////////////////////////////////////////// Setup()
void setup() {

  Serial.begin(9600);

  // WiFi config hostname
  WiFi.setHostname(hostname);
  WiFi.mode(WIFI_STA);

  // configuration of CAMERA Model AI Thinker start
      // #define CAMERA_MODEL_AI_THINKER // Has PSRAM
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = 5; //Y2_GPIO_NUM
    config.pin_d1 = 18; //Y3_GPIO_NUM
    config.pin_d2 = 19; //Y4_GPIO_NUM
    config.pin_d3 = 21; //Y5_GPIO_NUM
    config.pin_d4 = 36; //Y6_GPIO_NUM
    config.pin_d5 = 39; //Y7_GPIO_NUM
    config.pin_d6 = 34; //Y8_GPIO_NUM
    config.pin_d7 = 35; //Y9_GPIO_NUM
    config.pin_xclk = 0; //XCLK_GPIO_NUM
    config.pin_pclk = 22; //PCLK_GPIO_NUM
    config.pin_vsync = 25; //VSYNC_GPIO_NUM
    config.pin_href = 23; //HREF_GPIO_NUM
    config.pin_sscb_sda = 26; //SIOD_GPIO_NUM
    config.pin_sscb_scl = 27; //SIOC_GPIO_NUM
    config.pin_pwdn = 32; //PWDN_GPIO_NUM
    config.pin_reset = -1; //RESET_GPIO_NUM
    config.xclk_freq_hz = 20000000;
    config.frame_size = FRAMESIZE_UXGA;
    config.pixel_format = PIXFORMAT_JPEG; // for streaming
    config.grab_mode = CAMERA_GRAB_LATEST;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  // configuration of CAMERA Model AI Thinker end
  
  // Check whether the ESP32 Cam Board has additional PSRAM. 
  // If so, the resolution is set to UXGA (1600x1200).
  if(psramFound())
  {
      config.frame_size = FRAMESIZE_UXGA; // 1600x1200
      config.jpeg_quality = 10;
      config.fb_count = 2;
  } 
  else 
  {
      config.frame_size = FRAMESIZE_SVGA; // 800x600
      config.jpeg_quality = 12;
      config.fb_count = 1;
  }

  // Camara init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
      Serial.printf("Camera init failed with error 0x%x", err);
      return;
  }


  // Connect to Wi-Fi
  Serial.print("Connecting to WiFi ...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  // Print ESP Local IP Address
  Serial.println(" connected!");
  Serial.print("local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("ESP32-Cam Webserver Hostname: ");
  Serial.println(WiFi.getHostname());
  Serial.print("RSSI: ");
  Serial.print(WiFi.RSSI());
  Serial.println("dBm");

  /* Start the camera streaming server */
  startCameraServer();
}

//////////////////////////////////////////// loop()
void loop() {}