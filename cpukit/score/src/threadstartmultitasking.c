/**
 *  @file
 *
 *  @brief Start Thread Multitasking
 *  @ingroup ScoreThread
 */

/*
 *  COPYRIGHT (c) 1989-2006.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/threadimpl.h>

void _Thread_Start_multitasking( void )
{
  Per_CPU_Control *cpu_self = _Per_CPU_Get();
  Thread_Control  *heir;

#if defined(RTEMS_SMP)
  _Per_CPU_State_change( cpu_self, PER_CPU_STATE_UP );

  /*
   * Threads begin execution in the _Thread_Handler() function.   This
   * function will set the thread dispatch disable level to zero and calls
   * _Per_CPU_Release().
   */
  _Per_CPU_Acquire( cpu_self );
  cpu_self->thread_dispatch_disable_level = 1;
#endif

  heir = cpu_self->heir;

#if defined(RTEMS_SMP)
  cpu_self->executing->is_executing = false;
  heir->is_executing = true;
#endif

  cpu_self->dispatch_necessary = false;
  cpu_self->executing = heir;

   /*
    * Get the init task(s) running.
    *
    * Note: Thread_Dispatch() is normally used to dispatch threads.  As
    *       part of its work, Thread_Dispatch() restores floating point
    *       state for the heir task.
    *
    *       This code avoids Thread_Dispatch(), and so we have to restore
    *       (actually initialize) the floating point state "by hand".
    *
    *       Ignore the CPU_USE_DEFERRED_FP_SWITCH because we must always
    *       switch in the first thread if it is FP.
    */
#if ( CPU_HARDWARE_FP == TRUE ) || ( CPU_SOFTWARE_FP == TRUE )
   /*
    *  don't need to worry about saving BSP's floating point state
    */

   if ( heir->fp_context != NULL )
     _Context_Restore_fp( &heir->fp_context );
#endif

  _Profiling_Thread_dispatch_disable( cpu_self, 0 );

#if defined(_CPU_Start_multitasking)
  _CPU_Start_multitasking( &heir->Registers );
#else
  _CPU_Context_Restart_self( &heir->Registers );
#endif
}
