#include <atmel_start.h>

#include <string.h>
#include <stdio.h>
#include "ble_driver.h"
#include "simcom_driver.h"
#include "imu_driver.h"
#include "fram_driver.h"
#include "spi_can_driver.h"
#include "driver_examples.h"

#define USB_DEBUG 1		// Activa el debug por serial USB
#define USB_COMMS_LENGTH 64	// Largo de comandos serial USB

void MCU_low_power_mode(void );
void MCU_wakeup(void );
uint8_t simcom_gprs_connect(void);


/* Rutina para leer comandos por serial USB */
void usb_read_routine(void )
{
	if (usb_serial_bytes_available() > 0)
	{
		char inbuffer[USB_COMMS_LENGTH] = {0};
		uint16_t buffer_length = USB_COMMS_LENGTH;
		usb_serial_read(inbuffer, buffer_length);
		
		// Mostrar info del firmware
		if (strcmp(inbuffer, "info") == 0)
		{
			usb_serial_write("Somax fw v1dev\n", strlen("Somax fw v1dev\n"));
		}
		
		// Bankswap para cambio de firmware
		else if (strcmp(inbuffer, "bankswap") == 0)
		{
			usb_serial_write("Swapping fw\n", strlen("Swapping fw\n"));
			delay_ms(200);
			usbdc_detach();
			delay_ms(500);
			
			/* wait until it is ready to accept a new command */
			while (!NVMCTRL->STATUS.bit.READY);
			
			/* execute the command BKSWRST (Bank swap and system reset) */
			NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMD_BKSWRST | NVMCTRL_CTRLB_CMDEX_KEY;
			
			/* wait until command done */
			while (NVMCTRL->INTFLAG.bit.DONE);
			/* At the end of this command, code starts running from 0x00000 upon reset*/
		}
		
		// Enviar comandos al BLE
		else if (strncmp(inbuffer, "ble ", 4) == 0)
		{
			char *command = inbuffer + 4;
			char respble[64] = {0};
			ble_send_and_receive(command, respble);
			usb_serial_write("Received: ", strlen("Received: "));
			delay_ms(10);
			usb_serial_write(respble, strlen(respble));
			delay_ms(10);
			usb_serial_write("\n", strlen("\n"));
			
		}
		
		// Enviar comandos al Simcom
		else if (strncmp(inbuffer, "sim ", 4) == 0)
		{
			char *command = inbuffer + 4;
			strcat(command, "\r");
			char respsim[1024] = {0}; //era 64 el tamaño
			simcom_send_and_receive(command, respsim);
			usb_serial_write("Received: ", strlen("Received: "));
			delay_ms(10);
			usb_serial_write(respsim, strlen(respsim));
			delay_ms(1000);
			usb_serial_write("\n", strlen("\n"));
			
		}
		
		// Leer buffer de simcom
		else if (strcmp(inbuffer, "readsim") == 0)
		{
			char respsim[64] = {0};
			simcom_receive(respsim);
			usb_serial_write("Received: ", strlen("Received: "));
			delay_ms(10);
			usb_serial_write(respsim, strlen(respsim));
			delay_ms(10);
			usb_serial_write("\n", strlen("\n"));
			
		}
		
		// Check de acelerometro
		else if (strcmp(inbuffer, "acc check") == 0)
		{ 
			char resp = IMU_readRegister(0x0F); // reg WHO AM I respuesta es 00110011 = '3'
			char regvalue;
			regvalue = (resp == 0x33)? 'y' : 'n';
			usb_serial_write("Reading reg\n", strlen("Reading reg\n"));
			delay_ms(10);
			usb_serial_write(&regvalue, 1);
			delay_ms(10);
			usb_serial_write("\n", strlen("\n"));
			delay_ms(10);
			char isintactive;
			isintactive = gpio_get_pin_level(INT_ACC)? 'y' : 'n';
			IMU_readRegister(0x31);
			usb_serial_write(&isintactive, 1);
			delay_ms(10);
			usb_serial_write("\n", strlen("\n"));
			delay_ms(10);
		}
				
		// Comandos para probar spi		
		else if (strcmp(inbuffer, "spi check") == 0)
		{
			SPI_0_test();
		}
		
		else if (strcmp(inbuffer, "spi write") == 0)
		{
			SPI_0_test1();
		}
		
		// Comandos para probar escritura y lectura de fram
		else if (strcmp(inbuffer, "fram write") == 0)
		{
			char dataw = 'f';
			FRAM_bytewrite(0x0013, &dataw);
		}
		
		else if (strcmp(inbuffer, "fram read") == 0)
		{
			char datar[1] = {0};
			FRAM_byteread(0x0013, datar);
			delay_ms(10);
			usb_serial_write("Received: ", strlen("Received: "));
			delay_ms(10);
			usb_serial_write(datar, strlen(datar));
			delay_ms(10);
			usb_serial_write("\n", strlen("\n"));
		}
		
		// Entrar a modo bajo consumo
		else if (strcmp(inbuffer, "lowpwr") == 0)
		{
			MCU_low_power_mode();
		}
		else if (strcmp(inbuffer, "simcon") == 0)
		{
			simcom_gprs_connect();
		}
		else
		{
			usb_serial_write("Command not recognized\n", strlen("Command not recognized\n"));
		}
	}
}


