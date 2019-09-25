 class I2C {
  private:
    enum State {
      WAIT,
      START,
      DATA
    };

    uint8_t sda;
    uint8_t scl;
    uint8_t address;
    
    uint8_t* buff;
    uint32_t buffSize;
    
    uint8_t currentBit;
    uint32_t index;
    State state;
    bool prevClk;
    bool prevData;


  bool storeBit(int value){
    buff[index] = (buff[index] << 1) | (value & 1);
    currentBit = (currentBit + 1) % 8;
    bool byteComplete = false;
    if(currentBit == 0)
    {
      byteComplete = true;
    }
    return byteComplete;
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
    buffSize(bufferSize),
    prevClk(false),
    prevData(false),
    state(State::WAIT)
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
      bool clk = digitalRead(scl);
      bool data = digitalRead(sda);
      switch(state)
      {
        case WAIT:
          // Start condition triggers when the sda line is changed low while the scl
          // is one
          if(clk == 1 && prevClk == 1 && !data && prevData)
          {
            Serial.println("START BIT RECEIVED");
            currentBit = 0;
            index = 0;
            state = START;
          }
          break;
        case START:
          state = DATA;
          break;
        case DATA:
          // Only update data on the rising edge
          if(clk == 1 && prevClk == 0)
          {
            Serial.print("SDA: ");
            Serial.println(data);
            // Write to buffer
            // Will return true if a byte was completed
            if(storeBit(data))
            {
              Serial.println("Byte Received");
              Serial.print("Data: ");
              Serial.println(buff[index], BIN);
              index++;
              if(index == buffSize)
              {
                printBuffer();
                index = 0;
              }
            }
          }
          // Start condition triggers when the sda line is changed high while the scl
          // is one
          else if(clk == 1 && prevClk == 1 && data && !prevData)
          {
            Serial.println("STOP BIT RECEIEVED");
            state = WAIT;
          }
          break;
      }
      // Store current data
      prevClk = clk;
      prevData = data;
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
