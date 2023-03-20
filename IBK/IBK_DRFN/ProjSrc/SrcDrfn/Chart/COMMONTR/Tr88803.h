#if !defined (_HEADERFILE_TR_88803_H)
#define _HEADERFILE_TR_88803_H

// #include "../CommonTR/Tr88803.h"
//#include "../../_Include/DefineCode.h"

//-------------------------------------------------------------------------------
//                    ���TR��ȣ:88803                                           ##
//--------------------------------[INPUT ]---------------------------------------
//typedef struct
//{
//    char gb         [ 1];        /* '1':�ֽ�, '2':�ڽ���    */
//	char code       [ 6];        /* �����ڵ�                */
//} TR_CODEINFO;
//#define  TR_CODEINFO_SIZE sizeof(TR_CODEINFO)
// DefineCode.h���� TR_CODEINFO ����

#define	 SZKEY_CODEREQUEST	"A9999B0000"
#define  SZKEY_TRNO			"88803"

typedef struct _tr88803_i
{
    char  reserved  [ 8];       /* ������Ͻ� �ǵ����ִ� �� */
	char  mainkey	[ 5];       /* �з�Ű   A�ν���         */
	                            // A9999 B0000 �϶��� ���񸮽�Ʈ�� A,BŸ���� ������ ���� �� �ִ�.
	char  subkey    [ 5];       /* �з�-����з�Ű, B�ν��� */
	char  sort      [ 1];       /* 0 : code�� 
	                               1 : codename��           */
	char  datatype  [ 1];       /* ��û�ϴ� ������ Ÿ��     */
	                            /* A : TR_88803_A
								   B : TR_88803_B           */
	// �Ʒ��� A9999 B0000 �϶��� ����. �׿ܿ��� �����̽�.
	char  count     [ 4];       /* ������ ī��Ʈ            */
	char  data      [ 1];       /* ī��Ʈ��ũ TR_CODEINFO������ �ݺ�]   */
} TR_88803_I;
#define  TR_88803_I_SIZE sizeof(TR_88803_I)
#define  MakeSpaceTR_88803_I(xIn)	memset(&xIn, 0x20, TR_88803_I_SIZE)

//--------------------------------[OUTPUT ]---------------------------------------

typedef struct _tr_88803_A
{
    char gb         [ 1];               /* '1':�ֽ�, '2':�ڽ���    */
	char code       [ 6];				/* �����ڵ�			*/
	char hname      [20];				/* �����(�ѱ�)M	*/
} TR_88803_A;
#define  TR_88803_A_SIZE sizeof(TR_88803_A)


//typedef struct _tr_88803_B
//{
//    char gb         [ 1];               /* '1':�ֽ�, '2':�ڽ���    */
//	char code       [ 6];				/* �����ڵ�			*/
//	char hname      [20];				/* �����(�ѱ�)M	*/
//    char time       [ 6];				/* �ð�(HH:MM:SS)	*/
//    char sign       [ 1];				/* �����ȣ			*/
//    char change     [ 9];				/* �����			*/
//    char price      [ 9];				/* ���簡			*/
//    char chrate     [ 5];				/* �����			*/
//	char volume     [12];               /* �����ŷ���       */
//	char value      [12];               /* �ŷ����         */
//} TR_88803_B;
//#define  TR_88803_B_SIZE sizeof(TR_88803_B)

typedef struct _tr_88803_B
{
	char gb         [ 1];               /* '1':�ֽ�, '2':�ڽ���    */
	char code       [ 6];				/* �����ڵ�			*/
	char hname      [20];				/* �����(�ѱ�)M	*/
	char price      [10];				/* ���簡			*/
	char sign       [ 1];				/* �����ȣ			*/
	char change     [10];				/* �����			*/
	char chrate     [ 6];				/* �����			*/
	//char volume     [12];               /* �����ŷ���       */
	//char value      [12];               /* �ŷ����         */
} TR_88803_B;
#define  TR_88803_B_SIZE sizeof(TR_88803_B)

// TR_88803_C�� �������� Ÿ�� ojtaso
typedef struct _tr_88803_C
{
    char under           [ 1];   /* ���������� :  0:����  1:����  2:���� */
    char jg_tp           [ 1];   /* ����                                 */
    char jg_code         [15];   /* �����ڵ�                             */
    char jg_name         [30];   /* �����                               */
    char price           [ 9];   /* ���簡                               */
    char change          [ 6];   /* ���ϴ��                             */
    char change_rate     [ 5];   /* �����                               */
    char sign            [ 1];   /* �����ȣ                             */
    char volume          [12];   /* �����ŷ���                           */
    char offerho         [ 9];   /* �ŵ�ȣ��                             */
    char bidho           [ 9];   /* �ż�ȣ��                             */
    char ofremain        [12];   /* �ŵ��ܷ�                             */
    char bidremain       [12];   /* �ż��ܷ�                             */
    char offertot        [12];   /* �Ѹŵ��ܷ�                           */
    char bidtot          [12];   /* �Ѹż��ܷ�                           */
    char openam          [ 9];   /* �ð�                                 */
    char high            [ 9];   /* ��                                 */
    char low             [ 9];   /* ����                                 */
    char value           [14];   /* �ŷ��ݾ�                             */
    char uplmtprice      [ 9];   /* ���Ѱ�                               */
    char dnlmtprice      [ 9];   /* ���Ѱ�                               */
    char openyak         [ 9];   /* �̰�����������                       */
    char parvalue        [ 9];   /* �׸鰡                               */
    char presign         [ 1];   /* ���ϵ����ȣ                         */
    char updnlmtcont_tp  [ 1];   /* ���ӱ���                             */
    char updnlmtcont     [ 2];   /* ���ӱ���                             */
    char recprice        [ 9];   /* ���ذ�                               */
    char cvolume         [ 9];   /* ü�ᷮ                               */
    char open_chrate     [ 5];   /* �ð����ϴ����                       */
    char high_chrate     [ 5];   /* �����ϴ����                       */
    char low_chrate      [ 5];   /* �������ϴ����                       */
    char hightime        [ 6];   /* ���ð�                             */
    char lowtime         [ 6];   /* �����ð�                             */
    char expsign         [ 1];   /* ����ü�� ��ȣ                        */
    char expprice        [ 9];   /* ����ü�� ��                          */
    char expchang        [ 9];   /* ����ü�� ���                        */
    char expchgrt        [ 9];   /* ����ü�� �����                      */
    char expvol          [12];   /* ����ü�� �ŷ���                      */
    char color           [ 1];   /* 1:Blue 2:Yellow 3:Green 4:Red        */
} TR_88803_C;
#define  TR_88803_C_SIZE sizeof(TR_88803_C)

typedef struct _tr_88803_o
{
    char  reserved  [ 8];       /* ������Ͻ� �ǵ����ִ� �� */
	char  count     [ 4];       /* ������ ī��Ʈ            */
    // ������Ÿ�Կ� ���� ����Ÿ�� ���⿡ �ٿ��� ������.
	char  data      [ 1];		/*  ����Ÿ�κ�              */
} TR_88803_O;

#endif // _HEADERFILE_TR_88803_H