/* Funciones de interrupcion */

static void acc_interrupt_function(void)
{
	IMU_readRegister(0x31);
	delay_ms(10);
	gpio_set_pin_level(LED0, false);
	gpio_set_pin_level(LED1, false);
	gpio_set_pin_level(LED2, true);
	MCU_wakeup();
	delay_ms(500);
}

static void can_interrupt_function(void)
{
}


/* Funciones para entrar y salir de modo bajo consumo */

void MCU_low_power_mode(void )
{
	//IMU_readRegister(0x31);
	
	#if USB_DEBUG
		usbdc_detach();
	#endif
	
	gpio_set_pin_level(LED0, true);
	gpio_set_pin_level(LED1, true);
	gpio_set_pin_level(LED2, true);
	
	ble_send("AT+SLEEP");
	
	//simcom_send("AT+CPOF");	// Apaga completamente el simcom
	gpio_set_pin_level(GPRS_PEN, false);		// Alimentacion a antena gps
	
	spi_write_reg(0x0F,0x27);	// Sleep mode en chip SPI-CAN
	
	//IMU_readRegister(0x31);	// Limpiar interrupcion acc
	//while (gpio_get_pin_level(INT_ACC))
	//{
	//	gpio_set_pin_level(LED1, false);
	//	delay_ms(100);
	//}
	gpio_set_pin_level(LED1, true);
	//ext_irq_enable(INT_ACC);
	delay_ms(20);
	sleep(4); // 2:Idle 4:Standby 5:Hibernate 6:Backup 7:Off
	
}

void MCU_wakeup(void )
{
	ext_irq_disable(INT_ACC);
	
	#if USB_DEBUG
		usbdc_attach();
	#endif
	
	ble_send("AAAAAAA");
	
	gpio_set_pin_level(GPRS_PEN, true);		// Alimentacion a antena gps
	
	
	spi_write_reg(0x2B, 0x40);	// Enable interrupt en can-spi
	spi_write_reg(0x2C, 0x40);	// Generar interrupt en can-spi
	delay_ms(20);
	spi_write_reg(0x2C, 0x00);	// Limpiar interrupt en can-spi
	spi_write_reg(0x2B, 0x40);	// Diable interrupt en can-spi
	spi_write_reg(0x0F,0x87);	// Configuration mode en chip SPI-CAN (0x07 para Normal operation mode)

}



/**
 * Using SPI_0 to write "Hello World" using the IO abstraction.
 * Funciones usadas para probar comunicacion con el chip spi-can
 */
static uint8_t test_SPI_0[2] = {0x03, 0x0F};


void SPI_0_test(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI_0, &io);
	spi_m_sync_enable(&SPI_0);
	
	char buffer[1] = {0};
	gpio_set_pin_level(SPI_CS0, false);
	io_write(io, test_SPI_0, 2);
	io_read(io, buffer, 1);
	gpio_set_pin_level(SPI_CS0, true);
	char is87 = (*buffer == 0x87)? 'y' : 'n';
	usb_serial_write("Received: ", strlen("Received: "));
	delay_ms(10);
	usb_serial_write(&is87, strlen(&is87));
	delay_ms(10);
	usb_serial_write("\n", strlen("\n"));
}

static uint8_t test_SPI_1[3] = {0x02, 0x0F, 0x07};

