/*
 * GIE.c
 *
 * Created: 22/10/2023 2:10:44 PM
 *  Author: Kirollos
 */ 

void GIE_voidEnable(void)
{
	asm("SEI");
}

void GIE_voidDisable(void)
{
	asm("CLI");
}