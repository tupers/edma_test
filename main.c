/*
 *  ======== main.c ========
 */

#include <xdc/std.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/Log.h>
#include <ti/uia/events/UIABenchmark.h>

//#include <ti/sdo/fc/rman/rman.h>

//#include <ti/sdo/fc/ires/edma3chan/iresman_edma3Chan.h>
#include "srcimg.h"
#include "blockAlg.h"
int srcval = 0;
extern int checkval;
/*
 *  ======== taskFxn ========
 */
Void taskFxn(UArg a0, UArg a1)
{
    System_printf("enter taskFxn()\n");
    int i;
    Log_write1(UIABenchmark_start, (xdc_IArg) "part1");
    for(i=0;i<1280*720;i++)
    {
    	if(srcimg[i]>0)
    		srcval++;
    }
    Log_write1(UIABenchmark_stop, (xdc_IArg) "part1");

    blockAlgObject* blockObj = (blockAlgObject*)malloc(sizeof(blockAlgObject));
    blockAlg_init(1280,720,blockObj);
    Log_write1(UIABenchmark_start, (xdc_IArg) "part2");
    blockAlg_process(srcimg,blockObj);
    Log_write1(UIABenchmark_stop, (xdc_IArg) "part2");
    blockAlg_deinit(blockObj);
    free(blockObj);
    System_printf("srcval is %d,checkval is %d\n",srcval,checkval);
    System_printf("exit taskFxn()\n");

//    System_flush(); /* force SysMin output to console */
}

/*
 *  ======== main ========
 */
Int main()
{ 
    Task_Handle task;
    Task_Params param;
    Task_Params_init(&param);
    param.priority=6;
    Error_Block eb;

    System_printf("enter main()\n");

    Error_init(&eb);
    task = Task_create(taskFxn, &param, &eb);
    if (task == NULL) {
        System_printf("Task_create() failed!\n");
        BIOS_exit(0);
    }

    BIOS_start();    /* does not return */
    return(0);
}
