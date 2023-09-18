#include "x_frame.h"
#include "chip_cfg.h"

/*************************************************************
	BASE
*************************************************************/

#if X_FRAME_HAL_VER == X_HAL_XH_VER
    X_FRAME_WEAK void * const gx_xh_sys_id = NULL;
	X_FRAME_WEAK void * const gx_xh_iwdg_id = NULL;
#endif

#ifdef HOST_ADDR
uint8_t x_frame_host_addr = HOST_ADDR;

#else
#error "please add macro HOST_ADDR"
#endif

#ifdef PC_ADDR
uint8_t x_frame_pc_addr = PC_ADDR;
REG_CFG_ITEM(PC_ADDR_PARAM,x_frame_pc_addr, eu_uint8,  PARM_RW, PC_ADDR, 0, 255 );
#else
#error "please add macro PC_ADDR"
#endif

uint32_t g_product_type = PRODUCT_TYPE;
uint32_t g_product_detail = PRODUCT_DETAIL;


/*************************************************************
	RECORDER
*************************************************************/

#ifdef DEF_REC_DAT_FLG
const uint8_t x_frame_rec_dat_enable = DEF_REC_DAT_FLG;
#else
#error "RECORDER: please add macro DEF_REC_DAT_FLG, note: DEF_REC_DAT_FLG set 1 to open REC PUSH"
#endif

#define GEN_REC_CFG_ENTITY
    #include "rec_db.h"
#undef GEN_REC_CFG_ENTITY

#define GEN_REC_DAT_ENTITY
    #include "rec_db.h"
#undef GEN_REC_DAT_ENTITY

#define GEN_REC_INIT_ENTITY
    #include "rec_db.h"
#undef GEN_REC_INIT_ENTITY

#define GEN_REC_CAP_ACTION_ENTITY
    #include "rec_db.h"
#undef GEN_REC_CAP_ACTION_ENTITY

#define GEN_REC_CAP_PUSH_ENTITY
    #include "rec_db.h"
#undef GEN_REC_CAP_PUSH_ENTITY

#define GEN_REC_CAP_TRANS_CFG_ENTITY
    #include "rec_db.h"
#undef GEN_REC_CAP_TRANS_CFG_ENTITY

#define GEN_REC_CAP_TRANS_DAT_ENTITY
    #include "rec_db.h"
#undef GEN_REC_CAP_TRANS_DAT_ENTITY


/*************************************************************
	COMM CMD 
*************************************************************/

uint32_t g_sys_boot_cfg_part_id = SYS_BOOT_CFG_PART;
uint32_t g_sys_hw_ver_part_id = SYS_HW_VER_PART;
uint32_t g_sys_product_part_id = SYS_PRODUCT_PART;
uint32_t g_sys_rec_part_id = USER_REC_PART;


#if (defined HAL_DRV_MAIN_VER) && (defined HAL_DRV_SUB_VER) && (defined HAL_DRV_DBG_VER) && (defined HAL_DRV_TEST_VER)

uint32_t g_hal_drv_main_ver = HAL_DRV_MAIN_VER;
uint32_t g_hal_drv_sub_ver = HAL_DRV_SUB_VER;
uint32_t g_hal_drv_dbg_ver = HAL_DRV_DBG_VER;
uint32_t g_hal_drv_test_ver = HAL_DRV_TEST_VER;

#else

uint32_t g_hal_drv_main_ver = 0;
uint32_t g_hal_drv_sub_ver = 0;
uint32_t g_hal_drv_dbg_ver = 0;
uint32_t g_hal_drv_test_ver = 0;

#endif

extern int main(void);
#if PROGRAM_TYPE == PROGRAM_APL
const app_tail_info_t product_info_val INFO_SECTION = {
    {0, 0, 0},
    FW_TYPE_APL,
    (void *)&main,
    CHIP_NAME,
    JLINK_CHIP_NAME,
    PRODUCT_DETAIL,
    PRODUCT_TYPE,
    X_MAKE_VERSION(APP_MAIN_VER, APP_SUB_VER, APP_DBG_VER, APP_TEST_VER),
    0x0,
    COMP_TIME,
    COMP_DATE,
    HOST_ADDR,
    PRODUCT_NAME,
    0x01,
};

