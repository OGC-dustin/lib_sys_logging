/*
 * OGC Engineering
 * Library System Logging
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "lib_sys_logging.h"
#include "hal.h"

const char
lib_sys_logging_status_strings[ enum_LIB_SYS_LOGGING_STATUS_LIMIT ][ 8U ] = {
    [ enum_LIB_SYS_LOGGING_STATUS_EMPTY ]   = "EMPTY",
    [ enum_LIB_SYS_LOGGING_STATUS_PENDING ] = "PENDING",
    [ enum_LIB_SYS_LOGGING_STATUS_FULL ]    = "FULL",
};

const char
lib_sys_logging_level_strings[ enum_LIB_SYS_LOGGING_LEVEL_LIMIT ][ 8U ] = {
    [ enum_LIB_SYS_LOGGING_LEVEL_NONE ]     = "NONE",
    [ enum_LIB_SYS_LOGGING_LEVEL_FATAL ]    = "FATAL",
    [ enum_LIB_SYS_LOGGING_LEVEL_ERROR ]    = "ERROR",
    [ enum_LIB_SYS_LOGGING_LEVEL_WARNING ]  = "WARNING",
    [ enum_LIB_SYS_LOGGING_LEVEL_NOTICE ]   = "NOTICE",
    [ enum_LIB_SYS_LOGGING_LEVEL_INFO ]     = "INFO",
    [ enum_LIB_SYS_LOGGING_LEVEL_DEBUG ]    = "DEBUG",
    [ enum_LIB_SYS_LOGGING_LEVEL_TRACE ]    = "TRACE",
};

struct s_lib_sys_logging_status
lib_sys_logging_status = {
    enum_LIB_SYS_LOGGING_STATUS_EMPTY,
    LIB_SYS_LOGGING_LEVEL_LOCAL
};

enum e_lib_sys_logging_status
lib_sys_logging_get_threshold_local(
    enum e_lib_sys_logging_level*       level
    )
{
    if ( NULL != level )
    {
        *level = lib_sys_logging_status.level_threshold_local;
        return ( lib_sys_logging_status.status );
    }
    else
    {
        return ( enum_LIB_SYS_LOGGING_STATUS_ERROR );
    }
}

enum e_lib_sys_logging_status
lib_sys_logging_set_threshold_local(
    enum e_lib_sys_logging_level        level
    )
{
    if ( ( level > enum_LIB_SYS_LOGGING_LEVEL_NONE )
         && ( level < enum_LIB_SYS_LOGGING_LEVEL_LIMIT )
       )
    {
        lib_sys_logging_status.level_threshold_local = level;
        return ( lib_sys_logging_status.status );
    }
    else
    {
        return ( enum_LIB_SYS_LOGGING_STATUS_ERROR );
    }
}

enum e_lib_sys_logging_status
lib_sys_logging_set_log(
    enum e_lib_sys_logging_level        level,
    char*                               buffer,
    ...
    )
{
    if ( ( NULL != buffer )
         && ( level > enum_LIB_SYS_LOGGING_LEVEL_NONE )
         && ( level < enum_LIB_SYS_LOGGING_LEVEL_LIMIT )
       )
    {
        char temp1_buffer[ LIB_SYS_LOGGING_MESSAGE_SIZE ] = { '\0' };
        char temp2_buffer[ LIB_SYS_LOGGING_MESSAGE_SIZE ] = { '\0' };

        va_list args;
        va_start( args, buffer );
        vsnprintf( temp1_buffer, LIB_SYS_LOGGING_MESSAGE_SIZE, buffer, args );
        va_end( args );

        snprintf( temp2_buffer,
                  LIB_SYS_LOGGING_MESSAGE_SIZE,
                  "%llu|%s|%s\r\n",
                  hal_get_sys_tick(),
                  lib_sys_logging_level_strings[ level ],
                  temp1_buffer
                  );

        hal_cli_output_write( temp2_buffer, strlen( temp2_buffer ) );

        return ( lib_sys_logging_status.status );
    }
    else
    {
        return ( enum_LIB_SYS_LOGGING_STATUS_ERROR );
    }
}

#ifdef DEPLOYMENT_OPTION_RUN_UNIT_TESTS

#include <stdbool.h>

/* UNIT TEST function needed to complete unit testing of this library */
bool run_unit_tests__lib_sys_logging( void )
{
    enum e_lib_sys_logging_status return_value;
    char buffer[] = "Hello World!";
    enum e_lib_sys_logging_level test_level;

    /* TEST LEVEL THRESHOLD - test read and change of level threshold */
    return_value = lib_sys_logging_get_threshold_local(
            &test_level
            ); /* test read of stored local level threshold */
    if ( ( lib_sys_logging_status.status != return_value )
         || ( lib_sys_logging_status.level_threshold_local != test_level )
       )
    {
        return ( false );
    }

    return_value = lib_sys_logging_set_threshold_local(
            enum_LIB_SYS_LOGGING_LEVEL_TRACE
            ); /* test change of stored local level threshold */
    if ( ( lib_sys_logging_status.status != return_value )
         || ( lib_sys_logging_status.level_threshold_local != enum_LIB_SYS_LOGGING_LEVEL_TRACE )
       )
    {
        return ( false );
    }

    /* TEST LOGGING - with valid status, test sending a log */
    return_value = lib_sys_logging_set_log(
            enum_LIB_SYS_LOGGING_LEVEL_NONE,
            buffer
            );
    if ( enum_LIB_SYS_LOGGING_STATUS_ERROR != return_value )
    {
        return ( false );
    }

    return_value = lib_sys_logging_set_log(
            enum_LIB_SYS_LOGGING_LEVEL_LIMIT,
            buffer
            );
    if ( enum_LIB_SYS_LOGGING_STATUS_ERROR != return_value )
    {
        return ( false );
    }

    return_value = lib_sys_logging_set_log(
            enum_LIB_SYS_LOGGING_LEVEL_NOTICE,
            NULL
            );
    if ( enum_LIB_SYS_LOGGING_STATUS_ERROR != return_value )
    {
        return ( false );
    }

    return_value = lib_sys_logging_set_log(
            enum_LIB_SYS_LOGGING_LEVEL_WARNING,
            buffer
            );
    if ( lib_sys_logging_status.status != return_value )
    {
        return ( false );
    }

    return ( true );
}
#endif /* DEPLOYMENT_OPTION_RUN_UNIT_TESTS */
