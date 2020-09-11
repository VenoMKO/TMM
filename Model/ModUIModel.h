#pragma once
#include <wx/dataview.h>
#include <vector>
#include "Mod.h"

class ModUIModelNode {
public:
  ModUIModelNode()
  {}

  ModUIModelNode(const ModEntry& e, int idx)
    : Name(e.Mod.ModName)
    , Author(e.Mod.ModAuthor)
    , File(e.File + wxT(".gpk"))
    , Enabled(e.Enabled)
    , Index(idx)
  {}
  wxString Name;
  wxString Author;
  wxString File;
  bool Enabled = false;
  int Index = 0;
};

class ModUIModel : public wxDataViewVirtualListModel {
public:
  enum
  {
    Col_Check = 0,
    Col_Name,
    Col_Author,
    Col_File,
    Col_Max
  };

  ModUIModel(const std::vector<ModEntry>& entries);

  unsigned int GetColumnCount() const override
  {
    return Col_Max;
  }

  wxString GetColumnType(unsigned int col) const override
  {
    if (col == Col_Check)
    {
      return "bool";
    }
    return wxDataViewCheckIconTextRenderer::GetDefaultType();
  }
  
  unsigned int GetCount() const override
  {
    return Rows.size();
  }

  void GetValueByRow(wxVariant& variant, unsigned int row, unsigned int col) const override;

  bool GetAttrByRow(unsigned int row, unsigned int col, wxDataViewItemAttr& attr) const override;

  virtual bool SetValueByRow(const wxVariant& variant, unsigned int row, unsigned int col) override;

  const std::vector<ModUIModelNode>& GetRows() const
  {
    return Rows;
  }

private:
  std::vector<ModUIModelNode> Rows;
};