void SPI_0_test1(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI_0, &io);
	spi_m_sync_enable(&SPI_0);
	
	char buffer[1] = {0};
	gpio_set_pin_level(SPI_CS0, false);
	io_write(io, test_SPI_1, 3);
	gpio_set_pin_level(SPI_CS0, true);
}
 
 
 // Conectar GPRS
 uint8_t simcom_gprs_connect(void)
 {
	 char rec_buffer[64] = {0};
	 simcom_send_and_receive("ATE0\r", rec_buffer);	// Desactivar echo
	 delay_ms(100);
	 simcom_send_and_receive("AT+CPIN?\r", rec_buffer);	// Check tarjeta SIM
	 delay_ms(100);
	 usb_serial_write("Received: ", strlen("Received: "));
	 delay_ms(10);
	 usb_serial_write(rec_buffer, strlen(rec_buffer));
	 delay_ms(10);
	 usb_serial_write("\n", strlen("\n"));
	 if (strncmp(rec_buffer, "\n+CPIN: READY", 12) != 0)
		return 1;
	usb_serial_write("PASSED", strlen("PASSED"));
	simcom_send_and_receive("AT+CGREG?", rec_buffer);	// check the registration status of the device
	delay_ms(100);
	simcom_send_and_receive("AT+COPS?", rec_buffer);	// get the network information
	delay_ms(100);
	simcom_send_and_receive("AT+CSQ", rec_buffer);	// get the signal quality
	delay_ms(100);
	simcom_send_and_receive("AT+cgatt=1", rec_buffer);	// GPRS Attach
	delay_ms(100);
	if (strncmp(rec_buffer, "OK", 2) != 0)
		return 1;
	simcom_send_and_receive("AT+CGDCONT=1,\"IP\",\"iot.secure\"", rec_buffer);
	delay_ms(100);
	simcom_send_and_receive("AT+CGACT=1,1", rec_buffer);
	delay_ms(100);
	
	return 0;
 }
 

/* Funciones para el checkeo de modulos */

typedef union
{
	uint8_t raw;
	struct
	{
		bool ble : 1;
		bool acc : 1;
		bool fram : 1;
		bool sim : 1;
		bool can : 1;
		bool gps : 1;
		bool gprs : 1;
		bool NA3 : 1;
	} bits;
} dev_status_t;

dev_status_t status;

uint8_t initial_check(void )
{
	uint8_t errorcount = 0;
	status.raw = 0;
	
	// Check BLE comm
	char respble[64] = {0};
	ble_send_and_receive("AT", respble);
	delay_ms(100);
	ble_send_and_receive("AT+BAUD?", respble);
	usb_serial_write("respble: ", strlen("respble: "));
	delay_ms(20);
	usb_serial_write(respble, strlen(respble));
	status.bits.ble = (strncmp(respble, "OK+Get:", 2) == 0);
	errorcount += !status.bits.ble;
	delay_ms(10);
	
	// Check Simcom comm
	
	char respsim[64] = {0};
	simcom_send_and_receive("AT\r", respsim);
	delay_ms(10000);
	simcom_send_and_receive("ATE0\r", respsim);
	delay_ms(500);
	char respsim2[64] = {0};
	simcom_send_and_receive("AT\r", respsim2);
	delay_ms(500);
	usb_serial_write("respsim: ", strlen("respsim: "));
	delay_ms(200);
	usb_serial_write(respsim2, strlen(respsim2));
	delay_ms(200);
	status.bits.sim = (strncmp(respsim2, "\rOK", 1) == 0);
	errorcount += !status.bits.sim;
	delay_ms(10);
	
	
	// Check acc comm
	char acc_who_am_i = IMU_readRegister(0x0F);
	status.bits.acc = (acc_who_am_i == '3');
	errorcount += !status.bits.acc;
	delay_ms(10);
	
	// Check can spi comm
	uint8_t spi_ctrl_reg_value[1] = {0};
	spi_read_reg(0x0F, spi_ctrl_reg_value);
	status.bits.can = (*spi_ctrl_reg_value == 0x87);
	errorcount += !status.bits.can;
	delay_ms(10);
	
	// Check fram write y read
	char dataw = 'f';
	FRAM_bytewrite(0x0013, &dataw);
	char datar[1] = {0};
	FRAM_byteread(0x0013, datar);
	status.bits.fram = (*datar == 'f');
	errorcount += !status.bits.fram;
	delay_ms(10);
	
	return errorcount;
}


/* main */

//CAMBIO PARA PROBAR SUBIDA A GIT.

