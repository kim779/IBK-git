//
//// JSJ_ITCastle_New_040503
//// 10�ܰ� ȣ����ȸ
//
//typedef struct _D49890_I {
//	char shcode[6];             /* �����ڵ�M */
//} D49890_I;
//
//typedef struct _D49890_T {
//	char hname[20];              /* �����(�ѱ�)M  */
//	char price[9];               /* ���簡9(9),��  */
//	char sign[1];                /* �����ȣ  */
//	char change[9];              /* �����9(6)  */
//	char chgrate[10];            /* �����999.99  */
//	char volume[10];             /* �����ŷ���9(9),��  */
//	char totprice[12];           /* �����ŷ����(12),õ��M  */
////	char htime[6];               /* ȣ���ܷ����ؽð�HHMMSS  */
//
//	char offerho1[9];            /* �ŵ��ֿ켱ȣ��9(7)  */
//	char offerho2[9];            /* �ŵ�2����ȣ��9(7)  */
//	char offerho3[9];            /* �ŵ�3����ȣ��9(7)  */
//	char offerho4[9];            /* �ŵ�4����ȣ��9(7)  */
//	char offerho5[9];            /* �ŵ�5����ȣ��9(7)  */
//	//char offerho6[9];            /* �ŵ�6����ȣ��9(7)  */
//	//char offerho7[9];            /* �ŵ�7����ȣ��9(7)  */
//	//char offerho8[9];            /* �ŵ�8����ȣ��9(7)  */
//	//char offerho9[9];            /* �ŵ�9����ȣ��9(7)  */
//	//char offerho10[9];           /* �ŵ�10����ȣ��9(7)  */
//	char offerrem1[9];           /* �ŵ��ֿ켱�ܷ�9(8),��  */
//	char offerrem2[9];           /* �ŵ�2�����ܷ�9(8),��  */
//	char offerrem3[9];           /* �ŵ�3�����ܷ�9(8),��  */
//	char offerrem4[9];           /* �ŵ�4�����ܷ�9(8),��  */
//	char offerrem5[9];           /* �ŵ�5�����ܷ�9(8),��  */
//	//char offerrem6[9];           /* �ŵ�6�켱�ܷ�9(8),��  */
//	//char offerrem7[9];           /* �ŵ�7�����ܷ�9(8),��  */
//	//char offerrem8[9];           /* �ŵ�8�����ܷ�9(8),��  */
//	//char offerrem9[9];           /* �ŵ�9�����ܷ�9(8),��  */
//	//char offerrem10[9];          /* �ŵ�10�����ܷ�9(8),��  */
//
//	char bidho1[9];              /* �ż��ֿ켱ȣ��9(7)  */
//	char bidho2[9];              /* �ż�2����ȣ��9(7)  */
//	char bidho3[9];              /* �ż�3����ȣ��9(7)  */
//	char bidho4[9];              /* �ż�4����ȣ��9(7)  */
//	char bidho5[9];              /* �ż�5����ȣ��9(7)  */
//	//char bidho6[9];              /* �ż�6�켱ȣ��9(7)  */
//	//char bidho7[9];              /* �ż�7����ȣ��9(7)  */
//	//char bidho8[9];              /* �ż�8����ȣ��9(7)  */
//	//char bidho9[9];              /* �ż�9����ȣ��9(7)  */
//	//char bidho10[9];             /* �ż�10����ȣ��9(7)  */
//	char bidrem1[9];             /* �ż��ֿ켱�ܷ�9(8),��  */
//	char bidrem2[9];             /* �ż�2�����ܷ�9(8),��  */
//	char bidrem3[9];             /* �ż�3�����ܷ�9(8),��  */
//	char bidrem4[9];             /* �ż�4�����ܷ�9(8),��  */
//	char bidrem5[9];             /* �ż�5�����ܷ�9(8),��  */
//	//char bidrem6[9];             /* �ż�6�켱�ܷ�9(8),��  */
//	//char bidrem7[9];             /* �ż�7�����ܷ�9(8),��  */
//	//char bidrem8[9];             /* �ż�8�����ܷ�9(8),��  */
//	//char bidrem9[9];             /* �ż�9�����ܷ�9(8),��  */
//	//char bidrem10[9];            /* �ż�10�����ܷ�9(8),��  */
//
//	char offertotrem[9];         /* �Ѹŵ��ܷ�9(8),��  */
//	char bidtotrem[9];           /* �Ѹż��ܷ�9(8),��  */
//	char offerrem_ot[9];         /* �ð��ܸŵ��ܷ�9(8),��  */
//	char bidrem_ot[9];           /* �ð��ܸż��ܷ�9(8),��  */
//	char hprice[9];              /* ���Ѱ�9(9),��M  */
//	char lprice[9];              /* ���Ѱ�9(9),��M  */
//	char open[9];                /* �ð�9(7),��  */
//	char high[9];                /* ��9(7),��  */
//	char low[9];                 /* ����9(7),��  */
//
//	char prevol[10];             /* ���ϰŷ���9(9),��  */
//	char listing[10];            /* ����(Kospi)����(Kosdaq)�ֽļ�9listing  */
//} D49890_T;
//
//typedef struct _D49890_O
//{
//	char	jmcode			[ 6];		/* �����ڵ�			*/
//	char	baseprice		[ 9];		/* ���ذ�  			*/
//	char	expprice		[ 9];		/*  ���� ü�ᰡ�� */
//	char	expvol			[12];		/*  ���� ü����� */
//
//	D49890_T	table;		
//} D49890_O;