uint32_t g_app_main_ver = APP_MAIN_VER;
uint32_t g_app_sub_ver = APP_SUB_VER;
uint32_t g_app_dbg_ver = APP_DBG_VER;
uint32_t g_app_test_ver = APP_TEST_VER;


#elif PROGRAM_TYPE == PROGRAM_LOADER

const loader_tail_info_t loader_product_info_val INFO_SECTION =
    {
        PRODUCT_DETAIL,
        PRODUCT_TYPE,
        0x0,
        X_MAKE_VERSION(LOADER_MAIN_VER, LOADER_SUB_VER, LOADER_DBG_VER, LOADER_TEST_VER),
        COMP_TIME,
        COMP_DATE,
        HOST_ADDR,
        PRODUCT_NAME,

        CHIP_NAME,
        JLINK_CHIP_NAME,
        SYS_LOADER_PART_SIZE,
        SYS_APP_PART_SIZE,
        SYS_FLASH_SIZE,
        LOADER_INFO,
        0x01,
};

uint8_t g_boot_main_ver = LOADER_MAIN_VER;
uint8_t g_boot_sub_ver = LOADER_SUB_VER;
uint8_t g_boot_dbg_ver = LOADER_DBG_VER;
uint8_t g_boot_test_ver = LOADER_TEST_VER;
#else
#error "please set macro PROGRAM_TYPE == PROGRAM_APL or PROGRAM_LOADER"
#endif


/*************************************************************
	DBG
*************************************************************/

uint8_t g_enable_dbg_i = DEF_DBG_I_PRINT;
uint8_t g_enable_dbg_w = DEF_DBG_W_PRINT;
uint8_t g_enable_dbg_e = DEF_DBG_E_PRINT;

REG_CFG_ITEM(ENABLE_DBG_I,g_enable_dbg_i, eu_uint8,  PARM_RW, DEF_DBG_I_PRINT, 0.0f, 1.0f );
REG_CFG_ITEM(ENABLE_DBG_W,g_enable_dbg_w, eu_uint8,  PARM_RW, DEF_DBG_W_PRINT, 0.0f, 1.0f );
REG_CFG_ITEM(ENABLE_DBG_E,g_enable_dbg_e, eu_uint8,  PARM_RW, DEF_DBG_E_PRINT, 0.0f, 1.0f );

/*************************************************************
    BBOX
*************************************************************/        
#define GEN_BBOX_STRUCT_ENTITY
    #include "xf_bbox_db.h"
#undef GEN_BBOX_STRUCT_ENTITY
        
#define GEN_BBOX_GET_ENTITY
    #include "xf_bbox_db.h"
#undef GEN_BBOX_GET_ENTITY
        
#define GEN_BBOX_UPLOAD_ENTITY
    #include "xf_bbox_db.h"
#undef GEN_BBOX_UPLOAD_ENTITY
    
#define GEN_BBOX_SET_ENTITY
    #include "xf_bbox_db.h"
#undef GEN_BBOX_STRUCT_ENTITY
        
#define GEN_BBOX_INIT_ENTITY
    #include "xf_bbox_db.h"
#undef GEN_BBOX_GET_ENTITY
        
#define GEN_BBOX_REC_CFG_ENTITY
    #include "xf_bbox_db.h"
#undef GEN_BBOX_REC_CFG_ENTITY
    
#define GEN_BBOX_REC_DAT_ENTITY
    #include "xf_bbox_db.h"
#undef GEN_BBOX_REC_DAT_ENTITY
    
#define GEN_BBOX_REC_INIT_ENTITY
    #include "xf_bbox_db.h"
#undef GEN_BBOX_REC_INIT_ENTITY
    
/*************************************************************
    DRIVER
*************************************************************/

REG_INIT_MOD_RESOURCE(xh_core_init);

/*************************************************************
	END 
*************************************************************/



