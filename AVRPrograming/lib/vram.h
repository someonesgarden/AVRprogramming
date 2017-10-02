//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
#define VRAMWIDTH  128     // VRAM�h�b�g��
#define VRAMHEIGHT 48   // VRAM�h�b�g����
#define VRAMXMIN  0
#define VRAMXMAX  (VRAMWIDTH-1)
#define VRAMYMIN  0
#define VRAMYMAX  (VRAMHEIGHT-1)
#define VRAMSIZE   ((VRAMWIDTH/8)*VRAMHEIGHT)
#define VRSPSIZE 7

extern unsigned char vram[VRAMSIZE];	 // Bitmap Buffer

//--------------------------------------------------------------------------------
// VRAM������
// �EVRAM�̓��e�������܂��B
void vram_cls(void);


//--------------------------------------------------------------------------------
//�s�N�Z���`��
//����x�FX���W
//����x�FY���W
//����color�F�J���[�R�[�h�B0=�����A1=�_���A2=XOR1
//�g�p��Fvram_pset(5,8,1);   // ���W(5,8)�ɓ_��`���܂��B
void vram_pset(int x,int y,char color);

//--------------------------------------------------------------------------------
//���C���`��
//����x1�FX1���W
//����y1�FY1���W
//����x2�FX2���W
//����y2�FY2���W
//����color�F�J���[�R�[�h�B0=�����A1=�_���A2=XOR1
void vram_line(int x1 ,int y1 ,int x2 ,int y2 ,char color);

//--------------------------------------------------------------------------------
// ��ʃX�N���[��
// �EVRAM�̒��g�������I�ɃV�t�g���܂��B
// ����x1: X�����ړ��ʁB
// ����y1: Y�����ړ��ʁB
void vram_scroll(char x1,char y1);
void vram_locate(unsigned char x,unsigned char y);
char vram_point(int x,int y);
void vram_putch(char ch);
void vram_putdec(unsigned int x);
void vram_puthex(unsigned char x);
void vram_puthexw(unsigned int x);
void vram_putstr(char *p);
void vram_putstrpgm(PGM_P p);
void vram_spput(char x,char y, PGM_P p);
void vram_spclr(char x,char y);
void vram_bmpput(char x,char y,char w,char h, PGM_P p);
int fnc_abs(int a);
int fnc_sgn(int a);
char vram_gettate(void);
void vram_settate(char tate);
unsigned char vram_getw(void);
unsigned char vram_geth(void);
