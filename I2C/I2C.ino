 class I2C {
  private:
    uint8_t sda;
    uint8_t scl;
    uint8_t address;
    
    uint8_t* buff;
    uint32_t buffSize;
    
    uint8_t currentBit;
    uint32_t index;
    bool clk;


  void storeBit(int value){
    buff[index] = (buff[index] << 1) | (value & 1);
    currentBit = (currentBit + 1) % 8;
  }

  void printBuffer(){
    for(int i = 0; i < buffSize; i++){
        Serial.print("Index ");
        Serial.print(i);
        Serial.print(" - Data: ");
        Serial.println(buff[i], BIN);
      }
  }
    
  public:

    enum Mode {
      SLAVE,
      MASTER_TX,
      MASTER_RX
    };
  
    I2C(uint8_t sda, uint8_t scl, uint8_t address, uint32_t bufferSize)
    :
    sda(sda),
    scl(scl),
    address(address),
    currentBit(0),
    index(0),
    buffSize(bufferSize)
    {
      this->buff = new uint8_t[bufferSize];
    }
    
    ~I2C()
    {
      delete(this->buff);
    }

    void setMode(Mode mode){
      switch(mode){
        case(SLAVE):
          pinMode(sda, INPUT);
          pinMode(scl, INPUT);
          break;
        case(MASTER_TX):
          pinMode(sda, OUTPUT);
          pinMode(scl, OUTPUT);
          break;
        case(MASTER_RX):
          pinMode(sda, INPUT);
          pinMode(scl, OUTPUT);
          break;
        default:
          break;
      }
    }

    void poll(){
      if(digitalRead(scl) == 1 && clk == 0){
        clk = 1;
        int val = digitalRead(sda);
        Serial.print("SDA: ");
        Serial.println(val);
        // Write to buffer
        storeBit(val);
        if(currentBit == 0)
        {
          Serial.println("Byte Received");
          Serial.print("Data: ");
          Serial.println(buff[index], BIN);
          index++;
          if(index == buffSize){
            printBuffer();
            index = 0;
          }
        }
      } else if(digitalRead(scl) == 0 && clk == 1){
        clk = 0;
      }
    }
 };

I2C one(8, 9, 16, 20);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  one.setMode(I2C::SLAVE);
}

void loop() {
  // put your main code here, to run repeatedly:
  one.poll();
}
