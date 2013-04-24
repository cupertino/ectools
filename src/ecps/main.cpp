//============================================================================
// Name        : daq.cpp
// Author      : Dorian Rodde
// Version     : 0
// Date        : 2012.06.01
// Copyright   : Your copyright notice
// Description : A 'ps' like tool for energy consumption of processes.
//============================================================================

#include <iostream>


#include <libec/process.h>
#include <libec/grid/view/GridView.h>
#include <libec/tools/Color.h>
#include <libec/tools/Console.h>

#include <libec/log/Log.h>

using namespace cea;


int main()
{
    /* Variable */
    GridView grid;
    ProcessEnumerator pe;

    /* Log initialization */
    Log::create("out.log");

    /* Start Text */
	std::cout << "Process Energy Consumption Snapshot tool" << std::endl;

    /* Add Column NAME and stylize it */
	GridViewColumn* c1 = grid.addColumn("NAME", cea::STRING, 30);
    c1->getDefaultCellStyle()->setAlign(LEFT);

    /* Add Column PID and stylize it */
	GridViewColumn* c2 = grid.addColumn("PID", cea::INT, 10);
    c2->getDefaultCellStyle()->setAlign(RIGHT);

    /* Add Column USER and stylize it */
	GridViewColumn* c3 = grid.addColumn("USER", cea::INT, 10);
    c3->getDefaultCellStyle()->setAlign(LEFT);


    /* Set the column Width */
    grid.smartResizeColumns(Console::getSize().width);

    /* Enable Sort: sort By the Second Column DESCENDING */
    grid.enableSort(1,GridView::ASCENDING);


    /* Update process */
    pe.update();


    /* Fill the grid with all the Process */
    Process* p;
    for(unsigned int i = 0, max = pe.getProcessCount(); i < max; i++) {
        p = pe.getProcess(i);
        if (p != 0) {
            GridViewRow* r = grid.addRow();
            r->getCell(0)->setValue(p->getName());
            r->getCell(1)->setValue((int)p->getPid());
            r->getCell(2)->setValue(p->getUserId());
            grid.refreshRow(r);
        }
    }

    /* Render */
    grid.render(0,1);

    return 0;
}
