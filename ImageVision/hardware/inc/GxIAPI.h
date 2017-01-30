/**
@File      GxIAPI.h
@Brief     the interface for the GxIAPI dll module. 
@Author    Software Department
@Date      2013-12-12
@Version   1.0.1312.9123
@Copyright Copyright(C)2012-2013 Daheng Imavision
*/

#ifndef GX_GALAXY_H
#define GX_GALAXY_H


//////////////////////////////////////////////////////////////////////////
//	���Ͷ��壬�������Ͷ��ڱ�׼C��ͷ�ļ�stdint.h���ж��壬������΢��ı���ƽ̨
//	VS2010֮ǰ�İ汾�ж����������ļ�,�����ڴ���Ҫ�ض���
//////////////////////////////////////////////////////////////////////////

#if defined(_WIN32)
	#ifndef _STDINT_H 
		#ifdef _MSC_VER // Microsoft compiler
			#if _MSC_VER < 1600
				typedef __int8            int8_t;
				typedef __int16           int16_t;
				typedef __int32           int32_t;
				typedef __int64           int64_t;
				typedef unsigned __int8   uint8_t;
				typedef unsigned __int16  uint16_t;
				typedef unsigned __int32  uint32_t;
				typedef unsigned __int64  uint64_t;
			#else
				// In Visual Studio 2010 is stdint.h already included
				#include <stdint.h>
			#endif
		#else
			// Not a Microsoft compiler
			#include <stdint.h>
		#endif
	#endif 
#else
	// Linux
	#include <stdint.h>
#endif


//------------------------------------------------------------------------------
//  ����ϵͳƽ̨����
//------------------------------------------------------------------------------

#include <stddef.h>

#ifdef WIN32
	#ifndef _WIN32
		#define _WIN32
	#endif
#endif

#ifdef _WIN32
	#include <Windows.h>
	#define GX_DLLIMPORT   __declspec(dllimport)
	#define GX_DLLEXPORT   __declspec(dllexport)

	#define GX_STDC __stdcall
	#define GX_CDEC __cdecl

	#if defined(__cplusplus)
		#define GX_EXTC extern "C"
	#else
		#define GX_EXTC
	#endif
#else
	// remove the None #define conflicting with GenApi
	#undef None
	#if __GNUC__>=4
		#define GX_DLLIMPORT   __attribute__((visibility("default")))
		#define GX_DLLEXPORT   __attribute__((visibility("default")))

		#if defined(__i386__)
			#define GX_STDC __attribute__((stdcall))
			#define GX_CDEC __attribute__((cdecl))
		#else
			#define GX_STDC 
			#define GX_CDEC 
		#endif

		#if defined(__cplusplus)
			#define GX_EXTC extern "C"
		#else
			#define GX_EXTC
		#endif
	#else
		#error Unknown compiler
	#endif
#endif

#ifdef GX_GALAXY_DLL
	#define GX_DLLENTRY GX_EXTC GX_DLLEXPORT
#else
	#define GX_DLLENTRY GX_EXTC GX_DLLIMPORT
#endif


//------------------------------------------------------------------------------
//  �����붨��
//------------------------------------------------------------------------------
typedef int32_t GX_STATUS;

typedef enum GX_STATUS_LIST
{
	GX_STATUS_SUCCESS                =  0,           ///< �ɹ�
	GX_STATUS_ERROR                  = -1,           ///< ������������δ��ȷָ�����ڲ�����
	GX_STATUS_NOT_FOUND_TL           = -2,           ///< �Ҳ���TL��
	GX_STATUS_NOT_FOUND_DEVICE       = -3,           ///< �Ҳ����豸
	GX_STATUS_OFFLINE                = -4,           ///< ��ǰ�豸Ϊ����״̬
	GX_STATUS_INVALID_PARAMETER      = -5,           ///< ��Ч����,һ����ָ��ΪNULL�������IP�Ȳ�����ʽ��Ч
	GX_STATUS_INVALID_HANDLE         = -6,           ///< ��Ч���
	GX_STATUS_INVALID_CALL           = -7,           ///< ��Ч�Ľӿڵ���,רָ����ӿ��߼�����
	GX_STATUS_INVALID_ACCESS         = -8,           ///< ���ܵ�ǰ���ɷ��ʻ��豸����ģʽ����
	GX_STATUS_NEED_MORE_BUFFER       = -9,           ///< �û������buffer����:������ʱ�û�����buffersizeС��ʵ����Ҫ
	GX_STATUS_ERROR_TYPE             = -10,          ///< �û�ʹ�õ�FeatureID���ʹ��󣬱������ͽӿ�ʹ���˸����͵Ĺ�����
	GX_STATUS_OUT_OF_RANGE           = -11,          ///< �û�д���ֵԽ��
	GX_STATUS_NOT_IMPLEMENTED        = -12,          ///< ��ǰ��֧�ֵĹ���
	GX_STATUS_NOT_INIT_API           = -13,          ///< û�е��ó�ʼ���ӿ�
	GX_STATUS_TIMEOUT                = -14,          ///< ��ʱ����
}GX_STATUS_LIST;

//------------------------------------------------------------------------------
//  �����붨��
//------------------------------------------------------------------------------
typedef enum GX_FEATURE_TYPE
{
	GX_FEATURE_INT				   =0x10000000,  ///< ������
	GX_FEATURE_FLOAT               =0X20000000,  ///< ������
	GX_FEATURE_ENUM				   =0x30000000,  ///< ö��
	GX_FEATURE_BOOL				   =0x40000000,  ///< ����
	GX_FEATURE_STRING			   =0x50000000,  ///< �ַ���
	GX_FEATURE_BUFFER			   =0x60000000,  ///< buffer
	GX_FEATURE_COMMAND			   =0x70000000,  ///< ����
}GX_FEATURE_TYPE;

