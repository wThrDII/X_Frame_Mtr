#ifndef __KFIFO_H__
#define __KFIFO_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*************************************************************
** Function name:       DEF_KFIFO
** Descriptions:        定义KFIFO,使用该接口定义的fifo不需要再调用 kfifo_init
** Input parameters:    name: 参数名称，需符合c语言变量命名规范
**                      len：fifo大小
** Returned value:      None
** Remarks:             None
*************************************************************/
#define DEF_KFIFO(name,len)                         \
        uint8_t kfifo_buf_##name[STRIP_2N(len)];    \
        kfifo_t name =                              \
        {                                           \
            #name,                                  \
            sizeof(kfifo_buf_##name),               \
            kfifo_buf_##name,                       \
            0,                                      \
            0,                                      \
        };

typedef struct
{
    const char  *name;
	uint32_t    size;
    uint8_t     *p_buf;
    uint32_t    index_w;
    uint32_t    index_r;
} kfifo_t;

int32_t kfifo_init(const char* name, kfifo_t *p_kfifo, uint8_t *p_buf, uint32_t len);
uint32_t kfifo_push_in(kfifo_t *p_kfifo, uint8_t *p_buf, uint32_t len);
uint32_t kfifo_pull_out(kfifo_t *p_kfifo, uint8_t *p_buf, uint32_t len);

void kfifo_reset(kfifo_t *pfifo);
uint32_t kfifo_get_data_len(kfifo_t *p_kfifo);
uint32_t kfifo_get_rest_len(kfifo_t *p_kfifo);
uint32_t kfifo_get_w_index(kfifo_t *p_kfifo);
uint32_t kfifo_get_r_index(kfifo_t *p_kfifo);

#endif //----__KFIFO_H__
