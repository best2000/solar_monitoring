void ina226_setup() {
  Wire.beginTransmission(ina226);
  Wire.write(reg_conf);
  Wire.write(0b01001001);
  Wire.write(0b11100110);       
  Wire.endTransmission();
}

void ina219_setup() {
  Wire.beginTransmission(ina219_1);
  Wire.write(reg_conf);
  Wire.write(0b00111111);
  Wire.write(0b10011110);       
  Wire.endTransmission();
  Wire.beginTransmission(ina219_2);
  Wire.write(reg_conf);
  Wire.write(0b00111111);
  Wire.write(0b10011110);       
  Wire.endTransmission();
  Wire.beginTransmission(ina219_3);
  Wire.write(reg_conf);
  Wire.write(0b00111111);
  Wire.write(0b10011110);       
  Wire.endTransmission();
}

//v_bat
float readv_ina226() {
  Wire.beginTransmission(ina226);
  Wire.write(reg_vbus);                           
  Wire.requestFrom(ina226, 2); 
  byte msb = Wire.read();    
  byte lsb = Wire.read();          
  Wire.endTransmission(); 
  
  int bits = ((msb << 8) | lsb);
  float v = bits*0.00125;         
  return v;
}

//v_pv
float readv_ina219_1() {
  Wire.beginTransmission(ina219_1);
  Wire.write(reg_vbus);                           
  Wire.requestFrom(ina219_1, 2); 
  byte msb = Wire.read();    
  byte lsb = Wire.read();          
  Wire.endTransmission(); 
  
  int bits = ((msb << 5) | (lsb >> 3));
  float v = bits*0.004;
  if (v <= 1.0) {
    return 0;
  } else {
    v *= 5.81818;     
    return v;
  }
  
}

float v_raw;
//i_pv
float readv_ina219_2() {
  Wire.beginTransmission(ina219_2);
  Wire.write(reg_vbus);                           
  Wire.requestFrom(ina219_2, 2); 
  byte msb = Wire.read();    
  byte lsb = Wire.read();          
  Wire.endTransmission(); 
  
  int bits = ((msb << 5) | (lsb >> 3));
  float v = bits*0.004;
  v_raw = v;
  v -= 2.275;
  float i = abs(v/0.066);
  if (i < 0.75) {
    return 0;   
  } else {
    return abs(i); 
  }
}

//i_load 
float readv_ina219_3() {
  Wire.beginTransmission(ina219_3);
  Wire.write(reg_vbus);                           
  Wire.requestFrom(ina219_3, 2); 
  byte msb = Wire.read();    
  byte lsb = Wire.read();          
  Wire.endTransmission(); 
  
  int bits = ((msb << 5) | (lsb >> 3));
  float v = bits*0.004;
  v -= 2.275;
  float i = abs(v/0.04);      
  if (i < 1 || i >= 125.0) {
    return 0;   
  } else {
    return abs(i); 
  }
}

String readv_JSON() {
  return "{\"v_pv\":" + String(readv_ina219_1(), 2) + ",\"i_pv\":"+ String(readv_ina219_2(), 2) + ",\"v_bat\":" + String(readv_ina226(), 2) + ",\"i_load\":"+ String(readv_ina219_3(), 2) + ",\"i_pv_vraw\":"+ String(v_raw, 2)+"}";
}
