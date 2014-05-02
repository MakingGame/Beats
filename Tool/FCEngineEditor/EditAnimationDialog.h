#ifndef FCENGINEEDITOR_EditAnimationDialog_H__INCLUDE
#define FCENGINEEDITOR_EditAnimationDialog_H__INCLUDE
#include <wx/dialog.h>
#include <wx/aui/aui.h>
#include "Render/Animation.h"
#include "Render/Skin.h"
#include "Render/Skeleton.h"
#include "Render/Model.h"
#include "EditDialogBase.h"

class CAnimationGLWindow;
class TimeBarFrame;
class EditAnimationDialog : public EditDialogBase
{
    enum
    {
        ID_BUTTON_SELECTANIDIR,
        ID_BUTTON_SELECTSKEDIR,
        ID_BUTTON_SELECTSKIDIR,
        ID_BUTTON_CLEANLOG,
        ID_BUTTON_PLAY,
        ID_BUTTON_STOP,
        ID_BUTTON_PAUSE,
        ID_CHECKBOX_LOOP,
        ID_LISTBOX_ANIMATION,
        ID_LISTBOX_SKELETON,
        ID_LISTBOX_SKIN,
        ID_CHOICE_SKELETON,
        ID_BUTTON_SELECT,
        ID_CHECKBOX_SKELETONVISIBLE,
        ID_CHECKBOX_SKELETONBONEVISIBLE,
    };
    typedef EditDialogBase super;
public:
    EditAnimationDialog(wxWindow *parent, wxWindowID id, const wxString &title
        , const wxPoint &pos = wxDefaultPosition
        , const wxSize &size = wxDefaultSize
        , long style =  wxRESIZE_BORDER | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX | wxCAPTION
        , const wxString &name = wxDialogNameStr);
    virtual ~EditAnimationDialog();

    void InitWindow();
    void GetResourceList(wxString path);
    void ShowAnima();
    void OnSelectFile(wxCommandEvent& event);
    void OnClearLog(wxCommandEvent& event);
    void OnPlayAnimation(wxCommandEvent& event);
    void OnPauseAnimation(wxCommandEvent& event);
    void OnStopAnimation(wxCommandEvent& event);
    void OnLoopPlayAnimation(wxCommandEvent& event);
    void OnAnimationListBox(wxCommandEvent& event);
    void OnSkeketonListBox(wxCommandEvent& event);
    void OnSkinListBox(wxCommandEvent& event);
    void OnSkeletonChoice(wxCommandEvent& event);
    void OnSelectBoneButton(wxCommandEvent& event);
    void OnSkeletonCheckbox(wxCommandEvent& event);
    void OnSkeletonBoneCheckbox(wxCommandEvent& event);
    void InitTimeBar();
    wxWindow* GetCanvas() const;
    TimeBarFrame* GetTimeBar();
    SharePtr<CAnimation> GetAnimation();
    bool IsLoadAllAnimationResource();
    void SetAllSkeletonAndBoneVisible(bool bSke = false, bool bSkeBone = false);
    void SetSkeletonAndBoneVisible(bool bSke = false, bool bSkeBone = false);
    void DelListboxSelect();
    virtual void LanguageSwitch();
    void SetLanguage();
private:

    TimeBarFrame*       m_pTimeBar;
    wxAuiManager        m_Manager;
    CAnimationGLWindow* m_pAnimationGLWindow;
    wxWindow*           m_pTopRight;
    
    wxButton*           m_pButtonPlay;
    wxButton*           m_pButtonPause;
    wxButton*           m_pButtonStop;
    wxButton*           m_pButtonAnimaton;
    wxButton*           m_pButtonSkeleton;
    wxButton*           m_pButtonSkin;
    wxCheckBox*         m_pAnimationCheckBox;
    wxCheckBox*         m_pSKECheckBox;
    wxCheckBox*         m_pSKEBCheckBox;
    wxCheckBox*         m_pSkeletonCheckBox;
    wxCheckBox*         m_pSkinCheckBox;
    wxCheckBox*         m_pLoopCheckBox;
    wxTextCtrl*         m_pLog;
    wxListBox*          m_pAnimitionListBox;
    wxListBox*          m_pSkeletonListBox;
    wxListBox*          m_pSkinListBox;
    wxChoice*            m_pSkeletonChoice;
    wxButton*            m_pSkeSelectButton;
    wxLog*              m_pLogOld;
    SharePtr<CAnimation>    m_pAnimation;
    SharePtr<CSkin>         m_pSkin;
    SharePtr<CSkeleton>     m_pSkeleton;
    bool                    m_bAnimation;
    bool                    m_bSkeleton;
    bool                    m_bSkin;
    bool                    m_bIsLoop;
    bool                    m_bIsVisibleBone;
    bool                    m_bIsVisibleCoordinate;
    bool                    m_bIsSelectAll;
    bool                    m_bISLanguageSwitch;
    std::vector<ESkeletonBoneType>         m_skeletonBoneType;
DECLARE_EVENT_TABLE();
};
#endif