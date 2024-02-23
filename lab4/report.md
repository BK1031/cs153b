<div style="text-align:right;">Bharat Kathi</div>
<div style="text-align:right;">Michael Wong</div>
<div style="text-align:right;">ECE 153B</div>
<div style="text-align:right;">2/22/24</div>

# Lab 4 Report

## Q1

**Explain what the BRR in the UART setup is for and how to determine a value for it.**

`BRR` is the Buad Rate Register. The value we input during the UART Setup is to set the desired baud rate. This value is calculated using this formula: `BRR = (System Clock Frequency) / (Desired Baud Rate)`.

In our case, the system clock frequency was 16 MHz and we wanted a baud rate of 9600, so our `BRR = 16000000 / 9600 = 1666`.

## Q2
**Explain your choice of prescaler as well as your timings for your I2C connection to the temperature sensor. Show your computations that lead to your choice.**

The prescaler in the I2C connection is used to divide the system clock frequency to generate the SCL signal for the I2C communication. The choice of prescaler and the timings for the I2C connection depend on the desired I2C bus speed and the system clock frequency.

```
PRESC = 7
f_PRESC = f_I2CCLK/(PRESC+1) = 80 MHz/(7+1) = 10 MHz
t_PRESC = 0.1 us

SCLDEL = 9
t_SCLDEL = (SCLDEL+1)*t_PRESC >= 1 us

SDADEL = 12
t_SDADEL = (SDADEL+1)*t_PRESC >= 1.25 us

SCLH = 39
t_SCLH = (SCLH+1)*t_PRESC >= 4 us

SCLL = 46
t_SCLL = (SCLL+1)*t_PRESC >= 4.7 us
```

## Q3
**What messages do you send to the accelerometer to receive data from it? What does the data you send represent? What does the data you receive represent?**

The messages sent to the accelerometer are requests for specific types of data. Each message includes an address that tells the accelerometer which type of data to send back. For example, the function call `accWrite(0x32, 0x00)` is a request for the X-axis byte data.

The data sent in these messages is the address of the register in the accelerometer's memory where the requested data is stored. The accelerometer uses this address to locate the data and send it back.

The data received from the accelerometer represents the raw measurements of acceleration in the X, Y, and Z axes. These measurements are in the form of two's complement 16-bit values, where each bit represents 3.9 milli-g of acceleration. The data is then converted to the actual acceleration values by multiplying it with the scale factor from the accelerometer's datasheet.