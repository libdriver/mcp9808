### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

GPIO Pin: ALERT GPIO17.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(mcp9808 REQUIRED)
```

### 3. MCP9808

#### 3.1 Command Instruction

1. Show mcp9808 chip and driver information.

   ```shell
   mcp9808 (-i | --information)
   ```

2. Show mcp9808 help.

   ```shell
   mcp9808 (-h | --help)
   ```

3. Show mcp9808 pin connections of the current board.

   ```shell
   mcp9808 (-p | --port)
   ```

4. Run mcp9808 regiser test.

   ```shell
   mcp9808 (-t reg | --test=reg) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>]
   ```
   
5. Run mcp9808 read test, num is the test times.

   ```shell
   mcp9808 (-t read | --test=read) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>] [--times=<num>]
   ```

6. Run mcp9808 interrupt test, temperature is the critical temperature in degrees celsius, threshold is the temperature threshold in degrees celsius, num is the test times.

   ```shell
   mcp9808 (-t int | --test=int) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>] [--crit=<temperature>] [--high=<threshold>] [--low=<threshold>] [--times=<num>]
   ```
   
7. Run mcp9808 read function, num is the read times.

   ```shell
   mcp9808 (-e read | --example=read) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>] [--times=<num>]
   ```

8. Run mcp9808 interrupt function, temperature is the critical temperature in degrees celsius, threshold is the temperature threshold in degrees celsius, num is the test times.

   ```shell
   mcp9808 (-e int | --example=int) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>] [--crit=<temperature>] [--high=<threshold>] [--low=<threshold>] [--times=<num>]
   ```

#### 3.2 Command Example

```shell
./mcp9808 -i

mcp9808: chip is Microchip MCP9808.
mcp9808: manufacturer is Microchip.
mcp9808: interface is IIC.
mcp9808: driver version is 1.0.
mcp9808: min supply voltage is 2.7V.
mcp9808: max supply voltage is 5.5V.
mcp9808: max current is 0.40mA.
mcp9808: max temperature is 125.0C.
mcp9808: min temperature is -40.0C.
```

```shell
./mcp9808 -p

mcp9808: SCL connected to GPIO3(BCM).
mcp9808: SDA connected to GPIO2(BCM).
mcp9808: ALERT connected to GPIO17(BCM).
```

```shell
./mcp9808 -t reg --addr=0

mcp9808: chip is Microchip MCP9808.
mcp9808: manufacturer is Microchip.
mcp9808: interface is IIC.
mcp9808: driver version is 1.0.
mcp9808: min supply voltage is 2.7V.
mcp9808: max supply voltage is 5.5V.
mcp9808: max current is 0.40mA.
mcp9808: max temperature is 125.0C.
mcp9808: min temperature is -40.0C.
mcp9808: start register test.
mcp9808: mcp9808_set_addr/mcp9808_get_addr test.
mcp9808: set address 0.
mcp9808: check addr ok.
mcp9808: set address 1.
mcp9808: check addr ok.
mcp9808: set address 2.
mcp9808: check addr ok.
mcp9808: set address 3.
mcp9808: check addr ok.
mcp9808: set address 4.
mcp9808: check addr ok.
mcp9808: set address 5.
mcp9808: check addr ok.
mcp9808: set address 6.
mcp9808: check addr ok.
mcp9808: set address 7.
mcp9808: check addr ok.
mcp9808: mcp9808_set_temperature_threshold_hysteresis/mcp9808_get_temperature_threshold_hysteresis test.
mcp9808: set temperature threshold hysteresis 0C.
mcp9808: check temperature threshold hysteresis ok.
mcp9808: set temperature threshold hysteresis 1.5C.
mcp9808: check temperature threshold hysteresis ok.
mcp9808: set temperature threshold hysteresis 3.0C.
mcp9808: check temperature threshold hysteresis ok.
mcp9808: set temperature threshold hysteresis 6.0C.
mcp9808: check temperature threshold hysteresis ok.
mcp9808: mcp9808_set_shutdown/mcp9808_get_shutdown test.
mcp9808: enable shutdown.
mcp9808: check shutdown ok.
mcp9808: disable shutdown.
mcp9808: check shutdown ok.
mcp9808: mcp9808_clear_interrupt test.
mcp9808: check clear interrupt ok.
mcp9808: mcp9808_get_alert_output_asserted test.
mcp9808: alert output asserted is false.
mcp9808: mcp9808_set_alert_output/mcp9808_get_alert_output test.
mcp9808: enable alert output.
mcp9808: check alert output ok.
mcp9808: disable alert output.
mcp9808: check alert output ok.
mcp9808: mcp9808_set_alert_output_select/mcp9808_get_alert_output_select test.
mcp9808: set alert output select all.
mcp9808: check alert output select ok.
mcp9808: set alert output select only critical temperature.
mcp9808: check alert output select ok.
mcp9808: mcp9808_set_alert_output_polarity/mcp9808_get_alert_output_polarity test.
mcp9808: set alert output polarity low.
mcp9808: check alert output polarity ok.
mcp9808: set alert output polarity high.
mcp9808: check alert output polarity ok.
mcp9808: mcp9808_set_alert_output_mode/mcp9808_get_alert_output_mode test.
mcp9808: set alert comparator output mode.
mcp9808: check alert output mode ok.
mcp9808: set alert interrupt output mode.
mcp9808: check alert output mode ok.
mcp9808: mcp9808_set_temperature_high_threshold/mcp9808_get_temperature_high_threshold test.
mcp9808: set temperature high threshold 126.
mcp9808: check temperature high threshold ok.
mcp9808: mcp9808_set_temperature_low_threshold/mcp9808_get_temperature_low_threshold test.
mcp9808: set temperature low threshold 1012.
mcp9808: check temperature low threshold ok.
mcp9808: mcp9808_set_critical_temperature/mcp9808_get_critical_temperature test.
mcp9808: set critical temperature 397.
mcp9808: check critical temperature ok.
mcp9808: mcp9808_get_device_revision test.
mcp9808: revision is 0x00.
mcp9808: mcp9808_set_resolution/mcp9808_get_resolution test.
mcp9808: set resolution 0.5C.
mcp9808: check resolution ok.
mcp9808: set resolution 0.25C.
mcp9808: check resolution ok.
mcp9808: set resolution 0.125C.
mcp9808: check resolution ok.
mcp9808: set resolution 0.0625C.
mcp9808: check resolution ok.
mcp9808: mcp9808_temperature_convert_to_register/mcp9808_temperature_convert_to_data test.
mcp9808: set temperature 9.150C.
mcp9808: check temperature 9.000C.
mcp9808: set temperature -7.930C.
mcp9808: check temperature -7.750C.
mcp9808: mcp9808_set_critical_temperature_lock/mcp9808_get_critical_temperature_lock test.
mcp9808: disable critical temperature lock.
mcp9808: check critical temperature lock ok.
mcp9808: enable critical temperature lock.
mcp9808: check critical temperature lock ok.
mcp9808: mcp9808_set_temperature_threshold_lock/mcp9808_get_temperature_threshold_lock test.
mcp9808: disable temperature threshold lock.
mcp9808: check temperature threshold lock ok.
mcp9808: enable temperature threshold lock.
mcp9808: check temperature threshold lock ok.
mcp9808: if you run the register test, please power up again.
mcp9808: finish register test.
```

```shell
./mcp9808 -t read --addr=0 --times=3

