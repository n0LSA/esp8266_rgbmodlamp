#ifndef _MATRIXPARAM_H
#define _MATRIXPARAM_H

#include <Arduino.h>

class MatrixParam {
  // uint16_t   _this_x;
  // uint16_t   _this_y;
  uint8_t   _segments         = 0;
  uint16_t  _width_v          = 0;
  uint16_t  _width            = 0;
  uint16_t  _height           = 0; 
  bool      _matrixType       = false;
  uint8_t   _stripDirection   = 0;
  uint8_t   _connectionAngle  = 0;
  bool      _rotated_matrix   = false;  
public:
  MatrixParam(uint8_t p);
  ~MatrixParam(){};

  void thisXY(const uint16_t & mh, const uint16_t & mv, const uint16_t & x, const uint16_t & y, uint32_t & rX, uint32_t & rY);
  void get_width(uint16_t & result);
  void get_height(uint16_t & result);
  void get_width_v(uint16_t & result);
  void get_segements(uint8_t & result);
  void get_type(bool & result);
  void get_rotatedMatrix(bool & result);
};

#endif