Ble ble = {0};
Simcom simcom = {0};

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	ble_init();
	simcom_init();
	FRAM_init();
	IMU_init();
	spi_init();
	
	IMU_readRegister(0x31);		// Limpiar interrupciones de acc
	//ext_irq_register(INT_ACC, acc_interrupt_function);
	//ext_irq_register(INT_CAN, can_interrupt_function);
	//ext_irq_disable(INT_ACC);
	//ext_irq_disable(INT_CAN);

	gpio_set_pin_level(SPI_CS0, true);	// Chip select para spi-can
	gpio_set_pin_level(GPRS_PEN, true);	// Alimentacion a antena gps
	
	// Power-on de simcom
	gpio_set_pin_level(GPRS_nPWR, true);
	delay_ms(500);
	gpio_set_pin_level(GPRS_nPWR, false);
	
	// Inicializacion de chip spi-can en configuration mode
	spi_write_reg(0x2B, 0x40);	// Enable interrupt en can-spi
	spi_write_reg(0x2C, 0x40);	// Generar interrupt en can-spi
	delay_ms(20);
	spi_write_reg(0x2C, 0x00);	// Limpiar interrupt en can-spi
	spi_write_reg(0x2B, 0x40);	// Diable interrupt en can-spi
	spi_write_reg(0x0F,0x87);	// Configuration mode en chip SPI-CAN (0x07 para Normal operation mode)
	
	
	#if USB_DEBUG
		usbdc_attach();
		delay_ms(100);
		usb_serial_begin();
		delay_ms(100);
	#endif
	
	gpio_set_pin_level(LED0, false);
	gpio_set_pin_level(LED1, true);
	gpio_set_pin_level(LED2, true);
	
	delay_ms(15000);	// Esperar inicializacion de todos los modulos
	uint8_t errorcnt = initial_check();
	
		
	gpio_set_pin_level(LED0, false);
	gpio_set_pin_level(LED1, true);
	gpio_set_pin_level(LED2, true);
	delay_ms(333);
	gpio_set_pin_level(LED0, true);
	gpio_set_pin_level(LED1, false);
	gpio_set_pin_level(LED2, true);
	delay_ms(333);
	gpio_set_pin_level(LED0, true);
	gpio_set_pin_level(LED1, true);
	gpio_set_pin_level(LED2, false);
	delay_ms(333);
	
	//inicializa el calendario y por lo tanto el timestamp para todo el programa
	CALENDAR_0_example(); 
	
	//esto es como el getble inicial, después se referencia al struct ble a todo. Acá hubo problemas para inciar el ble, parece que mejor se tiene que hacer una función de Ble_Struct_init_ o algo así- 
	//ble_setTimer(TIME_TO_DELAY_BLE, &ble);
	//ble.tryCounter_ = 0; 
	//ble.tryCounter_ ++; 
	
	Simcom_struct_init(&simcom); 

	
	while (1) {
		if (errorcnt == 0)
		{ 
			gpio_set_pin_level(LED0, false);
			gpio_set_pin_level(LED1, false);
			gpio_set_pin_level(LED2, false);
			usb_read_routine();		
			
			
			usb_serialPrint("\n --- WHILE 1 --- \n");
			
			//ble_process(&ble);
			//Simcom_process(&simcom);  	
			
			delay_ms(3000);  //revisar precisión haciendo aún más corto el while
		}
		else
		{
			for(uint8_t i = 0; i<3; i++)
			{
			gpio_set_pin_level(LED0, !status.bits.acc);
			gpio_set_pin_level(LED1, !status.bits.ble);
			gpio_set_pin_level(LED2, !status.bits.can);
			usb_read_routine();
			delay_ms(300);
			gpio_set_pin_level(LED0, true);
			gpio_set_pin_level(LED1, true);
			gpio_set_pin_level(LED2, true);
			usb_read_routine();
			delay_ms(300);
			gpio_set_pin_level(LED0, !status.bits.fram);
			gpio_set_pin_level(LED1, !status.bits.sim);
			gpio_set_pin_level(LED2, !status.bits.gprs);
			usb_read_routine();
			delay_ms(300);
			gpio_set_pin_level(LED0, true);
			gpio_set_pin_level(LED1, true);
			gpio_set_pin_level(LED2, true);
			usb_read_routine();
			delay_ms(300);
			gpio_set_pin_level(LED0, !status.bits.gps);
			gpio_set_pin_level(LED1, false);
			gpio_set_pin_level(LED2, false);
			usb_read_routine();
			delay_ms(300);
			gpio_set_pin_level(LED0, true);
			gpio_set_pin_level(LED1, true);
			gpio_set_pin_level(LED2, true);
			usb_read_routine();
			delay_ms(700);
			}
		}
		//MCU_low_power_mode();
	}
}
