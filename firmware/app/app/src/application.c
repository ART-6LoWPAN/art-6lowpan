#define LOG_TAG    "app_task"

#include "app_task.h"
#include <stdlib.h>
#include <stdio.h>
#include <board.h>
#include <elog.h>
#include <elog_flash.h>
#include <easyflash.h>
#include <delay_conf.h>
#include <cpuusage.h>
#include <utils.h>
#include <shell.h>
#include <finsh.h>
#include <cm_backtrace.h>
#include <dfs_elm.h>
#include <dfs_fs.h>
#include <spi_flash.h>
#include <spi_flash_sfud.h>
#include <partition.h>

#define SOFTWARE_VERSION     "0.12.02"

static uint8_t cpu_usage_major, cpu_usage_minor;
static rt_uint32_t total_mem, used_mem, max_used_mem;

#define thread_sys_monitor_prio                30
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t thread_sys_monitor_stack[512];
struct rt_thread thread_sys_monitor;
rt_spi_flash_device_t nor_flash;

static void rtt_user_assert_hook(const char* ex, const char* func, rt_size_t line);
static void elog_user_assert_hook(const char* ex, const char* func, size_t line);
static rt_err_t exception_hook(void *context);

/**
 * 系统监控线程
 * @param parameter parameter
 */
void thread_entry_sys_monitor(void* parameter)
{
    while (1)
    {
        rt_memory_info(&total_mem, &used_mem, &max_used_mem);
//        rt_kprintf("Total_Mem:%ld  Used_Mem:%ld  Max_Used_Mem:%ld\n",total_mem,used_mem,max_used_mem);
        if(get_system_status() == SYSTEM_STATUS_RUN){
            cpu_usage_get(&cpu_usage_major, &cpu_usage_minor);
            SYSTEM_RUN_LED_ON;
            rt_thread_delay(DELAY_SYS_RUN_LED_ON);
            SYSTEM_RUN_LED_OFF;
            rt_thread_delay(DELAY_SYS_RUN_LED_OFF);
        } else if (get_system_status() == SYSTEM_STATUS_INIT){
            SYSTEM_RUN_LED_ON;
            rt_thread_delay(DELAY_SYS_INIT_LED_ON);
            SYSTEM_RUN_LED_OFF;
            rt_thread_delay(DELAY_SYS_INIT_LED_OFF);
        } else if (get_system_status() == SYSTEM_STATUS_FAULT){
            SYSTEM_RUN_LED_ON;
            rt_thread_delay(DELAY_SYS_FAULT_LED_ON);
            SYSTEM_RUN_LED_OFF;
            rt_thread_delay(DELAY_SYS_FAULT_LED_OFF);
        }
        IWDG_Feed(); //喂狗
    }
}

/**
 * 系统初始化线程
 * @param parameter parameter
 */
void sys_init_thread(void* parameter){
    set_system_status(SYSTEM_STATUS_INIT);

#ifdef RT_USING_COMPONENTS_INIT
    /* RT-Thread 组件初始化 */
    rt_components_init();
#endif

#if defined(RT_USING_FINSH) && !defined(RT_USING_POSIX)
    /* 设置 finsh 控制台设备名称 */
    finsh_set_device(RT_CONSOLE_DEVICE_NAME);
#endif

    /* 初始化 nor_flash Flash 设备 */
    if ((nor_flash = rt_sfud_flash_probe("nor_flash", "spi20")) == NULL) {
        /* Flash 初始化失败 */
        set_system_status(SYSTEM_STATUS_FAULT);
        return;
    }

    /* 初始化 Flash 上的各个分区 */
    extern int rt_partition_init(const char* flash_device, const struct rt_partition* parts, rt_size_t num);
    static const struct rt_partition partition[] = RT_PARTITION_DEFAULT_TABLE;
    rt_partition_init("nor_flash", partition, sizeof(partition) / sizeof(struct rt_partition));

    /* 初始化 EasyFlash 模块 */
    easyflash_init();

    /* 初始化日志系统 */
    elog_init();
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL & ~ELOG_FMT_P_INFO);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_ALL & ~ELOG_FMT_P_INFO);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_ALL & ~ELOG_FMT_P_INFO);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_P_INFO));
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_P_INFO));

#ifdef DEBUG
    elog_set_filter_lvl(ELOG_LVL_VERBOSE);
#elif RELEASE
    elog_set_filter_lvl(ELOG_LVL_INFO);
#endif

#ifdef ELOG_COLOR_ENABLE
    elog_set_text_color_enabled(true);
