/*
 * calibration.c
 *
 * Created: 4/19/2016 11:11:12 AM
 *  Author: renalyx
 */ 
#include "asf.h"
#include "calibration.h"
#include "Platform/Service/sv_interface.h"

volatile float pressure_final_apt;
volatile float pressure_final_vpt=0,pressure_final_ps1=0,pressure_final_ps2=0,pressure_final_ps3=0;
volatile float temprature_final_value_1=0,temprature_final_value_2=0,temprature_final_value_3=0;
volatile float cond_final_cs3;
volatile uint32_t hep_speed;
extern uint8_t sv_cntrl_setpumpspeed(sv_pumptype sv_pump_id,uint32_t speed);
typedef struct
{
	int16_t pressure;
	uint16_t millivolt;
}lut;

/*temptre  Temp1[20]=
{
	{1121,2557},{1159,2674},{1182,2741},{1208,2819},{1243,2925},{1275,3021},{1313,3140},{1392,3392},{1466,3640},{1528,3840},{1593,4025}
};*/
/*temptre  Temp1[20]=
{
	{1214,2890},{1254,3020},{1275,3080},{1300,3158},{1330,3248},{1354,3328},{1385,3426},{1450,3643},{1508,3845},{1560,4018}
};
/*temptre Temp2[20]=
{
	{1114,2438},{1164,2572},{1192,2645},{1226,2731},{1262,2833},{1301,2942},{1366,3125},{1442,3368},{1512,3570},{1576,3750},{1628,3920}
};*/
/*temptre Temp2[20]=
{
	{1232,2839},{1274,2965},{1296,3027},{1328,3114},{1352,3187},{1376,3259},{1434,3424},{1495,3624},{1558,3800},{1600,3950}
};

/*temptre Temp3[20]=
{
	{1373,2601},{1428,2716},{1450,2779},{1474,2854},{1502,2950},{1528,3036},{1555,3134},{1625,3376},{1679,3574},{1732,3765},{1775,3921}
};*/
/*temptre Temp3[20]=
{
	{1320,2796},{1350,2894},{1365,2956},{1385,3020},{1410,3095},{1436,3180},{1465,3265},{1520,3465},{1575,3645},{1615,3790},{1654,3921}
};*/
/*temptre Temp3[20]=
{
	{1317,2780},{1347,2890},{1367,2946},{1385,3011},{1408,3084},{1437,3178},{1467,3279},{1491,3371},{1546,3560},{1593,3720},{1636,3870}
};
/*temptre Temp3[20]=
{
	{1238,2582},{1271,2680},{1296,2740},{1316,2805},{1350,2905},{1378,2995},{1405,3075},{1465,3260},{1503,3436},{1555,3620},{1604,3780},{1635,3910}
};*/
/*temptre Temp1[100]=
{
	{1018,2600},{1027,2625},{1036,2650},{1045,2675},{1054,2700},{1063,2725},{1072,2750},{1081,2775},{1090,2800},{1099,2825},{1108,2850},{1117,2875},{1126,2900},{1135,2925},{1144,2950},{1153,2975},{1162,3000},{1171,3025},{1180,3050},{1189,3075},{1198,3100},{1207,3125},{1216,3150},{1223,3165},{1229,3182},{1237,3210},{1245,3231},{1253,3259},{1265,3285},{1275,3312},
	{1287,3343},{1296,3366},{1304,3393},{1309,3400},{1313,3418},{1326,3444},{1335,3474},{1343,3509},{1352,3527},{1361,3552},{1370,3579},{1383,3614},{1396,3630},{1405,3661},{1414,3689},{1419,3700},{1424,3716},{1433,3743},
	{1440,3766},{1448,3793},{1453,3800},{1461,3825},{1470,3850},{1479,3875},{1488,3900},{1497,3925},{1506,3950},{1515,3975},{1524,4000},{1533,4025},{1542,4050},{1551,4075},{1561,4100},{1570,4125},{1579,4150},{1588,4175},
	{1597,4200},{1606,4225},{1615,4250},{1624,4275},{1633,4300}
};
temptre Temp2[100]=
{
	{1018,2600},{1027,2625},{1036,2650},{1045,2675},{1054,2700},{1063,2725},{1072,2750},{1081,2775},{1090,2800},{1099,2825},{1108,2850},{1117,2875},{1126,2900},{1135,2925},{1144,2950},{1153,2975},{1162,3000},{1171,3025},{1180,3050},{1189,3075},{1198,3100},{1207,3125},{1216,3150},{1223,3165},{1229,3182},{1237,3210},{1245,3231},{1253,3259},{1265,3285},{1275,3312},
	{1287,3343},{1296,3366},{1304,3393},{1309,3400},{1313,3418},{1326,3444},{1335,3474},{1343,3509},{1352,3527},{1361,3552},{1370,3579},{1383,3614},{1396,3630},{1405,3661},{1414,3689},{1419,3700},{1424,3716},{1433,3743},
	{1440,3766},{1448,3793},{1453,3800},{1461,3825},{1470,3850},{1479,3875},{1488,3900},{1497,3925},{1506,3950},{1515,3975},{1524,4000},{1533,4025},{1542,4050},{1551,4075},{1561,4100},{1570,4125},{1579,4150},{1588,4175},
	{1597,4200},{1606,4225},{1615,4250},{1624,4275},{1633,4300}
};
temptre Temp3[100]=
{
	{1018,2600},{1027,2625},{1036,2650},{1045,2675},{1054,2700},{1063,2725},{1072,2750},{1081,2775},{1090,2800},{1099,2825},{1108,2850},{1117,2875},{1126,2900},{1135,2925},{1144,2950},{1153,2975},{1162,3000},{1171,3025},{1180,3050},{1189,3075},{1198,3100},{1207,3125},{1216,3150},{1223,3165},{1229,3182},{1237,3210},{1245,3231},{1253,3259},{1265,3285},{1275,3312},
	{1287,3343},{1296,3366},{1304,3393},{1309,3400},{1313,3418},{1326,3444},{1335,3474},{1343,3509},{1352,3527},{1361,3552},{1370,3579},{1383,3614},{1396,3630},{1405,3661},{1414,3689},{1419,3700},{1424,3716},{1433,3743},
	{1440,3766},{1448,3793},{1453,3800},{1461,3825},{1470,3850},{1479,3875},{1488,3900},{1497,3925},{1506,3950},{1515,3975},{1524,4000},{1533,4025},{1542,4050},{1551,4075},{1561,4100},{1570,4125},{1579,4150},{1588,4175},
	{1597,4200},{1606,4225},{1615,4250},{1624,4275},{1633,4300}
};*/ // old value
temptre Temp1[100]=
{
	{768,2300},{790,2350},{812,2400},{834,2450},{856,2500},{878,2550},{900,2600},{922,2650},{944,2700},{966,2750},{988,2800},{1010,2850},{1032,2900},{1054,2950},{1076,3000},{1098,3050},{1120,3100},{1142,3150},{1164,3200},{1186,3250},
	{1208,3300},{1230,3350},{1252,3400},{1276,3450},{1298,3500},{1320,3550},{1338,3600},{1358,3650},{1377,3700},{1399,3750},{1421,3800},{1443,3850},{1465,3900},{1487,3950},{1509,4000},{1531,4050},{1553,4100},{1575,4150},{1597,4200},{1619,4250},
	{1641,4300}
};
temptre Temp2[100]=
{
	{768,2300},{790,2350},{812,2400},{834,2450},{856,2500},{878,2550},{900,2600},{922,2650},{944,2700},{966,2750},{988,2800},{1010,2850},{1032,2900},{1054,2950},{1076,3000},{1098,3050},{1120,3100},{1142,3150},{1164,3200},{1186,3250},
	{1208,3300},{1230,3350},{1252,3400},{1276,3450},{1298,3500},{1320,3550},{1338,3600},{1358,3650},{1377,3700},{1399,3750},{1421,3800},{1443,3850},{1465,3900},{1487,3950},{1509,4000},{1531,4050},{1553,4100},{1575,4150},{1597,4200},{1619,4250},
	{1641,4300}
};

