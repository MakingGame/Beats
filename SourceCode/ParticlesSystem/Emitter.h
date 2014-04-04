
/**
*    Copyright (C) 2014, All right reserved
*    created:    2014/03/24
*    created:    24:3:2014   10:48
*    filename:     EMITTER.H
*    file base:    Emitter
*    file ext:    h
*    author:        GJ
*    
*    history:    
**/

#ifndef PARTICLE_EMITTER_H_INCLUDE
#define PARTICLE_EMITTER_H_INCLUDE

#include "Particle.h"
#include "Render/ShaderProgram.h"
#include "Render/Material.h"
#include "BallParticleEmitterEntity.h"
#include "PointParticleEmitterEntity.h"
#include "BoxParticleEmitterEntity.h"
#include "ConeParticleEmitterEntity.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentBase.h"
#include "ParticleProperty.h"

namespace FCEngine
{

    struct SEmitter
    {
        bool m_Constraint;//�Ƿ������Լ�����Լ�������ϵ��
        bool m_IsLoop;//�Ƿ�ѭ������
        unsigned int m_TotalCount;//��������
        unsigned int m_ShootVelocity;//�����ٶȣ�ÿһ�뷢���������
        float m_MinParticleVelocity;//������С��ʼ�ٶ�
        float m_MaxParticleVelocity;//��������ʼ�ٶ�
        float m_MinVelocityDecay;//�����ٶȵ���С˥��ϵ��
        float m_MaxVelocityDecay;//�����ٶȵ����˥��ϵ��
        float m_ForcePower;//�����ܵ���������С
        float m_MinLiveTime;//���ӵ���С����ʱ��(s)
        float m_MaxLiveTime;//���ӵ��������ʱ��(s)
        float m_DelayTime;//�ӳٷ���ʱ��(s)
        CMaterial* m_pMaterial;
        kmVec3 m_ShootOrigin;//����ԭ��
        kmVec3 m_ShootDeriction;//���䷽��
        kmVec3 m_ShootDegree;//����Ƕȣ���ΪXYZ��������
        kmVec3 m_ForceDeriction;//�����յ��������������������СΪ0�������Բ�������

        SEmitter( )
        {
            m_TotalCount = 10;
            m_pMaterial = NULL;
            kmVec3Fill( &m_ShootOrigin, 0.0, 0.0, 0.0 );
            kmVec3Fill( &m_ShootDeriction, 0.0, 1.0, 0.0 );
            kmVec3Fill( &m_ShootDegree, 0.0, 0.0, 0.0 );
            m_ShootVelocity = 5;
            m_MinParticleVelocity = 1.0;
            m_MaxParticleVelocity = 3.0;
            m_MinVelocityDecay = 0.0;
            m_MaxVelocityDecay = 0.0;
            m_ForcePower = 0.0;
            kmVec3Fill( &m_ForceDeriction, 0.0, -1.0, 0.0 );
            m_MinLiveTime = 3.0;
            m_MaxLiveTime = 5.0;
            m_DelayTime = 0.0;
            m_Constraint = true;
            m_IsLoop = false;
        }

        ~SEmitter( )
        {

        }
    };

    class ParticleEmitter : public CComponentBase
    {
        friend class ParticleSystem;
        typedef std::list< ParticleEntity* > TParticleList;
        DECLARE_REFLECT_GUID( ParticleEmitter, 0x1544A159, CComponentBase )
    public:
        ParticleEmitter( CSerializer& serializer );
        ~ParticleEmitter( );

    private:
        ParticleEmitter( );

        /*
        * Method:    beginShoot
        
        * Access:    private 
        * Returns:   void
        * Remark:     ���������ʼ��������,һ����øú���ʱ�����е�����ֵ�Ѿ��������
        //*/
        void BeginShoot( );

        /*
        * Method:    update
        
        * Access:    private 
        * Returns:   void
        * Param:     float dtt ��Ⱦÿһ֡��ʱ��
        * Remark:     ��Ⱦ֮ǰ�������з�����
        //*/
        void Update( float dtt );

        void Render( );


        void SetTotalCount( unsigned int count );

        unsigned int GetTotleCount( );


        void SetShootDeriction( const kmVec3& deriction );

        const kmVec3& GetShootDeriction( ) const;


        void SetShootOrigin( const kmVec3& origin );

        const kmVec3& GetShootOrigin( ) const;


        void SetShootDegree( const kmVec3& degree );