typedef struct _EU_s0001_OutRec1 {
    char    shcode[6];                            /* �����ڵ�(6)                     */
    char    recprice[8];                          /* ���ذ�(8)                       */
    char    yeprice[8];                           /* ����ü�ᰡ��(8)                 */
    char    yevolume[8];                          /* ����ü�����(8)                 */
    char    hname[40];                            /* �����(40)                      */
    char    price[8];                             /* ���簡(8)                       */
    char    sign[1];                              /* ���簡���ϴ�񱸺�(1)           */
    char    change[8];                            /* ���簡���ϴ��(8)               */
    char    rate[7];                              /* ���簡���ϴ������(7)         */
    char    volume[12];                           /* �ŷ���(12)                      */
    char    value[15];                            /* �ŷ����(15)                    */
    char    offerho1[8];                          /* �ŵ�ȣ��1(8)                    */
    char    offerho2[8];                          /* �ŵ�ȣ��2(8)                    */
    char    offerho3[8];                          /* �ŵ�ȣ��3(8)                    */
    char    offerho4[8];                          /* �ŵ�ȣ��4(8)                    */
    char    offerho5[8];                          /* �ŵ�ȣ��5(8)                    */
    char    offerrem1[8];                         /* �ŵ�ȣ������1(8)                */
    char    offerrem2[8];                         /* �ŵ�ȣ������2(8)                */
    char    offerrem3[8];                         /* �ŵ�ȣ������3(8)                */
    char    offerrem4[8];                         /* �ŵ�ȣ������4(8)                */
    char    offerrem5[8];                         /* �ŵ�ȣ������5(8)                */
    char    bidho1[8];                            /* �ż�ȣ��1(8)                    */
    char    bidho2[8];                            /* �ż�ȣ��2(8)                    */
    char    bidho3[8];                            /* �ż�ȣ��3(8)                    */
    char    bidho4[8];                            /* �ż�ȣ��4(8)                    */
    char    bidho5[8];                            /* �ż�ȣ��5(8)                    */
    char    bidrem1[8];                           /* �ż�ȣ������1(8)                */
    char    bidrem2[8];                           /* �ż�ȣ������2(8)                */
    char    bidrem3[8];                           /* �ż�ȣ������3(8)                */
    char    bidrem4[8];                           /* �ż�ȣ������4(8)                */
    char    bidrem5[8];                           /* �ż�ȣ������5(8)                */
    char    totofferrem[8];                       /* �ŵ�ȣ���Ѽ���(8)               */
    char    totbidrem[8];                         /* �ż�ȣ���Ѽ���(8)               */
    char    tmofferrem[8];                        /* �ð��ܸŵ��ܷ�(8)               */
    char    tmbidrem[8];                          /* �ð��ܸż��ܷ�(8)               */
    char    uplmtprice[8];                        /* ���Ѱ�(8)                       */
    char    dnlmtprice[8];                        /* ���Ѱ�(8)                       */
    char    preychange[8];                        /* ����ü�ᰡ�������簡���(8)     */
    char    high[8];                              /* ��(8)                         */
    char    low[8];                               /* ����(8)                         */
    char    jnilvolume[12];                       /* ���ϰŷ���(12)                  */
    char    listing[12];                          /* �����ֽļ�(12)                  */
} EU_s0001_OutRec1;