typedef enum GX_FEATURE_ID
{
	//---------------DeviceInfomation Section--------------------------
	GX_STRING_DEVICE_VENDOR_NAME      = 0 | GX_FEATURE_STRING,     ///< ��������
	GX_STRING_DEVICE_MODEL_NAME       = 1 | GX_FEATURE_STRING,     ///< �豸��������
	GX_STRING_DEVICE_FIRMWARE_VERSION = 2 | GX_FEATURE_STRING,     ///< �̼��汾
	GX_STRING_DEVICE_VERSION          = 3 | GX_FEATURE_STRING,     ///< �豸�汾
	GX_STRING_DEVICE_SERIAL_NUMBER    = 4 | GX_FEATURE_STRING,     ///< ���к�
	GX_STRING_FACTORY_SETTING_VERSION = 6 | GX_FEATURE_STRING,     ///< ���������汾
	

	//---------------ImageFormat Section--------------------------------
	GX_INT_SENSOR_WIDTH               = 1000 | GX_FEATURE_INT,     ///< ���������
	GX_INT_SENSOR_HEIGHT              = 1001 | GX_FEATURE_INT,     ///< �������߶�
	GX_INT_WIDTH_MAX                  = 1002 | GX_FEATURE_INT,     ///< �����
	GX_INT_HEIGHT_MAX                 = 1003 | GX_FEATURE_INT,     ///< ���߶�
	GX_INT_OFFSET_X                   = 1004 | GX_FEATURE_INT,     ///< X����ƫ��
	GX_INT_OFFSET_Y                   = 1005 | GX_FEATURE_INT,     ///< Y����ƫ��
	GX_INT_WIDTH                      = 1006 | GX_FEATURE_INT,     ///< ͼ����
	GX_INT_HEIGHT                     = 1007 | GX_FEATURE_INT,     ///< ͼ��߶�
	GX_INT_BINNING_HORIZONTAL         = 1008 | GX_FEATURE_INT,     ///< ˮƽ�ϲ�
	GX_INT_BINNING_VERTICAL           = 1009 | GX_FEATURE_INT,     ///< ��ֱ�ϲ�
	GX_INT_DECIMATION_HORIZONTAL      = 1010 | GX_FEATURE_INT,     ///< ˮƽ��ȡ
	GX_INT_DECIMATION_VERTICAL        = 1011 | GX_FEATURE_INT,     ///< ��ֱ��ȡ
	GX_ENUM_PIXEL_SIZE                = 1012 | GX_FEATURE_ENUM,    ///< ���ص��С,�ο�GX_PIXEL_SIZE_ENTRY
	GX_ENUM_PIXEL_COLOR_FILTER        = 1013 | GX_FEATURE_ENUM,    ///< Bayer��ʽ,�ο�GX_PIXEL_COLOR_FILTER_ENTRY
	GX_ENUM_PIXEL_FORMAT              = 1014 | GX_FEATURE_ENUM,    ///< ���ݸ�ʽ,�ο�GX_PIXEL_FORMAT_ENTRY
	GX_BOOL_REVERSE_X                 = 1015 | GX_FEATURE_BOOL,    ///< ˮƽ��תʹ��
	GX_BOOL_REVERSE_Y                 = 1016 | GX_FEATURE_BOOL,    ///< ��ֱ��תʹ��

	//---------------TransportLayer Section-------------------------------
	GX_INT_PAYLOAD_SIZE               = 2000 | GX_FEATURE_INT,     ///< ͼ�����ݴ�С 

	//---------------AcquisitionTrigger Section---------------------------
	GX_ENUM_ACQUISITION_MODE          = 3000 | GX_FEATURE_ENUM,    ///< �ɼ�ģʽ,�ο�GX_ACQUISITION_MODE_ENTRY
	GX_COMMAND_ACQUISITION_START      = 3001 | GX_FEATURE_COMMAND, ///< ��������
	GX_COMMAND_ACQUISITION_STOP       = 3002 | GX_FEATURE_COMMAND, ///< ͣ������
	GX_INT_ACQUISITION_SPEED_LEVEL    = 3003 | GX_FEATURE_INT,     ///< �ɼ��ٶȼ���
	GX_INT_ACQUISITION_FRAME_COUNT    = 3004 | GX_FEATURE_INT,     ///< �ɶ�֡ģʽ�µ�֡��
	GX_ENUM_TRIGGER_MODE              = 3005 | GX_FEATURE_ENUM,    ///< ����ģʽ,�ο�GX_TRIGGER_MODE_ENTRY
	GX_COMMAND_TRIGGER_SOFTWARE       = 3006 | GX_FEATURE_COMMAND, ///< ��������
	GX_ENUM_TRIGGER_ACTIVATION        = 3007 | GX_FEATURE_ENUM,    ///< ��������,�ο�GX_TRIGGER_ACTIVATION_ENTRY
	GX_ENUM_TRIGGER_SWITCH            = 3008 | GX_FEATURE_ENUM,    ///< ��������,�ο�GX_TRIGGER_SWITCH_ENTRY
	GX_FLOAT_EXPOSURE_TIME            = 3009 | GX_FEATURE_FLOAT,   ///< �ع�ʱ��
	GX_ENUM_EXPOSURE_AUTO             = 3010 | GX_FEATURE_ENUM,    ///< �Զ��ع⿪��,�ο�GX_EXPOSURE_AUTO_ENTRY
	GX_FLOAT_TRIGGER_FILTER_RAISING   = 3011 | GX_FEATURE_FLOAT,   ///< �����˲���������
	GX_FLOAT_TRIGGER_FILTER_FALLING   = 3012 | GX_FEATURE_FLOAT,   ///< �����˲����½���
	GX_ENUM_TRIGGER_SOURCE            = 3013 | GX_FEATURE_ENUM,    ///< ����Դ,�ο�GX_TRIGGER_SOURCE_ENTRY
	GX_ENUM_EXPOSURE_MODE             = 3014 | GX_FEATURE_ENUM,    ///< �ع�ģʽ���ο�GX_EXPOSURE_MODE_ENTRY


	//----------------DigitalIO Section----------------------------------
	GX_ENUM_USER_OUTPUT_SELECTOR      = 4000 | GX_FEATURE_ENUM,   ///< �û����Դѡ��,�ο�GX_USER_OUTPUT_SELECTOR_ENTRY
	GX_BOOL_USER_OUTPUT_VALUE         = 4001 | GX_FEATURE_BOOL,   ///< �û��������
	GX_ENUM_USER_OUTPUT_MODE          = 4002 | GX_FEATURE_ENUM,   ///< �û����ģʽѡ��,�ο�GX_USER_OUTPUT_MODE_ENTRY
	GX_ENUM_STROBE_SWITCH             = 4003 | GX_FEATURE_ENUM,   ///< �����ģʽ����,�ο�GX_STROBE_SWITCH_ENTRY

	//----------------AnalogControls Section----------------------------
	GX_ENUM_GAIN_AUTO                 = 5000 | GX_FEATURE_ENUM,   ///< �Զ����濪��,�ο�GX_GAIN_AUTO_ENTRY
	GX_ENUM_GAIN_SELECTOR             = 5001 | GX_FEATURE_ENUM,   ///< ����ͨ��ѡ��,�ο�GX_GAIN_SELECTOR_ENTRY	
	GX_ENUM_BLACKLEVEL_AUTO           = 5003 | GX_FEATURE_ENUM,   ///< �Զ��ڵ�ƽ����,�ο�GX_BLACKLEVEL_AUTO_ENTRY
	GX_ENUM_BLACKLEVEL_SELECTOR       = 5004 | GX_FEATURE_ENUM,   ///< �ڵ�ƽͨ��ѡ��,�ο�GX_BLACKLEVEL_SELECTOR_ENTRY	
	GX_ENUM_BALANCE_WHITE_AUTO        = 5006 | GX_FEATURE_ENUM,   ///< �Զ���ƽ�⿪��,�ο�GX_BALANCE_WHITE_AUTO_ENTRY
	GX_ENUM_BALANCE_RATIO_SELECTOR    = 5007 | GX_FEATURE_ENUM,   ///< ��ƽ��ͨ��,�ο�GX_BALANCE_RATIO_SELECTOR_ENTRY
	GX_FLOAT_BALANCE_RATIO            = 5008 | GX_FEATURE_FLOAT,  ///< ��ƽ��ͨ��ϵ��
	GX_ENUM_COLOR_CORRECT             = 5009 | GX_FEATURE_ENUM,   ///< ��ɫУ������,�ο�GX_COLOR_CORRECT_ENTRY
	GX_ENUM_DEAD_PIXEL_CORRECT        = 5010 | GX_FEATURE_ENUM,   ///< �Զ�����У������,�ο�GX_DEAD_PIXEL_CORRECT_ENTRY
	GX_FLOAT_GAIN                     = 5011 | GX_FEATURE_FLOAT,  ///< ����
	GX_FLOAT_BLACKLEVEL               = 5012 | GX_FEATURE_FLOAT,  ///< �ڵ�ƽ

	//---------------CustomFeature Section-------------------------
	GX_INT_ADC_LEVEL                  = 6000 | GX_FEATURE_INT,    ///< ADC����
	GX_INT_H_BLANKING                 = 6001 | GX_FEATURE_INT,    ///< ˮƽ����
	GX_INT_V_BLANKING                 = 6002 | GX_FEATURE_INT,    ///< ��ֱ����
	GX_STRING_USER_PASSWORD           = 6003 | GX_FEATURE_STRING, ///< �û�����������
	GX_STRING_VERIFY_PASSWORD         = 6004 | GX_FEATURE_STRING, ///< �û�������У������
	GX_BUFFER_USER_DATA               = 6005 | GX_FEATURE_BUFFER, ///< �û�����������
	GX_INT_GRAY_VALUE                 = 6006 | GX_FEATURE_INT,    ///< �����Ҷ�ֵ
	GX_ENUM_AA_LIGHT_ENVIRMENT        = 6007 | GX_FEATURE_ENUM,   ///< �Զ��ع⡢�Զ����棬���ջ�������,�ο�GX_AA_LIGHT_ENVIRMENT_ENTRY
	GX_INT_AAROI_OFFSETX              = 6008 | GX_FEATURE_INT,    ///< �Զ��ع⡢�Զ����棬����Ȥ����Xƫ��
	GX_INT_AAROI_OFFSETY              = 6009 | GX_FEATURE_INT,    ///< �Զ��ع⡢�Զ����棬����Ȥ����Yƫ��
	GX_INT_AAROI_WIDTH                = 6010 | GX_FEATURE_INT,    ///< �Զ��ع⡢�Զ����棬����Ȥ������
	GX_INT_AAROI_HEIGHT               = 6011 | GX_FEATURE_INT,    ///< �Զ��ع⡢�Զ����棬����Ȥ����߶�
	GX_FLOAT_AUTO_GAIN_MIN            = 6012 | GX_FEATURE_FLOAT,  ///< �Զ�������Сֵ
	GX_FLOAT_AUTO_GAIN_MAX            = 6013 | GX_FEATURE_FLOAT,  ///< �Զ��������ֵ
	GX_FLOAT_AUTO_EXPOSURE_TIME_MIN   = 6014 | GX_FEATURE_FLOAT,  ///< �Զ��ع���Сֵ
	GX_FLOAT_AUTO_EXPOSURE_TIME_MAX   = 6015 | GX_FEATURE_FLOAT,  ///< �Զ��ع����ֵ
	GX_BUFFER_FRAME_INFORMATION       = 6016 | GX_FEATURE_BUFFER, ///< ͼ��֡��Ϣ
	GX_INT_CONTRASTPARAM              = 6017 | GX_FEATURE_INT,    ///< �ԱȶȲ���
	GX_FLOAT_GAMMAPARAM               = 6018 | GX_FEATURE_FLOAT,  ///< ٤�����
	GX_INT_COLORCORRECTIONPARAM       = 6019 | GX_FEATURE_INT,    ///< ��ɫУ������
	GX_ENUM_IMAGE_GRAY_RAISE_SWITCH   = 6020 | GX_FEATURE_ENUM,   ///< ͼ���������쿪��,�ο�GX_IMAGE_GRAY_RAISE_SWITCH_ENTRY
	GX_ENUM_AWB_LAMP_HOUSE            = 6021 | GX_FEATURE_ENUM,   ///< �Զ���ƽ����ջ���,�ο�GX_AWB_LAMP_HOUSE_ENTRY



	//---------------UserSetControl Section-------------------------
	GX_ENUM_USER_SET_SELECTOR         = 7000 | GX_FEATURE_ENUM,    ///< ѡ�������,�ο�GX_USER_SET_SELECTOR_ENTRY
	GX_COMMAND_USER_SET_LOAD          = 7001 | GX_FEATURE_COMMAND, ///< ���ز�����
	GX_COMMAND_USER_SET_SAVE          = 7002 | GX_FEATURE_COMMAND, ///< ���������

	//---------------Deprecated Section-------------------------
	GX_STRING_DEVICE_ID               = 4    | GX_FEATURE_STRING, ///< ���к�[���ã�����GX_STRING_DEVICE_SERIAL_NUMBER]	
	GX_STRING_DEVICE_HARDWARE_VERSION = 5    | GX_FEATURE_STRING, ///< Ӳ���汾[����]
	GX_INT_GAIN                       = 5002 | GX_FEATURE_INT,    ///< ����[���ã�����GX_FLOAT_GAIN]
	GX_INT_BLACKLEVEL                 = 5005 | GX_FEATURE_INT,    ///< �ڵ�ƽ[���ã�����GX_FLOAT_BLACKLEVEL]
	GX_FLOAT_BALANCE_RATIO_SELECTOR   = 5007 | GX_FEATURE_ENUM,   ///< ��ƽ��ͨ��[���ã�����GX_ENUM_BALANCE_RATIO_SELECTOR]
	GX_INT_ROI_X                      = 6008 | GX_FEATURE_INT,    ///< �Զ��ع⡢�Զ����棬����Ȥ����Xƫ��[���ã�����GX_INT_AAROI_OFFSETX]
	GX_INT_ROI_Y                      = 6009 | GX_FEATURE_INT,    ///< �Զ��ع⡢�Զ����棬����Ȥ����Yƫ��[���ã�����GX_INT_AAROI_OFFSETY]
	GX_INT_ROI_WIDTH                  = 6010 | GX_FEATURE_INT,    ///< �Զ��ع⡢�Զ����棬����Ȥ������[���ã�����GX_INT_AAROI_WIDTH]
	GX_INT_ROI_HEIGHT                 = 6011 | GX_FEATURE_INT,    ///< �Զ��ع⡢�Զ����棬����Ȥ����߶�[���ã�����GX_INT_AAROI_HEIGHT]
	GX_INT_AUTO_GAIN_VALUEMIN         = 6012 | GX_FEATURE_INT,    ///< �Զ�������Сֵ[���ã�����GX_FLOAT_AUTO_GAIN_MIN]
	GX_INT_AUTO_GAIN_VALUEMAX         = 6013 | GX_FEATURE_INT,    ///< �Զ��������ֵ[���ã�����GX_FLOAT_AUTO_GAIN_MAX]
	GX_INT_AUTO_SHUTTER_VALUEMIN      = 6014 | GX_FEATURE_INT,    ///< �Զ��ع���Сֵ[���ã�����GX_FLOAT_AUTO_EXPOSURE_TIME_MIN]
	GX_INT_AUTO_SHUTTER_VALUEMAX      = 6015 | GX_FEATURE_INT,    ///< �Զ��ع����ֵ[���ã�����GX_FLOAT_AUTO_EXPOSURE_TIME_MAX]


}GX_FEATURE_ID;


