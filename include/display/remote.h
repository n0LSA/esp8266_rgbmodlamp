#ifndef REMOTE_H
#define REMOTE_H

  #include "../libextern.h"
  #include "effectslist.h"
  #include "../alml_webserver.h"


  #ifdef ALS_LAMPUDPMULTI
  class RemoteControl_udp : public UdpMulti, public Udp {
  #else
  class RemoteControl_udp : public Udp {
  #endif
    boolean multiSend_jsonDevice = true;  
    unsigned long timerUdpSend_jsonDevice;    

  public:
    unsigned long timerUdpSync;  
    uint8_t testHue;  

    RemoteControl_udp(){};
    ~RemoteControl_udp(){};
    virtual void send_appi(boolean eff, boolean prog, boolean reset = false, uint8_t effLoad = false);
    virtual void send_toIp(const String & transmit_buffer, IPAddress ip, uint16_t port);
    virtual void handleJson(DynamicJsonDocument & doc, boolean udpMulti = true);
    virtual void handleJson(uint8_t op);

  protected:
    virtual void handle();
    virtual void begin();
    unsigned long timerMultiSend_jsonDevice;   
    
  private:
  };
  // #ifdef ADS_PIN_IR
  // class RemoteControl : public  Effectslist, public RemoteControl_ir, public RemoteControl_udp { 
  // #else
  

  class RemoteControl : public  Effectslist, public RemoteControl_udp {
  public:
    RemoteControl();
    ~RemoteControl(){};
    void handle_udp();
    void begin();
    void send_update(boolean eff = true, boolean reset = true);
  };  



  RemoteControl * RemoteControlPtrGet();  

  void keyboard_print() ;
  void keyboard_getter(const String & v1) ;
#endif // REMOTE_H