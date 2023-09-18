/**
 * @brief shell.c.xtemp header file
 * @file shell.h
 * @version 1.0
 * @author hzf
 * @date 2022-7-29 16:15:16
 * @par Copyright:
 * Copyright (c) ECOFLOW 2017-2022. All rights reserved.
 *
 * @history
 * 1.Date        : 2022-7-29 16:15:16
 *   Author      : hzf
 *   Modification: Created file
 */

#ifndef __SHELL_H__
#define __SHELL_H__

/**
 * @brief 注册shell命令，公共
 *
 * @param [in]  name         cmd命令名 
 * @param [in]  maxargs      传输的最大参数个数
 * @param [in]  cmd          cmd回调函数名 
 * @param [in]  usage        在DataView使用"help"命令后，显示所有可用的cmd命令，usage为对应的cmd命令说明
 * @param [in]  detail       在DataView使用"help name（cmd命令名）"后,显示cmd命令名对应的cmd命令说明细节
 * 
 * @details 
 * @par 示例:注册一个shell命令，命令名为"test",传输的最大参数个数为10，cmd回调函数名为shell_test，
 *           在DataView使用help命令后，显示"test：shell_test_usage "以及其它可用的命令说明，使用
 *           "help test"后显示"shell test detail"
 * @code
 *   int32_t shell_test(uint8_t argc, uint8_t *argv[])
 *  {
 *  	len = argc;
 *      DBG_I("test");
 *  }
 *  REG_SHELL_CMD(test, 10, shell_test, "shell_test_usage", "shell test detail \r\n ");
 * @endcode
 */
#define REG_SHELL_CMD(name,maxargs,cmd,usage,detail)  \
			const shell_cmd_t    cmd_##name = {#name,maxargs,cmd,usage,detail};\
			AUTO_REG_ITEM(SHELL_##name,AUTO_REG_TYPE_SHELL,(void*)&cmd_##name);

/**
 * @brief 注册shell命令，私有，平台研发调试用
 *
 * @param [in]  name         cmd命令名 
 * @param [in]  maxargs      传输的最大参数个数
 * @param [in]  cmd          cmd回调函数名 
 * @param [in]  usage        在DataView使用"help"命令后，显示所有可用的cmd命令，usage为对应的cmd命令说明
 * @param [in]  detail       在DataView使用"help name（cmd命令名）"后,显示cmd命令名对应的cmd命令说明细节
 * 
 * @details 
 * @par 示例:注册一个shell命令，命令名为"test",传输的最大参数个数为10，cmd回调函数名为shell_test，
 *           在DataView使用help命令后，显示"test：shell_test_usage "以及其它可用的命令说明，使用
 *           "help test"后显示"shell test detail"
 * @code
 *   int32_t shell_test(uint8_t argc, uint8_t *argv[])
 *  {
 *  	len = argc;
 *      DBG_I("test");
 *  }
 *  REG_SHELL_CMD_X(test, 10, shell_test, "shell_test_usage", "shell test detail \r\n ");
 * @endcode
 */
#define REG_SHELL_CMD_X(name,maxargs,cmd,usage,detail)  \
			const shell_cmd_t    cmd_##name = {#name,maxargs,cmd,usage,detail};\
			AUTO_REG_ITEM(SHELL_X_##name,AUTO_REG_TYPE_SHELL_X,(void*)&cmd_##name);

/**
 * @brief 注册shell链路
 *
 * @param [in]  link_id        链路ID 
 *
 * @details 
 * @par 示例:注册一个链路ID为LINK_UART1的shell链路
 * @code
 *   REG_X_SHELL_LINK(LINK_UART1)
 * @endcode
 */		
#define REG_X_SHELL_LINK(link_id)                                               \
            extern int32_t  action_shell_in(hal_frame_info_t *p_frame_info);    \
            REG_LINK_ACTION(link_id, COMM_CMD_SET, CMD_ID_SHELL_IN, action_shell_in);

/**
 * @brief 数据格式化后打印，每隔16个字节换行
 *
 * @param [in]  pbuf        数据缓存指针 
 * @param [in]  len         打印的数据长度,单位为字节
 *
 * @details 数据格式化为：%02x
 * @par 示例:在DataView中打印显示：21 22 02
 * @code
 *  uint8_t buf[3] = {0x21,0x22,2};
 *  show_buf(buf,3)
 * @endcode
 */
#define show_buf(pbuf,len) real_show_buf(pbuf,len)

typedef int32_t (*pf_cmd)(uint8_t argc, uint8_t *argv[]);
typedef struct __shell_cmd
{
    const char 	*name;
    uint8_t			max_args;
    pf_cmd			cmd_func;
    const char		*usage;		/* Usage message	(short)	*/
    const char		*help;		/* Help  message	(long)	*/

}shell_cmd_t;



#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  // __cplusplus

void real_show_buf(uint8_t *pbuf, uint8_t len);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus


#endif  // __SHELL_H__