mcp9808: chip is Microchip MCP9808.
mcp9808: manufacturer is Microchip.
mcp9808: interface is IIC.
mcp9808: driver version is 1.0.
mcp9808: min supply voltage is 2.7V.
mcp9808: max supply voltage is 5.5V.
mcp9808: max current is 0.40mA.
mcp9808: max temperature is 125.0C.
mcp9808: min temperature is -40.0C.
mcp9808: start read test.
mcp9808: set resolution 0.5C.
mcp9808: temperature is 26.50C.
mcp9808: temperature is 26.50C.
mcp9808: temperature is 26.50C.
mcp9808: set resolution 0.25C.
mcp9808: temperature is 26.50C.
mcp9808: temperature is 26.50C.
mcp9808: temperature is 26.50C.
mcp9808: set resolution 0.125C.
mcp9808: temperature is 26.38C.
mcp9808: temperature is 26.50C.
mcp9808: temperature is 26.50C.
mcp9808: set resolution 0.0625C.
mcp9808: temperature is 26.50C.
mcp9808: temperature is 26.50C.
mcp9808: temperature is 26.50C.
mcp9808: finish read test.
```

```shell
./mcp9808 -t int --addr=0 --crit=26.0 --high=27.0 --low=25.0 --times=1

mcp9808: chip is Microchip MCP9808.
mcp9808: manufacturer is Microchip.
mcp9808: interface is IIC.
mcp9808: driver version is 1.0.
mcp9808: min supply voltage is 2.7V.
mcp9808: max supply voltage is 5.5V.
mcp9808: max current is 0.40mA.
mcp9808: max temperature is 125.0C.
mcp9808: min temperature is -40.0C.
mcp9808: start interrupt test.
mcp9808: irq critical temperature.
mcp9808: temperature is 26.00C.
mcp9808: find interrupt.
mcp9808: finish interrupt test.
```

```shell
./mcp9808 -e read --addr=0 --times=3

mcp9808: 1/3.
mcp9808: temperature is 26.50C.
mcp9808: 2/3.
mcp9808: temperature is 26.50C.
mcp9808: 3/3.
mcp9808: temperature is 26.50C.
```

```shell
./mcp9808 -e int --addr=0 --crit=27.0 --high=28.0 --low=25.0 --times=1

mcp9808: irq critical temperature.
mcp9808: temperature is 27.06C.
mcp9808: find interrupt.
```

```shell
./mcp9808 -h

Usage:
  mcp9808 (-i | --information)
  mcp9808 (-h | --help)
  mcp9808 (-p | --port)
  mcp9808 (-t reg | --test=reg) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>]
  mcp9808 (-t read | --test=read) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>] [--times=<num>]
  mcp9808 (-t int | --test=int) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>]
                                [--crit=<temperature>] [--high=<threshold>] [--low=<threshold>] [--times=<num>]
  mcp9808 (-e read | --example=read) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>] [--times=<num>]
  mcp9808 (-e int | --example=int) [--addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>]
                                   [--crit=<temperature>] [--high=<threshold>] [--low=<threshold>] [--times=<num>]

Options:
      --addr=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>
                        Set the addr.([default: 0])
      --crit=<temperature>
                        Set the critical temperature.([default: 27.0])
  -e <read | int>, --example=<read | int>
                        Run the driver example.
      --low=<threshold>
                        Set the low temperature threshold.([default: 25.0])
  -h, --help            Show the help.
      --high=<threshold>
                        Set the high temperature threshold.([default: 30.0])
  -i, --information     Show the chip information.
  -p, --port            Display the pin connections of the current board.
  -t <reg | read | int>, --test=<reg | read | int>
                        Run the driver test.
      --times=<num>     Set the running times.([default: 3])
```

#### 3.3 Command Problem

1. There is some unknown problem in the interrupt interface of mcp9808 on the raspberry board.
