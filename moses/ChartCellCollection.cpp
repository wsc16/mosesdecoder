// $Id$
// vim:tabstop=2
/***********************************************************************
 Moses - factored phrase-based language decoder
 Copyright (C) 2010 Hieu Hoang

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 ***********************************************************************/

#include "ChartCellCollection.h"
#include "ChartCellMBOT.h"
#include "InputType.h"
#include "WordsRange.h"

namespace Moses {

ChartCellCollectionBase::~ChartCellCollectionBase() {
 std::cerr << "KILLING COLLECTION BASE " << std::endl;
  m_source.clear();
  for (std::vector<std::vector<ChartCellBase*> >::iterator i = m_cells.begin(); i != m_cells.end(); ++i) 
    RemoveAllInColl(*i);

  //Fabienne Braune : clean up
  RemoveAllInColl(m_mbotSource);
}

class CubeCellFactory {
  public:
    explicit CubeCellFactory(ChartManager &manager) : m_manager(manager) {}
    ChartCell *operator()(size_t start, size_t end) const {
    //Fabienne Braune : check if we are in MBOT system. If yes create ChartCellLabelMBOT
    //Would it make sense to have an own MBOT factory but how to initialize it? ChartCellCollectionBase
    //is not a direct base of ChartCellMBOT
    	if(StaticData::Instance().IsMBOT())
    	{
    		return static_cast<ChartCell*> (new ChartCellMBOT(start, end, m_manager));
    	 }
      return new ChartCell(start, end, m_manager);
    }

  private:
    ChartManager &m_manager;
};

/** Costructor
 \param input the input sentence
 \param manager reference back to the manager
 */
ChartCellCollection::ChartCellCollection(const InputType &input, ChartManager &manager)
  :ChartCellCollectionBase(input, CubeCellFactory(manager)) {}

} // namespace

