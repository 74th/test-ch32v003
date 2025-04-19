all : flash

TARGET:=template
MINICHLINK:=$(shell dirname $(shell which minichlink))

TARGET_MCU?=CH32V003
include ~/ghq/github.com/cnlohr/ch32fun/ch32fun/ch32fun.mk

flash : cv_flash
clean : cv_clean
