#include "../../include/pixel/MatrixParam.h"
#include <altoolslib.h>

void MatrixParam::get_width(uint16_t & result){
  result = _width;
}
void MatrixParam::get_height(uint16_t & result){
  result = _height;
}
void MatrixParam::get_width_v(uint16_t & result){
  result = _width_v;
}
void MatrixParam::get_segements(uint8_t & result){
  result = _segments;
}
void MatrixParam::get_type(bool & result){
  result = _matrixType;
}
void MatrixParam::get_rotatedMatrix(bool & result){
  result = _rotated_matrix;
}

MatrixParam::MatrixParam(uint8_t p){
#ifdef ADS_MATRIXPARAM
  char    buffer[80];
  String  buffer_str;
  int           rSize;
  const char**  list;

  buffer[0]=0;
  sprintf(buffer, "%s", ADS_MATRIXPARAM);
  buffer_str = String(buffer);

  list = al_tools::explode(buffer_str, ',', rSize);  
  if (rSize < 0) {
    list = al_tools::explode(buffer_str, '.', rSize);
    if (rSize>0){
      _width            = atoi(list[0]);
      _height           = atoi(list[1]);
      _matrixType       = atoi(list[2]);
      _connectionAngle  = atoi(list[3]);
      _stripDirection   = atoi(list[4]);
      for(int i = 0; i < rSize; ++i) {delete list[i];}delete[] list;        
    }
  } else {
    for(int i = 0; i < rSize; ++i) Serial.printf("1 [%d] %s\n", i, list[i]);  
    buffer_str = al_tools::ch_toString(list[p]);  
    Serial.println(buffer_str);
    for(int i = 0; i < rSize; ++i) {delete list[i];}delete[] list;
    list = al_tools::explode(buffer_str, '.', rSize);  
    if (rSize>0){
      for(int i = 0; i < rSize; ++i) Serial.printf("2 [%d] %s\n", i, list[i]);  
      _width            = atoi(list[0]);
      _height           = atoi(list[1]);
      _matrixType       = atoi(list[2]);
      _connectionAngle  = atoi(list[3]);
      _stripDirection   = atoi(list[4]);
      for(int i = 0; i < rSize; ++i) {delete list[i];}delete[] list;        
    }
  }

  if (_connectionAngle == 0 && _stripDirection == 0){
    _width_v = _width;
  }
  else if (_connectionAngle == 0 && _stripDirection == 1){
    _width_v = _height;
    _rotated_matrix = true;
  }
  else if (_connectionAngle == 1 && _stripDirection == 0){
    _width_v = _width;
  }
  else if (_connectionAngle == 1 && _stripDirection == 3){
    _width_v = _height;
    _rotated_matrix = true;
  }
  else if (_connectionAngle == 2 && _stripDirection == 2){
    _width_v = _width;
  }
  else if (_connectionAngle == 2 && _stripDirection == 3){
    _width_v = _height;
    _rotated_matrix = true;
  }
  else if (_connectionAngle == 3 && _stripDirection == 2){
    _width_v = _width;
  }
  else if (_connectionAngle == 3 && _stripDirection == 1){
    _width_v = _height;
    _rotated_matrix = true;
  }
  else{
    _width_v = _width;
  }  
#endif
}
void MatrixParam::thisXY(const uint16_t & mh, const uint16_t & mv, const uint16_t & x, const uint16_t & y, uint32_t & rX, uint32_t & rY){
  if (_connectionAngle == 0 && _stripDirection == 0){
    rX = (mv ? (_width - x - 1) : x);
    rY = (mh ? (_height - y - 1) : y);
  }
  else if (_connectionAngle == 0 && _stripDirection == 1){
    rX = (mv ? (_height - y - 1) : y);
    rY = (mh ? (_width - x - 1) : x);
  }
  else if (_connectionAngle == 1 && _stripDirection == 0){
    rX = (mv ? (_width - x - 1) : x);
    rY = (mh ? y : (_height - y - 1));
  }
  else if (_connectionAngle == 1 && _stripDirection == 3){
    rX = (mv ? y : (_height - y - 1));
    rY = (mh ? (_width - x - 1) : x);
  }
  else if (_connectionAngle == 2 && _stripDirection == 2){
    rX = (mv ? x : (_width - x - 1));
    rY = (mh ? y : (_height - y - 1));
  }
  else if (_connectionAngle == 2 && _stripDirection == 3){
    rX = (mv ? y : (_height - y - 1));
    rY = (mh ? x : (_width - x - 1));
  }
  else if (_connectionAngle == 3 && _stripDirection == 2){
    rX = (mv ? x : (_width - x - 1));
    rY = (mh ? (_height - y - 1) : y);
  }
  else if (_connectionAngle == 3 && _stripDirection == 1){
    rX = (mv ? (_height - y - 1) : y);
    rY = (mh ? x : (_width - x - 1));
  }
  else{
    rX = x;
    rY = y;
  } 
}