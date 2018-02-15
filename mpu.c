#include "mpu.h"
#include "slcd.h"

int read(uint8_t RegisterAdress){
	
	uint8_t a;
	uint8_t b;
	
	I2C_Enable(I2C1);
	I2C_Start(I2C1);
	I2C_WriteByte(I2C1,0xD0);
	I2C_WriteByte(I2C1,RegisterAdress);
	I2C_Restart(I2C1);
	I2C_WriteByte(I2C1, 0xD1);
	sendACK(I2C1);
	a = I2C_ReadByte(I2C1,a);
	sendNACK(I2C1);
	b = I2C_ReadByte(I2C1,b);
	I2C_Stop(I2C1);
	I2C_Disable(I2C1);
	return (a << 8)+ b;	
}

void setRegister(uint8_t Adress, uint8_t value){
	
	I2C_Enable(I2C1);
	I2C_Start(I2C1);
	I2C_WriteByte(I2C1,0xD0);
	I2C_WriteByte(I2C1,Adress);
	I2C_WriteByte(I2C1,value);
	I2C_Stop(I2C1);
	I2C_Disable(I2C1);
}


void Init(){
	
	setRegister(0x6b,0x01	);
	delay_mc(100);
	setRegister(0x68,0x07	);
	delay_mc(100);
	setRegister(0x6B,0x00);
	delay_mc(100);
	setRegister(0x1B,0x03	); //1b-2000 dps 13-1000dps B-500dps	 3-250dps
	 
}

uint16_t readX(){
	
	return read(0x43);
}

uint16_t readZ(){
	
	return read(0x47);
}

uint16_t readY(){
	
	return read(0x45);
}

void reset(void){
	
		setRegister( PWR_MGMT_1, 0x80);  // set bit7 to reset the device
    delay_mc(100);    // wait 100 ms to stabilize      

}

void calibrate(float* dest1, float* dest2){
	
		uint8_t data[12];       // data array to hold acc and gyro x,y,z data
    uint16_t fifo_count, packet_count, count;   
    int32_t accel_bias[3] = {0,0,0}; 
    int32_t gyro_bias[3] = {0,0,0};
    float aRes = 2.0/32768.0;   
    float gRes = 250.0/32768.0;
    uint16_t accelsensitivity = 16384; // = 1/aRes = 16384 LSB/g
    uint16_t gyrosensitivity = 131;    // = 1/gRes = 131 LSB/dps
		
    reset();     // Reset device
		
		setRegister(PWR_MGMT_1, 0x01);    // PLL with X axis gyroscope reference is used to improve stability
    setRegister( PWR_MGMT_2, 0x00);    // Disable accel only low power mode 
    delay_mc(200);
		
		/* Configure device for bias calculation */
    setRegister( INT_ENABLE, 0x00);   // Disable all interrupts
    setRegister( FIFO_EN, 0x00);      // Disable FIFO
    setRegister( PWR_MGMT_1, 0x00);   // Turn on internal clock source
    setRegister( I2C_MST_CTRL, 0x00); // Disable I2C master
    setRegister( USER_CTRL, 0x00);    // Disable FIFO and I2C master modes
    setRegister( USER_CTRL, 0x04);    // Reset FIFO
    delay_mc(15);

		setRegister( CONFIG, 0x01);       // Set low-pass filter to 188 Hz
    setRegister( SMPLRT_DIV, 0x00);   // Set sample rate to 1 kHz
    setRegister( ACCEL_CONFIG, 0x00); // Set accelerometer full-scale to 2 g, maximum sensitivity
    setRegister( GYRO_CONFIG, 0x00);  // Set gyro full-scale to 250 degrees per second, maximum sensitivity
		
		/* Configure FIFO to capture accelerometer and gyro data for bias calculation */
    
		setRegister(USER_CTRL, 0x40);   // Enable FIFO  
    setRegister( FIFO_EN, 0x78);     // Enable accelerometer and gyro for FIFO  (max size 1024 bytes in MPU-6050)
    delay_mc(80);                                    // Sample rate is 1 kHz, accumulates 80 samples in 80 milliseconds. 
    // accX: 2 byte, accY: 2 byte, accZ: 2 byte. gyroX: 2 byte, gyroY: 2 byte, gyroZ: 2 byte.   12*80=960 byte < 1024 byte  
		
		/* At end of sample accumulation, turn off FIFO sensor read */
    setRegister( FIFO_EN, 0x00);             // Disable FIFO
		uint8_t i ;
				for(  i=0;i<2;i++)
				{
				data[i]=read( FIFO_R_W);  
				} // Read FIFO sample count
    fifo_count = ((uint16_t)data[0] << 8) | data[1];
    packet_count = fifo_count/12;                          // The number of sets of full acc and gyro data for averaging. packet_count = 80 in this case
    
		for(count=0; count<packet_count; count++)
    {
        int16_t accel_temp[3]={0,0,0}; 
        int16_t gyro_temp[3]={0,0,0};
				uint8_t i ;
				for(  i=0;i<12;i++)
				{
				data[i]=read( FIFO_R_W); // read data for averaging
				}
        
        /* Form signed 16-bit integer for each sample in FIFO */
        accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  ) ; 
        accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  ) ;
        accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  ) ;    
        gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  ) ;
        gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  ) ;
        gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]) ;
        
        /* Sum individual signed 16-bit biases to get accumulated signed 32-bit biases */
        accel_bias[0] += (int32_t) accel_temp[0]; 
        accel_bias[1] += (int32_t) accel_temp[1];
        accel_bias[2] += (int32_t) accel_temp[2];  
        gyro_bias[0]  += (int32_t) gyro_temp[0];
        gyro_bias[1]  += (int32_t) gyro_temp[1];
        gyro_bias[2]  += (int32_t) gyro_temp[2];
    }
		/* Normalize sums to get average count biases */
    accel_bias[0] /= (int32_t) packet_count; 
    accel_bias[1] /= (int32_t) packet_count;
    accel_bias[2] /= (int32_t) packet_count;
    gyro_bias[0]  /= (int32_t) packet_count;
    gyro_bias[1]  /= (int32_t) packet_count;
    gyro_bias[2]  /= (int32_t) packet_count;
		
		/* Remove gravity from the z-axis accelerometer bias calculation */  
    if(accel_bias[2] > 0) {accel_bias[2] -= (int32_t) accelsensitivity;}  
    else {accel_bias[2] += (int32_t) accelsensitivity;}
		
		/* Output scaled accelerometer biases for manual subtraction in the main program */
    dest1[0] = accel_bias[0]*aRes;
    dest1[1] = accel_bias[1]*aRes;
    dest1[2] = accel_bias[2]*aRes;
		
		/* Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup */
    data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
    data[1] = (-gyro_bias[0]/4)       & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
    data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
    data[3] = (-gyro_bias[1]/4)       & 0xFF;
    data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
    data[5] = (-gyro_bias[2]/4)       & 0xFF;
		
		/* Push gyro biases to hardware registers */
    setRegister( XG_OFFS_USRH, data[0]); 
    setRegister(XG_OFFS_USRL, data[1]);
    setRegister( YG_OFFS_USRH, data[2]);
    setRegister( YG_OFFS_USRL, data[3]);
    setRegister( ZG_OFFS_USRH, data[4]);
    setRegister(ZG_OFFS_USRL, data[5]);
		
	  dest2[0] = gyro_bias[0]*gRes;   
    dest2[1] = gyro_bias[1]*gRes;
    dest2[2] = gyro_bias[2]*gRes;
}

