#include <LPC21xx.H>
#include "can.h"

#define	CAN_MOD_RM		1

#define	CAN_BTR_BRP_value		 	 29
#define	CAN_BTR_SJW_value			(0		<< 14)
#define	CAN_BTR_TSEG1_value		(0xC	<< 16)
#define	CAN_BTR_TSEG2_value		(0x1	<< 20)
#define	CAN_BTR_SAM_value			(0x0	<< 23)

//CAN1 TX pin - TD1
#define CAN1_PINSEL1_RD1	(0x01 << 18)	//pin P0.25
#define	CAN2_PINSEL1_RD2	(0x01 << 14)	//pin P0.23
#define	CAN2_PINSEL1_TD2	(0x01 << 16)	//pin P0.24

#define	CAN_TXMSG_PRIO	 0x00
#define	CAN_TXMSG_DLC		(0x1	<< 16)
#define	CAN_TXMSG_RTR		(0x1	<< 30)
#define	CAN_TXMSG_FF		(0x1	<< 31)

#define	VPBDIV_NOSCALING_value	0x01

#define	CAN_AFMR_ACCOFF		 0x01
#define	CAN_AFMR_ACCBP		(0x01 << 1)

#define	CAN_SR_TCS1		0x04
#define	CAN_SR_RBS		0x01

#define	CAN_CMR_TR			0x01
#define	CAN_CMR_RRB			0x04

#define CAN_ID_MASK		0x7FF
#define	CAN_MSG_MASK	0x00FF

#define	CAN_TRANSMIT_MSG_ID		0x12

void Can1_InitAsTX(){
	
	VPBDIV = VPBDIV_NOSCALING_value;
	
	PINSEL1 |= CAN1_PINSEL1_RD1;
	
	C1MOD |= CAN_MOD_RM;
	C1BTR = (CAN_BTR_BRP_value | CAN_BTR_SJW_value | CAN_BTR_TSEG1_value | CAN_BTR_TSEG2_value | CAN_BTR_SAM_value);
	
	C1TFI1 = (CAN_TXMSG_PRIO | CAN_TXMSG_DLC);
	
	C1MOD &= (~CAN_MOD_RM);
	
}
void Can2_InitAsRX(){
	
	VPBDIV = VPBDIV_NOSCALING_value;
	
	PINSEL1 |= (CAN2_PINSEL1_RD2 | CAN2_PINSEL1_TD2);
	
	C2MOD |= CAN_MOD_RM;
	C2BTR = (CAN_BTR_BRP_value | CAN_BTR_SJW_value | CAN_BTR_TSEG1_value | CAN_BTR_TSEG2_value | CAN_BTR_SAM_value);
	
	AFMR |= (CAN_AFMR_ACCOFF | CAN_AFMR_ACCBP);
	
	C2MOD &= (~CAN_MOD_RM);
}
unsigned char ucCan1_TxFree(){
	return (C1SR & CAN_SR_TCS1);
}
void Can1_SendByte(unsigned int value){
	while (ucCan1_TxFree()==0) { }
	C1TID1 = CAN_TRANSMIT_MSG_ID & CAN_ID_MASK;
	C1TDA1 = value & CAN_MSG_MASK;
	C1CMR = CAN_CMR_TR;
}
unsigned char ucCan2_RxReady(){
	return (C2SR & CAN_SR_RBS);
}
unsigned int ucCan2_ReceiveByte(){
	static unsigned int ret = 0;
	while (ucCan2_RxReady()==0) { }
	ret = C2RDA & CAN_MSG_MASK;
	C2CMR = CAN_CMR_RRB;
	return ret;
}
