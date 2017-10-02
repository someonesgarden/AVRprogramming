//---------------------------------------------------------------
// �X�C�b�`�p�֐�
#include <avr/io.h>

#define SW_PORT  PORTC		// �v���A�b�v�p�|�[�g
#define SW_DDR   DDRC       // �|�[�g���͐ݒ�
#define SW_PIN   PINC       // ���̓|�[�g

//---------------------------------------------------------------
// �X�C�b�`������
//   ����:����������|�[�g�̃}�X�N
void sw_init(unsigned char bitmask)
{
 	SW_DDR  &= ~bitmask; 	//input
	SW_PORT |= bitmask; 	//pullup
}


//---------------------------------------------------------------
// �X�C�b�`�ǂݎ��
//   ����:���o����|�[�g�̃}�X�N
//   �߂�l�F0=�X�C�b�`�I�t / 1=�X�C�b�`�I��
char sw_get(unsigned char bitmask)
{
	if(SW_PIN & bitmask)
		return(0);  // �X�C�b�`������ĂȂ�
	else
		return(1);  // �X�C�b�`������Ă���
}

