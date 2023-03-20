#pragma once
#pragma pack(1)
#include "grid.h"

//======================================================================
// Ŭ���̾�Ʈ �α� ������ ����
//======================================================================
//**#define	TR_LOGIO	"pihologm"
#define	TR_LOGIO	"pidologm"

/* ========== �Է� ========== */
struct  _limid	{
	char    func[1]{};			// 'I':�Է�, 'Q':��ȸ
	char    mapx[8]{};			// main map name
	char	kind[1]{};			// ����define kind
	char    data[1]{};			// log message (MAX 1024)
};  
#define	sz_limid	sizeof(struct _limid)

struct	_limod {
	char    retc[1]{};			// return code 'S': OK, 'E': ERR
};
#define	sz_limod	sizeof(struct _limod)

/* ========== ��ȸ ========== */
struct _lsmid	{
	char    func[1]{};			// '1':�Է�, '2':��ȸ
	char    mapx[8]{};			// main MAP(DLL) name
	char	kind[1]{};			// ����define kind
	struct	_gridHi	gridHi{};
};
#define	sz_lsmid	sizeof(struct _lsmid)

struct  _lsgrid {
	char    logm[512]{};
};
#define	sz_lsgrid	sizeof(struct _lsgrid)

struct	_lsmod	{
	char    nrec[4]{};
	struct  _lsgrid		lsgrid[1]{};
};
#define	sz_lsmod	sizeof(struct _lsmod)
#pragma pack()