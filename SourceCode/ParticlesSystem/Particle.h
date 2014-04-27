
/**
*    Copyright (C) 2014, All right reserved
*    created:    2014/03/24
*    created:    24:3:2014   10:47
*    filename:     PARTICLE.H
*    file base:    Particle
*    file ext:    h
*    author:        GJ
*    
*    history:    
**/

#ifndef PARTICLE_PARTICLE_H_INCLUDE
#define PARTICLE_PARTICLE_H_INCLUDE

#include "ColorParticleAnimation.h"
#include "RotateParticleAnimation.h"
#include "ScaleParticleAnimation.h"
#include "ParticleProperty.h"

namespace FCEngine
{

    struct SParticle
    {
        float m_Velocity;//�˶��ٶ�(s)
        float m_VelocityDecay;//�ٶ�˥��ϵ��
        float m_ForcePower;//����������С
        float m_LiveTime;//��������ʱ�䣬����Ϊ��λ
        kmVec3 m_ForceDirection;//������������
        kmVec3 m_BirthPosition;//����λ��
        kmVec3 m_BirthDirection;//���䷽��

        SParticle( )
        {
            kmVec3Fill( &m_BirthPosition, 0.0, 0.0, 0.0 );
            kmVec3Fill( &m_BirthDirection, 0.0, 1.0, 0.0 );
            m_Velocity = 1;
            m_VelocityDecay = 0;
            m_ForcePower = 0;
            kmVec3Fill( &m_ForceDirection, 0.0, -1.0, 0.0 );
            m_LiveTime = 1000;
        }
        ~SParticle( )
        {

        }
    };

    class ParticleEntity
    {
        friend class ParticleEmitter;
    private:
        ParticleEntity( );

        /*
        * Method:    update
        
        * Access:    public 
        * Returns:   void
        * Param:     float dtt��ǰ֡��Ⱦʱ��
        * Remark:    ����ʱ�����µ�ǰ���ӵ�λ�á��������Ϣ
        //*/
        void Update( float dtt );

        /*
        * Method:    computeVertices
        
        * Access:    public 
        * Returns:   void
        * Param:     CVertexPTC * vertices Emitter�Ķ�����Ϣ��ʼָ��
        * Param:     OUT unsigned int& idx ��ǰ�Ѿ���ĵ�ĸ���,���ظ���һ������
        * Param:     unsigned short* indexBuffer ���������ʼָ��
        * Remark:    ÿһ��������emitter�Ķ���buffer��ռ��һ�οռ䣬
        �ú�������ָ�������Լ����������ӿռ��
        //*/
        void ComputeVertices(CVertexPTC** vertices, unsigned short** indexBuffer, OUT unsigned int& idx );

        void SetBirthPosition( const kmVec3& birthPosition );

        void SetBirthDirection( const kmVec3& direction );

        void SetVelocity( float velocity );

        void SetVelocityDecay( float velocityDecay );

        void SetForcePower( float power );

        void SetForceDirection( const kmVec3& direction );

        void SetLiveTime( float time );


        /*
        * Method:    isLive
        
        * Access:    private 
        * Returns:   bool true = �� false = ����
        * Remark:	 ���ص�ǰ���ӵĴ��״̬
        //*/
        bool IsLive( );

        /*
        * Method:    activate
        
        * Access:    private 
        * Returns:   void
        * Remark:    ���¼�����������
        //*/
        void Activate( );

        void SetProperty( ParticleProperty* pProperty );

    public:
        ~ParticleEntity( );

    private:
        bool m_IsAlive;//���״̬
        kmVec3 m_CurrentPosition;//��ǰ������λ��
        float m_CurrentLiveTime;//��ǰ�Ѿ����ʱ��
        SParticle* m_pParticle;
        float m_CurrentVelocity;//��ǰ�����˶��ٶ�
        kmMat4 m_RotateMat4;//��ת����
        ParticleProperty* m_pParticleProperty;
    };
};

#endif // Particle_H_INCLUDE
