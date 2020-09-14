#include "ModUIModel.h"



void ModUIModel::GetValueByRow(wxVariant& variant, unsigned int row, unsigned int col) const
{
	switch (col)
	{
	case Col_Check:
		variant = Rows[row].Enabled;
		break;
	case Col_Name:
		variant = Rows[row].Mod.ModName;
		break;
	case Col_Author:
		variant = Rows[row].Mod.ModAuthor;
		break;
	case Col_File:
		variant = Rows[row].File + ".gpk";
		break;
	case Col_Max:
		wxFAIL_MSG("invalid column");
	}
}

bool ModUIModel::GetAttrByRow(unsigned int row, unsigned int col, wxDataViewItemAttr& attr) const
{
	return false;
}

bool ModUIModel::SetValueByRow(const wxVariant& variant, unsigned int row, unsigned int col)
{
	if (col == Col_Check)
	{
		Rows[row].Enabled = variant.GetBool();
		return Delegate->OnModStateChange(Rows[row]);
	}
  return false;
}