//------------------------------------------------------------------------------
//  �������
//------------------------------------------------------------------------------
typedef void* GX_DEV_HANDLE;           ///< �豸���

//------------------------------------------------------------------------------
//  ö�����Ͷ���
//------------------------------------------------------------------------------

typedef enum GX_ACCESS_MODE
{
	GX_ACCESS_NONE          =1,        ///< 
	GX_ACCESS_READONLY      =2,        ///< ֻ����ʽ
	GX_ACCESS_CONTROL       =3,        ///< ���Ʒ�ʽ
	GX_ACCESS_EXCLUSIVE     =4,        ///< ��ռ��ʽ
}GX_ACCESS_MODE;

typedef enum GX_OPEN_MODE
{
	GX_OPEN_SN              =0,        ///< ͨ��SN��
	GX_OPEN_IP              =1,        ///< ͨ��IP��
	GX_OPEN_MAC             =2,        ///< ͨ��MAC��
	GX_OPEN_INDEX           =3,        ///< ͨ��Index��
}GX_OPEN_MODE;

//------------------------------------------------------------------------------------

typedef enum GX_PIXEL_SIZE_ENTRY
{
	GX_PIXEL_SIZE_BPP8  = 8,
	GX_PIXEL_SIZE_BPP10 = 10,
	GX_PIXEL_SIZE_BPP12 = 12,
	GX_PIXEL_SIZE_BPP16 = 16,
	GX_PIXEL_SIZE_BPP24 = 24,
	GX_PIXEL_SIZE_BPP30 = 30,
	GX_PIXEL_SIZE_BPP32 = 32,
	GX_PIXEL_SIZE_BPP36 = 36,
	GX_PIXEL_SIZE_BPP48 = 48,
	GX_PIXEL_SIZE_BPP64 = 64,
}GX_PIXEL_SIZE_ENTRY;

