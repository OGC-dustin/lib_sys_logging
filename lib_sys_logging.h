#ifndef LIB_SYS_LOGGING_H
#define LIB_SYS_LOGGING_H

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

enum e_lib_sys_logging_status lib_sys_logging_init( 
    struct s_lib_sys_logging_status*    status,
    enum e_lib_sys_logging_level        level,
    void( *target )( char*, size_t )
    );

enum e_lib_sys_logging_status lib_sys_logging_get_threshold_local(
    struct s_lib_sys_logging_status*    status,
    enum e_lib_sys_logging_level*       level
    );

enum e_lib_sys_logging_status lib_sys_logging_set_threshold_local(
    struct s_lib_sys_logging_status*    status,
    enum e_lib_sys_logging_level        level
    );

enum e_lib_sys_logging_status lib_sys_logging_set_log(
    struct s_lib_sys_logging_status*    status,
    enum e_lib_sys_logging_level        level,
    char*                               buffer,
    ...
    );

#endif /* LIB_SYS_LOGGING_H */
