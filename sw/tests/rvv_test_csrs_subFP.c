// Katy <3

#include "regs/cheshire.h"
#include "dif/clint.h"
#include "dif/uart.h"
#include "params.h"
#include "util.h"
#include "encoding.h"
#include "rvv_test.h"

#include "cheshire_util.h"

//remove when we have assembler support


int main(void) {
  cheshire_start();

    // Vector configuration parameters and variables
    uint64_t avl = RVV_TEST_AVL(64);
    uint64_t vl;
    vcsr_dump_t vcsr_state = {0};

    RVV_TEST_CLEAN_EXCEPTION();

    // Helper variables and arrays
    // None for this test

    //////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////
    // START OF TESTS
    //////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////
    // TEST: MSTATUS.VS read implementation
    //////////////////////////////////////////////////////////////////
    // Enalbe RVV
    enable_rvv();
    RVV_TEST_ASSERT_EXCEPTION ( 0 );

    //////////////////////////////////////////////////////////////////
    // TEST: CSR read implementation
    //////////////////////////////////////////////////////////////////
    vcsr_dump ( vcsr_state );
    RVV_TEST_ASSERT_EXCEPTION ( 0 );

    //////////////////////////////////////////////////////////////////
    // TEST: CSR write implementation
    //////////////////////////////////////////////////////////////////
    vl = reset_v_state ( avl );
    RVV_TEST_ASSERT_EXCEPTION ( 0 );

    //////////////////////////////////////////////////////////////////
    // TEST: CSR write exception implementation
    //////////////////////////////////////////////////////////////////
	  asm volatile ("csrw	    vl, 0");
    RVV_TEST_ASSERT_EXCEPTION ( 1 );
    RVV_TEST_CLEAN_EXCEPTION ();

	  asm volatile ("csrw	    vlenb, 0");
    RVV_TEST_ASSERT_EXCEPTION ( 1 );
    RVV_TEST_CLEAN_EXCEPTION ();

	  asm volatile ("csrw	    vtype, 0");
    RVV_TEST_ASSERT_EXCEPTION ( 1 );
    RVV_TEST_CLEAN_EXCEPTION ();


    //////////////////////////////////////////////////////////////////
    // TEST: CSR sub-FP8 write and read
    //////////////////////////////////////////////////////////////////
    uint64_t val; 
    val = 1; 
    asm volatile ("csrw %0, %1" :: "i"(CSR_VSF), "r"(val));
    RVV_TEST_ASSERT_EXCEPTION ( 1 );
    asm volatile ("csrr %0, %1" : "=r"(val) : "i"(CSR_VSF));

    RVV_TEST_ASSERT(val ==1); 

    asm volatile ("csrw %0, %1" :: "i"(CSR_VEFT), "r"(val));
    RVV_TEST_ASSERT_EXCEPTION ( 1 );
    asm volatile ("csrr %0, %1" : "=r"(val) : "i"(CSR_VEFT));
    RVV_TEST_ASSERT(val ==1); 

    asm volatile ("csrw %0, %1" :: "i"(CSR_VSFT), "r"(val));
    RVV_TEST_ASSERT_EXCEPTION ( 1 );
    asm volatile ("csrr %0, %1" : "=r"(val) : "i"(CSR_VSFT));
    RVV_TEST_ASSERT(val ==1); 

    //////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////
    // END OF TESTS
    //////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////

#if (PRINTF == 1)
  printf("Test SUCCESS!\r\n");
#endif

  cheshire_end();

  return 0;
}
