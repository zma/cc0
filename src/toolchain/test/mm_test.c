long N_SIZE;
long g_array[268435456];
long END_ARRAY;

void write_runner()
{

	//long *p;
	*(long*)0x100000208 = 0x222222;
	*(long*)0x100000208 = *(long*)0x100000120;	

	//p = (long *)0x300000000;
	// hard code here
	// the way to get the address of "g_array" is:
	//  1. ./disacc mm_test.c mm_test debug
	//  2. find the main function
	//  3. in the main function, find the line corresponding to
	//      p = &N_SIZE;
	//      add the address of N_SIZE by 8, it is what we need!
	*((long *)0x300000000) = 0x400010010;

write_j1:
    if (*((long *)0x300000000) < END_ARRAY) {
		asm("mov $888888:msq, 0(0x300000000):msq");
		asm("mov $888888:msq, 8(0x300000000):msq");
		asm("mov $888888:msq, 16(0x300000000):msq");
		asm("mov $888888:msq, 24(0x300000000):msq");
		asm("mov $888888:msq, 32(0x300000000):msq");
		asm("mov $888888:msq, 40(0x300000000):msq");
		asm("mov $888888:msq, 48(0x300000000):msq");
		asm("mov $888888:msq, 56(0x300000000):msq");
		asm("mov $888888:msq, 64(0x300000000):msq");
		asm("mov $888888:msq, 72(0x300000000):msq");
		asm("mov $888888:msq, 80(0x300000000):msq");
		asm("mov $888888:msq, 88(0x300000000):msq");
		asm("mov $888888:msq, 96(0x300000000):msq");
		asm("mov $888888:msq, 104(0x300000000):msq");
		asm("mov $888888:msq, 112(0x300000000):msq");
		asm("mov $888888:msq, 120(0x300000000):msq");
		asm("mov $888888:msq, 128(0x300000000):msq");
		asm("mov $888888:msq, 136(0x300000000):msq");
		asm("mov $888888:msq, 144(0x300000000):msq");
		asm("mov $888888:msq, 152(0x300000000):msq");
		asm("mov $888888:msq, 160(0x300000000):msq");
		asm("mov $888888:msq, 168(0x300000000):msq");
		asm("mov $888888:msq, 176(0x300000000):msq");
		asm("mov $888888:msq, 184(0x300000000):msq");
		asm("mov $888888:msq, 192(0x300000000):msq");
		asm("mov $888888:msq, 200(0x300000000):msq");
		asm("mov $888888:msq, 208(0x300000000):msq");
		asm("mov $888888:msq, 216(0x300000000):msq");
		asm("mov $888888:msq, 224(0x300000000):msq");
		asm("mov $888888:msq, 232(0x300000000):msq");
		asm("mov $888888:msq, 240(0x300000000):msq");
		asm("mov $888888:msq, 248(0x300000000):msq");
		asm("mov $888888:msq, 256(0x300000000):msq");
		asm("mov $888888:msq, 264(0x300000000):msq");
		asm("mov $888888:msq, 272(0x300000000):msq");
		asm("mov $888888:msq, 280(0x300000000):msq");
		asm("mov $888888:msq, 288(0x300000000):msq");
		asm("mov $888888:msq, 296(0x300000000):msq");
		asm("mov $888888:msq, 304(0x300000000):msq");
		asm("mov $888888:msq, 312(0x300000000):msq");
		asm("mov $888888:msq, 320(0x300000000):msq");
		asm("mov $888888:msq, 328(0x300000000):msq");
		asm("mov $888888:msq, 336(0x300000000):msq");
		asm("mov $888888:msq, 344(0x300000000):msq");
		asm("mov $888888:msq, 352(0x300000000):msq");
		asm("mov $888888:msq, 360(0x300000000):msq");
		asm("mov $888888:msq, 368(0x300000000):msq");
		asm("mov $888888:msq, 376(0x300000000):msq");
		asm("mov $888888:msq, 384(0x300000000):msq");
		asm("mov $888888:msq, 392(0x300000000):msq");
		asm("mov $888888:msq, 400(0x300000000):msq");
		asm("mov $888888:msq, 408(0x300000000):msq");
		asm("mov $888888:msq, 416(0x300000000):msq");
		asm("mov $888888:msq, 424(0x300000000):msq");
		asm("mov $888888:msq, 432(0x300000000):msq");
		asm("mov $888888:msq, 440(0x300000000):msq");
		asm("mov $888888:msq, 448(0x300000000):msq");
		asm("mov $888888:msq, 456(0x300000000):msq");
		asm("mov $888888:msq, 464(0x300000000):msq");
		asm("mov $888888:msq, 472(0x300000000):msq");
		asm("mov $888888:msq, 480(0x300000000):msq");
		asm("mov $888888:msq, 488(0x300000000):msq");
		asm("mov $888888:msq, 496(0x300000000):msq");
		asm("mov $888888:msq, 504(0x300000000):msq");
		asm("mov $888888:msq, 512(0x300000000):msq");
		asm("mov $888888:msq, 520(0x300000000):msq");
		asm("mov $888888:msq, 528(0x300000000):msq");
		asm("mov $888888:msq, 536(0x300000000):msq");
		asm("mov $888888:msq, 544(0x300000000):msq");
		asm("mov $888888:msq, 552(0x300000000):msq");
		asm("mov $888888:msq, 560(0x300000000):msq");
		asm("mov $888888:msq, 568(0x300000000):msq");
		asm("mov $888888:msq, 576(0x300000000):msq");
		asm("mov $888888:msq, 584(0x300000000):msq");
		asm("mov $888888:msq, 592(0x300000000):msq");
		asm("mov $888888:msq, 600(0x300000000):msq");
		asm("mov $888888:msq, 608(0x300000000):msq");
		asm("mov $888888:msq, 616(0x300000000):msq");
		asm("mov $888888:msq, 624(0x300000000):msq");
		asm("mov $888888:msq, 632(0x300000000):msq");
		asm("mov $888888:msq, 640(0x300000000):msq");
		asm("mov $888888:msq, 648(0x300000000):msq");
		asm("mov $888888:msq, 656(0x300000000):msq");
		asm("mov $888888:msq, 664(0x300000000):msq");
		asm("mov $888888:msq, 672(0x300000000):msq");
		asm("mov $888888:msq, 680(0x300000000):msq");
		asm("mov $888888:msq, 688(0x300000000):msq");
		asm("mov $888888:msq, 696(0x300000000):msq");
		asm("mov $888888:msq, 704(0x300000000):msq");
		asm("mov $888888:msq, 712(0x300000000):msq");
		asm("mov $888888:msq, 720(0x300000000):msq");
		asm("mov $888888:msq, 728(0x300000000):msq");
		asm("mov $888888:msq, 736(0x300000000):msq");
		asm("mov $888888:msq, 744(0x300000000):msq");
		asm("mov $888888:msq, 752(0x300000000):msq");
		asm("mov $888888:msq, 760(0x300000000):msq");
		asm("mov $888888:msq, 768(0x300000000):msq");
		asm("mov $888888:msq, 776(0x300000000):msq");
		asm("mov $888888:msq, 784(0x300000000):msq");
		asm("mov $888888:msq, 792(0x300000000):msq");
		asm("mov $888888:msq, 800(0x300000000):msq");
		asm("mov $888888:msq, 808(0x300000000):msq");
		asm("mov $888888:msq, 816(0x300000000):msq");
		asm("mov $888888:msq, 824(0x300000000):msq");
		asm("mov $888888:msq, 832(0x300000000):msq");
		asm("mov $888888:msq, 840(0x300000000):msq");
		asm("mov $888888:msq, 848(0x300000000):msq");
		asm("mov $888888:msq, 856(0x300000000):msq");
		asm("mov $888888:msq, 864(0x300000000):msq");
		asm("mov $888888:msq, 872(0x300000000):msq");
		asm("mov $888888:msq, 880(0x300000000):msq");
		asm("mov $888888:msq, 888(0x300000000):msq");
		asm("mov $888888:msq, 896(0x300000000):msq");
		asm("mov $888888:msq, 904(0x300000000):msq");
		asm("mov $888888:msq, 912(0x300000000):msq");
		asm("mov $888888:msq, 920(0x300000000):msq");
		asm("mov $888888:msq, 928(0x300000000):msq");
		asm("mov $888888:msq, 936(0x300000000):msq");
		asm("mov $888888:msq, 944(0x300000000):msq");
		asm("mov $888888:msq, 952(0x300000000):msq");
		asm("mov $888888:msq, 960(0x300000000):msq");
		asm("mov $888888:msq, 968(0x300000000):msq");
		asm("mov $888888:msq, 976(0x300000000):msq");
		asm("mov $888888:msq, 984(0x300000000):msq");
		asm("mov $888888:msq, 992(0x300000000):msq");
		asm("mov $888888:msq, 1000(0x300000000):msq");
		asm("mov $888888:msq, 1008(0x300000000):msq");
		asm("mov $888888:msq, 1016(0x300000000):msq");
		asm("mov $888888:msq, 1024(0x300000000):msq");
		asm("mov $888888:msq, 1032(0x300000000):msq");
		asm("mov $888888:msq, 1040(0x300000000):msq");
		asm("mov $888888:msq, 1048(0x300000000):msq");
		asm("mov $888888:msq, 1056(0x300000000):msq");
		asm("mov $888888:msq, 1064(0x300000000):msq");
		asm("mov $888888:msq, 1072(0x300000000):msq");
		asm("mov $888888:msq, 1080(0x300000000):msq");
		asm("mov $888888:msq, 1088(0x300000000):msq");
		asm("mov $888888:msq, 1096(0x300000000):msq");
		asm("mov $888888:msq, 1104(0x300000000):msq");
		asm("mov $888888:msq, 1112(0x300000000):msq");
		asm("mov $888888:msq, 1120(0x300000000):msq");
		asm("mov $888888:msq, 1128(0x300000000):msq");
		asm("mov $888888:msq, 1136(0x300000000):msq");
		asm("mov $888888:msq, 1144(0x300000000):msq");
		asm("mov $888888:msq, 1152(0x300000000):msq");
		asm("mov $888888:msq, 1160(0x300000000):msq");
		asm("mov $888888:msq, 1168(0x300000000):msq");
		asm("mov $888888:msq, 1176(0x300000000):msq");
		asm("mov $888888:msq, 1184(0x300000000):msq");
		asm("mov $888888:msq, 1192(0x300000000):msq");
		asm("mov $888888:msq, 1200(0x300000000):msq");
		asm("mov $888888:msq, 1208(0x300000000):msq");
		asm("mov $888888:msq, 1216(0x300000000):msq");
		asm("mov $888888:msq, 1224(0x300000000):msq");
		asm("mov $888888:msq, 1232(0x300000000):msq");
		asm("mov $888888:msq, 1240(0x300000000):msq");
		asm("mov $888888:msq, 1248(0x300000000):msq");
		asm("mov $888888:msq, 1256(0x300000000):msq");
		asm("mov $888888:msq, 1264(0x300000000):msq");
		asm("mov $888888:msq, 1272(0x300000000):msq");
		asm("mov $888888:msq, 1280(0x300000000):msq");
		asm("mov $888888:msq, 1288(0x300000000):msq");
		asm("mov $888888:msq, 1296(0x300000000):msq");
		asm("mov $888888:msq, 1304(0x300000000):msq");
		asm("mov $888888:msq, 1312(0x300000000):msq");
		asm("mov $888888:msq, 1320(0x300000000):msq");
		asm("mov $888888:msq, 1328(0x300000000):msq");
		asm("mov $888888:msq, 1336(0x300000000):msq");
		asm("mov $888888:msq, 1344(0x300000000):msq");
		asm("mov $888888:msq, 1352(0x300000000):msq");
		asm("mov $888888:msq, 1360(0x300000000):msq");
		asm("mov $888888:msq, 1368(0x300000000):msq");
		asm("mov $888888:msq, 1376(0x300000000):msq");
		asm("mov $888888:msq, 1384(0x300000000):msq");
		asm("mov $888888:msq, 1392(0x300000000):msq");
		asm("mov $888888:msq, 1400(0x300000000):msq");
		asm("mov $888888:msq, 1408(0x300000000):msq");
		asm("mov $888888:msq, 1416(0x300000000):msq");
		asm("mov $888888:msq, 1424(0x300000000):msq");
		asm("mov $888888:msq, 1432(0x300000000):msq");
		asm("mov $888888:msq, 1440(0x300000000):msq");
		asm("mov $888888:msq, 1448(0x300000000):msq");
		asm("mov $888888:msq, 1456(0x300000000):msq");
		asm("mov $888888:msq, 1464(0x300000000):msq");
		asm("mov $888888:msq, 1472(0x300000000):msq");
		asm("mov $888888:msq, 1480(0x300000000):msq");
		asm("mov $888888:msq, 1488(0x300000000):msq");
		asm("mov $888888:msq, 1496(0x300000000):msq");
		asm("mov $888888:msq, 1504(0x300000000):msq");
		asm("mov $888888:msq, 1512(0x300000000):msq");
		asm("mov $888888:msq, 1520(0x300000000):msq");
		asm("mov $888888:msq, 1528(0x300000000):msq");
		asm("mov $888888:msq, 1536(0x300000000):msq");
		asm("mov $888888:msq, 1544(0x300000000):msq");
		asm("mov $888888:msq, 1552(0x300000000):msq");
		asm("mov $888888:msq, 1560(0x300000000):msq");
		asm("mov $888888:msq, 1568(0x300000000):msq");
		asm("mov $888888:msq, 1576(0x300000000):msq");
		asm("mov $888888:msq, 1584(0x300000000):msq");
		asm("mov $888888:msq, 1592(0x300000000):msq");
		asm("mov $888888:msq, 1600(0x300000000):msq");
		asm("mov $888888:msq, 1608(0x300000000):msq");
		asm("mov $888888:msq, 1616(0x300000000):msq");
		asm("mov $888888:msq, 1624(0x300000000):msq");
		asm("mov $888888:msq, 1632(0x300000000):msq");
		asm("mov $888888:msq, 1640(0x300000000):msq");
		asm("mov $888888:msq, 1648(0x300000000):msq");
		asm("mov $888888:msq, 1656(0x300000000):msq");
		asm("mov $888888:msq, 1664(0x300000000):msq");
		asm("mov $888888:msq, 1672(0x300000000):msq");
		asm("mov $888888:msq, 1680(0x300000000):msq");
		asm("mov $888888:msq, 1688(0x300000000):msq");
		asm("mov $888888:msq, 1696(0x300000000):msq");
		asm("mov $888888:msq, 1704(0x300000000):msq");
		asm("mov $888888:msq, 1712(0x300000000):msq");
		asm("mov $888888:msq, 1720(0x300000000):msq");
		asm("mov $888888:msq, 1728(0x300000000):msq");
		asm("mov $888888:msq, 1736(0x300000000):msq");
		asm("mov $888888:msq, 1744(0x300000000):msq");
		asm("mov $888888:msq, 1752(0x300000000):msq");
		asm("mov $888888:msq, 1760(0x300000000):msq");
		asm("mov $888888:msq, 1768(0x300000000):msq");
		asm("mov $888888:msq, 1776(0x300000000):msq");
		asm("mov $888888:msq, 1784(0x300000000):msq");
		asm("mov $888888:msq, 1792(0x300000000):msq");
		asm("mov $888888:msq, 1800(0x300000000):msq");
		asm("mov $888888:msq, 1808(0x300000000):msq");
		asm("mov $888888:msq, 1816(0x300000000):msq");
		asm("mov $888888:msq, 1824(0x300000000):msq");
		asm("mov $888888:msq, 1832(0x300000000):msq");
		asm("mov $888888:msq, 1840(0x300000000):msq");
		asm("mov $888888:msq, 1848(0x300000000):msq");
		asm("mov $888888:msq, 1856(0x300000000):msq");
		asm("mov $888888:msq, 1864(0x300000000):msq");
		asm("mov $888888:msq, 1872(0x300000000):msq");
		asm("mov $888888:msq, 1880(0x300000000):msq");
		asm("mov $888888:msq, 1888(0x300000000):msq");
		asm("mov $888888:msq, 1896(0x300000000):msq");
		asm("mov $888888:msq, 1904(0x300000000):msq");
		asm("mov $888888:msq, 1912(0x300000000):msq");
		asm("mov $888888:msq, 1920(0x300000000):msq");
		asm("mov $888888:msq, 1928(0x300000000):msq");
		asm("mov $888888:msq, 1936(0x300000000):msq");
		asm("mov $888888:msq, 1944(0x300000000):msq");
		asm("mov $888888:msq, 1952(0x300000000):msq");
		asm("mov $888888:msq, 1960(0x300000000):msq");
		asm("mov $888888:msq, 1968(0x300000000):msq");
		asm("mov $888888:msq, 1976(0x300000000):msq");
		asm("mov $888888:msq, 1984(0x300000000):msq");
		asm("mov $888888:msq, 1992(0x300000000):msq");
		asm("mov $888888:msq, 2000(0x300000000):msq");
		asm("mov $888888:msq, 2008(0x300000000):msq");
		asm("mov $888888:msq, 2016(0x300000000):msq");
		asm("mov $888888:msq, 2024(0x300000000):msq");
		asm("mov $888888:msq, 2032(0x300000000):msq");
		asm("mov $888888:msq, 2040(0x300000000):msq");
		*((long *)0x300000000) = *((long *)0x300000000) + 2048;
		goto write_j1;
    }

    commit;
}

