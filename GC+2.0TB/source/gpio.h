#pragma once

//#define HW_GPIO_BASE 0x0d8000C0
#define HW_GPIO_BASE 0xCD0000C0

#define HW_GPIOB_OUT		(*((vu32*)(HW_GPIO_BASE + 0x00)))
#define HW_GPIOB_DIR		(*((vu32*)(HW_GPIO_BASE + 0x04)))
#define HW_GPIOB_IN			(*((vu32*)(HW_GPIO_BASE + 0x08)))
#define HW_GPIOB_INTLVL		(*((vu32*)(HW_GPIO_BASE + 0x0C)))
#define HW_GPIOB_INTFLAG	(*((vu32*)(HW_GPIO_BASE + 0x10)))
#define HW_GPIOB_INTMASK	(*((vu32*)(HW_GPIO_BASE + 0x14)))
#define HW_GPIOB_INMIR		(*((vu32*)(HW_GPIO_BASE + 0x18)))
#define HW_GPIO_ENABLE		(*((vu32*)(HW_GPIO_BASE + 0x1C)))
#define HW_GPIO_OUT			(*((vu32*)(HW_GPIO_BASE + 0x20)))
#define HW_GPIO_DIR			(*((vu32*)(HW_GPIO_BASE + 0x24)))
#define HW_GPIO_IN			(*((vu32*)(HW_GPIO_BASE + 0x28)))
#define HW_GPIO_INTLVL		(*((vu32*)(HW_GPIO_BASE + 0x2C)))
#define HW_GPIO_INTFLAG		(*((vu32*)(HW_GPIO_BASE + 0x30)))
#define HW_GPIO_INTMASK		(*((vu32*)(HW_GPIO_BASE + 0x34)))
#define HW_GPIO_INMIR		(*((vu32*)(HW_GPIO_BASE + 0x38)))
#define HW_GPIO_OWNER		(*((vu32*)(HW_GPIO_BASE + 0x3C)))