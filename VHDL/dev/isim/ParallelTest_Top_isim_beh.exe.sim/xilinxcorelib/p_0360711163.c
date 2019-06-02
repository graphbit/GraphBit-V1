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

/* This file is designed for use with ISim build 0x7708f090 */

#define XSI_HIDE_SYMBOL_SPEC true
#include "xsi.h"
#include <memory.h>
#ifdef __GNUC__
#include <stdlib.h>
#else
#include <malloc.h>
#define alloca _alloca
#endif
extern char *STD_TEXTIO;
static const char *ng1 = "meminitfile";
extern char *STD_STANDARD;



void xilinxcorelib_p_0360711163_sub_2285329871_3076064829(char *t0, char *t1, char *t2, char *t3, int t4, int t5, char *t6, char *t7, char *t8)
{
    char t9[1072];
    char t10[56];
    char t19[8];
    char t31[8];
    char t37[8];
    char t43[8];
    char t49[8];
    char t54[16];
    char t79[8];
    char t87[8];
    char *t11;
    char *t12;
    unsigned int t13;
    char *t14;
    char *t15;
    char *t16;
    char *t17;
    char *t18;
    char *t20;
    char *t21;
    char *t22;
    char *t23;
    char *t24;
    char *t25;
    char *t26;
    char *t27;
    char *t28;
    char *t29;
    char *t30;
    char *t32;
    char *t33;
    char *t34;
    char *t35;
    char *t36;
    char *t38;
    char *t39;
    char *t40;
    char *t41;
    char *t42;
    char *t44;
    char *t45;
    char *t46;
    char *t47;
    char *t48;
    char *t50;
    char *t51;
    int t52;
    unsigned int t53;
    char *t55;
    char *t56;
    int t57;
    unsigned int t58;
    char *t59;
    char *t60;
    char *t61;
    char *t62;
    char *t63;
    char *t64;
    char *t65;
    char *t66;
    char *t67;
    char *t68;
    int t69;
    char *t70;
    unsigned int t71;
    unsigned int t72;
    unsigned int t73;
    unsigned int t74;
    int t75;
    char *t76;
    char *t77;
    char *t78;
    char *t80;
    char *t81;
    char *t82;
    unsigned int t83;
    char *t84;
    char *t85;
    char *t86;
    char *t88;
    char *t89;
    char *t90;
    unsigned char t91;
    char *t92;
    char *t93;
    char *t94;
    char *t95;
    char *t96;
    char *t97;
    char *t98;
    char *t99;
    int t100;
    char *t101;
    int t102;
    unsigned char t103;
    unsigned char t105;
    unsigned char t106;
    int t107;
    int t108;

LAB0:    t11 = ((STD_TEXTIO) + 3440);
    t12 = (t3 + 12U);
    t13 = *((unsigned int *)t12);
    t13 = (t13 * 1U);
    t14 = (t9 + 4U);
    t15 = xsi_create_file_variable_in_buffer(0, ng1, t11, t2, t13, 1);
    *((char **)t14) = t15;
    t16 = (t9 + 12U);
    t17 = ((STD_STANDARD) + 384);
    t18 = (t16 + 88U);
    *((char **)t18) = t17;
    t20 = (t16 + 56U);
    *((char **)t20) = t19;
    xsi_type_set_default_value(t17, t19, 0);
    t21 = (t16 + 80U);
    *((unsigned int *)t21) = 4U;
    t22 = (t9 + 132U);
    t23 = ((STD_TEXTIO) + 3280);
    t24 = (t22 + 56U);
    *((char **)t24) = t23;
    t25 = (t22 + 40U);
    *((char **)t25) = 0;
    t26 = (t22 + 64U);
    *((int *)t26) = 1;
    t27 = (t22 + 48U);
    *((char **)t27) = 0;
    t28 = (t9 + 204U);
    t29 = ((STD_STANDARD) + 192);
    t30 = (t28 + 88U);
    *((char **)t30) = t29;
    t32 = (t28 + 56U);
    *((char **)t32) = t31;
    xsi_type_set_default_value(t29, t31, 0);
    t33 = (t28 + 80U);
    *((unsigned int *)t33) = 1U;
    t34 = (t9 + 324U);
    t35 = ((STD_STANDARD) + 0);
    t36 = (t34 + 88U);
    *((char **)t36) = t35;
    t38 = (t34 + 56U);
    *((char **)t38) = t37;
    xsi_type_set_default_value(t35, t37, 0);
    t39 = (t34 + 80U);
    *((unsigned int *)t39) = 1U;
    t40 = (t9 + 444U);
    t41 = ((STD_STANDARD) + 384);
    t42 = (t40 + 88U);
    *((char **)t42) = t41;
    t44 = (t40 + 56U);
    *((char **)t44) = t43;
    xsi_type_set_default_value(t41, t43, 0);
    t45 = (t40 + 80U);
    *((unsigned int *)t45) = 4U;
    t46 = (t9 + 564U);
    t47 = ((STD_STANDARD) + 384);
    t48 = (t46 + 88U);
    *((char **)t48) = t47;
    t50 = (t46 + 56U);
    *((char **)t50) = t49;
    xsi_type_set_default_value(t47, t49, 0);
    t51 = (t46 + 80U);
    *((unsigned int *)t51) = 4U;
    t52 = (1 - t5);
    t53 = (t52 * -1);
    t53 = (t53 + 1);
    t53 = (t53 * 1U);
    t55 = (t54 + 0U);
    t56 = (t55 + 0U);
    *((int *)t56) = t5;
    t56 = (t55 + 4U);
    *((int *)t56) = 1;
    t56 = (t55 + 8U);
    *((int *)t56) = -1;
    t57 = (1 - t5);
    t58 = (t57 * -1);
    t58 = (t58 + 1);
    t56 = (t55 + 12U);
    *((unsigned int *)t56) = t58;
    t56 = (t9 + 684U);
    t59 = ((STD_STANDARD) + 1008);
    t60 = (t56 + 88U);
    *((char **)t60) = t59;
    t61 = xsi_get_memory(t53);
    t62 = (t56 + 56U);
    *((char **)t62) = t61;
    xsi_type_set_default_value(t59, t61, t54);
    t63 = (t56 + 64U);
    *((char **)t63) = t54;
    t64 = (t56 + 80U);
    *((unsigned int *)t64) = t53;
    t65 = (t56 + 136U);
    *((char **)t65) = t61;
    t66 = (t56 + 124U);
    *((int *)t66) = 0;
    t67 = (t56 + 128U);
    t68 = (t54 + 12U);
    t58 = *((unsigned int *)t68);
    t69 = (t58 - 1);
    *((int *)t67) = t69;
    t70 = (t56 + 120U);
    t72 = (t53 > 2147483644);
    if (t72 == 1)
        goto LAB2;

LAB3:    t73 = (t53 + 3);
    t74 = (t73 / 16);
    t71 = t74;

LAB4:    *((unsigned int *)t70) = t71;
    t75 = (t4 * t5);
    t76 = (t9 + 828U);
    t77 = ((STD_STANDARD) + 384);
    t78 = (t76 + 88U);
    *((char **)t78) = t77;
    t80 = (t76 + 56U);
    *((char **)t80) = t79;
    *((int *)t79) = t75;
    t81 = (t76 + 80U);
    *((unsigned int *)t81) = 4U;
    t82 = (t7 + 12U);
    t83 = *((unsigned int *)t82);
    t84 = (t9 + 948U);
    t85 = ((STD_STANDARD) + 384);
    t86 = (t84 + 88U);
    *((char **)t86) = t85;
    t88 = (t84 + 56U);
    *((char **)t88) = t87;
    *((unsigned int *)t87) = t83;
    t89 = (t84 + 80U);
    *((unsigned int *)t89) = 4U;
    t90 = (t10 + 4U);
    t91 = (t2 != 0);
    if (t91 == 1)
        goto LAB6;

LAB5:    t92 = (t10 + 12U);
    *((char **)t92) = t3;
    t93 = (t10 + 20U);
    *((int *)t93) = t4;
    t94 = (t10 + 24U);
    *((int *)t94) = t5;
    t95 = (t10 + 28U);
    *((char **)t95) = t6;
    t96 = (t10 + 36U);
    *((char **)t96) = t7;
    t97 = (t10 + 44U);
    *((char **)t97) = t8;
    t98 = (t76 + 56U);
    t99 = *((char **)t98);
    t100 = *((int *)t99);
    t98 = (t84 + 56U);
    t101 = *((char **)t98);
    t102 = *((int *)t101);
    t103 = (t100 == t102);
    if (t103 == 0)
        goto LAB7;

LAB8:    t52 = *((int *)t8);
    t103 = (t52 > 0);
    if (t103 == 1)
        goto LAB12;

LAB13:    t91 = (unsigned char)0;

LAB14:    if (t91 != 0)
        goto LAB9;

LAB11:    t11 = (t46 + 56U);
    t12 = *((char **)t11);
    t11 = (t12 + 0);
    *((int *)t11) = t4;

LAB10:    t11 = (t8 + 0);
    *((int *)t11) = 0;
    t11 = (t40 + 56U);
    t12 = *((char **)t11);
    t11 = (t12 + 0);
    *((int *)t11) = 0;

LAB15:    t11 = (t9 + 4U);
    t12 = *((char **)t11);
    t103 = std_textio_endfile(t12);
    t105 = (!(t103));
    if (t105 == 1)
        goto LAB19;

LAB20:    t91 = (unsigned char)0;

LAB21:    if (t91 != 0)
        goto LAB16;

LAB18:
LAB1:    t11 = (t56 + 80);
    t52 = *((int *)t11);
    t12 = (t56 + 136U);
    t14 = *((char **)t12);
    xsi_put_memory(t52, t14);
    xsi_access_variable_delete(t22);
    t15 = (t9 + 4U);
    t17 = *((char **)t15);
    xsi_delete_file_variable(t17);
    return;
LAB2:    t71 = 2147483647;
    goto LAB4;

LAB6:    *((char **)t90) = t2;
    goto LAB5;

LAB7:    t98 = (t0 + 3596);
    xsi_report(t98, 22U, (unsigned char)3);
    goto LAB8;

LAB9:    t69 = *((int *)t8);
    t11 = (t46 + 56U);
    t12 = *((char **)t11);
    t11 = (t12 + 0);
    *((int *)t11) = t69;
    goto LAB10;

LAB12:    t57 = *((int *)t8);
    t105 = (t57 <= t4);
    t91 = t105;
    goto LAB14;

LAB16:    t14 = (t9 + 4U);
    t17 = *((char **)t14);
    std_textio_readline(STD_TEXTIO, t1, t17, t22);
    t11 = (t56 + 56U);
    t12 = *((char **)t11);
    t11 = (t34 + 56U);
    t14 = *((char **)t11);
    t11 = (t14 + 0);
    std_textio_read13(STD_TEXTIO, t1, t22, t12, t54, t11);
    t52 = (t5 - 1);
    t57 = 0;
    t69 = t52;

LAB22:    if (t57 <= t69)
        goto LAB23;

LAB25:    t52 = *((int *)t8);
    t57 = (t52 + 1);
    t11 = (t8 + 0);
    *((int *)t11) = t57;
    t11 = (t40 + 56U);
    t12 = *((char **)t11);
    t52 = *((int *)t12);
    t57 = (t52 + t5);
    t11 = (t40 + 56U);
    t14 = *((char **)t11);
    t11 = (t14 + 0);
    *((int *)t11) = t57;
    goto LAB15;

LAB17:;
LAB19:    t52 = *((int *)t8);
    t14 = (t46 + 56U);
    t15 = *((char **)t14);
    t57 = *((int *)t15);
    t106 = (t52 < t57);
    t91 = t106;
    goto LAB21;

LAB23:    t11 = (t56 + 56U);
    t12 = *((char **)t11);
    t75 = (t57 + 1);
    t11 = (t54 + 0U);
    t100 = *((int *)t11);
    t14 = (t54 + 8U);
    t102 = *((int *)t14);
    t107 = (t75 - t100);
    t13 = (t107 * t102);
    t15 = (t54 + 4U);
    t108 = *((int *)t15);
    xsi_vhdl_check_range_of_index(t100, t108, t102, t75);
    t53 = (1U * t13);
    t58 = (0 + t53);
    t17 = (t12 + t58);
    t91 = *((unsigned char *)t17);
    t18 = (t28 + 56U);
    t20 = *((char **)t18);
    t18 = (t20 + 0);
    *((unsigned char *)t18) = t91;
    t11 = (t28 + 56U);
    t12 = *((char **)t11);
    t91 = *((unsigned char *)t12);
    t103 = (t91 == (unsigned char)49);
    if (t103 != 0)
        goto LAB26;

LAB28:    t11 = (t28 + 56U);
    t12 = *((char **)t11);
    t91 = *((unsigned char *)t12);
    t103 = (t91 == (unsigned char)48);
    if (t103 != 0)
        goto LAB29;

LAB30:    t11 = (t40 + 56U);
    t12 = *((char **)t11);
    t52 = *((int *)t12);
    t75 = (t52 + t57);
    t11 = (t7 + 0U);
    t100 = *((int *)t11);
    t14 = (t7 + 8U);
    t102 = *((int *)t14);
    t107 = (t75 - t100);
    t13 = (t107 * t102);
    t15 = (t7 + 4U);
    t108 = *((int *)t15);
    xsi_vhdl_check_range_of_index(t100, t108, t102, t75);
    t53 = (1U * t13);
    t58 = (0 + t53);
    t17 = (t6 + t58);
    *((unsigned char *)t17) = (unsigned char)1;

LAB27:
LAB24:    if (t57 == t69)
        goto LAB25;

LAB31:    t52 = (t57 + 1);
    t57 = t52;
    goto LAB22;

LAB26:    t11 = (t40 + 56U);
    t14 = *((char **)t11);
    t52 = *((int *)t14);
    t75 = (t52 + t57);
    t11 = (t7 + 0U);
    t100 = *((int *)t11);
    t15 = (t7 + 8U);
    t102 = *((int *)t15);
    t107 = (t75 - t100);
    t13 = (t107 * t102);
    t17 = (t7 + 4U);
    t108 = *((int *)t17);
    xsi_vhdl_check_range_of_index(t100, t108, t102, t75);
    t53 = (1U * t13);
    t58 = (0 + t53);
    t18 = (t6 + t58);
    *((unsigned char *)t18) = (unsigned char)3;
    goto LAB27;

LAB29:    t11 = (t40 + 56U);
    t14 = *((char **)t11);
    t52 = *((int *)t14);
    t75 = (t52 + t57);
    t11 = (t7 + 0U);
    t100 = *((int *)t11);
    t15 = (t7 + 8U);
    t102 = *((int *)t15);
    t107 = (t75 - t100);
    t13 = (t107 * t102);
    t17 = (t7 + 4U);
    t108 = *((int *)t17);
    xsi_vhdl_check_range_of_index(t100, t108, t102, t75);
    t53 = (1U * t13);
    t58 = (0 + t53);
    t18 = (t6 + t58);
    *((unsigned char *)t18) = (unsigned char)2;
    goto LAB27;

}