temptre Temp3[100]=
{
	{768,2300},{790,2350},{812,2400},{834,2450},{856,2500},{878,2550},{900,2600},{922,2650},{944,2700},{966,2750},{988,2800},{1010,2850},{1032,2900},{1054,2950},{1076,3000},{1098,3050},{1120,3100},{1142,3150},{1164,3200},{1186,3250},
	{1208,3300},{1230,3350},{1252,3400},{1276,3450},{1298,3500},{1320,3550},{1338,3600},{1358,3650},{1377,3700},{1399,3750},{1421,3800},{1443,3850},{1465,3900},{1487,3950},{1509,4000},{1531,4050},{1553,4100},{1575,4150},{1597,4200},{1619,4250},
	{1641,4300}
};
/*conduc CS3[20]=
{
	{1317,2780},{1347,2890},{1367,2946},{1385,3011},{1408,3084},{1437,3178},{1467,3279},{1491,3371},{1546,3560},{1593,3720},{1636,3870}
};*/
/*volatile lut vpt1[20]=
{
	{-470,900},{-400,1040},{-300,1200},{-200,1300},{-100,1460},{0,1650},{100,1810},{200,1960},{300,2120},{400,2300},{500,2410},{600,2620},{700,2800},{800,2920},{900,3080},{1000,3240}
};

volatile lut apt[20]=
{
	{-470,900},{-400,1040},{-300,1200},{-200,1300},{-100,1460},{0,1650},{100,1810},{200,1960},{300,2120},{400,2300},{500,2410},{600,2620},{700,2800},{800,2920},{900,3080},{1000,3240}
};*/
volatile lut apt[20]=
{
	{-250,1270},{-200,1350},{-150,1430},{-100,1510},{-50,1590},{0,1670},{50,1750},{100,1840},{150,1920},{200,2000},{250,2080},{300,2160},{350,2240},{400,2320}
};

