#ifndef SORT_H
#define SORT_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>


extern void qsort_2_ways(void *a,int left,int right,int size,int(*compare)(void const*,void const*));

extern void qsort_3_ways(void *a,int left,int right,int size,int(*compare)(void const*,void const*));

extern void insertionsort(void *a,int sophantu,int size,int(*compare)(void const *,void const *));

extern void selectionsort(void *a,int sophantu,int size,int(*compare)(void const *,void const *));

extern void mergesort(void *a,void *temp,int left,int right,int size,int(*compare)(void const *,void const *));

extern void shakersort(void *a,int sophantu,int size,int(*compare)(void const *,void const *));

extern void shellsort(void *a,int sophantu,int size,int(*compare)(void const *,void const *));

extern void heapsort(void *a,int sophantu,int size,int(*compare)(void const *,void const *));

extern void exchx(void *a,int i,int j,int size);
extern void merge(void *a,void *temp,int left,int mid,int right,int size,int(*compare)(void const *,void const *));// tron

#endif
