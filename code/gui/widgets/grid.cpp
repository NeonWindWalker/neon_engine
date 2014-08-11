#include "grid.h"

namespace GUI
{

void Grid::onUpdateLayout()
{
	Scalar rowsTotalAutoSize = 0;
	Scalar columnsTotalAutoSize = 0;

	for(uint nRow=0; nRow < rows.size(); ++nRow)
		if(rows[nRow].type == EGridSizeType_Auto){
			rows[nRow].realSize = 0;
			for(StrongPtr<Widget>* ch = childs.first(); ch != childs.end(); ++ch)
				if(*ch && (*ch)->row == nRow)
					maximize(rows[nRow].realSize, (*ch)->marginBound().height());
			rowsTotalAutoSize += rows[nRow].realSize;
		}
		else if(rows[nRow].type == EGridSizeType_Pixel){
			rows[nRow].realSize = rows[nRow].size;
			rowsTotalAutoSize += rows[nRow].realSize;
		}

	for(uint nColumn=0; nColumn < columns.size(); ++nColumn)
		if(columns[nColumn].type == EGridSizeType_Auto){
			columns[nColumn].realSize = 0;
			for(StrongPtr<Widget>* ch = childs.first(); ch != childs.end(); ++ch)
				if(*ch && (*ch)->column == nColumn)
					maximize(columns[nColumn].realSize, (*ch)->marginBound().width());
			columnsTotalAutoSize += columns[nColumn].realSize;
		}
		else if(columns[nColumn].type == EGridSizeType_Pixel){
			columns[nColumn].realSize = columns[nColumn].size;
			columnsTotalAutoSize += columns[nColumn].realSize;
		}

	Scalar totRowPercentage = 0;
	for(uint nRow=0; nRow < rows.size(); ++nRow)
		if(rows[nRow].type == EGridSizeType_Percent)
			totRowPercentage += rows[nRow].size;

	totRowPercentage = Scalar(1.0) / totRowPercentage;

	for(uint nRow=0; nRow < rows.size(); ++nRow)
		if(rows[nRow].type == EGridSizeType_Percent){
			rows[nRow].realSize = (size.y - rowsTotalAutoSize) * rows[nRow].size * totRowPercentage;
			maximize(rows[nRow].realSize, Scalar(0));
		}

	Scalar totColumnPercentage = 0;
	for(uint nColumn=0; nColumn < columns.size(); ++nColumn)
		if(columns[nColumn].type == EGridSizeType_Percent)
			totColumnPercentage += columns[nColumn].size;

	totColumnPercentage = Scalar(1.0) / totColumnPercentage;

	for(uint nColumn=0; nColumn < columns.size(); ++nColumn)
		if(columns[nColumn].type == EGridSizeType_Percent){
			columns[nColumn].realSize = (size.x - columnsTotalAutoSize) * columns[nColumn].size * totColumnPercentage;
			maximize(columns[nColumn].realSize, Scalar(0));
		}

	if(!rows.empty()){
		size.y = 0;
		for(GridSize* r = rows.first(); r != rows.end(); ++r){
			r->realPosition = size.y;
			size.y += r->realSize;
		}
	}
	if(!columns.empty()){
		size.x = 0;
		for(GridSize* c = columns.first(); c != columns.end(); ++c){
			c->realPosition = size.x;
			size.x += c->realSize;
		}
	}

	for(StrongPtr<Widget>* ch = childs.first(); ch != childs.end(); ++ch)
	{
		Widget* w = *ch;
		if(!w)
			continue;

		Bound bnd = childLocalBound();

		if(w->row < rows.size()){
			bnd.min.y = rows[w->row].realPosition;
			bnd.max.y = bnd.min.y + rows[w->row].realSize;
		}

		if(w->column < columns.size()){
			bnd.min.x = columns[w->column].realPosition;
			bnd.max.x = bnd.min.x + columns[w->column].realSize;
		}

		w->updateLayout_FromParent(bnd);
	}
}

}