#endif
    /* 初始化EasyLogger的Flash插件 */
    elog_flash_init();
    /* 启动EasyLogger */
    elog_start();

    /* 设置EasyLogger断言钩子 */
    elog_assert_set_hook(elog_user_assert_hook);

    /* 设置硬件异常钩子 */
    rt_hw_exception_install(exception_hook);

    /* 设置RTT断言钩子 */
    rt_assert_set_hook(rtt_user_assert_hook);

    /* CmBacktrace 组件初始化 */
    cm_backtrace_init("ART-6LoWPAN", HARDWARE_VERSION, SOFTWARE_VERSION);

    /* 挂载 nor-flash 的 fatfs 分区作为根目录 */
    if (dfs_mount("fatfs", "/", "elm", 0, 0) == 0) {
        log_i("File System initialized!");
    } else {
        log_e("File System initialization failed!");
    }

    extern void contiki_init(void);
    contiki_init();

    set_system_status(SYSTEM_STATUS_RUN);
}

static void rtt_user_assert_hook(const char* ex, const char* func, rt_size_t line) {
    rt_enter_critical();

#ifdef ELOG_ASYNC_OUTPUT_ENABLE
    elog_async_enabled(false);
#endif

    elog_output_lock_enabled(false);
    elog_flash_lock_enabled(false);
    elog_a("rtt", "(%s) has assert failed at %s:%ld.", ex, func, line);
    cm_backtrace_assert(cmb_get_sp());
    elog_flash_flush();
    while(1);
}

static void elog_user_assert_hook(const char* ex, const char* func, size_t line) {
    rt_enter_critical();

#ifdef ELOG_ASYNC_OUTPUT_ENABLE
    elog_async_enabled(false);
#endif

    elog_output_lock_enabled(false);
    elog_flash_lock_enabled(false);
    elog_a("elog", "(%s) has assert failed at %s:%ld.\n", ex, func, line);
    cm_backtrace_assert(cmb_get_sp());
    elog_flash_flush();
    while(1);
}

static rt_err_t exception_hook(void *context) {
    extern long list_thread(void);
    uint8_t _continue = 1;

    rt_enter_critical();

#ifdef ELOG_ASYNC_OUTPUT_ENABLE
    elog_async_enabled(false);
#endif

    elog_output_lock_enabled(false);
    elog_flash_lock_enabled(false);

#ifdef RT_USING_FINSH
    list_thread();
#endif

    cm_backtrace_fault(*((uint32_t *)(cmb_get_sp() + sizeof(uint32_t) * 8)), cmb_get_sp() + sizeof(uint32_t) * 9);
    elog_flash_flush();
    while (_continue == 1);

    return RT_EOK;
}

/**
 * RT-Thread 的 kprintf 打印信息输出的底层接口
 *
 * @param str 打印信息
 */
void rt_hw_console_output(const char *str) {
    extern void output_log_to_console_or_flash(bool console, bool flash, const char *log, size_t size);
    /* 同时输出打印信息至控制台及 flash 中 */
    output_log_to_console_or_flash(true, true, str, rt_strlen(str));
}

int rt_application_init(void)
{
    rt_thread_t thread = NULL;
    rt_thread_init(&thread_sys_monitor,
                   "sys_monitor",
                   thread_entry_sys_monitor,
                   RT_NULL,
                   thread_sys_monitor_stack,
                   sizeof(thread_sys_monitor_stack),
                   thread_sys_monitor_prio, 5);
    rt_thread_startup(&thread_sys_monitor);

    thread = rt_thread_create("sys init", sys_init_thread, NULL, 1024, 10, 10);
    if (thread != NULL) {
        rt_thread_startup(thread);
    }

    return 0;
}

/**
 * This function will startup RT-Thread RTOS.
 */
void rtthread_startup(void)
{
    /* init board */
    rt_hw_board_init();

    /* show version */
    rt_show_version();

    /* init tick */
    rt_system_tick_init();

    /* init kernel object */
    rt_system_object_init();

    /* init timer system */
    rt_system_timer_init();

#ifdef RT_USING_HEAP
    /* init memory system */
    rt_system_heap_init((void*)STM32_SRAM_BEGIN, (void*)STM32_SRAM_END);
#endif

    /* init scheduler system */
    rt_system_scheduler_init();

    /* initialize timer */
    rt_system_timer_init();

    /* init timer thread */
    rt_system_timer_thread_init();

    /* Add CPU usage to system */
    cpu_usage_init();

    /* init application */
    rt_application_init();

    /* init idle thread */
    rt_thread_idle_init();

    /* start scheduler */
    rt_system_scheduler_start();

    /* never reach here */
    return;
}

int main(void) {
    /* disable interrupt first */
    rt_hw_interrupt_disable();

    /* startup RT-Thread RTOS */
    rtthread_startup();

    return 0;
}
