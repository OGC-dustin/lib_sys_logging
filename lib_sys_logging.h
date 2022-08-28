#ifndef LIB_SYS_LOGGING_H
#define LIB_SYS_LOGGING_H

/* =====================================================================================================================
 *                                                                                                   DEFAULT DEFINITIONS
 * =====================================================================================================================
 */
/* define LIB_SYS_LOGGING_LEVEL_LOCAL in deployment header to change the default threshold level of local logs
 * that are displayed.
 */
#ifndef LIB_SYS_LOGGING_LEVEL_LOCAL
#define LIB_SYS_LOGGING_LEVEL_LOCAL enum_LIB_SYS_LOGGING_LEVEL_DEBUG
#warning "LIB_SYS_LOGGING: DEFAULT value used for LIB_SYS_LOGGING_LEVEL_LOCAL"
#endif /* LIB_SYS_LOGGING_LEVEL_LOCAL */

/* define LIB_SYS_LOGGING_MESSAGE_SIZE in deployment header to change the default string length limit when processing
 * and storing logs.
 */
#ifndef LIB_SYS_LOGGING_MESSAGE_SIZE
#define LIB_SYS_LOGGING_MESSAGE_SIZE 128
#warning "LIB_SYS_LOGGING: DEFAULT value used for LIB_SYS_LOGGING_MESSAGE_SIZE"
#endif /* LIB_SYS_LOGGING_MESSAGE_SIZE */

/* =====================================================================================================================
 *                                                                                           Enumerations and Structures
 * =====================================================================================================================
 */
/*                                                                                                     Library Status */
enum e_lib_sys_logging_status
{
    enum_LIB_SYS_LOGGING_STATUS_ERROR = -1,
    enum_LIB_SYS_LOGGING_STATUS_EMPTY = 0,
    enum_LIB_SYS_LOGGING_STATUS_PENDING,
    enum_LIB_SYS_LOGGING_STATUS_FULL,
    enum_LIB_SYS_LOGGING_STATUS_LIMIT
};

enum e_lib_sys_logging_level
{
    enum_LIB_SYS_LOGGING_LEVEL_NONE = 0, /* set in case value is NULL */
    enum_LIB_SYS_LOGGING_LEVEL_FATAL,
    enum_LIB_SYS_LOGGING_LEVEL_ERROR,
    enum_LIB_SYS_LOGGING_LEVEL_WARNING,
    enum_LIB_SYS_LOGGING_LEVEL_NOTICE,
    enum_LIB_SYS_LOGGING_LEVEL_INFO,
    enum_LIB_SYS_LOGGING_LEVEL_DEBUG,
    enum_LIB_SYS_LOGGING_LEVEL_TRACE,
    enum_LIB_SYS_LOGGING_LEVEL_LIMIT
};

struct s_lib_sys_logging_status
{
    enum e_lib_sys_logging_status status;
    enum e_lib_sys_logging_level level_threshold_local;
    void( *local_output_target )( char*, size_t );
};

enum e_lib_sys_logging_status
lib_sys_logging_get_threshold_local(
    enum e_lib_sys_logging_level*       level
    );

enum e_lib_sys_logging_status
lib_sys_logging_set_threshold_local(
    enum e_lib_sys_logging_level        level
    );

enum e_lib_sys_logging_status
lib_sys_logging_set_log(
    enum e_lib_sys_logging_level        level,
    char*                               buffer,
    ...
    );

#ifdef DEPLOYMENT_OPTION_RUN_UNIT_TESTS
#include <stdbool.h>
bool
run_unit_tests__lib_sys_logging(
    void
    );
#endif /* DEPLOYMENT_OPTION_RUN_UNIT_TESTS */

#endif /* LIB_SYS_LOGGING_H */
