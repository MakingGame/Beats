#ifndef RESOURCE_RESOURCEPUBLIC_H__INCLUDE
#define RESOURCE_RESOURCEPUBLIC_H__INCLUDE

enum EResourceType
{
    eRT_Texture,
    eRT_Skeleton,
    eRT_Skin,
    eRT_Material,
    eRT_ShaderProgram,
    eRT_SpriteAnimation,

    eRT_Count,
    eRT_Force32Bit = 0xFFFFFFFF
};

static const TCHAR* pszSkeletonBoneName[] = 
{
    _T("eSBT_Null"),
    _T("eSBT_Root"),
    _T("eSBT_Pelvis"),
    _T("eSBT_Spine"),

    _T("eSBT_Spine1"),
    _T("eSBT_Spine2"),
    _T("eSBT_Spine3"),
    _T("eSBT_Neck"),
    _T("eSBT_Head"),
    _T("eSBT_LeftClavicle"),
    _T("eSBT_RightClavicle"),
    _T("eSBT_LeftUpperArm"),
    _T("eSBT_RightUpperArm"),
    _T("eSBT_LeftForearm"),
    _T("eSBT_RightForearm"),
    _T("eSBT_LeftHand"),
    _T("eSBT_RightHand"),
    _T("eSBT_LeftFinger1"),
    _T("eSBT_LeftFinger2"), 
    _T("eSBT_LeftFinger3"), 
    _T("eSBT_LeftFinger4"),
    _T("eSBT_LeftFinger5"),
    _T("eSBT_RightFinger1"),
    _T("eSBT_RightFinger2"), 
    _T("eSBT_RightFinger3"), 
    _T("eSBT_RightFinger4"),
    _T("eSBT_RightFinger5"),

    _T("eSBT_LeftThigh"),
    _T("eSBT_RightThigh"),
    _T("eSBT_LeftCalf"),
    _T("eSBT_RightCalf"),
    _T("eSBT_LeftFoot"),
    _T("eSBT_RightFoot"),
    _T("eSBT_LeftToe1"),
    _T("eSBT_LeftToe2"), 
    _T("eSBT_LeftToe3"), 
    _T("eSBT_LeftToe4"),
    _T("eSBT_LeftToe5"),
    _T("eSBT_RightToe1"),
    _T("eSBT_RightToe2"), 
    _T("eSBT_RightToe3"), 
    _T("eSBT_RightToe4"),
    _T("eSBT_RightToe5"),

    _T("eSBT_LeftFinger11"),
    _T("eSBT_LeftFinger21"), 
    _T("eSBT_LeftFinger31"), 
    _T("eSBT_LeftFinger41"),
    _T("eSBT_LeftFinger51"),
    _T("eSBT_RightFinger11"),
    _T("eSBT_RightFinger21"), 
    _T("eSBT_RightFinger31"), 
    _T("eSBT_RightFinger41"),
    _T("eSBT_RightFinger51"),
    _T("eSBT_LeftFinger12"),
    _T("eSBT_LeftFinger22"), 
    _T("eSBT_LeftFinger32"), 
    _T("eSBT_LeftFinger42"),
    _T("eSBT_LeftFinger52"),
    _T("eSBT_RightFinger12"),
    _T("eSBT_RightFinger22"), 
    _T("eSBT_RightFinger32"), 
    _T("eSBT_RightFinger42"),
    _T("eSBT_RightFinger52"),

    _T("eSBT_LeftFinger13"),
    _T("eSBT_LeftFinger23"), 
    _T("eSBT_LeftFinger33"), 
    _T("eSBT_LeftFinger43"),
    _T("eSBT_LeftFinger53"),
    _T("eSBT_RightFinger13"),
    _T("eSBT_RightFinger23"), 
    _T("eSBT_RightFinger33"), 
    _T("eSBT_RightFinger43"),
    _T("eSBT_RightFinger53"),
    
    _T("eSBT_LeftForeTwist"),
    _T("eSBT_RightForeTwist"),
    _T("eSBT_LeftUpArmTwist"),
    _T("eSBT_RightUpArmTwist"),
    _T("eSBT_UserDefineBone"),
    _T("eSBT_UserDefineBone1"),
    _T("eSBT_UserDefineBone2"),
    _T("eSBT_UserDefineBone3"),
    _T("eSBT_UserDefineBone4"),
    _T("eSBT_Count"),
};

