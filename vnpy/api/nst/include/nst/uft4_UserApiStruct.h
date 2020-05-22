#ifndef _UFT4_APIDATASTRUCT_H_
#define _UFT4_APIDATASTRUCT_H_

#include "os_common.h"
#include "uft4_UserApiDataType.h"

 


typedef struct tagUftFtdcInputOrderField
{
	///�������
	T_UFT_FtdcOrderSysIDType	OrderSysID;
	///��Ա����
	T_UFT_FtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	T_UFT_FtdcClientIDType	ClientID;
	///�����û�����
	T_UFT_FtdcUserIDType	UserID;
	///��Լ����
	T_UFT_FtdcInstrumentIDType	InstrumentID;
	///�����۸�����
	T_UFT_FtdcOrderPriceTypeType	OrderPriceType;
	///��������
	T_UFT_FtdcDirectionType	Direction;
	///��Ͽ�ƽ��־
	T_UFT_FtdcCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	T_UFT_FtdcCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	T_UFT_FtdcPriceType	LimitPrice;
	///����
	T_UFT_FtdcVolumeType	VolumeTotalOriginal;
	///��Ч������
	T_UFT_FtdcTimeConditionType	TimeCondition;
	///GTD����
	T_UFT_FtdcDateType	GTDDate;
	///�ɽ�������
	T_UFT_FtdcVolumeConditionType	VolumeCondition;
	///��С�ɽ���
	T_UFT_FtdcVolumeType	MinVolume;
	///��������
	T_UFT_FtdcContingentConditionType	ContingentCondition;
	///ֹ���
	T_UFT_FtdcPriceType	StopPrice;
	///ǿƽԭ��
	T_UFT_FtdcForceCloseReasonType	ForceCloseReason;
	///���ر������
	T_UFT_FtdcOrderLocalIDType	OrderLocalID;
	///�Զ������־
	T_UFT_FtdcBoolType	IsAutoSuspend;
	///ҵ��Ԫ
	T_UFT_FtdcBusinessUnitType	BusinessUnit;

}CUftFtdcInputOrderField;




// �ͻ��ֱֲ�
typedef struct tagUftQryInvestorPositionField
{
	char		futu_code[7];		//	��Լ����	HsFuturesCode;ͨ��CFutuCode.Get��ȡ
	char		entrust_bs;			//	��������	HsType����0���򣻡�1����
	int32_t		OldEnableAmount;	//	�ϲֿ�������	HsQuantity
	int32_t		OldCurrentAmount;	//	�ϲֲֳ�����	HsQuantity
	int32_t		TodayEnableAmount;//	���տ�������	HsQuantity
	int32_t		TodayCurrentAmount;//	���ճֲ�����	HsQuantity
	T_UFT_FtdcMoneyType	OldOpenBalance;	//	�ϲֲֳֳɽ���	HsAmount
	T_UFT_FtdcMoneyType	TodayOpenBalance;	//	��ֲֳֳɽ���	HsAmount
	T_UFT_FtdcMoneyType	HoldMargin;		//	�ֱֲ�֤��	HsAmount
} QryInvestorPositionField;

// ����¼������
typedef struct tagUftReqOrderInsertField
{
	CUftFtdcInputOrderField	Order;
	int64_t						OrderRef;		//��������ʶ
	uint8_t                     MaginCheck;     //�����Ƿ���д���Żݡ�0��������Ƿ��Żݣ���1�����
} CUftReqOrderInsertField;


//������̨��Ӧ
typedef struct tagUftAnsOrderInsertField
{
	int64_t						OrderRef;		//��������ʶ
	uint32_t					OrderIndex;		//�������
	uint32_t					ProcessTime;	// ί�д���ʱ��
	uint32_t					TotalTime;		// ����Ӧ��ʱ��
	uint32_t					ExchangeTime;	// ������ʱ��
	uint32_t					SenderID;
	uint32_t					LocalTimeDelay; //����������ʱ
	char                        StockCode[8];
	char						Direction;
	char						EntrustType;
	double						Price;
	int32_t						Amount;
} CUftAnsOrderInsertField;

