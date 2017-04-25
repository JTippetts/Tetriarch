#ifndef TEST_H
#define TEST_H

void foo();
void bar();

extern double table[10];

#endif

#ifdef TEST_IMPL
    #include "test.inl"
#endif
