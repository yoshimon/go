object NewProjectForm: TNewProjectForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'New Project'
  ClientHeight = 161
  ClientWidth = 387
  Color = clBtnFace
  Constraints.MaxHeight = 200
  Constraints.MinHeight = 200
  Constraints.MinWidth = 353
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  DesignSize = (
    387
    161)
  PixelsPerInch = 96
  TextHeight = 13
  object ButtonBevel: TBevel
    AlignWithMargins = True
    Left = 8
    Top = 113
    Width = 371
    Height = 48
    Margins.Left = 8
    Margins.Right = 8
    Margins.Bottom = 0
    Align = alBottom
    Shape = bsTopLine
    ExplicitLeft = 0
    ExplicitTop = 344
    ExplicitWidth = 481
  end
  object ProjectDirectoryLabel: TLabel
    Left = 8
    Top = 8
    Width = 97
    Height = 13
    Caption = 'Project Directory'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object OutputDirLabel: TLabel
    Left = 8
    Top = 64
    Width = 95
    Height = 13
    Caption = 'Output Directory'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object OkButton: TButton
    Left = 223
    Top = 128
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 2
  end
  object CancelButton: TButton
    Left = 304
    Top = 128
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
  object ProjectDirectoryEdit: TButtonedEdit
    Left = 8
    Top = 27
    Width = 371
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    Images = MainForm.EditImages
    RightButton.ImageIndex = 0
    RightButton.Visible = True
    TabOrder = 0
    OnExit = ProjectDirectoryEditExit
    OnRightButtonClick = ProjectDirectoryEditRightButtonClick
  end
  object OutputDirEdit: TButtonedEdit
    Left = 8
    Top = 83
    Width = 371
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    Images = MainForm.EditImages
    RightButton.ImageIndex = 0
    RightButton.Visible = True
    TabOrder = 1
    OnRightButtonClick = ProjectDirectoryEditRightButtonClick
  end
end