volatile lut vpt1[20]=
{
	{-250,1280},{-200,1360},{-150,1440},{-100,1520},{-50,1600},{0,1680},{50,1760},{100,1850},{150,1930},{200,2010},{250,2090},{300,2170},{350,2250},{400,2330}
};
volatile lut ps_1[50]=
{
	{-600,390},{-550,450},{-500,510},{-450,570},{-400,630},{-350,690},{-300,750},{-250,810},{-200,870},{-150,930},{-100,980},{-50,1040},{0,1100},{50,1170},{100,1245},{150,1321},{200,1396},{250,1466},{300,1540},{350,1615},{400,1690},{450,1765},{500,1840},{550,1915},{600,1990},{650,2065},{700,2140},{750,2215},{800,2290},{850,2365},{900,2440},{950,2515},{1000,2590},{1050,2665},{1100,2740},{1150,2815},{1200,2890}
};
volatile lut ps_2[50]=
{
	{-600,390},{-550,450},{-500,510},{-450,570},{-400,630},{-350,690},{-300,750},{-250,810},{-200,870},{-150,930},{-100,980},{-50,1040},{0,1100},{50,1170},{100,1245},{150,1321},{200,1396},{250,1466},{300,1540},{350,1615},{400,1690},{450,1765},{500,1840},{550,1915},{600,1990},{650,2065},{700,2140},{750,2215},{800,2290},{850,2365},{900,2440},{950,2515},{1000,2590},{1050,2665},{1100,2740},{1150,2815},{1200,2890}
};
volatile lut ps_3[50]=
{
	{-600,390},{-550,450},{-500,510},{-450,570},{-400,630},{-350,690},{-300,750},{-250,810},{-200,870},{-150,930},{-100,980},{-50,1040},{0,1100},{50,1170},{100,1245},{150,1321},{200,1396},{250,1466},{300,1540},{350,1615},{400,1690},{450,1765},{500,1840},{550,1915},{600,1990},{650,2065},{700,2140},{750,2215},{800,2290},{850,2365},{900,2440},{950,2515},{1000,2590},{1050,2665},{1100,2740},{1150,2815},{1200,2890}
};
volatile hep hp[20]=
{
	{10,25},{9,35},{8,45},{7,55},{6,65},{5,80},{4,100},{2,200},{1,350},{5000,3000},{3000,5500}
};
volatile conduc cond[50]=
{
	{2300,899},{2350,962},{2400,1024},{2450,1086},{2500,1146},{2550,1205},{2600,1264},{2624,1285},{2640,1310},{2659,1335},{2689,1365},{2714,1390},{2738,1420},{2753,1450},{2783,1480},{2811,1510},{2874,1550},{2923,1600},{2940,1650},{2979,1670},{3017,1700},{3050,1747},{3080,1776},{3100,1796},{3120,1815},{3150,1844}
};
void hep_calibration(uint32_t inpt_ml)
{
	int i;
	int16_t slope;
	//uint16_t tempdata=sensordata;
	for (i=0;i<20;i++)
	{
		if (inpt_ml == hp[i].ml)
		{
			hep_speed=hp[i].speed;
			//sv_cntrl_setpumpspeed(HEPARINPUMP,hep_speed);
			break;
		}
		else if ((inpt_ml > hp[i].ml) && (inpt_ml < hp[i+1].ml))
		{
			slope = ((hp[i+1].ml-hp[i].ml)/(hp[i+1].speed-hp[i].speed));
			hep_speed = slope * (inpt_ml-hp[i].ml) + hp[i].speed;
			//sv_cntrl_setpumpspeed(HEPARINPUMP,hep_speed);
			break;
		}
	}
	
}
void calibration_apt(uint16_t sensordata)
{
	int i;
	int16_t slope;
	//uint16_t tempdata=sensordata;
	for (i=0;i<20;i++)
	{
		if (sensordata == apt[i].millivolt)
		{
			pressure_final_apt=apt[i].pressure;
		}
		else if ((sensordata > apt[i].millivolt) && (sensordata < apt[i+1].millivolt))
		{
			slope = ((apt[i+1].millivolt-apt[i].millivolt)/(apt[i+1].pressure-apt[i].pressure))*10;
			pressure_final_apt = (slope/10) * (sensordata-apt[i].millivolt) + apt[i].pressure;
		}
	}
}
void calibration_vpt(uint16_t sensordata)
{
	int i;
	int16_t slope;
	//uint16_t tempdata=sensordata;
	for (i=0;i<20;i++)
	{
		if (sensordata == vpt1[i].millivolt)
		{
			pressure_final_vpt=vpt1[i].pressure;
			break;
		}
		else if ((sensordata > vpt1[i].millivolt) && (sensordata < vpt1[i+1].millivolt))
		{
			slope = ((vpt1[i+1].millivolt-vpt1[i].millivolt)/(vpt1[i+1].pressure-vpt1[i].pressure))*10;
			pressure_final_vpt = (slope/10)*(sensordata-vpt1[i].millivolt) + vpt1[i].pressure;
			break;
		}
	}
	
}
void calibration_ps1(uint16_t sensordata)
{
	int i;
	int16_t slope;
	//uint16_t tempdata=sensordata;
	for (i=0;i<50;i++)
	{
		if (sensordata == ps_1[i].millivolt)
		{
			pressure_final_ps1=ps_1[i].pressure;
			break;
		}
		else if ((sensordata > ps_1[i].millivolt) && (sensordata < ps_1[i+1].millivolt))
		{
			slope = ((ps_1[i+1].millivolt-ps_1[i].millivolt)/(ps_1[i+1].pressure-ps_1[i].pressure))*10;
			pressure_final_ps1 = (slope/10) * (sensordata-ps_1[i].millivolt) + ps_1[i].pressure;
			break;
		}
	}
}
void calibration_ps2(uint16_t sensordata)
{
	int i;
	int16_t slope;
	//uint16_t tempdata=sensordata;
	for (i=0;i<50;i++)
	{
		if (sensordata == ps_2[i].millivolt)
		{
			pressure_final_ps2=ps_2[i].pressure;
			break;
		}
		else if ((sensordata > ps_2[i].millivolt) && (sensordata < ps_2[i+1].millivolt))
		{
			slope = ((ps_2[i+1].millivolt-ps_2[i].millivolt)/(ps_2[i+1].pressure-ps_2[i].pressure))*10;
			pressure_final_ps2 = (slope/10) * (sensordata-ps_2[i].millivolt) + ps_2[i].pressure;
			break;
		}
	}
}
void calibration_ps3(uint16_t sensordata)
{
	int i;
	int16_t slope;
	//uint16_t tempdata=sensordata;
	for (i=0;i<50;i++)
	{
		if (sensordata == ps_3[i].millivolt)
		{
			pressure_final_ps3=ps_3[i].pressure;
			break;
		}
		else if ((sensordata > ps_3[i].millivolt) && (sensordata < ps_3[i+1].millivolt))
		{
			slope = ((ps_3[i+1].millivolt-ps_3[i].millivolt)/(ps_3[i+1].pressure-ps_3[i].pressure))*10;
			pressure_final_ps3 = (slope/10) * (sensordata-ps_3[i].millivolt) + ps_3[i].pressure;
			break;
		}
	}
}

