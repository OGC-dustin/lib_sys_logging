/*
 * OGC Engineering
 * Library System Logging
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lib_sys_logging.h"

const char lib_sys_logging_status_strings[ enum_LIB_SYS_LOGGING_STATUS_LIMIT ][ 8U ] =
{
    [ enum_LIB_SYS_LOGGING_STATUS_EMPTY ]   = "EMPTY",
    [ enum_LIB_SYS_LOGGING_STATUS_PENDING ] = "PENDING",
    [ enum_LIB_SYS_LOGGING_STATUS_FULL ]    = "FULL",
};

const char lib_sys_logging_level_strings[ enum_LIB_SYS_LOGGING_LEVEL_LIMIT ][ 8U ] =
{
    [ enum_LIB_SYS_LOGGING_LEVEL_NONE ]     = "NONE",
    [ enum_LIB_SYS_LOGGING_LEVEL_FATAL ]    = "FATAL",
    [ enum_LIB_SYS_LOGGING_LEVEL_ERROR ]    = "ERROR",
    [ enum_LIB_SYS_LOGGING_LEVEL_WARNING ]  = "WARNING",
    [ enum_LIB_SYS_LOGGING_LEVEL_NOTICE ]   = "NOTICE",
    [ enum_LIB_SYS_LOGGING_LEVEL_INFO ]     = "INFO",
    [ enum_LIB_SYS_LOGGING_LEVEL_DEBUG ]    = "DEBUG",
    [ enum_LIB_SYS_LOGGING_LEVEL_TRACE ]    = "TRACE",
};

enum e_lib_sys_logging_status lib_sys_logging_init( 
    struct s_lib_sys_logging_status*    status,
    enum e_lib_sys_logging_level        level,
    void( *target )( char*, size_t )
    )
{
    if ( ( NULL != status )
         && ( NULL != target )
         && ( level > enum_LIB_SYS_LOGGING_LEVEL_NONE )
         && ( level < enum_LIB_SYS_LOGGING_LEVEL_LIMIT )
       )
    {
        status->status = enum_LIB_SYS_LOGGING_STATUS_EMPTY;
        status->level_threshold_local = level;
        status->local_output_target = target;
        return ( status->status );
    }
    else
    {
        return ( enum_LIB_SYS_LOGGING_STATUS_ERROR );
    }
}

enum e_lib_sys_logging_status lib_sys_logging_get_threshold_local(
    struct s_lib_sys_logging_status*    status,
    enum e_lib_sys_logging_level*       level
    )
{
    if ( NULL != status )
    {
        *level = status->level_threshold_local;
        return ( status->status );
    }
    else
    {
        return ( enum_LIB_SYS_LOGGING_STATUS_ERROR );
    }
}

enum e_lib_sys_logging_status lib_sys_logging_set_threshold_local(
    struct s_lib_sys_logging_status*    status,
    enum e_lib_sys_logging_level        level
    )
{
    if ( ( NULL != status )
         && ( level > enum_LIB_SYS_LOGGING_LEVEL_NONE )
         && ( level < enum_LIB_SYS_LOGGING_LEVEL_LIMIT )
       )
    {
        status->level_threshold_local = level;
        return ( status->status );
    }
    else
    {
        return ( enum_LIB_SYS_LOGGING_STATUS_ERROR );
    }
}

enum e_lib_sys_logging_status lib_sys_logging_set_log(
    struct s_lib_sys_logging_status*    status,
    enum e_lib_sys_logging_level        level,
    char*                               buffer,
    ...
    )
{
    if ( ( NULL != status )
         && ( NULL != buffer )
         && ( level > enum_LIB_SYS_LOGGING_LEVEL_NONE )
         && ( level < enum_LIB_SYS_LOGGING_LEVEL_LIMIT )
       )
    {
        char temp1_buffer[ 1024 ] = { '\0' };
        char temp2_buffer[ 1024 ] = { '\0' };

        va_list args;
        va_start( args, buffer );
        vsnprintf( temp1_buffer, 1024, buffer, args );
        va_end( args );

        snprintf( temp2_buffer,
                  1024,
                  "| %5s | %s\r\n",
                  lib_sys_logging_level_strings[ level ],
                  temp1_buffer
                  );

        status->local_output_target( temp2_buffer, strlen( temp2_buffer ) );

        return ( status->status );
    }
    else
    {
        return ( enum_LIB_SYS_LOGGING_STATUS_ERROR );
    }
}