//������������Ӧ
typedef struct tagUftRspnOrderInsertField
{
	int64_t						OrderRef;		//��������ʶ
	uint32_t					OrderIndex;     //����ԭ������OrderIndex
	uint32_t					ProcessTime;	// ί�д���ʱ��
	uint32_t					TotalTime;		// ����Ӧ��ʱ��
	uint32_t					ExchangeTime;	// ������ʱ��
	T_UFT_FtdcOrderSysIDType	OrderSysID;		//�������������
	char						OrderStatus;	//����״̬
} CUftRspnOrderInsertField;//API EXPOSURE




typedef struct tagUftAnsOrderActionField
{
	int64_t						OrderRef;		//��������ʶ
	uint32_t					OrderIndex;
	T_UFT_FtdcOrderSysIDType	OrderSysID;		//�������
	T_UFT_FtdcOrderLocalIDType	OrderLocalID;	//���ر������
	uint32_t					SenderID;
} CUftAnsOrderActionField;



//��������
typedef struct tagUftRspnOrderActionField
{
	T_UFT_FtdcOrderSysIDType		OrderSysID;		//�������
	T_UFT_FtdcOrderLocalIDType	OrderLocalID;	//���ر������
	int64_t						OrderRef;		//��������ʶ
	uint32_t					OrderIndex;	
} CUftRspnOrderActionField;



//�����ر�
typedef struct tagUftRtnnOrderField
{
	int64_t						OrderRef;		//��������ʶ
	uint32_t					OrderIndex;     //�������
	uint32_t					ProcessTime;	// ί�д���ʱ��
	uint32_t					TotalTime;		// ����Ӧ��ʱ��
	uint32_t					ExchangeTime;	// ������ʱ��
	T_UFT_FtdcOrderSysIDType	OrderSysID;		//�������������
	T_UFT_FtdcVolumeType		VolumeTotal;	//ʣ������
	T_UFT_FtdcOrderStatusType	OrderStatus;	//����״̬
	char                        StockCode[8];
	char						EntrustType;
	char						Direction;
	double						Price;
	int32_t						Amount;
} CUftRtnnOrderField;



//�ɽ��ر�
typedef struct tagUftRtnnTradeField
{
	T_UFT_FtdcPriceType				BusinessPrice;		// �ɽ��۸�	HsFutuPrice
	int32_t							BusinessTime;		// �ɽ�ʱ��	HsTime
	int32_t							BusinessAmount;		// �ɽ�����	HsQuantity
	double							fare;				// ������
	int64_t							OrderRef;			//��������ʶ
	uint32_t						OrderIndex;			// �������ر�ʶ
	T_UFT_FtdcTradeIDType			BusinessId;		// �ɽ����	
	T_UFT_FtdcOrderSysIDType 		OrderSysID;			// ������ţ�char[13], ����δ����
	int8_t							session_id;		//�ỰID
	char						    Direction;          //������־
	char						    OffsetFlag;         //��ƽ��־
	char						    HedgeFlag;			//Ͷ���ױ���־
	int8_t            			    reserve[5];			//ռλ
	char							InstrumentID[17];     //��Լ��	
	char							StockCode[9];
} CUftRtnnTradeField;


//�ʽ��ѯӦ��
typedef struct tagUftAnsQueryFundField
{
	T_UFT_FtdcMoneyType			EnabledBalance;	//�����ʽ�
	T_UFT_FtdcMoneyType			Deposit;		//�������
	T_UFT_FtdcMoneyType			BeginBalance;	// ��ʼȨ��
	T_UFT_FtdcMoneyType			Margin;			// ��֤��
	T_UFT_FtdcMoneyType			Profit;			//ƽ��ӯ��
	double						TotalFare;		//������
	double						CurrBalance;	// ��ǰȨ��
	double						floating_gain_loss; //�ֲ�ӯ��
	double						FrozenBalance;	// �����ʽ�
	uint32_t					SenderID;
	double						EnablePurQuota;	// �����޹����
} CUftAnsQueryFundField;


