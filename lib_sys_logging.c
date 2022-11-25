/*
 * OGC Engineering
 * Library System Logging
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>

#include "lib_sys_logging.h"
#include "hal.h"

#define HEADER ( 12U ) /* leave room in output for header, adjust if needed */

struct s_lib_sys_logging_status lib_sys_logging_status = {
    LIB_SYS_LOGGING_LEVEL_LOCAL,
    NULL,
    LIB_SYS_LOGGING_LEVEL_REMOTE,
    NULL,
};

const char \
lib_sys_logging_level_strings[ e_LIB_SYS_LOGGING_LEVEL_LIMIT ][ 8U ] = {
    [ e_LIB_SYS_LOGGING_LEVEL_NONE ]     = "NONE",
    [ e_LIB_SYS_LOGGING_LEVEL_FATAL ]    = "FATAL",
    [ e_LIB_SYS_LOGGING_LEVEL_ERROR ]    = "ERROR",
    [ e_LIB_SYS_LOGGING_LEVEL_WARNING ]  = "WARNING",
    [ e_LIB_SYS_LOGGING_LEVEL_NOTICE ]   = "NOTICE",
    [ e_LIB_SYS_LOGGING_LEVEL_INFO ]     = "INFO",
    [ e_LIB_SYS_LOGGING_LEVEL_DEBUG ]    = "DEBUG",
    [ e_LIB_SYS_LOGGING_LEVEL_TRACE ]    = "TRACE",
};

int16_t lib_sys_logging_init(
    void( *local_target )( uint8_t*, size_t ),
    void( *remote_target )( uint8_t*, size_t )
    )
{
    bool did_we_update_anything = false;
    if ( NULL != local_target )
    {
        lib_sys_logging_status.local_output_target = local_target;
        did_we_update_anything = true;
    }
    if ( NULL != remote_target )
    {
        lib_sys_logging_status.remote_output_target = remote_target;
        did_we_update_anything = true;
    }
    if ( true == did_we_update_anything )
    {
        return ( 0 ); /* TODO: replace with success */
    }
    else
    {
        return ( 1 ); /* TODO: replace with invalid args error */
    }
}

int16_t lib_sys_logging_get_threshold_local(
    enum e_lib_sys_logging_level* level
    )
{
    if ( NULL != level )
    {
        *level = lib_sys_logging_status.level_threshold_local;
        return ( 0 ); /* TODO: replace with success */
    }
    else
    {
        return ( 1 ); /* TODO: replace with invalid arg error */
    }
}

int16_t lib_sys_logging_set_threshold_local(
    enum e_lib_sys_logging_level level
    )
{
    if ( ( level >= e_LIB_SYS_LOGGING_LEVEL_NONE )
         && ( level < e_LIB_SYS_LOGGING_LEVEL_LIMIT )
       )
    {
        lib_sys_logging_status.level_threshold_local = level;
        return ( 0 ); /* TODO: replace with success */
    }
    return ( 1 ); /* TODO: replace with invalid arg error */
}

int16_t lib_sys_logging_get_threshold_remote(
    enum e_lib_sys_logging_level* level
    )
{
    if ( NULL != level )
    {
        *level = lib_sys_logging_status.level_threshold_remote;
        return ( 0 ); /* TODO: replace with success */
    }
    return ( 1 ); /* TODO: replace with invalid arg error */
}

int16_t lib_sys_logging_set_threshold_remote(
    enum e_lib_sys_logging_level level
    )
{
    if ( ( level >= e_LIB_SYS_LOGGING_LEVEL_NONE )
         && ( level < e_LIB_SYS_LOGGING_LEVEL_LIMIT )
       )
    {
        lib_sys_logging_status.level_threshold_remote = level;
        return ( 0 ); /* TODO: replace with success */
    }
    return ( 1 ); /* TODO: replace with invalid arg error */
}

int16_t lib_sys_logging_set_log(
    enum e_lib_sys_logging_level level,
    char* buffer,
    ...
    )
{
    /* validate incoming message and some valid target/need for output */
    if ( ( NULL != buffer )
         && ( level > e_LIB_SYS_LOGGING_LEVEL_NONE )
         && ( level < e_LIB_SYS_LOGGING_LEVEL_LIMIT )
         && ( ( lib_sys_logging_status.level_threshold_local <= level )
              || ( lib_sys_logging_status.level_threshold_remote <= level )
            )
         && ( ( NULL != lib_sys_logging_status.local_output_target )
              || ( NULL != lib_sys_logging_status.remote_output_target )
            )
       )
    {
        uint8_t temp1_buffer[ LIB_SYS_LOGGING_MESSAGE_SIZE - HEADER ] = { '\0' };
        uint8_t temp2_buffer[ LIB_SYS_LOGGING_MESSAGE_SIZE ] = { '\0' };

        va_list args;
        va_start( args, buffer );
        vsnprintf( ( char *)temp1_buffer, LIB_SYS_LOGGING_MESSAGE_SIZE, buffer, args );
        va_end( args );

        snprintf( ( char *)temp2_buffer,
                LIB_SYS_LOGGING_MESSAGE_SIZE,
                "%llu|%s|%s\r\n",
                hal_get_sys_tick(),
                lib_sys_logging_level_strings[ level ],
                temp1_buffer
                );

        temp2_buffer[ LIB_SYS_LOGGING_MESSAGE_SIZE - 1U ] = '\0';
        temp2_buffer[ LIB_SYS_LOGGING_MESSAGE_SIZE - 2U ] = '.';
        temp2_buffer[ LIB_SYS_LOGGING_MESSAGE_SIZE - 3U ] = '.';
        temp2_buffer[ LIB_SYS_LOGGING_MESSAGE_SIZE - 4U ] = '.';

        if ( ( lib_sys_logging_status.level_threshold_local >= level )
             && ( NULL != lib_sys_logging_status.local_output_target )
           )
        {
            lib_sys_logging_status.local_output_target( temp2_buffer,
                                                        strlen( temp2_buffer )
                                                      );
        }
        if ( ( lib_sys_logging_status.level_threshold_remote >= level )
             && ( NULL != lib_sys_logging_status.remote_output_target )
           )
        {
            lib_sys_logging_status.remote_output_target( temp2_buffer,
                                                         strlen( temp2_buffer )
                                                       );
        }
        return ( 0 ); /*  TODO: replace with success */
    }
    return ( 1 ); /* TODO: replace with invalid arg error */
}

