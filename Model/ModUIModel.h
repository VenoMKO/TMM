#pragma once
#include <wx/dataview.h>
#include <vector>
#include "Mod.h"

class ModListDelegate {
public:
  virtual ~ModListDelegate()
  {}
  virtual bool OnModStateChange(ModEntry& mod) = 0;
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

  ModUIModel(std::vector<ModEntry>& entries, ModListDelegate* del)
    : Rows(entries)
    , Delegate(del)
  {}

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

private:
  std::vector<ModEntry>& Rows;
  ModListDelegate* Delegate = nullptr;
};