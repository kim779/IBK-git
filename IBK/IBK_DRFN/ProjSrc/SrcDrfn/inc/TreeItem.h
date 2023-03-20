#if !defined(AFX__TREEITEM_LIST__HEADER__)
#define  AFX__TREEITEM_LIST__HEADER__

// #include "../CommonTR/TreeItem.h"

//-------------------------------------------------------------------------------##
//                    ���TR��ȣ:88888                                           ##
//--------------------------------[INPUT ]---------------------------------------##

typedef struct _tr88888_i
{
    char  reserved  [ 8];       /* ������Ͻ� �ǵ����ִ� �� */
	char  mainkey	[ 4];       /* �з�Ű                   */
	char  subkey    [ 3];       /* �з�-����з�Ű          */
	char  sort      [ 1];       /* 0 : code�� 
	                               1 : codename��           */
	char  datatype  [ 1];       /* ��û�ϴ� ������ Ÿ��     */
	                            /* A : TR_88888_A
								   B : TR_88888_B           */
} TR_88888_I;
#define  TR_88888_I_SIZE sizeof(TR_88888_I)

//--------------------------------[OUTPUT ]---------------------------------------##

typedef struct _tr_88888_A
{
	char code       [ 6];				/* �����ڵ�			*/
	char hname      [20];				/* �����(�ѱ�)M	*/
} TR_88888_A;

typedef struct _tr_88888_B
{
	char code       [ 6];				/* �����ڵ�			*/
	char hname      [20];				/* �����(�ѱ�)M	*/
    char time       [ 6];				/* �ð�(HH:MM:SS)	*/
    char sign       [ 1];				/* �����ȣ			*/
    char change     [ 9];				/* �����			*/
    char price      [ 9];				/* ���簡			*/
    char chrate     [ 5];				/* �����			*/
	char volume     [12];               /* �����ŷ���       */
} TR_88888_B;



typedef struct _tr_88888_o
{
    char  reserved  [ 8];       /* ������Ͻ� �ǵ����ִ� �� */
	char  count     [ 4];       /* ������ ī��Ʈ            */
    // ������Ÿ�Կ� ���� ����Ÿ�� ���⿡ �ٿ��� ������.
	//
} TR_88888_O;

#endif //AFX__TREEITEM_LIST__HEADER__