typedef enum GX_PIXEL_COLOR_FILTER_ENTRY
{
	GX_COLOR_FILTER_NONE     = 0,                        ///<��
	GX_COLOR_FILTER_BAYER_RG = 1,                        ///<RG��ʽ
	GX_COLOR_FILTER_BAYER_GB = 2,                        ///<GB��ʽ
	GX_COLOR_FILTER_BAYER_GR = 3,                        ///<GR��ʽ
	GX_COLOR_FILTER_BAYER_BG = 4,                        ///<BG��ʽ
}GX_PIXEL_COLOR_FILTER_ENTRY;

#define GX_PIXEL_MONO                  ( 0x01000000 )
#define GX_PIXEL_COLOR                 ( 0x02000000 )

#define GX_PIXEL_8BIT                  ( 0x00080000 )
#define GX_PIXEL_10BIT                 ( 0x000A0000 )
#define GX_PIXEL_12BIT                 ( 0x000C0000 )
#define GX_PIXEL_16BIT                 ( 0x00100000 )
#define GX_PIXEL_24BIT                 ( 0x00180000 )
#define GX_PIXEL_30BIT                 ( 0x001E0000 )
#define GX_PIXEL_32BIT                 ( 0x00200000 )
#define GX_PIXEL_36BIT                 ( 0x00240000 )
#define GX_PIXEL_48BIT                 ( 0x00300000 )
#define GX_PIXEL_64BIT                 ( 0x00400000 )

