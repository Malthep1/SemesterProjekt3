#include "RPIUart.hpp"


static int uart0_filestream = -1;

void RPIUart::initializeUART(){
	
	uart0_filestream = open("/dev/ttyS0", O_RDWR | O_NONBLOCK);//NON BLOCKING READ WRITE
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("ERROR - UART UNAVAILABLE\n");
	}
	
	//UART CONFIG
	tcgetattr(uart0_filestream, &options);
	//options.c_cflag = B115200 | CS8 | CLOCAL | CREAD; //Baud || SIZE || ignore modem status lines || enable receiver
	//options.c_iflag = IGNPAR; //
	////options.c_lflag = 0;
	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);
	printf("UART INITIALIZED\n");
}

int RPIUart::transmitBytes(unsigned char* command){
	//Sending bytes
	unsigned char tx_buffer[6];
	unsigned char *p_tx_buffer;
	
	int clength = strlen((char*) command);
	printf("clength = %i\n",clength);

	size_t i;
	tx_buffer[0] = 'S';
	for (i = 1; i < clength + 1; i++){
		tx_buffer[i] = command[i-1];
	}
	tx_buffer[i] = 'E';
	if (uart0_filestream != -1)
	{
		int count = write(uart0_filestream, tx_buffer, i+1);		//Filestream, bytes to write, number of bytes to write
		if (count < 0)
		{
			printf("UART TRANSMIT ERROR\n");
            return -1;
		}
        printf("Succesfully wrote bytes:  \n");
		i = 0;
		while(i < sizeof(tx_buffer))
		{
			printf("%c", tx_buffer[i]);
			i++;
		}
		printf("\n");
	}
    return 0;
}

unsigned char* RPIUart::readBytes(){
	// return pointer
	unsigned char* p_rx_buffer;
		
	if (uart0_filestream != -1)
	{
		// Read up to 255 characters from the port if they are there
		unsigned char rx_buffer[11];
		p_rx_buffer = &rx_buffer[0];
		int rx_length = read(uart0_filestream, rx_buffer, 10);		//Filestream, buffer to store in, number of bytes to read (max)
		if (rx_length <= 0)
		{	
			rx_buffer[0] = '\0';
		}
		else
		{	
			
			//Bytes received
			rx_buffer[rx_length] = '\0';
			printf("%i bytes read : %s\n", rx_length, rx_buffer);
		}
		tcflush(uart0_filestream, TCIFLUSH);
	}
	else{
		printf("FILESTREAM FUCKIGN DIED");
	}
    return p_rx_buffer;
}