/**
 *
 */

 class I2C {
	private:
		uint8_t sda;
		uint8_t scl;
		uint8_t address;
		uint8_t* buffer;
	public:
	
		I2C(uint8_t sda, uint8_t scl, uint8_t address, uint32_t bufferSize)
		:
		sda(sda)
		, scl(scl)
		, address(address)
		{
			this->buffer = new uint8_t[bufferSize];
		}
		
		~I2C()
		{
			delete(this->buffer);
		}
 };