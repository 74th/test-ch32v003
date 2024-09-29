all : flash

TARGET:=lchika
MINICHLINK:=$(shell dirname $(shell which minichlink))
CH32V003FUN:=./ch32v003fun

ifeq ($(shell uname), Darwin)
PREFIX=~/.platformio/packages/toolchain-riscv/bin/riscv-none-embed
endif

include ./ch32v003fun/ch32v003fun.mk

flash : cv_flash
clean : cv_clean
