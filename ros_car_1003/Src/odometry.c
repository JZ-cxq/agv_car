 #include "odometry.h"
#include "stm32f1xx_hal.h"
/***********************************************  ���  *****************************************************************/

float position_x=0,position_y=0,oriention=0,velocity_linear=0,velocity_angular=0;

/***********************************************  ����  *****************************************************************/


extern float odometry_right,odometry_left;//���ڵõ����������ٶ�

/***********************************************  ����  *****************************************************************/

float wheel_interval= 455.0;//    ���
float multiplier=0.5f;           //��Ƶ4
float deceleration_ratio=30.0f;  //���ٱ�30
float wheel_diameter=310.0f;     //����ֱ������λmm
float pi_1_2=1.570796f;			 //��/2
float pi=3.141593f;              //��
float pi_3_2=4.712389f;			 //��*3/2
float pi_2_1=6.283186f;			 //��*2
float dt=0.005f;                 //����ʱ����5ms
float line_number=1024.0f;       //��������
float oriention_interval=0;  //dtʱ���ڷ���仯ֵ

float sin_=0;        //�Ƕȼ���ֵ
float cos_=0;

float delta_distance=0,delta_oriention=0;   //����ʱ�������˶��ľ���

float const_frame=0,const_angle=0,distance_sum=0,distance_diff=0;

float oriention_1=0;

uint8_t once=1;

/****************************************************************************************************************/

//��̼Ƽ��㺯��
void odometry(float right,float left)
{	
	if(once)  //����������һ��
	{
		const_frame=wheel_diameter*pi/(line_number*multiplier*deceleration_ratio);
		const_angle=const_frame/wheel_interval;
		once=0;
	}
    
	distance_sum = 0.5f*(right+left);//�ں̵ܶ�ʱ���ڣ�С����ʻ��·��Ϊ�����ٶȺ�
	distance_diff = right-left;//�ں̵ܶ�ʱ���ڣ�С����ʻ�ĽǶ�Ϊ�����ٶȲ�

    //���������ֵķ��򣬾�����ʱ���ڣ�С����ʻ��·�̺ͽǶ���������
	if((odometry_right>0)&&(odometry_left>0))            //���Ҿ���
	{
		delta_distance =25*distance_sum;
		delta_oriention = 0*distance_diff;
	}
	else if((odometry_right<0)&&(odometry_left<0))       //���Ҿ���
	{
		delta_distance = -25*distance_sum;
		delta_oriention = 0*distance_diff;
	}
	else if((odometry_right>0)&&(odometry_left<0))       //�����Ҹ�
	{
		delta_distance = 0*distance_diff;//-38
		delta_oriention = -0*distance_sum;		
	}
	else if((odometry_right<0)&&(odometry_left>0))       //������
	{
		delta_distance = 0*distance_diff;
		delta_oriention = 38 *distance_sum;
	}
	else
	{
		delta_distance=0;
		delta_oriention=0;
	}
    
	oriention_interval = delta_oriention * const_angle;//����ʱ�����ߵĽǶ�	
	oriention = oriention + oriention_interval;//�������̼Ʒ����
	oriention_1 = oriention + 0.5f * oriention_interval;//��̼Ʒ��������λ���仯���������Ǻ�������
	
  sin_ = sin(oriention_1);//���������ʱ����y����
	cos_ = cos(oriention_1);//���������ʱ����x����
	
  position_x = position_x + delta_distance * cos_ * const_frame;//�������̼�x����
	position_y = position_y + delta_distance * sin_ * const_frame;//�������̼�y����
    
	velocity_linear = delta_distance*const_frame / dt;//�������̼����ٶ�
	velocity_angular = oriention_interval / dt;//�������̼ƽ��ٶ�
	
    //����ǽǶȾ���
	if(oriention > pi)
	{
		oriention -= pi_2_1;
	}
	else
	{
		if(oriention < -pi)
		{
			oriention += pi_2_1;
		}
	}
}
