#if !defined(AFX_FF7DE5F5_27EC_4edf_9D00_090C502CE25B__INCLUDED_)
#define AFX_FF7DE5F5_27EC_4edf_9D00_090C502CE25B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//DrMapTrDef.h

#pragma	pack(1)

#define  KEY_SIZE 23
struct mid
{
	char    trxc[8];            /* ���񽺸�                 */
	char    usr1[12];           /* ���� �Է�1               */
	char    usr2[12];           /* ���� �Է�2               */
	char    usr3[12];           /* ���� �Է�3               */

	char	cod2[100];
};
#define	SZ_HEADSIZE_MID		(sizeof(mid)-100)
//
// 	struct grid
// 	{
// 		char    date[ 8];           /* ��¥                     */
// 		char    siga[10];           /* �ð�                     */
// 		char    koga[10];           /* ��                     */
// 		char    jega[10];           /* ����                     */
// 		char    jgga[10];           /* ����                     */
// 		char    gvol[10];           /* �ŷ���                   */
// 		char    gamt[18];           /* �ŷ����                 */
// 		char    jchk[ 1];           /* �Ǹ���                   */
// 	};

struct mod 
{
	char    trxc[8];            /* ���񽺸�                 */
	char    usr1[12];           /* ���� �Է�1               */
	char    usr2[12];           /* ���� �Է�2               */
	char    usr3[12];           /* ���� �Է�3               */
	
// 	char    cod2[32];           /* ���� �ڵ�                */    
// 	char    siga[12];           /* �ð�                     */
// 	char    koga[12];           /* ��                     */
// 	char    jega[12];           /* ����                     */
// 	char    jgga[12];           /* ����                     */
// 	char    gvol[ 9];           /* �ŷ���                   */
// 	char    msga[12];           /* �ż�ȣ��                 */
// 	char    mdga[12];           /* �ŵ�ȣ��                 */
// 	char    shga[12];           /* ���Ѱ�                   */
// 	char    hhga[12];           /* ���Ѱ�                   */
// 	char    jsga[12];           /* ���� �ð�                */
// 	char    jhga[12];           /* ���� ��                */
// 	char    jjga[12];           /* ���� ����                */
// 	char    jcur[12];           /* ���� ����                */
// 	char    jvol[ 9];           /* ���� �ŷ���              */
// 	char    diff[10];           /* ���� ���                */
// 	char    rate[ 8];           /* �����                   */
// 	char    gjga[12];           /* ���ذ�                   */
// 	char    rtim1[6];           /* ����۽ð�               */
// 	char    rtim2[6];           /* �帶���ð�               */
// 	char    bday[ 8];           /* ������                   */
// 	char    giho[ 1];           /* ���ϴ���ȣ             */
// 	char    nkey[KEY_SIZE];     /* Next Key                 */
// 	char    gapk[ 9];           /* ������                   */
// 	char    fill[ 3];           /* �ι���üũ               */
// 	char    nrec[ 4];           /* �ݺ��Ǽ�                 */
// 	struct  grid    grid[1];
};

// 	typedef struct
// 	{
// 		char cBozoSize[6];	//�����޼��� ������ 
// 		char Bozo;			//����޼��� Name=value; 
// 		char nCount[4];		//��Ʈ������
// 		struct  grid    grid[1];
// 	}TEST_HD;

#pragma	pack()

#endif // !defined(AFX_FF7DE5F5_27EC_4edf_9D00_090C502CE25B__INCLUDED_)