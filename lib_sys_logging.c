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
    if ( ( NULL != status )
         && ( NULL != level )
       )
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

#ifdef DEPLOYMENT_OPTION_RUN_UNIT_TESTS

#include <stdbool.h>

/* TEST FUNCTION needed to test target of logging function */
void unit_test_target_function( char* buffer, size_t length )
{
    volatile static bool test_value;
    if ( ( NULL != buffer )
         && ( length > 0 )
       )
    {
        test_value = true;
    }
    else
    {
        test_value = false;
    }
}

/* UNIT TEST function needed to complete unit testing of this library */
bool run_unit_tests__lib_sys_logging( void )
{
    struct s_lib_sys_logging_status unit_test_status = { enum_LIB_SYS_LOGGING_STATUS_LIMIT, enum_LIB_SYS_LOGGING_LEVEL_LIMIT, NULL };
    enum e_lib_sys_logging_status return_value;
    char buffer[] = "Hello World!";
    enum e_lib_sys_logging_level test_level;

    /* TEST INIT - test each argument, these should all return ERROR due to validation test failures */
    return_value = lib_sys_logging_init(
            NULL,
            enum_LIB_SYS_LOGGING_LEVEL_NOTICE,
            unit_test_target_function
            ); /* NULL status struct pointer should return ERROR */
    if ( enum_LIB_SYS_LOGGING_STATUS_ERROR != return_value )
    {
        return ( false );
    }

    return_value = lib_sys_logging_init(
            &unit_test_status,
            enum_LIB_SYS_LOGGING_LEVEL_NOTICE,
            NULL
            ); /* NULL target function should return ERROR */
    if ( enum_LIB_SYS_LOGGING_STATUS_ERROR != return_value )
    {
        return ( false );
    }

    return_value = lib_sys_logging_init(
            &unit_test_status,
            enum_LIB_SYS_LOGGING_LEVEL_NONE,
            unit_test_target_function
            ); /* level too low should return ERROR */
    if ( enum_LIB_SYS_LOGGING_STATUS_ERROR != return_value )
    {
        return ( false );
    }

    return_value = lib_sys_logging_init(
            &unit_test_status,
            enum_LIB_SYS_LOGGING_LEVEL_LIMIT,
            unit_test_target_function
            ); /* level too high should return ERROR */
    if ( enum_LIB_SYS_LOGGING_STATUS_ERROR != return_value )
    {
        return ( false );
    }

    /* use this as an example of expected initialization - should return EMPTY */
    return_value = lib_sys_logging_init(
            &unit_test_status,
            enum_LIB_SYS_LOGGING_LEVEL_NOTICE,
            unit_test_target_function
            );
    if ( enum_LIB_SYS_LOGGING_STATUS_EMPTY != return_value )
    {
        return ( false );
    }

    /* TEST LEVEL THRESHOLD - With a valid status, test read and change of level reporting threshold */

    return_value = lib_sys_logging_get_threshold_local(
            NULL,
            &test_level
            ); /* NULL status struct should return ERROR */
    if ( enum_LIB_SYS_LOGGING_STATUS_ERROR != return_value )
    {
        return ( false );
    }

    return_value = lib_sys_logging_get_threshold_local(
            &unit_test_status,
            NULL
            ); /* NULL argument for level should return ERROR */
    if ( enum_LIB_SYS_LOGGING_STATUS_ERROR != return_value )
    {
        return ( false );
    }

    return_value = lib_sys_logging_get_threshold_local(
            &unit_test_status,
            &test_level
            ); /* test read of stored local level threshold */
    if ( ( unit_test_status.status != return_value )
         || ( unit_test_status.level_threshold_local != test_level )
       )
    {
        return ( false );
    }

    return_value = lib_sys_logging_set_threshold_local(
            &unit_test_status,
            enum_LIB_SYS_LOGGING_LEVEL_TRACE
            ); /* test change of stored local level threshold */
    if ( ( unit_test_status.status != return_value )
         || ( unit_test_status.level_threshold_local != enum_LIB_SYS_LOGGING_LEVEL_TRACE )
       )
    {
        return ( false );
    }

    /* TEST LOGGING - with valid status, test sending a log */

    return_value = lib_sys_logging_set_log(
            NULL,
            enum_LIB_SYS_LOGGING_LEVEL_NOTICE,
            buffer
            );
    if ( enum_LIB_SYS_LOGGING_STATUS_ERROR != return_value )
    {
        return ( false );
    }

    return_value = lib_sys_logging_set_log(
            &unit_test_status,
            enum_LIB_SYS_LOGGING_LEVEL_NONE,
            buffer
            );
    if ( enum_LIB_SYS_LOGGING_STATUS_ERROR != return_value )
    {
        return ( false );
    }

    return_value = lib_sys_logging_set_log(
            &unit_test_status,
            enum_LIB_SYS_LOGGING_LEVEL_LIMIT,
            buffer
            );
    if ( enum_LIB_SYS_LOGGING_STATUS_ERROR != return_value )
    {
        return ( false );
    }

    return_value = lib_sys_logging_set_log(
            &unit_test_status,
            enum_LIB_SYS_LOGGING_LEVEL_NOTICE,
            NULL
            );
    if ( enum_LIB_SYS_LOGGING_STATUS_ERROR != return_value )
    {
        return ( false );
    }

    return_value = lib_sys_logging_set_log(
            &unit_test_status,
            enum_LIB_SYS_LOGGING_LEVEL_NOTICE,
            buffer
            );
    if ( unit_test_status.status != return_value )
    {
        return ( false );
    }

    return ( true );
}
#endif /* DEPLOYMENT_OPTION_RUN_UNIT_TESTS */