typedef enum GX_PIXEL_FORMAT_ENTRY
{
	GX_PIXEL_FORMAT_UNDEFINED          = (0),
	GX_PIXEL_FORMAT_MONO8              = (GX_PIXEL_MONO  | GX_PIXEL_8BIT  | 0x0001),//0x1080001,
	GX_PIXEL_FORMAT_MONO8_SIGNED       = (GX_PIXEL_MONO  | GX_PIXEL_8BIT  | 0x0002),//0x1080002,
	GX_PIXEL_FORMAT_MONO10             = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0003),//0x1100003,
	GX_PIXEL_FORMAT_MONO10_PACKED      = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x0004),//0x10c0004,
	GX_PIXEL_FORMAT_MONO12             = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0005),//0x1100005,
	GX_PIXEL_FORMAT_MONO12_PACKED      = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x0006),//0x10c0006,
	GX_PIXEL_FORMAT_MONO14             = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0025),//0x1100025,
	GX_PIXEL_FORMAT_MONO16             = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0007),//0x1100007,
	GX_PIXEL_FORMAT_BAYER_GR8          = (GX_PIXEL_MONO  | GX_PIXEL_8BIT  | 0x0008),//0x1080008,               
	GX_PIXEL_FORMAT_BAYER_RG8          = (GX_PIXEL_MONO  | GX_PIXEL_8BIT  | 0x0009),//0x1080009,                
	GX_PIXEL_FORMAT_BAYER_GB8          = (GX_PIXEL_MONO  | GX_PIXEL_8BIT  | 0x000A),//0x108000A,
	GX_PIXEL_FORMAT_BAYER_BG8          = (GX_PIXEL_MONO  | GX_PIXEL_8BIT  | 0x000B),//0x108000B,
	GX_PIXEL_FORMAT_BAYER_GR10         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x000C),//0x110000C,                
	GX_PIXEL_FORMAT_BAYER_RG10         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x000D),//0x110000D,
	GX_PIXEL_FORMAT_BAYER_GB10         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x000E),//0x110000E,
	GX_PIXEL_FORMAT_BAYER_BG10         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x000F),//0x110000F,
	GX_PIXEL_FORMAT_BAYER_GR12         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0010),//0x1100010,              
	GX_PIXEL_FORMAT_BAYER_RG12         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0011),//0x1100011,
	GX_PIXEL_FORMAT_BAYER_GB12         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0012),//0x1100012,
	GX_PIXEL_FORMAT_BAYER_BG12         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0013),//0x1100013,
	GX_PIXEL_FORMAT_BAYER_GR10_PACKED  = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x0026),//0x10C0026,                
	GX_PIXEL_FORMAT_BAYER_RG10_PACKED  = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x0027),//0x10C0027,
	GX_PIXEL_FORMAT_BAYER_GB10_PACKED  = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x0028),//0x10C0028,
	GX_PIXEL_FORMAT_BAYER_BG10_PACKED  = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x0029),//0x10C0029,
	GX_PIXEL_FORMAT_BAYER_GR12_PACKED  = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x002A),//0x10C002A,              
	GX_PIXEL_FORMAT_BAYER_RG12_PACKED  = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x002B),//0x10C002B,
	GX_PIXEL_FORMAT_BAYER_GB12_PACKED  = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x002C),//0x10C002C,
	GX_PIXEL_FORMAT_BAYER_BG12_PACKED  = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x002D),//0x10C002D,
	GX_PIXEL_FORMAT_BAYER_GR16         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x002E),//0x110002E,                
	GX_PIXEL_FORMAT_BAYER_RG16         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x002F),//0x110002F,
	GX_PIXEL_FORMAT_BAYER_GB16         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0030),//0x1100030,
	GX_PIXEL_FORMAT_BAYER_BG16         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0031),//0x1100031,
	GX_PIXEL_FORMAT_RGB8_PACKED        = (GX_PIXEL_COLOR | GX_PIXEL_24BIT | 0x0014),//0x2180014,
	GX_PIXEL_FORMAT_BGR8_PACKED        = (GX_PIXEL_COLOR | GX_PIXEL_24BIT | 0x0015),//0x2180015,
	GX_PIXEL_FORMAT_RGBA8_PACKED       = (GX_PIXEL_COLOR | GX_PIXEL_32BIT | 0x0016),//0x2200016,
	GX_PIXEL_FORMAT_BGRA8_PACKED       = (GX_PIXEL_COLOR | GX_PIXEL_32BIT | 0x0017),//0x2200017,
	GX_PIXEL_FORMAT_RGB10_PACKED       = (GX_PIXEL_COLOR | GX_PIXEL_48BIT | 0x0018),//0x2300018,
	GX_PIXEL_FORMAT_BGR10_PACKED       = (GX_PIXEL_COLOR | GX_PIXEL_48BIT | 0x0019),//0x2300019,
	GX_PIXEL_FORMAT_RGB12_PACKED       = (GX_PIXEL_COLOR | GX_PIXEL_48BIT | 0x001A),//0x230001A,
	GX_PIXEL_FORMAT_BGR12_PACKED       = (GX_PIXEL_COLOR | GX_PIXEL_48BIT | 0x001B),//0x230001B,
	GX_PIXEL_FORMAT_RGB16_PACKED       = (GX_PIXEL_COLOR | GX_PIXEL_48BIT | 0x0033),//0x2300033,
	GX_PIXEL_FORMAT_BGR10V1_PACKED     = (GX_PIXEL_COLOR | GX_PIXEL_32BIT | 0x001C),//0x220001C,
	GX_PIXEL_FORMAT_BGR10V2_PACKED     = (GX_PIXEL_COLOR | GX_PIXEL_32BIT | 0x001D),//0x220001D,
	GX_PIXEL_FORMAT_RGB12V1_PACKED     = (GX_PIXEL_COLOR | GX_PIXEL_36BIT | 0x0034),//0x2240034,
	GX_PIXEL_FORMAT_YUV411_PACKED      = (GX_PIXEL_COLOR | GX_PIXEL_12BIT | 0x001E),//0x20C001E,
	GX_PIXEL_FORMAT_YUV422_PACKED      = (GX_PIXEL_COLOR | GX_PIXEL_16BIT | 0x001F),//0x210001F,
	GX_PIXEL_FORMAT_YUV422YUYV_PACKED  = (GX_PIXEL_COLOR | GX_PIXEL_16BIT | 0x0032),//0x2100032,
	GX_PIXEL_FORMAT_YUV444_PACKED      = (GX_PIXEL_COLOR | GX_PIXEL_24BIT | 0x0020),//0x2180020,
	GX_PIXEL_FORMAT_RGB8_PLANAR        = (GX_PIXEL_COLOR | GX_PIXEL_24BIT | 0x0021),//0x2180021,
	GX_PIXEL_FORMAT_RGB10_PLANAR       = (GX_PIXEL_COLOR | GX_PIXEL_48BIT | 0x0022),//0x2300022,
	GX_PIXEL_FORMAT_RGB12_PLANAR       = (GX_PIXEL_COLOR | GX_PIXEL_48BIT | 0x0023),//0x2300023,
	GX_PIXEL_FORMAT_RGB16_PLANAR       = (GX_PIXEL_COLOR | GX_PIXEL_48BIT | 0x0024),//0x2300024,
}GX_PIXEL_FORMAT_ENTRY;

