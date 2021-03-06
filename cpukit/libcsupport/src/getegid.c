/**
 *  @file
 *
 *  @brief Get Real User, Effective User, Ral Group, and Effective Group IDs
 *  @ingroup libcsupport
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

#include <rtems/seterr.h>
#include <rtems/userenv.h>

/**
 *  4.2.1 Get Real User, Effective User, Ral Group, and Effective Group IDs,
 *        P1003.1b-1993, p. 84
 */
gid_t getegid( void )
{
  return _POSIX_types_Egid;
}