enum ESkeletonBoneType
{
    eSBT_Null = -1,
    eSBT_Root,
    eSBT_Pelvis,
    eSBT_Spine,

    eSBT_Spine1,
    eSBT_Spine2,
    eSBT_Spine3,
    eSBT_Neck,
    eSBT_Head,
    eSBT_LeftClavicle,
    eSBT_RightClavicle,
    eSBT_LeftUpperArm,
    eSBT_RightUpperArm,
    eSBT_LeftForearm,
    eSBT_RightForearm,
    eSBT_LeftHand,
    eSBT_RightHand,
    eSBT_LeftFinger1,
    eSBT_LeftFinger2, 
    eSBT_LeftFinger3, 
    eSBT_LeftFinger4,
    eSBT_LeftFinger5,
    eSBT_RightFinger1,
    eSBT_RightFinger2, 
    eSBT_RightFinger3, 
    eSBT_RightFinger4,
    eSBT_RightFinger5,

    eSBT_LeftThigh,
    eSBT_RightThigh,
    eSBT_LeftCalf,
    eSBT_RightCalf,
    eSBT_LeftFoot,
    eSBT_RightFoot,
    eSBT_LeftToe1,
    eSBT_LeftToe2, 
    eSBT_LeftToe3, 
    eSBT_LeftToe4,
    eSBT_LeftToe5,
    eSBT_RightToe1,
    eSBT_RightToe2, 
    eSBT_RightToe3, 
    eSBT_RightToe4,
    eSBT_RightToe5,

    eSBT_LeftFinger11,
    eSBT_LeftFinger21, 
    eSBT_LeftFinger31, 
    eSBT_LeftFinger41,
    eSBT_LeftFinger51,
    eSBT_RightFinger11,
    eSBT_RightFinger21, 
    eSBT_RightFinger31, 
    eSBT_RightFinger41,
    eSBT_RightFinger51,
    eSBT_LeftFinger12,
    eSBT_LeftFinger22, 
    eSBT_LeftFinger32, 
    eSBT_LeftFinger42,
    eSBT_LeftFinger52,
    eSBT_RightFinger12,
    eSBT_RightFinger22, 
    eSBT_RightFinger32, 
    eSBT_RightFinger42,
    eSBT_RightFinger52,

    eSBT_LeftFinger13,
    eSBT_LeftFinger23, 
    eSBT_LeftFinger33, 
    eSBT_LeftFinger43,
    eSBT_LeftFinger53,
    eSBT_RightFinger13,
    eSBT_RightFinger23, 
    eSBT_RightFinger33, 
    eSBT_RightFinger43,
    eSBT_RightFinger53,

    eSBT_LeftForeTwist,
    eSBT_RightForeTwist,
    eSBT_LeftUpArmTwist,
    eSBT_RightUpArmTwist,

    eSBT_Tooth,

    eSBT_UserDefineBone,
    eSBT_UserDefineBone1,
    eSBT_UserDefineBone2,
    eSBT_UserDefineBone3,
    eSBT_UserDefineBone4,
    eSBT_Count,
};


static const TCHAR* pszAnimationTypeName[] = 
{
    _T("ANI_Unknow"),
    _T("ANI_Fight1"),
    _T("ANI_Fight2"),
    _T("ANI_Underfire1"),
    _T("ANI_Underfire2"),
    _T("ANI_Underfire3"),
    _T("ANI_Converse"),
    _T("ANI_Up"),
    _T("ANI_Rest"),
    _T("ANI_Walk"),
    _T("ANI_Run"),
};
static const TCHAR* pszAnimationFileName[] = 
{
    _T("orgfight1.ani"),
    _T("orgfight2.ani"),
    _T("orgunderfire1.ani"),
    _T("orgunderfire2.ani"),
    _T("orgunderfire3.ani"),
    _T("orgconverse.ani"),
    _T("orgup.ani"),
    _T("orgrest.ani"),
    _T("orgwalk.ani"),
    _T("orgrun.ani"),
};
enum EAnimationType
{
    ANI_TYPE_UNKNOW = -1,
    ANI_TYPE_FIGHT1 = 0,
    ANI_TPYE_FIGHT2,
    ANI_TYPE_UNDERFIRE1,
    ANI_TYPE_UNDERFIRE2,
    ANI_TYPE_UNDERFIRE3,
    ANI_TYPE_CONVERSE,
    ANI_TYPE_UP,
    ANI_TYPE_REST,
    ANI_TYPE_WALK,
    ANI_TYEP_RUN,
};

#endif