#ifdef DEPLOYMENT_OPTION_RUN_UNIT_TESTS

#include <stdbool.h>

void our_local_target( uint8_t* buffer, size_t length )
{
    /* TODO: how do we test that we made it here with proper data */
}

void our_remote_target( uint8_t* buffer, size_t length )
{
    /* TODO: how do we test that we made it here with proper data */
}

/* UNIT TEST function needed to complete unit testing of this library */
bool run_unit_tests__lib_sys_logging( void )
{
    int16_t ret;
    char buffer[] = "Hello World!";
    enum e_lib_sys_logging_level test_level;

    /* TEST LOCAL LEVEL THRESHOLD - test read and change of level threshold */
    ret = lib_sys_logging_get_threshold_local( &test_level );
    /* test read of stored local level threshold */
    if ( ( 1U == ret )
         || ( lib_sys_logging_status.level_threshold_local != test_level )
       )
    {
        return ( false );
    }

    ret = lib_sys_logging_set_threshold_local( e_LIB_SYS_LOGGING_LEVEL_DEBUG );
    /* test change of stored local level threshold */
    if ( ( 1U == ret )
         || ( lib_sys_logging_status.level_threshold_local != e_LIB_SYS_LOGGING_LEVEL_DEBUG )
       )
    {
        return ( false );
    }

    /* TEST REMOTE LEVEL THRESHOLD - test read and change of level threshold */
    ret = lib_sys_logging_get_threshold_remote( &test_level );
    /* test read of stored remote level threshold */
    if ( ( 1U == ret )
         || ( lib_sys_logging_status.level_threshold_remote != test_level )
       )
    {
        return ( false );
    }

    ret = lib_sys_logging_set_threshold_remote( e_LIB_SYS_LOGGING_LEVEL_DEBUG );
    /* test change of stored remote level threshold */
    if ( ( 1U == ret )
         || ( lib_sys_logging_status.level_threshold_remote != e_LIB_SYS_LOGGING_LEVEL_DEBUG )
       )
    {
        return ( false );
    }

    /* TEST LOGGING - with valid status, test sending a log */
    ret = lib_sys_logging_set_log( e_LIB_SYS_LOGGING_LEVEL_NOTICE, buffer );
    /* this is valid but we have not set a target yet so we should fail */
    if ( 1U != ret )
    {
        return ( false );
    }

    /* set a target for local and remote output testing */
    ret = lib_sys_loggint_init( our_local_target, our_remote_target );
    if ( ( 1U == ret )
         || ( lib_sys_logging_status.local_output_target != our_local_target )
         || ( lib_sys_logging_status.remote_output_target != our_remote_target )
       )
    {
        return ( false );
    }

    ret = lib_sys_logging_set_log( e_LIB_SYS_LOGGING_LEVEL_NOTICE, buffer );
    /* this is valid and we have now set a target so we should success */
    if ( 1U == ret )
    {
        return ( false );
    }


    ret = lib_sys_logging_set_log( e_LIB_SYS_LOGGING_LEVEL_trace, buffer );
    /* this should return failure since TRACE is above current thresholds */
    if ( 1U != ret )
    {
        return ( false );
    }

    ret = lib_sys_logging_set_log( e_LIB_SYS_LOGGING_LEVEL_NONE, buffer );
    /* this should return failure since NONE is not valid level for new logs */
    if ( 1U != ret )
    {
        return ( false );
    }

    ret = lib_sys_logging_set_log( e_LIB_SYS_LOGGING_LEVEL_LIMIT, buffer );
    /* this should return failure since LIMIT is also not valid for new logs */
    if ( 1U != ret )
    {
        return ( false );
    }

    ret = lib_sys_logging_set_log( e_LIB_SYS_LOGGING_LEVEL_NOTICE, NULL );
    /* this should return failure since buffer is NULL and is nothing to log */
    if ( 1U != ret )
    {
        return ( false );
    }

    return ( true );
}
#endif /* DEPLOYMENT_OPTION_RUN_UNIT_TESTS */
