# ch32v003fun EEPROM

https://github.com/cnlohr/ch32v003fun/tree/master/examples/i2c_slave

32byte EEPROM の作例

- write の 最初の 1byte でオフセットを設定し、続けてオフセットからデータを書き込める
- read は 32byte 順番に読める

Circuit Python での動作テスト: [circuitpython-test.py](circuitpython-test.py)

## 実装すべきこと

- `I2C1_ER_IRQHandler` で、`I2C1->START1` の `I2C_STAR1_BERR`、`I2C_STAR1_ARLO`、`I2C_STAR1_AF` をクリアすること
- `I2C1_EV_IRQHandler` で、`I2C1->CTLR1 |= I2C_CTLR1_ACK;` で ACK すること
- `I2C1_EV_IRQHandler` で、`I2C1->START1 & I2C_STAR1_ADDR` を、リクエスト開始とみなすこと
- `I2C1_EV_IRQHandler` で、`I2C1->START1 & I2C_STAR1_RXNE` を、master -> slave へのデータ転送とみなして、`I2C1->DATAR` から 1byte 読み込むこと。
- `I2C1_EV_IRQHandler` で、`I2C1->START1 & I2C_STAR1_TXE` を、slave -> master へのデータ転送とみなして、`I2C1->DATAR` に 1byte 書き込むこと。

## ログ

```
initialize
initialize done
EV STAR1: 0x0002 STAR2: 0x0002
ADDR
EV STAR1: 0x0044 STAR2: 0x0002
RXNE write event: pos:0
EV STAR1: 0x0040 STAR2: 0x0002
RXNE write event: pos:0
EV STAR1: 0x0044 STAR2: 0x0002
RXNE write event: pos:1
EV STAR1: 0x0040 STAR2: 0x0002
RXNE write event: pos:2
EV STAR1: 0x0044 STAR2: 0x0002
RXNE write event: pos:3
EV STAR1: 0x0040 STAR2: 0x0002
RXNE write event: pos:4
EV STAR1: 0x0044 STAR2: 0x0002
RXNE write event: pos:5
EV STAR1: 0x0040 STAR2: 0x0002
RXNE write event: pos:6
EV STAR1: 0x0044 STAR2: 0x0002
RXNE write event: pos:7
EV STAR1: 0x0040 STAR2: 0x0002
RXNE write event: pos:8
EV STAR1: 0x0044 STAR2: 0x0002
RXNE write event: pos:9
EV STAR1: 0x0040 STAR2: 0x0002
RXNE write event: pos:10
EV STAR1: 0x0044 STAR2: 0x0002
RXNE write event: pos:11
EV STAR1: 0x0040 STAR2: 0x0002
RXNE write event: pos:12
EV STAR1: 0x0044 STAR2: 0x0002
RXNE write event: pos:13
EV STAR1: 0x0040 STAR2: 0x0002
RXNE write event: pos:14
EV STAR1: 0x0044 STAR2: 0x0002
RXNE write event: pos:15
EV STAR1: 0x0040 STAR2: 0x0002
RXNE write event: pos:16
EV STAR1: 0x0044 STAR2: 0x0002
RXNE write event: pos:17
EV STAR1: 0x0040 STAR2: 0x0002
RXNE write event: pos:18
EV STAR1: 0x0044 STAR2: 0x0002
RXNE write event: pos:19
EV STAR1: 0x0040 STAR2: 0x0002
RXNE write event: pos:20
EV STAR1: 0x0044 STAR2: 0x0002
RXNE write event: pos:21
EV STAR1: 0x0040 STAR2: 0x0002
RXNE write event: pos:22
EV STAR1: 0x0044 STAR2: 0x0002
RXNE write event: pos:23
EV STAR1: 0x0040 STAR2: 0x0002
RXNE write event: pos:24
EV STAR1: 0x0044 STAR2: 0x0002
RXNE write event: pos:25
EV STAR1: 0x0040 STAR2: 0x0002
RXNE write event: pos:26
EV STAR1: 0x0044 STAR2: 0x0002
RXNE write event: pos:27
EV STAR1: 0x0040 STAR2: 0x0002
RXNE write event: pos:28
EV STAR1: 0x0044 STAR2: 0x0002
RXNE write event: pos:29
EV STAR1: 0x0040 STAR2: 0x0002
RXNE write event: pos:30
EV STAR1: 0x0044 STAR2: 0x0002
RXNE write event: pos:31
EV STAR1: 0x0082 STAR2: 0x0006
ADDR
TXE write event: pos:0
EV STAR1: 0x0080 STAR2: 0x0006
TXE write event: pos:1
EV STAR1: 0x0084 STAR2: 0x0006
TXE write event: pos:2
EV STAR1: 0x0080 STAR2: 0x0006
TXE write event: pos:3
EV STAR1: 0x0084 STAR2: 0x0006
TXE write event: pos:4
EV STAR1: 0x0080 STAR2: 0x0006
TXE write event: pos:5
EV STAR1: 0x0084 STAR2: 0x0006
TXE write event: pos:6
EV STAR1: 0x0080 STAR2: 0x0006
TXE write event: pos:7
EV STAR1: 0x0084 STAR2: 0x0006
TXE write event: pos:8
EV STAR1: 0x0080 STAR2: 0x0006
TXE write event: pos:9
EV STAR1: 0x0084 STAR2: 0x0006
TXE write event: pos:10
EV STAR1: 0x0080 STAR2: 0x0006
TXE write event: pos:11
EV STAR1: 0x0084 STAR2: 0x0006
TXE write event: pos:12
EV STAR1: 0x0080 STAR2: 0x0006
TXE write event: pos:13
EV STAR1: 0x0084 STAR2: 0x0006
TXE write event: pos:14
EV STAR1: 0x0080 STAR2: 0x0006
TXE write event: pos:15
EV STAR1: 0x0084 STAR2: 0x0006
TXE write event: pos:16
EV STAR1: 0x0080 STAR2: 0x0006
TXE write event: pos:17
EV STAR1: 0x0084 STAR2: 0x0006
TXE write event: pos:18
EV STAR1: 0x0080 STAR2: 0x0006
TXE write event: pos:19
EV STAR1: 0x0084 STAR2: 0x0006
TXE write event: pos:20
EV STAR1: 0x0080 STAR2: 0x0006
TXE write event: pos:21
EV STAR1: 0x0084 STAR2: 0x0006
TXE write event: pos:22
EV STAR1: 0x0080 STAR2: 0x0006
TXE write event: pos:23
EV STAR1: 0x0084 STAR2: 0x0006
TXE write event: pos:24
EV STAR1: 0x0080 STAR2: 0x0006
TXE write event: pos:25
EV STAR1: 0x0084 STAR2: 0x0006
TXE write event: pos:26
EV STAR1: 0x0080 STAR2: 0x0006
TXE write event: pos:27
EV STAR1: 0x0084 STAR2: 0x0006
TXE write event: pos:28
EV STAR1: 0x0080 STAR2: 0x0006
TXE write event: pos:29
EV STAR1: 0x0084 STAR2: 0x0006
TXE write event: pos:30
EV STAR1: 0x0080 STAR2: 0x0006
TXE write event: pos:31
EV STAR1: 0x0084 STAR2: 0x0006
TXE write event: pos:32
ER STAR1: 0x0400
```