void calibration_cond(float sensordata)
{
	int i;
	float dummy_var = sensordata;
	float slope;
	//uint16_t tempdata=sensordata;
	for (i=0;i<50;i++)
	{
		if (dummy_var == cond[i].volts)
		{
			cond_final_cs3=cond[i].conductivity;
		}
		else if ((dummy_var > cond[i].volts) && (dummy_var < cond[i+1].volts))
		{
			slope = ((cond[i+1].volts-cond[i].volts)/(cond[i+1].conductivity-cond[i].conductivity));
			cond_final_cs3 = slope*(dummy_var-cond[i].volts) + cond[i].conductivity;
		}
		/*else if (sensordata < cond[0].volts )
		{
			slope = (cond[i].volts)/(cond[i].conductivity);
			cond_final_cs3 = slope*(sensordata-cond[i].volts) + cond[i].conductivity;
		}*/
	}
}
void calibration_tmp(float millivolts, temp_state temp_var)
{
	
	int i;
	float dummy_var = millivolts;
	uint32_t dummy_var1;
	float slope;
	float var;
	if (temp_var==TS1)
	{
		for (i=0;i<100;i++)
		{
			if (dummy_var == Temp1[i].volts)
			{
				dummy_var1=Temp1[i].temperature;
				temprature_final_value_1=dummy_var1/100;
				break;
			}
			else if ((dummy_var > Temp1[i].volts) && (dummy_var < Temp1[i+1].volts))
			{
				//temprature_final_value =dummy_var;
				slope = (((Temp1[i+1].volts - Temp1[i].volts)*1000)/(Temp1[i+1].temperature - Temp1[i].temperature));
				//temprature_final_value =slope*1000;
				temprature_final_value_1 = ((slope*(dummy_var - Temp1[i].volts))/1000 + Temp1[i].temperature);
				temprature_final_value_1 =temprature_final_value_1/100;
				//temprature_final_value =1500;
				break;
			}
			/*else if (dummy_var < Temp1[i].volts)
			{
				temprature_final_value_1 = (31.47*(dummy_var/1000)-9.829)*10;
				break;
			}*/
			
		}
		//millivolts = millivolts/1000;
		//temprature_final_value_1 = 1.830098345 * millivolts *millivolts +  22.62524406 *millivolts + 0.581851503  ;
	}
	else if (temp_var==TS2)
	{
		
		for (i=0;i<100;i++)
		{
			if (dummy_var == Temp2[i].volts)
			{
				dummy_var1=Temp2[i].temperature;
				temprature_final_value_2=dummy_var1/100;
				break;
			}
			else if ((dummy_var > Temp2[i].volts) && (dummy_var < Temp2[i+1].volts))
			{
				//temprature_final_value =dummy_var;
				slope = (((Temp2[i+1].volts - Temp2[i].volts)*1000)/(Temp2[i+1].temperature - Temp2[i].temperature));
				//temprature_final_value = slope;
				temprature_final_value_2 = ((slope*(dummy_var - Temp2[i].volts))/1000 + Temp2[i].temperature)/100;
				//temprature_final_value =1500;
				break;
			}
		/*	else if (dummy_var < Temp2[i].volts)
			{
				var = (29.11*(dummy_var/1000)-8.345)*10;
				temprature_final_value_2=var;
				break;
			}*/
			
		}
		//millivolts = millivolts/1000;
		//temprature_final_value_2 = 1.830098345 * millivolts *millivolts +  22.62524406 *millivolts + 0.581851503  ;
	}
	
	else if (temp_var == TS3)
	{
		for (i=0;i<100;i++)
		{
			if (dummy_var == Temp3[i].volts)
			{
				dummy_var1=Temp3[i].temperature;
				temprature_final_value_3=dummy_var1/100;
				break;
			}
			else if ((dummy_var > Temp3[i].volts) && (dummy_var < Temp3[i+1].volts))
			{
				//temprature_final_value =dummy_var;
				slope = (((Temp3[i+1].volts - Temp3[i].volts)*1000)/(Temp3[i+1].temperature - Temp3[i].temperature));
				//temprature_final_value = slope;
				temprature_final_value_3 = ((slope*(dummy_var - Temp3[i].volts))/1000 + Temp3[i].temperature)/100;
				//temprature_final_value =1500;
				break;
			}
		/*	else if (dummy_var < Temp3[i].volts)
			{
				var = (33.58 * (dummy_var/1000) - 16.39)*10;
				//var=dummy_var/100;
				temprature_final_value_3=var;
				break;
			}*/

			
		}
		//millivolts = millivolts/1000;
		//temprature_final_value_3 = 1.830098345 * millivolts *millivolts +  22.62524406 *millivolts + 0.581851503  ;
		
	}
	
}