typedef enum GX_ACQUISITION_MODE_ENTRY
{
	GX_ACQ_MODE_SINGLE_FRAME = 0,                          ///<��֡ģʽ
	GX_ACQ_MODE_MULITI_FRAME = 1,                          ///<��֡ģʽ
	GX_ACQ_MODE_CONTINUOUS   = 2,                          ///<����ģʽ
}GX_ACQUISITION_MODE_ENTRY;

typedef enum GX_TRIGGER_MODE_ENTRY
{
	GX_TRIGGER_MODE_OFF = 0,                             ///< �رմ���ģʽ
	GX_TRIGGER_MODE_ON  = 1,                             ///< �򿪴���ģʽ
}GX_TRIGGER_MODE_ENTRY;

typedef enum GX_TRIGGER_SOURCE_ENTRY
{
	GX_TRIGGER_SOURCE_SOFTWARE = 0,                      ///< ����
	GX_TRIGGER_SOURCE_LINE0    = 1,                      ///< ����Դ0
}GX_TRIGGER_SOURCE_ENTRY;

typedef enum GX_TRIGGER_ACTIVATION_ENTRY
{
	GX_TRIGGER_ACTIVATION_FALLINGEDGE = 0,               ///< �½��ش���
	GX_TRIGGER_ACTIVATION_RISINGEDGE  = 1,               ///< �����ش���
}GX_TRIGGER_ACTIVATION_ENTRY;

typedef enum GX_TRIGGER_SWITCH_ENTRY
{
	GX_TRIGGER_SWITCH_OFF = 0,                           ///< �ر��ⴥ��
	GX_TRIGGER_SWITCH_ON  = 1,                           ///< ���ⴥ��
}GX_TRIGGER_SWITCH_ENTRY;

typedef enum GX_EXPOSURE_MODE_ENTRY
{
	GX_EXPOSURE_MODE_OFF            = 0,                 ///< 
	GX_EXPOSURE_MODE_TIMED          = 1,                 ///< �ع�ʱ��Ĵ��������ع�ʱ��
	GX_EXPOSURE_MODE_TRIGGERWIDTH   = 2,                 ///< �����źſ�ȿ����ع�ʱ��
}GX_EXPOSURE_MODE_ENTRY;

typedef enum GX_EXPOSURE_AUTO_ENTRY
{
	GX_EXPOSURE_AUTO_OFF        = 0,                     ///< �ر��Զ��ع�
	GX_EXPOSURE_AUTO_CONTINUOUS = 1,                     ///< �����Զ��ع�
	GX_EXPOSURE_AUTO_ONCE       = 2,                     ///< �����Զ��ع�
}GX_EXPOSURE_AUTO_ENTRY;

typedef enum GX_USER_OUTPUT_SELECTOR_ENTRY
{
	GX_USER_OUTPUT_SELECTOR_OUTPUT0 = 1,                   ///<���0
	GX_USER_OUTPUT_SELECTOR_OUTPUT1 = 2,                   ///<���1
	GX_USER_OUTPUT_SELECTOR_OUTPUT2 = 4,                   ///<���2
}GX_USER_OUTPUT_SELECTOR_ENTRY;

typedef enum GX_USER_OUTPUT_MODE_ENTRY
{
	GX_USER_OUTPUT_MODE_STROBE      = 0,                   ///<�����
	GX_USER_OUTPUT_MODE_USERDEFINED = 1,                   ///<�û��Զ���
}GX_USER_OUTPUT_MODE_ENTRY;

typedef enum GX_STROBE_SWITCH_ENTRY
{
	GX_STROBE_SWITCH_OFF = 0,                            ///< �ر�����ƿ���
	GX_STROBE_SWITCH_ON  = 1,                            ///< ������ƿ���
}GX_STROBE_SWITCH_ENTRY;

