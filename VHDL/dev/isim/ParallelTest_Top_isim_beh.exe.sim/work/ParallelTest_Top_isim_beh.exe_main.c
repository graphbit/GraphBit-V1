/**********************************************************************/
/*   ____  ____                                                       */
/*  /   /\/   /                                                       */
/* /___/  \  /                                                        */
/* \   \   \/                                                       */
/*  \   \        Copyright (c) 2003-2009 Xilinx, Inc.                */
/*  /   /          All Right Reserved.                                 */
/* /---/   /\                                                         */
/* \   \  /  \                                                      */
/*  \___\/\___\                                                    */
/***********************************************************************/

#include "xsi.h"

struct XSI_INFO xsi_info;

char *IEEE_P_2592010699;
char *STD_STANDARD;
char *IEEE_P_3620187407;
char *IEEE_P_3499444699;
char *IEEE_P_2717149903;
char *STD_TEXTIO;
char *XILINXCORELIB_P_0347517956;
char *XILINXCORELIB_P_0360711163;
char *XILINXCORELIB_P_1983681889;
char *IEEE_P_1242562249;


int main(int argc, char **argv)
{
    xsi_init_design(argc, argv);
    xsi_register_info(&xsi_info);

    xsi_register_min_prec_unit(-12);
    ieee_p_2592010699_init();
    ieee_p_3499444699_init();
    ieee_p_3620187407_init();
    ieee_p_1242562249_init();
    work_a_1810621361_1872798230_init();
    work_a_0768645600_4213641838_init();
    work_a_4091204014_3212880686_init();
    work_a_3343383091_0272409134_init();
    work_a_3329564118_3306564128_init();
    work_a_2719491174_3467788105_init();
    work_a_2905254507_3212880686_init();
    work_a_0372223081_3212880686_init();
    std_textio_init();
    ieee_p_2717149903_init();
    xilinxcorelib_p_0347517956_init();
    xilinxcorelib_p_0360711163_init();
    xilinxcorelib_p_1983681889_init();
    xilinxcorelib_a_1908178445_3212880686_init();
    work_a_2507496106_3929382911_init();
    work_a_4055272702_3299577371_init();
    work_a_3798590249_3212880686_init();


    xsi_register_tops("work_a_3798590249_3212880686");

    IEEE_P_2592010699 = xsi_get_engine_memory("ieee_p_2592010699");
    xsi_register_ieee_std_logic_1164(IEEE_P_2592010699);
    STD_STANDARD = xsi_get_engine_memory("std_standard");
    IEEE_P_3620187407 = xsi_get_engine_memory("ieee_p_3620187407");
    IEEE_P_3499444699 = xsi_get_engine_memory("ieee_p_3499444699");
    IEEE_P_2717149903 = xsi_get_engine_memory("ieee_p_2717149903");
    STD_TEXTIO = xsi_get_engine_memory("std_textio");
    XILINXCORELIB_P_0347517956 = xsi_get_engine_memory("xilinxcorelib_p_0347517956");
    XILINXCORELIB_P_0360711163 = xsi_get_engine_memory("xilinxcorelib_p_0360711163");
    XILINXCORELIB_P_1983681889 = xsi_get_engine_memory("xilinxcorelib_p_1983681889");
    IEEE_P_1242562249 = xsi_get_engine_memory("ieee_p_1242562249");

    return xsi_run_simulation(argc, argv);

}
