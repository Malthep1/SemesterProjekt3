#include "RPIUart.hpp"


static int uart0_filestream = -1;

void RPIUart::initializeUART(){
	
	uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);//NON BLOCKING READ WRITE
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("ERROR - UART UNAVAILABLE\n");
	}
	
	//UART CONFIG
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD; //Baud || SIZE || ignore modem status lines || enable receiver
	options.c_iflag = IGNPAR; //
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);
}

int RPIUart::transmitBytes(unsigned char* command){
	//Sending bytes
	unsigned char tx_buffer[10];
	unsigned char *p_tx_buffer;
	std::cout << "SENDING ";
	p_tx_buffer = &tx_buffer[0];
	*p_tx_buffer++ = 'S';
	int clength = strlen((char*) command);
	for (size_t i = 0; i < clength; i++){
		*p_tx_buffer++ = command[i];
	}
	*p_tx_buffer++ = 'E';
	for (size_t j = 0; j < 10; j++)
	{
		std::cout << tx_buffer[j];
	}
	std::cout << "\n";
	if (uart0_filestream != -1)
	{
		int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));		//Filestream, bytes to write, number of bytes to write
		if (count < 0)
		{
			std::cout << "UART TRANSMIT ERROR\n";
            return -1;
		}
        printf("Succesfully wrote bytes: \"%s\"  on UART serial0 \n", tx_buffer);
	}

    return 0;
}

unsigned char* RPIUart::readBytes(){
	// Incomming bytes.
	unsigned char* p_rx_buffer;
		
	if (uart0_filestream != -1)
	{
		// Read up to 255 characters from the port if they are there
		unsigned char rx_buffer[11];
		p_rx_buffer = &rx_buffer[0];
		int rx_length = read(uart0_filestream, (void*)rx_buffer, 10);		//Filestream, buffer to store in, number of bytes to read (max)
		if (rx_length <= 0)
		{
			std::cout << "No Bytes to read\n";
		}
		else
		{
			//Bytes received
			rx_buffer[rx_length] = '\0';
			printf("%i bytes read : %s\n", rx_length, rx_buffer);
		}
		
	}
    return p_rx_buffer;
}

//seems better than built in delays
void delay(int number_of_seconds)
{
    int ms = 1000 * number_of_seconds;
    clock_t start = clock();
    while (clock() < start + ms);
}