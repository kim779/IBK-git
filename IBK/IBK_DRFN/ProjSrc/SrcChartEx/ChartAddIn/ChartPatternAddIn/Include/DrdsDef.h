
// ********************************
// Drds.cfg �� ������ ����ü ���
// �ǽð����� �������� ������ ���
//*********************************



// DRDS COMMAND "S31" ���簡
typedef struct
{
	char	shcode   		[ 7];			//�����ڵ�              
    char    time     		[ 6];			//�ð�(HH:MM:SS)        
    char    sign     		[ 1];			//�����ȣ              
    char    change   		[ 9];			//�����                
    char    price    		[ 9];			//���簡                
    char    chgrate  		[ 7];			//�����                
    char    open     		[ 9];			//����ð�              
    char    high     		[ 9];			//��                  
    char    low      		[ 9];			//����                  
    char    offerho  		[ 9];			//�ŵ�ȣ��              
    char    bidho    		[ 9];			//�ż�ȣ��              
    char    volume   		[10];			//�����ŷ���            
    char    cvolume  		[10];			//�����ŷ���            
    char    value    		[12];			//�ŷ����              
    char    maegb    		[ 1];			//�Ÿű���              
    char    status   		[ 1];			//�屸��                
    char    cbgb     		[ 1];			//CB����                
    char    stop     		[ 1];			//STOP                  
    char    outvol   		[ 9];			//�ð��ܰŷ���          
    char    outval   		[12];			//�ð��ܰŷ����        
    char    outrepvol		[ 9];			//�ð��ܴ뷮�Ÿ�        
    char    outrepval		[12];			//�ð��ܴ뷮�ŸŴ��    
} STPriceData;		
#define SZ_PriceData		sizeof(STPriceData)


// DRDS COMMAND "S31" �ð� ������(������ 1��)
typedef struct
{
	char    garbage    		[ 3];			//Garbage    
    char    time    		[ 6];			//�ð�                
  
} ST_OneTimeData;		
#define SZ_OneTimeData		sizeof(ST_OneTimeData)


// DRDS COMMAND "S31" ���� ������(������ 1��)
typedef struct
{
	char    garbage    		[ 3];			//Garbage    
    char    volume    		[ 10];			//����
  
} ST_OneVolumeData;		
#define SZ_OneVolumeData	sizeof(ST_OneVolumeData)


// DRDS COMMAND "S31" ���簡 �Ǵ� �ð�, ��, ����(������ 1��)
typedef struct
{
	char    garbage    		[ 3];			//Garbage    
    char    price    		[ 9];			//���簡, �ð�, ��, ����
  
} ST_OnePriceData;		
#define SZ_OnePriceData		sizeof(ST_OnePriceData)