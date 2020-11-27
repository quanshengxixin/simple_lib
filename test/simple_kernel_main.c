#include <stdio.h>

#include "simple_kernel.h"
#include "simple_log.h"

int main(int argc, char **argv)
{
    set_loglevel(SIMPLE_LOG_TYPE_KERNEL, SIMPLE_LOG_DBG);
    simple_log_msg(SIMPLE_LOG_TYPE_KERNEL, "data type is int");
    int int_a = 10, int_b = 19, int_c = -20;
    simple_log_msg(SIMPLE_LOG_TYPE_KERNEL, "min(%d, %d) = %d", int_a, int_b, min(int_a, int_b));
    simple_log_msg(SIMPLE_LOG_TYPE_KERNEL, "max(%d, %d) = %d", int_a, int_b, max(int_a, int_b));
    simple_log_msg(SIMPLE_LOG_TYPE_KERNEL, "min3(%d, %d, %d) = %d", int_a, int_b, int_c, min3(int_a, int_b, int_c));
    simple_log_msg(SIMPLE_LOG_TYPE_KERNEL, "max3(%d, %d, %d) = %d", int_a, int_b, int_c, max3(int_a, int_b, int_c));

    simple_log_msg(SIMPLE_LOG_TYPE_KERNEL, "");
    simple_log_msg(SIMPLE_LOG_TYPE_KERNEL, "data type is float");
    float float_a = 10.5, float_b = 19.5, float_c = -20.6;
    simple_log_msg(SIMPLE_LOG_TYPE_KERNEL, "min(%f, %f) = %f", float_a, float_b, min(float_a, float_b));
    simple_log_msg(SIMPLE_LOG_TYPE_KERNEL, "max(%f, %f) = %f", float_a, float_b, max(float_a, float_b));
    simple_log_msg(SIMPLE_LOG_TYPE_KERNEL, "min3(%f, %f, %f) = %f", float_a, float_b, float_c, min3(float_a, float_b, float_c));
    simple_log_msg(SIMPLE_LOG_TYPE_KERNEL, "max3(%f, %f, %f) = %f", float_a, float_b, float_c, max3(float_a, float_b, float_c));

    simple_log_msg(SIMPLE_LOG_TYPE_KERNEL, "");
    simple_log_msg(SIMPLE_LOG_TYPE_KERNEL, "data type is double");
    double double_a = 10.5, double_b = 19.5, double_c = -20.6;
    simple_log_msg(SIMPLE_LOG_TYPE_KERNEL, "min(%lf, %lf) = %lf", double_a, double_b, min(double_a, double_b));
    simple_log_msg(SIMPLE_LOG_TYPE_KERNEL, "max(%lf, %lf) = %lf", double_a, double_b, max(double_a, double_b));
    simple_log_msg(SIMPLE_LOG_TYPE_KERNEL, "min3(%lf, %lf, %lf) = %lf", double_a, double_b, double_c, min3(double_a, double_b, double_c));
    simple_log_msg(SIMPLE_LOG_TYPE_KERNEL, "max3(%lf, %lf, %lf) = %lf", double_a, double_b, double_c, max3(double_a, double_b, double_c));


    return 0;
}