typedef enum GX_GAIN_AUTO_ENTRY
{
	GX_GAIN_AUTO_OFF        = 0,                         ///< �ر��Զ�����
	GX_GAIN_AUTO_CONTINUOUS = 1,                         ///< �����Զ�����
	GX_GAIN_AUTO_ONCE       = 2,                         ///< �����Զ�����
}GX_GAIN_AUTO_ENTRY;

typedef enum GX_GAIN_SELECTOR_ENTRY
{
	GX_GAIN_SELECTOR_ALL   = 0,                          ///< ��������ͨ��
	GX_GAIN_SELECTOR_RED   = 1,                          ///< ��ͨ������
	GX_GAIN_SELECTOR_GREEN = 2,                          ///< ��ͨ������
	GX_GAIN_SELECTOR_BLUE  = 3,                          ///< ��ͨ������
}GX_GAIN_SELECTOR_ENTRY;

typedef enum GX_BLACKLEVEL_AUTO_ENTRY
{
	GX_BLACKLEVEL_AUTO_OFF        = 0,                   ///< �ر��Զ��ڵ�ƽ
	GX_BLACKLEVEL_AUTO_CONTINUOUS = 1,                   ///< �����Զ��ڵ�ƽ
	GX_BLACKLEVEL_AUTO_ONCE       = 2,                   ///< �����Զ��ڵ�ƽ
}GX_BLACKLEVEL_AUTO_ENTRY;

typedef enum GX_BLACKLEVEL_SELECTOR_ENTRY
{
	GX_BLACKLEVEL_SELECTOR_ALL   = 0,                    ///< ���кڵ�ƽͨ��
	GX_BLACKLEVEL_SELECTOR_RED   = 1,                    ///< ��ͨ���ڵ�ƽ
	GX_BLACKLEVEL_SELECTOR_GREEN = 2,                    ///< ��ͨ���ڵ�ƽ
	GX_BLACKLEVEL_SELECTOR_BLUE  = 3,                    ///< ��ͨ���ڵ�ƽ
}GX_BLACKLEVEL_SELECTOR_ENTRY;

typedef enum GX_BALANCE_WHITE_AUTO_ENTRY
{
	GX_BALANCE_WHITE_AUTO_OFF        = 0,                ///< �ر��Զ���ƽ��
	GX_BALANCE_WHITE_AUTO_CONTINUOUS = 1,                ///< �����Զ���ƽ��
	GX_BALANCE_WHITE_AUTO_ONCE       = 2,                ///< �����Զ���ƽ��
}GX_BALANCE_WHITE_AUTO_ENTRY;

typedef enum GX_BALANCE_RATIO_SELECTOR_ENTRY
{
	GX_BALANCE_RATIO_SELECTOR_RED   = 0,                   ///<��ͨ��
	GX_BALANCE_RATIO_SELECTOR_GREEN = 1,                   ///<��ͨ��
	GX_BALANCE_RATIO_SELECTOR_BLUE  = 2,                   ///<��ͨ��
}GX_BALANCE_RATIO_SELECTOR_ENTRY;

typedef enum GX_COLOR_CORRECT_ENTRY
{
	GX_COLOR_CORRECT_OFF = 0,                            ///< �ر��Զ���ɫУ��
	GX_COLOR_CORRECT_ON  = 1,                            ///< ���Զ���ɫУ��
}GX_COLOR_CORRECT_ENTRY;

typedef enum GX_DEAD_PIXEL_CORRECT_ENTRY
{
	GX_DEAD_PIXEL_CORRECT_OFF = 0,                       ///< �ر��Զ�����У��
	GX_DEAD_PIXEL_CORRECT_ON  = 1,                       ///< ���Զ�����У��
}GX_DEAD_PIXEL_CORRECT_ENTRY;

typedef enum GX_AA_LIGHT_ENVIRMENT_ENTRY
{
	GX_AA_LIGHT_ENVIRMENT_NATURELIGHT = 0,                 ///<��Ȼ��
	GX_AA_LIGHT_ENVIRMENT_AC50HZ      = 1,                 ///<50�����չ��
	GX_AA_LIGHT_ENVIRMENT_AC60HZ      = 2,                 ///<60�����չ��
}GX_AALIGHT_ENVIRMENT_ENTRY;

typedef enum GX_USER_SET_SELECTOR_ENTRY
{
	GX_ENUM_USER_SET_SELECTOR_DEFAULT  = 0,                 ///<Ĭ�ϲ�����
	GX_ENUM_USER_SET_SELECTOR_USERSET0 = 1,                 ///<�û�������0
}GX_USER_SET_SELECTOR_ENTRY;

typedef enum GX_IMAGE_GRAY_RAISE_SWITCH_ENTRY
{
	GX_IMAGE_GRAY_RAISE_SWITCH_OFF = 0,                     ///< ͼ���������쿪��
	GX_IMAGE_GRAY_RAISE_SWITCH_ON  = 1,                     ///< ͼ���������쿪��
}GX_IMAGE_GRAY_RAISE_SWITCH_ENTRY;

typedef enum GX_AWB_LAMP_HOUSE_ENTRY
{
	GX_AWB_LAMP_HOUSE_ADAPTIVE      = 0,                      ///< ����Ӧ��Դ
	GX_AWB_LAMP_HOUSE_D65           = 1,                      ///< ָ��D65��Դ
	GX_AWB_LAMP_HOUSE_FLUORESCENCE  = 2,                      ///< ָ��ӫ���
	GX_AWB_LAMP_HOUSE_INCANDESCENT  = 3,                      ///< ָ���׳��
}GX_AWB_LAMP_HOUSE_ENTRY;

//------------------------------------------------------------------------------
//  �ṹ�����Ͷ���
//------------------------------------------------------------------------------
typedef struct GX_DEVICE_BASE_INFO 
{
	char szVendorName[32];             ///< ��������
	char szModelName[32];              ///< �豸��������
	char szSN[32];                     ///< �豸���к�
	char szDisplayName[64];            ///< �豸չʾ����
	char reserved[512];                ///< ����
}GX_DEVICE_BASE_INFO;