//ί�в�ѯ
typedef struct tagUftReqQueryOrderField
{
	T_UFT_FtdcInstrumentIDType	InstrumentID;	//��Լ���룻InstrumentID[0]Ϊ'\0'��ʾ������
	int32_t						BeginTime;		//ί����ʼʱ�䣻Ϊ0��ʾ������
	int32_t						EndTime;		 //ί����ֹʱ�䣻Ϊ0��С��BeginTime��ʾ������
	T_UFT_FtdcOrderSysIDType	OrderSysID;		 //������������ţ�ΪOrderSysID[0]Ϊ'\0'��ʾ������
	uint32_t					OrderIndex;		 //���ر���������Ϊ0xFFFFFFFF��ʾ������
	uint32_t					EntrustSenderID; //�ͻ��˱�����ţ�Ϊ0xFFFFFFFF��ʾ������
	int64_t						OrderRef;		 //�ͻ��˵�������ʶ��Ϊ-1��ʾ������
	uint32_t					Position;		  //��λ��������ָ����ҳ��ȡ�ü�¼�ż�֮��ļ�¼���״�����0������ʹ��Ӧ���е�Positionֵ
	uint8_t						all_session_data;	//Ϊ1��ʾ��ȡ���лỰ������		

} CUftReqQueryOrderField;

//ί�в�ѯӦ��
typedef struct tagUftAnsQueryOrderField
{
	CUftFtdcInputOrderField	    Order;
	int64_t						OrderRef;		//��������ʶ
	char						MaginCheck;		// �Ƿ���д���Żݡ�0��������Ƿ��Żݣ���1�����
	int32_t						VolumeTotal;	//ί��ʣ������
	char						OrderStatus;	// ����״̬���������ѱ������ɣ��ɽ����ϵ�
	int8_t						session_id;		//�ỰID
	int8_t            			reserve;		//ռλ
	int32_t						OrderTime;		// ί��ʱ��
	uint32_t					SenderID;
	int32_t						ErrorID;
	uint32_t					OrderIndex;
	char						StockCode[8];   //��Ʊ���룬��\0�������������Կո���
	char						Direction;      //������־1��2��
	char						EntrustType;    //ί������'0'�޼�ί�У�'1'�������ţ�'2'���ַ�����ʣ��ת�޼ۣ�'3'�м������ɽ�ʣ�೷����'4'�м�ȫ��ɽ�������'5'�м������嵵ȫ��ɽ�ʣ�೷��
	char						CashMargin;		//���ñ�ʶ1=Cash����ͨ���ף�2=Open ������ȯ���֣�3=Close ������ȯƽ��
	int8_t						TGWNo;			//���������ر�ţ�0������1~nָ����ΧΪָ�����ر���
	double						Price;			//ί�м۸�N13(4)
	double						Amount;			//ί������N15(2)
	char						OrderSysID[17]; //�������������
	char						ClOrdID[11];	//���ص��� ��\0������
} CUftAnsQueryOrderField;

//�ɽ���ѯ
typedef struct tagUftReqQueryTradeField
{
	T_UFT_FtdcInstrumentIDType	InstrumentID;	//��Լ���룻InstrumentID[0]Ϊ'\0'��ʾ������
	int32_t						BeginTime;		//�ɽ���ʼʱ�䣻Ϊ0��ʾ������
	int32_t						EndTime;		//�ɽ���ֹʱ�䣻Ϊ0��С��BeginTime��ʾ������
	T_UFT_FtdcOrderSysIDType	OrderSysID;		//������������ţ�ΪOrderSysID[0]Ϊ'\0'��ʾ������
	uint32_t					OrderIndex;		//���ر���������Ϊ0xFFFFFFFF��ʾ������
	int64_t						OrderRef;		//�ͻ��˵�������ʶ��Ϊ-1��ʾ������
	uint32_t					Position;		//��λ��������ָ����ҳ��ȡ�ü�¼�ż�֮��ļ�¼���״�����0������ʹ��Ӧ���е�Positionֵ
	uint8_t						all_session_data; //Ϊ1��ʾ��ȡ���лỰ������		

} CUftReqQueryTradeField;