void null_runner()
{
//	long *p;
	*(long*)0x100000208 = 0x333333;
	*(long*)0x100000208 = *(long*)0x100000120;	

	//p = (long *)0x300000000;
	// hard code here
	// the way to get the address of "g_array" is:
	//  1. ./disacc mm_test.c mm_test debug
	//  2. find the main function
	//  3. in the main function, find the line corresponding to
	//      p = &N_SIZE;
	//      add the address of N_SIZE by 8, it is what we need!
	//*p = 0x400010010;
	*((long *)0x300000000)=0x400010010;
null_j1:
    if (*((long *)0x300000000) < END_ARRAY) {
		*((long *)0x300000000) = *((long *)0x300000000) + 2048;
		goto null_j1;
	}
   commit;
}

void read_runner()
{
	long i;
//	long *p;
	*(long*)0x100000208 = 0x111111;
	*(long*)0x100000208 = *(long*)0x100000120;	
//	p = (long *)0x300000000;
	// hard code here
	// the way to get the address of "g_array" is:
	//  1. ./disacc mm_test.c mm_test debug
	//  2. find the main function
	//  3. in the main function, find the line corresponding to
	//      p = &N_SIZE;
	//      add the address of N_SIZE by 8, it is what we need!
	*((long *)0x300000000) = 0x400010010;

read_j1:
    if (*((long *)0x300000000) < END_ARRAY) {
		asm("mov 0(0x300000000):msq, 0x200000010:msq");
		asm("mov 8(0x300000000):msq, 0x200000010:msq");
		asm("mov 16(0x300000000):msq, 0x200000010:msq");
		asm("mov 24(0x300000000):msq, 0x200000010:msq");
		asm("mov 32(0x300000000):msq, 0x200000010:msq");
		asm("mov 40(0x300000000):msq, 0x200000010:msq");
		asm("mov 48(0x300000000):msq, 0x200000010:msq");
		asm("mov 56(0x300000000):msq, 0x200000010:msq");
		asm("mov 64(0x300000000):msq, 0x200000010:msq");
		asm("mov 72(0x300000000):msq, 0x200000010:msq");
		asm("mov 80(0x300000000):msq, 0x200000010:msq");
		asm("mov 88(0x300000000):msq, 0x200000010:msq");
		asm("mov 96(0x300000000):msq, 0x200000010:msq");
		asm("mov 104(0x300000000):msq, 0x200000010:msq");
		asm("mov 112(0x300000000):msq, 0x200000010:msq");
		asm("mov 120(0x300000000):msq, 0x200000010:msq");
		asm("mov 128(0x300000000):msq, 0x200000010:msq");
		asm("mov 136(0x300000000):msq, 0x200000010:msq");
		asm("mov 144(0x300000000):msq, 0x200000010:msq");
		asm("mov 152(0x300000000):msq, 0x200000010:msq");
		asm("mov 160(0x300000000):msq, 0x200000010:msq");
		asm("mov 168(0x300000000):msq, 0x200000010:msq");
		asm("mov 176(0x300000000):msq, 0x200000010:msq");
		asm("mov 184(0x300000000):msq, 0x200000010:msq");
		asm("mov 192(0x300000000):msq, 0x200000010:msq");
		asm("mov 200(0x300000000):msq, 0x200000010:msq");
		asm("mov 208(0x300000000):msq, 0x200000010:msq");
		asm("mov 216(0x300000000):msq, 0x200000010:msq");
		asm("mov 224(0x300000000):msq, 0x200000010:msq");
		asm("mov 232(0x300000000):msq, 0x200000010:msq");
		asm("mov 240(0x300000000):msq, 0x200000010:msq");
		asm("mov 248(0x300000000):msq, 0x200000010:msq");
		asm("mov 256(0x300000000):msq, 0x200000010:msq");
		asm("mov 264(0x300000000):msq, 0x200000010:msq");
		asm("mov 272(0x300000000):msq, 0x200000010:msq");
		asm("mov 280(0x300000000):msq, 0x200000010:msq");
		asm("mov 288(0x300000000):msq, 0x200000010:msq");
		asm("mov 296(0x300000000):msq, 0x200000010:msq");
		asm("mov 304(0x300000000):msq, 0x200000010:msq");
		asm("mov 312(0x300000000):msq, 0x200000010:msq");
		asm("mov 320(0x300000000):msq, 0x200000010:msq");
		asm("mov 328(0x300000000):msq, 0x200000010:msq");
		asm("mov 336(0x300000000):msq, 0x200000010:msq");
		asm("mov 344(0x300000000):msq, 0x200000010:msq");
		asm("mov 352(0x300000000):msq, 0x200000010:msq");
		asm("mov 360(0x300000000):msq, 0x200000010:msq");
		asm("mov 368(0x300000000):msq, 0x200000010:msq");
		asm("mov 376(0x300000000):msq, 0x200000010:msq");
		asm("mov 384(0x300000000):msq, 0x200000010:msq");
		asm("mov 392(0x300000000):msq, 0x200000010:msq");
		asm("mov 400(0x300000000):msq, 0x200000010:msq");
		asm("mov 408(0x300000000):msq, 0x200000010:msq");
		asm("mov 416(0x300000000):msq, 0x200000010:msq");
		asm("mov 424(0x300000000):msq, 0x200000010:msq");
		asm("mov 432(0x300000000):msq, 0x200000010:msq");
		asm("mov 440(0x300000000):msq, 0x200000010:msq");
		asm("mov 448(0x300000000):msq, 0x200000010:msq");
		asm("mov 456(0x300000000):msq, 0x200000010:msq");
		asm("mov 464(0x300000000):msq, 0x200000010:msq");
		asm("mov 472(0x300000000):msq, 0x200000010:msq");
		asm("mov 480(0x300000000):msq, 0x200000010:msq");
		asm("mov 488(0x300000000):msq, 0x200000010:msq");
		asm("mov 496(0x300000000):msq, 0x200000010:msq");
		asm("mov 504(0x300000000):msq, 0x200000010:msq");
		asm("mov 512(0x300000000):msq, 0x200000010:msq");
		asm("mov 520(0x300000000):msq, 0x200000010:msq");
		asm("mov 528(0x300000000):msq, 0x200000010:msq");
		asm("mov 536(0x300000000):msq, 0x200000010:msq");
		asm("mov 544(0x300000000):msq, 0x200000010:msq");
		asm("mov 552(0x300000000):msq, 0x200000010:msq");
		asm("mov 560(0x300000000):msq, 0x200000010:msq");
		asm("mov 568(0x300000000):msq, 0x200000010:msq");
		asm("mov 576(0x300000000):msq, 0x200000010:msq");
		asm("mov 584(0x300000000):msq, 0x200000010:msq");
		asm("mov 592(0x300000000):msq, 0x200000010:msq");
		asm("mov 600(0x300000000):msq, 0x200000010:msq");
		asm("mov 608(0x300000000):msq, 0x200000010:msq");
		asm("mov 616(0x300000000):msq, 0x200000010:msq");
		asm("mov 624(0x300000000):msq, 0x200000010:msq");
		asm("mov 632(0x300000000):msq, 0x200000010:msq");
		asm("mov 640(0x300000000):msq, 0x200000010:msq");
		asm("mov 648(0x300000000):msq, 0x200000010:msq");
		asm("mov 656(0x300000000):msq, 0x200000010:msq");
		asm("mov 664(0x300000000):msq, 0x200000010:msq");
		asm("mov 672(0x300000000):msq, 0x200000010:msq");
		asm("mov 680(0x300000000):msq, 0x200000010:msq");
		asm("mov 688(0x300000000):msq, 0x200000010:msq");
		asm("mov 696(0x300000000):msq, 0x200000010:msq");
		asm("mov 704(0x300000000):msq, 0x200000010:msq");
		asm("mov 712(0x300000000):msq, 0x200000010:msq");
		asm("mov 720(0x300000000):msq, 0x200000010:msq");
		asm("mov 728(0x300000000):msq, 0x200000010:msq");
		asm("mov 736(0x300000000):msq, 0x200000010:msq");
		asm("mov 744(0x300000000):msq, 0x200000010:msq");
		asm("mov 752(0x300000000):msq, 0x200000010:msq");
		asm("mov 760(0x300000000):msq, 0x200000010:msq");
		asm("mov 768(0x300000000):msq, 0x200000010:msq");
		asm("mov 776(0x300000000):msq, 0x200000010:msq");
		asm("mov 784(0x300000000):msq, 0x200000010:msq");
		asm("mov 792(0x300000000):msq, 0x200000010:msq");
		asm("mov 800(0x300000000):msq, 0x200000010:msq");
		asm("mov 808(0x300000000):msq, 0x200000010:msq");
		asm("mov 816(0x300000000):msq, 0x200000010:msq");
		asm("mov 824(0x300000000):msq, 0x200000010:msq");
		asm("mov 832(0x300000000):msq, 0x200000010:msq");
		asm("mov 840(0x300000000):msq, 0x200000010:msq");
		asm("mov 848(0x300000000):msq, 0x200000010:msq");
		asm("mov 856(0x300000000):msq, 0x200000010:msq");
		asm("mov 864(0x300000000):msq, 0x200000010:msq");
		asm("mov 872(0x300000000):msq, 0x200000010:msq");
		asm("mov 880(0x300000000):msq, 0x200000010:msq");
		asm("mov 888(0x300000000):msq, 0x200000010:msq");
		asm("mov 896(0x300000000):msq, 0x200000010:msq");
		asm("mov 904(0x300000000):msq, 0x200000010:msq");
		asm("mov 912(0x300000000):msq, 0x200000010:msq");
		asm("mov 920(0x300000000):msq, 0x200000010:msq");
		asm("mov 928(0x300000000):msq, 0x200000010:msq");
		asm("mov 936(0x300000000):msq, 0x200000010:msq");
		asm("mov 944(0x300000000):msq, 0x200000010:msq");
		asm("mov 952(0x300000000):msq, 0x200000010:msq");
		asm("mov 960(0x300000000):msq, 0x200000010:msq");
		asm("mov 968(0x300000000):msq, 0x200000010:msq");
		asm("mov 976(0x300000000):msq, 0x200000010:msq");
		asm("mov 984(0x300000000):msq, 0x200000010:msq");
		asm("mov 992(0x300000000):msq, 0x200000010:msq");
		asm("mov 1000(0x300000000):msq, 0x200000010:msq");
		asm("mov 1008(0x300000000):msq, 0x200000010:msq");
		asm("mov 1016(0x300000000):msq, 0x200000010:msq");
		asm("mov 1024(0x300000000):msq, 0x200000010:msq");
		asm("mov 1032(0x300000000):msq, 0x200000010:msq");
		asm("mov 1040(0x300000000):msq, 0x200000010:msq");
		asm("mov 1048(0x300000000):msq, 0x200000010:msq");
		asm("mov 1056(0x300000000):msq, 0x200000010:msq");
		asm("mov 1064(0x300000000):msq, 0x200000010:msq");
		asm("mov 1072(0x300000000):msq, 0x200000010:msq");
		asm("mov 1080(0x300000000):msq, 0x200000010:msq");
		asm("mov 1088(0x300000000):msq, 0x200000010:msq");
		asm("mov 1096(0x300000000):msq, 0x200000010:msq");
		asm("mov 1104(0x300000000):msq, 0x200000010:msq");
		asm("mov 1112(0x300000000):msq, 0x200000010:msq");
		asm("mov 1120(0x300000000):msq, 0x200000010:msq");
		asm("mov 1128(0x300000000):msq, 0x200000010:msq");
		asm("mov 1136(0x300000000):msq, 0x200000010:msq");
		asm("mov 1144(0x300000000):msq, 0x200000010:msq");
		asm("mov 1152(0x300000000):msq, 0x200000010:msq");
		asm("mov 1160(0x300000000):msq, 0x200000010:msq");
		asm("mov 1168(0x300000000):msq, 0x200000010:msq");
		asm("mov 1176(0x300000000):msq, 0x200000010:msq");
		asm("mov 1184(0x300000000):msq, 0x200000010:msq");
		asm("mov 1192(0x300000000):msq, 0x200000010:msq");
		asm("mov 1200(0x300000000):msq, 0x200000010:msq");
		asm("mov 1208(0x300000000):msq, 0x200000010:msq");
		asm("mov 1216(0x300000000):msq, 0x200000010:msq");
		asm("mov 1224(0x300000000):msq, 0x200000010:msq");
		asm("mov 1232(0x300000000):msq, 0x200000010:msq");
		asm("mov 1240(0x300000000):msq, 0x200000010:msq");
		asm("mov 1248(0x300000000):msq, 0x200000010:msq");
		asm("mov 1256(0x300000000):msq, 0x200000010:msq");
		asm("mov 1264(0x300000000):msq, 0x200000010:msq");
		asm("mov 1272(0x300000000):msq, 0x200000010:msq");
		asm("mov 1280(0x300000000):msq, 0x200000010:msq");
		asm("mov 1288(0x300000000):msq, 0x200000010:msq");
		asm("mov 1296(0x300000000):msq, 0x200000010:msq");
		asm("mov 1304(0x300000000):msq, 0x200000010:msq");
		asm("mov 1312(0x300000000):msq, 0x200000010:msq");
		asm("mov 1320(0x300000000):msq, 0x200000010:msq");
		asm("mov 1328(0x300000000):msq, 0x200000010:msq");
		asm("mov 1336(0x300000000):msq, 0x200000010:msq");
		asm("mov 1344(0x300000000):msq, 0x200000010:msq");
		asm("mov 1352(0x300000000):msq, 0x200000010:msq");
		asm("mov 1360(0x300000000):msq, 0x200000010:msq");
		asm("mov 1368(0x300000000):msq, 0x200000010:msq");
		asm("mov 1376(0x300000000):msq, 0x200000010:msq");
		asm("mov 1384(0x300000000):msq, 0x200000010:msq");
		asm("mov 1392(0x300000000):msq, 0x200000010:msq");
		asm("mov 1400(0x300000000):msq, 0x200000010:msq");
		asm("mov 1408(0x300000000):msq, 0x200000010:msq");
		asm("mov 1416(0x300000000):msq, 0x200000010:msq");
		asm("mov 1424(0x300000000):msq, 0x200000010:msq");
		asm("mov 1432(0x300000000):msq, 0x200000010:msq");
		asm("mov 1440(0x300000000):msq, 0x200000010:msq");
		asm("mov 1448(0x300000000):msq, 0x200000010:msq");
		asm("mov 1456(0x300000000):msq, 0x200000010:msq");
		asm("mov 1464(0x300000000):msq, 0x200000010:msq");
		asm("mov 1472(0x300000000):msq, 0x200000010:msq");
		asm("mov 1480(0x300000000):msq, 0x200000010:msq");
		asm("mov 1488(0x300000000):msq, 0x200000010:msq");
		asm("mov 1496(0x300000000):msq, 0x200000010:msq");
		asm("mov 1504(0x300000000):msq, 0x200000010:msq");
		asm("mov 1512(0x300000000):msq, 0x200000010:msq");
		asm("mov 1520(0x300000000):msq, 0x200000010:msq");
		asm("mov 1528(0x300000000):msq, 0x200000010:msq");
		asm("mov 1536(0x300000000):msq, 0x200000010:msq");
		asm("mov 1544(0x300000000):msq, 0x200000010:msq");
		asm("mov 1552(0x300000000):msq, 0x200000010:msq");
		asm("mov 1560(0x300000000):msq, 0x200000010:msq");
		asm("mov 1568(0x300000000):msq, 0x200000010:msq");
		asm("mov 1576(0x300000000):msq, 0x200000010:msq");
		asm("mov 1584(0x300000000):msq, 0x200000010:msq");
		asm("mov 1592(0x300000000):msq, 0x200000010:msq");
		asm("mov 1600(0x300000000):msq, 0x200000010:msq");
		asm("mov 1608(0x300000000):msq, 0x200000010:msq");
		asm("mov 1616(0x300000000):msq, 0x200000010:msq");
		asm("mov 1624(0x300000000):msq, 0x200000010:msq");
		asm("mov 1632(0x300000000):msq, 0x200000010:msq");
		asm("mov 1640(0x300000000):msq, 0x200000010:msq");
		asm("mov 1648(0x300000000):msq, 0x200000010:msq");
		asm("mov 1656(0x300000000):msq, 0x200000010:msq");
		asm("mov 1664(0x300000000):msq, 0x200000010:msq");
		asm("mov 1672(0x300000000):msq, 0x200000010:msq");
		asm("mov 1680(0x300000000):msq, 0x200000010:msq");
		asm("mov 1688(0x300000000):msq, 0x200000010:msq");
		asm("mov 1696(0x300000000):msq, 0x200000010:msq");
		asm("mov 1704(0x300000000):msq, 0x200000010:msq");
		asm("mov 1712(0x300000000):msq, 0x200000010:msq");
		asm("mov 1720(0x300000000):msq, 0x200000010:msq");
		asm("mov 1728(0x300000000):msq, 0x200000010:msq");
		asm("mov 1736(0x300000000):msq, 0x200000010:msq");
		asm("mov 1744(0x300000000):msq, 0x200000010:msq");
		asm("mov 1752(0x300000000):msq, 0x200000010:msq");
		asm("mov 1760(0x300000000):msq, 0x200000010:msq");
		asm("mov 1768(0x300000000):msq, 0x200000010:msq");
		asm("mov 1776(0x300000000):msq, 0x200000010:msq");
		asm("mov 1784(0x300000000):msq, 0x200000010:msq");
		asm("mov 1792(0x300000000):msq, 0x200000010:msq");
		asm("mov 1800(0x300000000):msq, 0x200000010:msq");
		asm("mov 1808(0x300000000):msq, 0x200000010:msq");
		asm("mov 1816(0x300000000):msq, 0x200000010:msq");
		asm("mov 1824(0x300000000):msq, 0x200000010:msq");
		asm("mov 1832(0x300000000):msq, 0x200000010:msq");
		asm("mov 1840(0x300000000):msq, 0x200000010:msq");
		asm("mov 1848(0x300000000):msq, 0x200000010:msq");
		asm("mov 1856(0x300000000):msq, 0x200000010:msq");
		asm("mov 1864(0x300000000):msq, 0x200000010:msq");
		asm("mov 1872(0x300000000):msq, 0x200000010:msq");
		asm("mov 1880(0x300000000):msq, 0x200000010:msq");
		asm("mov 1888(0x300000000):msq, 0x200000010:msq");
		asm("mov 1896(0x300000000):msq, 0x200000010:msq");
		asm("mov 1904(0x300000000):msq, 0x200000010:msq");
		asm("mov 1912(0x300000000):msq, 0x200000010:msq");
		asm("mov 1920(0x300000000):msq, 0x200000010:msq");
		asm("mov 1928(0x300000000):msq, 0x200000010:msq");
		asm("mov 1936(0x300000000):msq, 0x200000010:msq");
		asm("mov 1944(0x300000000):msq, 0x200000010:msq");
		asm("mov 1952(0x300000000):msq, 0x200000010:msq");
		asm("mov 1960(0x300000000):msq, 0x200000010:msq");
		asm("mov 1968(0x300000000):msq, 0x200000010:msq");
		asm("mov 1976(0x300000000):msq, 0x200000010:msq");
		asm("mov 1984(0x300000000):msq, 0x200000010:msq");
		asm("mov 1992(0x300000000):msq, 0x200000010:msq");
		asm("mov 2000(0x300000000):msq, 0x200000010:msq");
		asm("mov 2008(0x300000000):msq, 0x200000010:msq");
		asm("mov 2016(0x300000000):msq, 0x200000010:msq");
		asm("mov 2024(0x300000000):msq, 0x200000010:msq");
		asm("mov 2032(0x300000000):msq, 0x200000010:msq");
		asm("mov 2040(0x300000000):msq, 0x200000010:msq");
		*((long *)0x300000000) = *((long *)0x300000000) + 2048;
		goto read_j1;
    }
// For correctness test (When uncomment this, the throughput for read should not be counted)
/*
	i = 0;
correct_j1:
	if (i < N_SIZE) {
		if(g_array[i]!=0x9342+i){
			*(long*)0x100000208 = 0x51515151;	// if this appear, an error occur
		}
		i = i + 1;
		goto correct_j1;
	}
	*(long*)0x100000208 = 0x12344321;	// the test finished;
*/

    runner {write_runner(); } 
    commit;
}

void main(){
//	long *p;
	long i;

//	long *array_ptr;
	
    N_SIZE = 268435456;	// 2GB
	END_ARRAY = 0x400010010 + N_SIZE*8;
//	p = &N_SIZE;
//	p = &no_use;
	i=0;

//	array_ptr = (long *)0x300000000;
//	*array_ptr = 0x400010010;
	
//	asm("mov $888888:msq, 8(0x300000000):msq");
	*(long*)0x100000208 = 0x999999;
	*(long*)0x100000208 = *(long*)0x100000120;	
//	*(long*)0x100000208 = g_array[1];
	
main_j1:
	if (i < N_SIZE) {
		g_array[i]=0x9342+i;
		i = i + 1;
		goto main_j1;
	}
	runner {null_runner(); }
	runner {read_runner(); }
	commit;
}

