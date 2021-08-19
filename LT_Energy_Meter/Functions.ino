
float lineFreq()
{
  byte d[] = {0x11, 0x04, 0x00, 0x2C, 0x00, 0x02, 0xB2, 0x92};
  byte result[10];
  int count = 0;
  digitalWrite(modbus, HIGH);  // Enable RS485 Transmit
  SSerial.write(d, 8);
  delay(10);
  digitalWrite(modbus, LOW);  // Disable RS485 Transmit
  while (SSerial.available())
  {
    byteReceived = SSerial.read();
    result[count] = byteReceived;
    count++;
  }
  if ( SSerial.readBytes(result, 9) == 9)
  {
    float r = result[5] * 256 + result[6];
    return (r * 0.01);
  }
  else return 0;
}

float voltageRphase()
{
  byte d[] = {0x11, 0x04, 0x00, 0x00, 0x00, 0x02, 0x73, 0x5B };
  byte result[10];
  int count = 0;
  digitalWrite(modbus, HIGH);  // Enable RS485 Transmit
  SSerial.write(d, 8);
  delay(10);
  digitalWrite(modbus, LOW);  // Disable RS485 Transmit
  while (SSerial.available())
  {
    byteReceived = SSerial.read();
    result[count] = byteReceived;
    count++;
  }
  if ( SSerial.readBytes(result, 9) == 9)
  {
    float r = result[5] * 256 + result[6];
    return (r * 0.01);
  }
  else return 0;
}

float voltageYphase()
{
  byte d[] = {0x11, 0x04, 0x00, 0x02, 0x00, 0x02, 0xD2, 0x9B };
  byte result[10];
  int count = 0;
  digitalWrite(modbus, HIGH);  // Enable RS485 Transmit
  SSerial.write(d, 8);
  delay(10);
  digitalWrite(modbus, LOW);  // Disable RS485 Transmit
  while (SSerial.available())
  {
    byteReceived = SSerial.read();
    result[count] = byteReceived;
    count++;
  }
  if ( SSerial.readBytes(result, 9) == 9)
  {
    float r = result[5] * 256 + result[6];
    return (r * 0.01);
  }
  else return 0;

}

float voltageBphase()
{
  byte d[] = {0x11, 0x04, 0x00, 0x04, 0x00, 0x02, 0x32, 0x9A};
  byte result[10];
  int count = 0;
  digitalWrite(modbus, HIGH);  // Enable RS485 Transmit
  SSerial.write(d, 8);
  delay(10);
  digitalWrite(modbus, LOW);  // Disable RS485 Transmit
  while (SSerial.available())
  {
    byteReceived = SSerial.read();
    result[count] = byteReceived;
    count++;
  }
  if ( SSerial.readBytes(result, 9) == 9)
  {
    float r = result[5] * 256 + result[6];
    return (r * 0.01);
  }
  else return 0;
}


void printResult(FirebaseData &data)
{

  if (data.dataType() == "int")
    Serial.println(data.intData());
  else if (data.dataType() == "float")
    Serial.println(data.floatData(), 5);
  else if (data.dataType() == "double")
    printf("%.9lf\n", data.doubleData());
  else if (data.dataType() == "boolean")
    Serial.println(data.boolData() == 1 ? "true" : "false");
  else if (data.dataType() == "string")
    Serial.println(data.stringData());
  else if (data.dataType() == "json")
  {
    Serial.println();
    FirebaseJson &json = data.jsonObject();
    //Print all object data
    Serial.println("Pretty printed JSON data:");
    String jsonStr;
    json.toString(jsonStr, true);
    Serial.println(jsonStr);
    Serial.println();
    Serial.println("Iterate JSON data:");
    Serial.println();
    size_t len = json.iteratorBegin();
    String key, value = "";
    int type = 0;
    for (size_t i = 0; i < len; i++)
    {
      json.iteratorGet(i, type, key, value);
      Serial.print(i);
      Serial.print(", ");
      Serial.print("Type: ");
      Serial.print(type == FirebaseJson::JSON_OBJECT ? "object" : "array");
      if (type == FirebaseJson::JSON_OBJECT)
      {
        Serial.print(", Key: ");
        Serial.print(key);
      }
      Serial.print(", Value: ");
      Serial.println(value);
    }
    json.iteratorEnd();
  }
  else if (data.dataType() == "array")
  {
    Serial.println();
    //get array data from FirebaseData using FirebaseJsonArray object
    FirebaseJsonArray &arr = data.jsonArray();
    //Print all array values
    Serial.println("Pretty printed Array:");
    String arrStr;
    arr.toString(arrStr, true);
    Serial.println(arrStr);
    Serial.println();
    Serial.println("Iterate array values:");
    Serial.println();
    for (size_t i = 0; i < arr.size(); i++)
    {
      Serial.print(i);
      Serial.print(", Value: ");

      FirebaseJsonData &jsonData = data.jsonData();
      //Get the result data from FirebaseJsonArray object
      arr.get(jsonData, i);
      if (jsonData.typeNum == FirebaseJson::JSON_BOOL)
        Serial.println(jsonData.boolValue ? "true" : "false");
      else if (jsonData.typeNum == FirebaseJson::JSON_INT)
        Serial.println(jsonData.intValue);
      else if (jsonData.typeNum == FirebaseJson::JSON_FLOAT)
        Serial.println(jsonData.floatValue);
      else if (jsonData.typeNum == FirebaseJson::JSON_DOUBLE)
        printf("%.9lf\n", jsonData.doubleValue);
      else if (jsonData.typeNum == FirebaseJson::JSON_STRING ||
               jsonData.typeNum == FirebaseJson::JSON_NULL ||
               jsonData.typeNum == FirebaseJson::JSON_OBJECT ||
               jsonData.typeNum == FirebaseJson::JSON_ARRAY)
        Serial.println(jsonData.stringValue);
    }
  }
  else if (data.dataType() == "blob")
  {

    Serial.println();

    for (int i = 0; i < data.blobData().size(); i++)
    {
      if (i > 0 && i % 16 == 0)
        Serial.println();

      if (i < 16)
        Serial.print("0");

      Serial.print(data.blobData()[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
  else if (data.dataType() == "file")
  {

    Serial.println();

    File file = data.fileStream();
    int i = 0;

    while (file.available())
    {
      if (i > 0 && i % 16 == 0)
        Serial.println();

      int v = file.read();

      if (v < 16)
        Serial.print("0");

      Serial.print(v, HEX);
      Serial.print(" ");
      i++;
    }
    Serial.println();
    file.close();
  }
  else
  {
    Serial.println(data.payload());
  }
}
