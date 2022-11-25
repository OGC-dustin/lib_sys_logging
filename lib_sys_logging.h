#ifndef LIB_SYS_LOGGING_H
#define LIB_SYS_LOGGING_H

#include <stdint.h>

/* =============================================================================
 *                                                           DEFAULT DEFINITIONS
 * =============================================================================
 */
/* define LIB_SYS_LOGGING_LEVEL_LOCAL in makefile or pre-include deployment
 * header to change default threshold level of local logs that are displayed
 */
#ifndef LIB_SYS_LOGGING_LEVEL_LOCAL
#define LIB_SYS_LOGGING_LEVEL_LOCAL ( e_LIB_SYS_LOGGING_LEVEL_INFO )
#warning "LIB_SYS_LOGGING: DEFAULT value used for LIB_SYS_LOGGING_LEVEL_LOCAL"
#endif /* LIB_SYS_LOGGING_LEVEL_LOCAL */

/* define LIB_SYS_LOGGING_LEVEL_REMOTE in makefile or pre-include deployment
 * header to change default threshold level of remote logs that are displayed.
 */
#ifndef LIB_SYS_LOGGING_LEVEL_REMOTE
#define LIB_SYS_LOGGING_LEVEL_REMOTE ( e_LIB_SYS_LOGGING_LEVEL_WARNING )
#warning "LIB_SYS_LOGGING: DEFAULT value used for LIB_SYS_LOGGING_LEVEL_REMOTE"
#endif /* LIB_SYS_LOGGING_LEVEL_REMOTE */

/* define LIB_SYS_LOGGING_MESSAGE_SIZE in makefile or pre-include deployment
 * header to change default string length limit when processing and storing logs
 * Strings longer than this limit will be cut short and end with "...".
 */
#ifndef LIB_SYS_LOGGING_MESSAGE_SIZE
#define LIB_SYS_LOGGING_MESSAGE_SIZE ( ( uint8_t )128 )
#warning "LIB_SYS_LOGGING: DEFAULT value used for LIB_SYS_LOGGING_MESSAGE_SIZE"
#endif /* LIB_SYS_LOGGING_MESSAGE_SIZE */

/* =============================================================================
 *                                                   Enumerations and Structures
 * =============================================================================
 */
/*                                                                Level Enums */
enum e_lib_sys_logging_level
{
    e_LIB_SYS_LOGGING_LEVEL_NONE = 0,
    e_LIB_SYS_LOGGING_LEVEL_FATAL,       /* 3 a.m. call, unable to recover */
    e_LIB_SYS_LOGGING_LEVEL_ERROR,       /* hotfix, recoverable, lost data */
    e_LIB_SYS_LOGGING_LEVEL_WARNING,     /* helps guide next developments */
    e_LIB_SYS_LOGGING_LEVEL_NOTICE,      /* added for remote statistics */
    e_LIB_SYS_LOGGING_LEVEL_INFO,        /* major milestones, normal ops */
    e_LIB_SYS_LOGGING_LEVEL_DEBUG,       /* major forks in logic */
    e_LIB_SYS_LOGGING_LEVEL_TRACE,       /* details with supporting args */
    e_LIB_SYS_LOGGING_LEVEL_LIMIT
};

struct s_lib_sys_logging_status
{
    enum e_lib_sys_logging_level level_threshold_local;
    void( *local_output_target )( uint8_t*, size_t );
    enum e_lib_sys_logging_level level_threshold_remote;
    void( *remote_output_target )( uint8_t*, size_t );
};

int16_t lib_sys_logging_init(
    void( *local_target )( uint8_t*, size_t ),
    void( *remote_target )( uint8_t*, size_t )
    );

int16_t lib_sys_logging_get_threshold_local(
    enum e_lib_sys_logging_level* level
    );

int16_t lib_sys_logging_set_threshold_local(
    enum e_lib_sys_logging_level level
    );

int16_t lib_sys_logging_get_threshold_remote(
    enum e_lib_sys_logging_level* level
    );

int16_t lib_sys_logging_set_threshold_remote(
    enum e_lib_sys_logging_level level
    );

int16_t lib_sys_logging_set_log(
    enum e_lib_sys_logging_level level,
    char* buffer,
    ...
    );

#ifdef DEPLOYMENT_OPTION_RUN_UNIT_TESTS
#include <stdbool.h>
bool run_unit_tests__lib_sys_logging( void );
#endif /* DEPLOYMENT_OPTION_RUN_UNIT_TESTS */

#endif /* LIB_SYS_LOGGING_H */
