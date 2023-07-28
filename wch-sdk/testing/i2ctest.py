import board
from busio import I2C

i2c = I2C(board.GP3, board.GP2)
i2c.try_lock()
buf = bytearray(4)
i2c.readfrom_into(0x0a, buf)