void xilinxcorelib_p_0360711163_sub_2639106180_3076064829(char *t0, char *t1, char *t2, char *t3, int t4, int t5, char *t6, char *t7, int t8)
{
    char t10[48];
    char *t11;
    unsigned char t12;
    char *t13;
    char *t14;
    char *t15;
    char *t16;
    unsigned char t17;
    char *t18;
    char *t19;
    char *t20;

LAB0:    t11 = (t10 + 4U);
    t12 = (t2 != 0);
    if (t12 == 1)
        goto LAB3;

LAB2:    t13 = (t10 + 12U);
    *((char **)t13) = t3;
    t14 = (t10 + 20U);
    *((int *)t14) = t4;
    t15 = (t10 + 24U);
    *((int *)t15) = t5;
    t16 = (t10 + 28U);
    t17 = (t6 != 0);
    if (t17 == 1)
        goto LAB5;

LAB4:    t18 = (t10 + 36U);
    *((char **)t18) = t7;
    t19 = (t10 + 44U);
    *((int *)t19) = t8;
    if ((unsigned char)0 == 0)
        goto LAB6;

LAB7:    if ((unsigned char)0 == 0)
        goto LAB8;

LAB9:    if ((unsigned char)0 == 0)
        goto LAB10;

LAB11:
LAB1:    return;
LAB3:    *((char **)t11) = t2;
    goto LAB2;

LAB5:    *((char **)t16) = t6;
    goto LAB4;

LAB6:    t20 = (t0 + 3618);
    xsi_report(t20, 84U, (unsigned char)0);
    goto LAB7;

LAB8:    t20 = (t0 + 3702);
    xsi_report(t20, 65U, (unsigned char)0);
    goto LAB9;

LAB10:    t20 = (t0 + 3767);
    xsi_report(t20, 26U, (unsigned char)0);
    goto LAB11;

}


extern void xilinxcorelib_p_0360711163_init()
{
	static char *se[] = {(void *)xilinxcorelib_p_0360711163_sub_2285329871_3076064829,(void *)xilinxcorelib_p_0360711163_sub_2639106180_3076064829};
	xsi_register_didat("xilinxcorelib_p_0360711163", "isim/ParallelTest_Top_isim_beh.exe.sim/xilinxcorelib/p_0360711163.didat");
	xsi_register_subprogram_executes(se);
}
