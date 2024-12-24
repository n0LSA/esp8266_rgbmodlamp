#ifndef ALML_WEBSERVER_H
#define ALML_WEBSERVER_H

#include <Arduino.h>

#include "libextern.h"

static const char WRMTP_001 [] PROGMEM = "HTTP_GET";
static const char WRMTP_002 [] PROGMEM = "HTTP_POST";
static const char WRMTP_003 [] PROGMEM = "HTTP_DELETE";
static const char WRMTP_004 [] PROGMEM = "HTTP_PUT";
static const char WRMTP_005 [] PROGMEM = "HTTP_PATCH";
static const char WRMTP_006 [] PROGMEM = "HTTP_HEAD";
static const char WRMTP_007 [] PROGMEM = "HTTP_OPTIONS";
static const char WRMTP_008 [] PROGMEM = "HTTP_ANY";
static const char* const WRMTP_ARR[] PROGMEM = {
  WRMTP_001, WRMTP_002, WRMTP_003, WRMTP_004,
  WRMTP_005, WRMTP_006, WRMTP_007, WRMTP_008
};

static const char WS_REGISTER_TP_001 [] PROGMEM = "STA register HTTP_GET request";
static const char WS_REGISTER_TP_002 [] PROGMEM = "STA register HTTP_POST request";

static const char WSTP_001 [] PROGMEM = "text/html";
static const char WSTP_002 [] PROGMEM = "application/json";
static const char WSTP_003 [] PROGMEM = "text/plain";
static const char WSTP_004 [] PROGMEM = "FS INIT ERROR";
static const char WSTP_005 [] PROGMEM = "FileNotFound";
static const char* const WSTP_ARR[] PROGMEM = {
  WSTP_001,           WSTP_002,           WSTP_003, 
  WSTP_004,           WSTP_005
};
typedef enum : uint8_t {
  WSTE_TXTJSON=0,    WSTE_APPJSON,      WSTE_TXTPLAIN,
  WSTE_FSINITERROR,  WSTE_FILENOTFONUD
} ENUM_WSTP;
typedef enum : uint8_t {
  WSRM_FROMDEF=0,    WSRM_FROMCALLBACK
} ENUM_WSRM;

typedef struct  WSREP_FLAG{ 
    ENUM_WSTP _cType;
    ENUM_WSRM _rType ;
} WSREP_flag;
class Webserver {

  typedef std::function<void(const String & v1)> callback_function_t;


  // Webserver_request       * _requestArray       = nullptr;
  uint8_t                 _requestArrayCnt      = 0;
  uint8_t                 _requestArrayCntMax   = 0;

  // String                  _httpCallbackData       = "";
  // AsyncWebServerRequest   * _httpCallbackRequest    = NULL;
  
  AsyncWebSocketClient    * _socketClient     = NULL;
  AsyncWebSocket          * _socketServer     = NULL;
  boolean                 _socketIsConnected  = false;
  callback_function_t     _socketCallback     = nullptr;
  



public:
  boolean _httpTrace = true;
  boolean _socketTrace = true;
  String uploadFOpath = "/";


  Webserver();
  // ~Webserver(){};

  void request_array(uint8_t v1);
  // Webserver_request * get_requestArray(uint8_t p);
  // uint8_t request_new(const char * const & , WebRequestMethod, ENUM_WSTP);
  // uint8_t request_new(const char * const & , WebRequestMethod, ENUM_WSTP, _webServerRequest_f);

  void http_send(AsyncWebServerRequest *, const int &, ENUM_WSTP, const String &);

  // void set_socketCallbackData(const String & v1);


  void begin();
  void setup();
  void setupAp();

  void device_api(DynamicJsonDocument & doc, String & reply) ;

  void handle();  
  void httpHandle(AsyncWebServerRequest * request, const String & v1, uint8_t mod = 0);

  void socket_send(const String & message);
  void set_socketCallback(callback_function_t v1);
  void set_socketIsConnected(boolean v1);
  void set_socketClient(AsyncWebSocketClient * v1);
  void set_socketServer(AsyncWebSocket * v1);
  void socketHandle(const String & v1);  
  boolean socketIsConnected();  
};


#ifdef ALS_LAMPUDPMULTI
  class UdpMulti
  {
    IPAddress _multiIp          = {239, 0, 0, 57};
    uint16_t  _multiPort        = 9200;
    uint8_t   * _packetBuffer   = nullptr;
    uint16_t  _packetBufferSize = 0;
    WiFiUDP   _server;

  public:
    UdpMulti();
    ~UdpMulti(){};

    void send(const String & transmit_buffer);  
    void begin();
    void stop();
    boolean receive();
    void get_packetBuffer(String & result);
  };
#endif


  class UdpClients
  {
    IPAddress _ip;
    uint16_t _port;
    uint32_t _lastReceivedMsg;
  public:
    UdpClients(IPAddress,uint16_t);
    ~UdpClients();
    uint16_t get_port() {return _port;}
    IPAddress get_ip() {return _ip;}
    
  };

  class Udp
  {
    LList<UdpClients *>  _list;

    IPAddress _clientIp;
    uint16_t _clientPort;
    
    // adri_timer   * _timerSendPacket;
    uint16_t  _port       = 4210;
    uint8_t   * _packetBuffer   = nullptr;
    uint16_t  _packetBufferSize = 0;
    WiFiUDP   _server;

  public:
      typedef std::function<void(String)> callback_function_t;


    Udp();
    ~Udp(){};
    void client_add();
    void set_port(uint16_t port);
    void remote_print();
    IPAddress remoteIP();
    bool receive() ;
    void send_toIp(const String & transmit_buffer) ;
    void send_toIp(const String & transmit_buffer, IPAddress ip, uint16_t port) ;
    void send_toIp(const String & transmit_buffer, const char * ip, uint16_t port) ;
    void send_toIp(const String & transmit_buffer, const String & s_ip, const String & s_port);
    void send_toIp(const char* buffer,const String & s_ip, const String & s_port);
    void send_toIp(const char* buffer,const char * ip, uint16_t port) ;
    void send(const String & transmit_buffer);
    void parse();
    void parse_set_func(callback_function_t callback);  
    void begin();
    void stop();
    void loop();
    void get_packetBuffer(String & result);

  private :
    callback_function_t _parseC = nullptr;    
    
  };
    
  // extern WiFiUDP _udpMultiServer;
  extern Webserver _Webserver;
  // extern Udp _Udp;
  // extern UdpMulti _UdpMulti;
  // extern char _udpMulti_packetBuffer[];
  // extern uint8_t * _udpMulti_packetBuffer;
  // extern char _udp_packetBuffer[];
#endif // ALML_WEBSERVER_H