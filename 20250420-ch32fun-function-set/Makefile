all : flash

TARGET:=template

TARGET_MCU?=CH32V003
MINICHLINK:=$(shell dirname $(shell which minichlink))
include ./ch32fun/ch32fun.mk

flash : cv_flash
clean : cv_clean
