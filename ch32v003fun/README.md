# ch32v003fun EEPROM

https://github.com/cnlohr/ch32v003fun/tree/master/examples/i2c_slave

32bit EEPROM の作例

- write の 最初の 1bit でオフセットを設定し、続けてオフセットからデータを書き込める
- read は 32bit 順番に読める

Circuit Python での動作テスト: [circuitpython-test.py](circuitpython-test.py)