//�ɽ���ѯӦ��
typedef struct tagUftAnsQueryTradeField
{
	T_UFT_FtdcPriceType				BusinessPrice;		// �ɽ��۸�	HsFutuPrice
	int32_t							BusinessTime;		// �ɽ�ʱ��	HsTime
	int32_t							BusinessAmount;		// �ɽ�����	HsQuantity
	double							fare;				// ������	
	int64_t							OrderRef;			//��������ʶ
	uint32_t						OrderIndex;			// �������ر�ʶ
	T_UFT_FtdcTradeIDType			BusinessId;		    // �ɽ����	HsChar32
	T_UFT_FtdcOrderSysIDType		OrderSysID;			// ������ţ�char[16], ����δ����
	int8_t							session_id;			//�ỰID
	char							Direction;          //������־
	char							OffsetFlag;         //��ƽ��־
	char					 		InstrumentID[17];	  //��Լ��	
	char							StockCode[8];       //��Ʊ���룬��\0������

} CUftAnsQueryTradeField;

//�ֲֲ�ѯ
typedef struct tagUftReqQueryPositionField
{
	T_UFT_FtdcInstrumentIDType	InstrumentID;	//��Լ���룻InstrumentID[0]Ϊ'\0'��ʾ������
} CUftReqQueryPositionField;

//�ֲֲ�ѯӦ��
typedef struct tagUftAnsQueryPositionField
{
	//char		futu_code[7];		//	��Լ����	HsFuturesCode;ͨ��CFutuCode.Get��ȡ
	//char		futu_code[9];		//	
	char		futu_code[16];		//֧��ͭ��Ȩ����Լ���ȴ�9λ��չ��16ȫ
	char		entrust_bs;			//	��������	HsType����0���򣻡�1����
	T_UFT_FtdcHedgeFlagType HedgeFlag;	//Ͷ���ױ���־	
//	char		reserve[7];
	char		reserve[6];
	int32_t		OldEnableAmount;	//	�ϲֿ�������	HsQuantity
	int32_t		OldCurrentAmount;	//	�ϲֲֳ�����	HsQuantity
	int32_t		TodayEnableAmount;//	���տ�������	HsQuantity
	int32_t		TodayCurrentAmount;//	���ճֲ�����	HsQuantity
	T_UFT_FtdcMoneyType	OldOpenBalance;	//	�ϲֲֳֳɽ���	HsAmount
	T_UFT_FtdcMoneyType	TodayOpenBalance;	//	��ֲֳֳɽ���	HsAmount
	T_UFT_FtdcMoneyType	HoldMargin;		//	�ֱֲ�֤��	HsAmount
	char		StockCode[8];		//��Ʊ���룬��\0�������������Կո���
	int64_t		EnableAmount;		//	��������	HsQuantity 4
	int64_t		CurrentAmount;		//	�ֲ�����	HsQuantity 4
} CUftAnsQueryPositionField;





// ����Ӧ��280�ֽ�
typedef struct tagUftRspErrorField
{
	int32_t			ErrorNo;
	uint32_t		OrderIndex;			// �������ر�ʶ
	int64_t			OrderRef;			//��������ʶ
	char			ErrorMsg[256];
	uint32_t		SendID;
} CUftRspErrorField;



//��Լ״̬
typedef struct tagUftRtnInstrumentStatusField
{
	///���������
	T_UFT_FtdcSettlementGroupIDType	SettlementGroupID;
	///��Լ����
	T_UFT_FtdcInstrumentIDType	InstrumentID;
	///��Լ����״̬
	T_UFT_FtdcInstrumentStatusType	Status;
	///���׽׶α��
	T_UFT_FtdcTradingSegmentSNType	TradingSegmentSN;
	///���뱾״̬ʱ��
	T_UFT_FtdcTimeType	EnterTime;
	///���뱾״̬ԭ��
	T_UFT_FtdcInstStatusEnterReasonType	EnterReason;
	///���뱾״̬����

	//T_UFT_FtdcDateType	EnterDate;
    //֤ȯNST
	uint16_t					PlatformID;
	uint16_t					PlatformState;
}CUftRtnInstrumentStatusField;



typedef struct tagUftRspTest
{
	int32_t TimeDelay;	
}CUftRspTest;

typedef struct tagUftRspUserLoginField
{													   
	T_UFT_ORDERREF_TYPE MaxOrderRef;
	T_UFT_TRADEDATE_TYPE TradeDate;
}CUftRspUserLoginField;

typedef struct tagUftRspInfoField
{
	///�������
	T_UFT_ErrorIDType	ErrorID;
	///������Ϣ
	T_UFT_ErrorMsgType	ErrorMsg;
}CUftRspInfoField;

#endif
