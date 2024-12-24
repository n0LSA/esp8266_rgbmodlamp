
#include <almllib.h>

void setup() {
  Serial.begin(115200);

  for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime > 5000); ) { }
  delay(1000);

  HeapStatu::setupHeap_v1();

  Serial.println();
  Serial.printf_P(PSTR("\n#############\nAdrilighting MultiStrip\n#############\n\n"));

  #ifdef FILESYSTEM
    #if defined(ESP8266)
      FILESYSTEM.begin();
    #elif defined(ESP32)
      FILESYSTEM.begin(true);
    #endif
  #endif

  #ifdef ALT_DEBUG_TARCE
    ALT_debugBuffer = new char[1024];  
    _DebugPrintList.add("main");  
    #ifdef DEBUG_ALMLREMOTE
      _DebugPrintList.add(ALML_DEBUGREGION_REMOTE);    
    #endif
    #ifdef DEBUG_TASK
      _DebugPrintList.add(ALML_DEBUGREGION_TASK);    
    #endif
    #ifdef DEBUG_WEBSERVER
      _DebugPrintList.add(ALML_DEBUGREGION_WEBSERVER);  
    #endif  
    _DebugPrintList.add(ALML_DEBUGREGION_EFFECT);  
    _DebugPrintList.add(WCEVO_DEBUGREGION_WCEVO);  
    _DebugPrintList.add(WCEVO_DEBUGREGION_AP);  
    _DebugPrintList.add(WCEVO_DEBUGREGION_STA);  
  #endif 

  #ifdef DEBUG_KEYBOARD
    _Sr_menu.add((const char *)"alml_api",    (const char *)"y", []() { keyboard_print(); });
    _Sr_menu.add((const char *)"alml_getter", (const char *)"!", [](const String & v1, const String & v2) {  
      Serial.printf_P(PSTR("CMD: %s - VAL: %s\n"), v1.c_str(), v2.c_str());
      keyboard_getter(v1);    
    }, SR_MM::SRMM_KEYVAL);  
    _Sr_menu.add((const char *)"wifi_api",    (const char *)"r", []() { WCEVO_managerPtrGet()->keyboard_print(); });
    _Sr_menu.add((const char *)"wifi_getter", (const char *)"@", [](const String & v1, const String & v2) {  
      Serial.printf("CMD: %s - VAL: %s\n", v1.c_str(), v2.c_str());
      WCEVO_managerPtrGet()->keyboard_getter(v1);    
    }, SR_MM::SRMM_KEYVAL);    
  #endif 

  WCEVO_managerPtrGet()->set_cm(WCEVO_CM_STAAP);
  WCEVO_managerPtrGet()->set_cmFail(WCEVO_CF_NEXTAP);
  WCEVO_managerPtrGet()->credentials_add((const char *)"SSID_1", (const char *)"PSK_1");
  WCEVO_managerPtrGet()->credentials_add((const char *)"SSID_2", (const char *)"PSK_2");
  // #ifdef ESP8266
  //   WiFi.setPhyMode(WIFI_PHY_MODE_11N);
  // #endif  
  const size_t cfgSize = 0x4000;
  size_t cfgAddr = ESP.getFlashChipSize() - cfgSize;

  for (size_t offset = 0; offset < cfgSize; offset += SPI_FLASH_SEC_SIZE) {
      if (!ESP.flashEraseSector((cfgAddr + offset) / SPI_FLASH_SEC_SIZE)) {
          break;
      }
  }
  WiFi.disconnect(false);
  WiFi.persistent(false);
  WiFi.begin();
  WiFi.mode(WIFI_OFF);
  delay(200);   
  WiFi.enableSTA(true);

  new Device(ADS_NAME, ADS_OC);
}

void loop() {
  DevicePtrGet()->loop();
  // WCEVO_managerPtrGet()->handleConnection();
}

