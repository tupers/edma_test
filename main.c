/*
 *  ======== main.c ========
 */

#include <xdc/std.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>

#include <ti/sysbios/knl/Task.h>

#include <ti/sdo/fc/rman/rman.h>

//#include <ti/sdo/fc/ires/edma3chan/iresman_edma3Chan.h>
#include "block_alg.h"
#include "srcimg.h"

/*
 *  ======== taskFxn ========
 */
Void taskFxn(UArg a0, UArg a1)
{
    System_printf("enter taskFxn()\n");
    alg_struct myAlg;
    alg_init(1280,720,&myAlg);
    alg_process(srcimg,&myAlg);

//    Task_sleep(10);
//    edmaTest();
    //RMAN_init();




    System_printf("exit taskFxn()\n");

//    System_flush(); /* force SysMin output to console */
}

/*
 *  ======== main ========
 */
Int main()
{ 
    Task_Handle task;
    Error_Block eb;

    System_printf("enter main()\n");

    Error_init(&eb);
    task = Task_create(taskFxn, NULL, &eb);
    if (task == NULL) {
        System_printf("Task_create() failed!\n");
        BIOS_exit(0);
    }

    BIOS_start();    /* does not return */
    return(0);
}