        const kmVec3& GetShootDegree( ) const;

        /*
        * Method:    setShootVelocity
        
        * Access:    private 
        * Returns:   void
        * Param:     float velocity
        * Remark:     �������ӵķ����ٶ�
        //*/
        void SetShootVelocity( unsigned int velocity );

        unsigned int GetShootVelocity( );

        /*
        * Method:    setMinParticleVelocity
        
        * Access:    private 
        * Returns:   void
        * Param:     float velocity
        * Remark:     ���÷���������ӵ���С�˶��ٶ�
        //*/
        void SetMinParticleVelocity( float velocity );

        float GetMinParticleVelocity( );


        void SetMaxParticleVelocity( float velocity );

        float GetMaxParticleVelocity( );

        /*
        * Method:    setMinParticleVelocityDecay
        
        * Access:    private 
        * Returns:   void
        * Param:     float decay
        * Remark:     ����������С˥����
        //*/
        void SetMinParticleVelocityDecay( float decay );

        float GetMinParticleVelocityDecay( );


        void SetMaxParticleVelocityDecay( float decay );

        float GetMaxParticleVelocityDecay();


        void SetForcePower( float power );

        float GetForcePower( );


        void SetFroceDeriction( const kmVec3& deriction );

        const kmVec3& GetFroceDeriction( ) const;


        void SetMinLiveTime( float time );

        float GetMinLiveTime( );


        void SetMaxLiveTime( float time );

        float GetMaxLiveTime( );


        void SetDelayTime( float time );

        float GetDelayTime( );


        /*
        * Method:    setParticleConstraint
        
        * Access:    private 
        * Returns:   void
        * Param:     bool bConstraint true Լ���ڷ���������ϵ�ڣ� false ��������������ϵ��
        * Remark:     ���������Ƿ�Լ���ڷ�����������ϵ�ڲ�
        //*/
        void SetParticleConstraint( bool bConstraint );

        bool GetParticleConstraint( );

        /*
        * Method:    setEmitterAttribute
        
        * Access:    private 
        * Returns:   void
        * Param:     const Emitter & pEmitter
        * Remark:    ����һ�������������Եĺ��������ڴ��ļ�ʱȫ�����Ը�ֵ
        //*/
        void SetEmitterAttribute( const SEmitter& pEmitter );

        const SEmitter& GetEmitterAttribute( ) const;


        void SetPlayLoop( bool bLoop );

        bool GetPlayLoop( );

    private:
        /*
        * Method:    shootParicle
        
        * Access:    private 
        * Returns:   void
        * Param:	 unsigned int count ����������
        * Remark:	 һ�η���������
        //*/
        void ShootParicle( unsigned int count );

        /*
        * Method:    setParticleInitProperty
        
        * Access:    private 
        * Returns:   void
        * Param:     ParticleEntity * pParticle
        * Remark:    ����һ���������ӵĳ�ʼ����
        //*/
        void SetParticleInitProperty( ParticleEntity** pParticleEntity );

        /*
        * Method:    GetParticleFromRecycle
        
        * Access:    private 
        * Returns:   ParticleEntity*
        * Remark:     ���Ѿ����յ����������¼���һ������
        //*/
        ParticleEntity* GetParticleFromRecycle( ); 

        /*
        * Method:    updateActiveParticles
        
        * Access:    private 
        * Returns:   void
        * Param:     unsigned int dtt
        * Remark:    ���������Ѿ������������
        //*/
        void UpdateActiveParticles( float dtt );

        void InitVAOAndVBO( int count );

    private:

        SEmitter* m_pEmitter;
        CVertexPTC* m_VertData;
        unsigned short* m_IndexBuffer;
        TParticleList m_AliveParticleList;//������б�
        TParticleList m_ParticleRecycle;//���������б�

        GLuint m_VAO;
        GLuint m_VBO[ 2 ];

        unsigned int m_CurrentCount;//��ǰ�Ѿ���������Ӹ���
        float m_BeginTime;

        float m_MinPreShootTime;//����һ�����ӵ���С����ʱ��
        float m_DeltaTime;//��һ֡�������ӵ�ʱ���

        bool m_bInit;

        CShaderProgram* m_pParticleProgram;
        CMaterial* m_pMaterial;//���ò���

        ParticleEmitterEntityBase* m_pEmitterBase;
        ParticleProperty* m_pParticleProperty;
    };
};

#endif // Emitter_H_INCLUDE