typedef struct GX_DEVICE_IP_INFO 
{
	char szMAC[32];                 ///< MAC��ַ
	char szIP[32];                  ///< IP��ַ
	char szSubNetMask[32];          ///< ��������
	char szGateWay[32];             ///< ����	
	char reserved[512];             ///< ����
}GX_DEVICE_IP_INFO;

typedef struct GX_OPEN_PARAM 
{
	char           *pszContent;        ///< �����������
	GX_OPEN_MODE   openMode;           ///< �򿪷�ʽ
	GX_ACCESS_MODE accessMode;         ///< ����ģʽ
}GX_OPEN_PARAM;

typedef struct GX_FRAME_CALLBACK_PARAM
{
	void*         pUserParam;          ///< �û�˽������
	int32_t       status;              ///< ��ȡ��֡ͼ��ķ���״̬
	const  void*  pImgBuf;             ///< ���ص�ͼ��buffer��ַ
	int32_t       nImgSize;            ///< ���ص�ͼ���С
	int32_t       reserved[8];         ///< ����
}GX_FRAME_CALLBACK_PARAM;

typedef struct GX_FRAME_DATA
{
	int32_t       nStatus;             ///< ��ȡ��֡�ķ���״̬
	void*         pImgBuf;             ///< ���ص�ͼ��buffer
	int32_t       nWidth;              ///< ͼ��Ŀ�
	int32_t       nHeight;             ///< ͼ��ĸ�
	int32_t       nPixelFormat;        ///< ͼ���PixFormat
	int32_t       nImgSize;            ///< ͼ���С
	int32_t       reserved[7];         ///< ����
}GX_FRAME_DATA;

typedef struct GX_INT_RANGE
{
	int64_t nMin;                      ///< ����ֵ��Сֵ
	int64_t nMax;                      ///< ����ֵ���ֵ
	int64_t nInc;                      ///< ����ֵ����
	int32_t reserved[8];               ///< ����
}GX_INT_RANGE;

typedef struct GX_FLOAT_RANGE
{
	double  dMin;                       ///< ��������Сֵ
	double  dMax;                       ///< ���������ֵ
	double  dInc;                       ///< �����Ͳ���
	char    szUnit[8];                  ///< �����͵�λ
	int32_t reserved[8];                ///< ����
}GX_FLOAT_RANGE;

typedef struct GX_ENUM_DESCRIPTION
{
	int64_t nValue;                    ///< ö��ֵ
	char    szSymbolic[64];            ///< �ַ�����
	int32_t reserved[8];               ///< ����
}GX_ENUM_DESCRIPTION;
//------------------------------------------------------------------------------
//  �ص��������Ͷ���
//------------------------------------------------------------------------------
typedef void (GX_STDC* GXCaptureCallBack) (GX_FRAME_CALLBACK_PARAM *pFrameData); ///< �ɼ��ص�������ʽ����

//------------------------------------------------------------------------------
//  ��׼C API���ܺ�������
//------------------------------------------------------------------------------
#define GX_API GX_EXTC GX_STATUS GX_STDC

GX_API GXInitLib();
GX_API GXCloseLib();

GX_API GXUpdateDeviceList         (uint32_t* punNumDevices, int32_t unTimeOut);
GX_API GXGetAllDeviceBaseInfo     (GX_DEVICE_BASE_INFO* pDeviceInfo, size_t* pBufferSize);
GX_API GXGetAllDeviceIPInfo       (GX_DEVICE_IP_INFO* pDeviceInfo, size_t* pBufferSize);
GX_API GXGetLastError             (GX_STATUS *pErrorCode, char *pszErrText, size_t *pSize);

GX_API GXOpenDeviceByIndex        (uint32_t nDeviceIndex, GX_DEV_HANDLE* phDevice);   // ������
GX_API GXOpenDevice               (GX_OPEN_PARAM* pOpenParam, GX_DEV_HANDLE* phDevice);
GX_API GXCloseDevice              (GX_DEV_HANDLE hDevice);

GX_API GXIsImplemented		      (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, bool* pbIsImplemented);
GX_API GXIsReadable               (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, bool* pbIsReadable);
GX_API GXIsWritable               (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, bool* pbIsWritable);
GX_API GXGetIntRange		      (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, GX_INT_RANGE* pIntRange);
GX_API GXGetInt				      (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, int64_t* pnValue);
GX_API GXSetInt				      (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, int64_t nValue);
GX_API GXGetFloatRange		      (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, GX_FLOAT_RANGE* pFloatRange);
GX_API GXSetFloat                 (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, double dValue);
GX_API GXGetFloat                 (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, double* pdValue);
GX_API GXGetEnumEntryNums         (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, uint32_t* pnEntryNums);
GX_API GXGetEnumDescription       (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, GX_ENUM_DESCRIPTION* pEnumDescription, size_t* pBufferSize);
GX_API GXGetEnum			      (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, int64_t* pnValue);
GX_API GXSetEnum			      (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, int64_t nValue);
GX_API GXGetBool			      (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, bool* pbValue);
GX_API GXSetBool			      (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, bool bValue);
GX_API GXGetStringLength	      (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, size_t* pnSize);
GX_API GXGetString			      (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, char* pszContent, size_t* pnSize);
GX_API GXSetString			      (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, char* pszContent);
GX_API GXGetBufferLength	      (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, size_t* pnSize);
GX_API GXGetBuffer			      (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, uint8_t* pBuffer, size_t* pnSize);
GX_API GXSetBuffer			      (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, uint8_t* pBuffer, size_t nSize);
GX_API GXSendCommand		      (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID);
GX_API GXGetFeatureName           (GX_DEV_HANDLE hDevice, GX_FEATURE_ID featureID, char* pszName, size_t* pnSize); 

GX_API GXRegisterCaptureCallback  (GX_DEV_HANDLE hDevice, void *pUserParam, GXCaptureCallBack callBackFun);
GX_API GXUnregisterCaptureCallback(GX_DEV_HANDLE hDevice);
GX_API GXGetImage                 (GX_DEV_HANDLE hDevice, GX_FRAME_DATA *pFrameData, int32_t nTimeout);

#endif  //GX_GALAXY_H