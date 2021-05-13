/*******************************************************************************
 * Name         : sum.c
 * Author       : Sanjana Madhu and Lasya Josyula
 * Date         : 05/07/2021
 * Description  : Lab 12
 * Pledge : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include "sum.h"
/**
 * TODO:
 * Takes in an array of integers and its length.
 * Returns the sum of integers in the array.
 */
int sum_array(int *array, const int length) {
    int total = 0;
    for (int i = 0; i < length; i++) {
        total += array[i];
    }
    return total;
}
